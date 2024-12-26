set allow-duplicate-variables := true
set allow-duplicate-recipes := true

[private]
default:
    @just --list

create_day day:
    #!/usr/bin/bash
    set -eou pipefail
    if [[ "{{ day }}" -lt 1 || "{{ day }}" -gt 25 ]]; then
        echo "Improper Day..."
        exit 1
    fi
    if [[ "{{ day }}" -lt 10 ]]; then
        day="0$((10#{{ day }}))"
    else
        day={{ day }}
    fi
    mkdir -p ./src/${day}
    touch ./src/${day}/test_file.txt
    touch ./src/${day}/input_file.txt
    if [[ -f ./src/${day}/solve.c ]]; then
        exit 0
    fi
    cat > ./src/${day}/solve.c << 'EOF'
    #define AOC_UTILS_IMPLEMENTATION
    #include "aoc.h"

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
        printf("Part1 Time: %.03f ms\n",
            (double)(toc - tic) / CLOCKS_PER_SEC * 1000);

        // Part 2
        solution = 0;
        tic = clock();

        toc = clock();
        printf("Part2 Solution: %ld\n", solution);
        printf("Part2 Time: %.03f ms\n",
            (double)(toc - tic) / CLOCKS_PER_SEC * 1000);

        free(buffer_t.buffer);

        return EXIT_SUCCESS;
    }
    EOF

build day:
    #!/usr/bin/bash
    set -eou pipefail
    if [[ "{{ day }}" -lt 1 || "{{ day }}" -gt 25 ]]; then
        echo "Improper Day..."
        exit 1
    fi
    if [[ "{{ day }}" -lt 10 ]]; then
        day="0$((10#{{ day }}))"
    else
        day={{ day }}
    fi
    set -x
    mkdir -p build
    gcc -o ./build/${day}.binary ./src/${day}/solve.c -Werror -Wall -I../lib

run day: (build day)
    #!/usr/bin/bash
    set -eou pipefail
    if [[ "{{ day }}" -lt 1 || "{{ day }}" -gt 25 ]]; then
        echo "Improper Day..."
        exit 1
    fi
    if [[ "{{ day }}" -lt 10 ]]; then
        day="0$((10#{{ day }}))"
    else
        day={{ day }}
    fi
    echo -e "\033[1mTest File\033[0m"
    ./build/${day}.binary ./src/${day}/test_file.txt
    echo -e "\033[1mInput File\033[0m"
    ./build/${day}.binary ./src/${day}/input_file.txt
