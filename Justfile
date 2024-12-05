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
    touch ${year}/src/{{ day }}/test_file.txt
    touch ${year}/src/{{ day }}/input_file.txt
    if [[ -f src/{{ day }}/solve.c ]]; then
        exit 0
    fi
    cat > ${year}/src/{{ day }}/solve.c << 'EOF'
    #define AOC_UTILS_IMPLEMENTATION
    #include "../../../lib/aoc.h"

    #define DELIM " \n"

    int main(int argc, char **argv)
    {
        if (argc < 2)
        {
            printf("Missing file\n");
            return EXIT_FAILURE;
        }

        char *filename = argv[1];
        int solution = 0;

        // Read in File
        char *buffer = read_file(filename);
        if (!buffer)
        {
            return EXIT_FAILURE;
        }
        int lines = count_lines(filename);
        if (!lines)
        {
            fprintf(stderr, "%s is empty\n", filename);
            free(buffer);
            return EXIT_FAILURE;
        }

        // Part 1
        printf("Part1 Solution: %d\n", solution);

        // Part 2
        solution = 0;
        printf("Part2 Solution: %d\n", solution);

        if (buffer)
            free(buffer);

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

