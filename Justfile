[private]
default:
    @just --list

create_day day:
    #!/usr/bin/bash
    set -eou pipefail
    year="$(basename {{ invocation_dir() }})"
    if [[ ! "${year}" =~ [0-9]{4} ]]; then
        echo "Not in a Year..."
        exit 1
    fi
    if [[ ! "{{ day }}" =~ ^[0-2][0-9]$ ]]; then
        echo "Improper Day..."
        exit 1
    fi
    mkdir -p ${year}/src/{{ day }}
    echo "" > ${year}/src/{{ day }}/test_file.txt
    touch ${year}/src/{{ day }}/input_file.txt
    if [[ -f src/{{ day }}/solve.c ]]; then
        exit 0
    fi
    cat > ${year}/src/{{ day }}/solve.c << 'EOF'
    #define AOC_UTILS_IMPLEMENTATION
    #include "../../../lib/aoc.h"

    int main(int argc, char **argv)
    {
        if (argc < 2)
        {
            printf("Missing file\n");
            return EXIT_FAILURE;
        }

        char *filename = argv[1];
        size_t solution = 0;

        // Read in File
        Buffer_t buffer_t = read_file_buffer(filename);
        char * buffer = buffer_t.buffer;
        int count = buffer_t.count;
        int lines = buffer_t.lines;
        if (!buffer)
        {
            return EXIT_FAILURE;
        }
        if (!count)
        {
            fprintf(stderr, "%s is empty\n", filename);
            free(buffer);
            return EXIT_FAILURE;
        }
        if (!lines)
        {
            fprintf(stderr, "%s does not have a trailing newline\n", filename);
        }

        // Part 1
        clock_t tic = clock();

        clock_t toc = clock();
        printf("Part1 Solution: %ld\n", solution);
        printf("Part1 Time: %.03f ms\n", (double)(toc - tic) / CLOCKS_PER_SEC * 1000);

        // Part 2
        solution = 0;
        tic = clock();

        toc = clock();
        printf("Part2 Solution: %ld\n", solution);
        printf("Part2 Time: %.03f ms\n", (double)(toc - tic) / CLOCKS_PER_SEC * 1000);

        free(buffer_t.buffer);

        return EXIT_SUCCESS;
    }
    EOF

build day:
    #!/usr/bin/bash
    set -eou pipefail
    year="$(basename {{ invocation_dir() }})"
    if [[ ! "${year}" =~ [0-9]{4} ]]; then
        echo "Not in a Year..."
        exit 1
    fi
    if [[ ! "{{ day }}" =~ ^[0-2][0-9]$ ]]; then
        echo "Improper Day?"
        exit 1
    fi
    set -x
    mkdir -p build
    gcc -o {{ invocation_dir() }}/build/{{ day }}.binary {{ invocation_dir() }}/src/{{ day }}/solve.c -Werror -Wall

run day: (build day)
    #!/usr/bin/bash
    set -eou pipefail
    year="$(basename {{ invocation_dir() }})"
    if [[ ! "${year}" =~ [0-9]{4} ]]; then
        echo "Not in a Year..."
        exit 1
    fi
    echo -e "\033[1mTest File\033[0m"
    "${year}"/build/{{ day }}.binary "${year}/src/{{ day }}/test_file.txt"
    echo -e "\033[1mInput File\033[0m"
    "${year}"/build/{{ day }}.binary "${year}/src/{{ day }}/input_file.txt"

