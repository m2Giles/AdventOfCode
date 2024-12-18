#define AOC_UTILS_IMPLEMENTATION
#include "../../../lib/aoc.h"

#include <time.h>

#define DELIM " \n"

static bool pm_match(long target, long result, int *args, int num_args)
{
    bool m = false;
    bool p = false;

    if (result < args[0])
    {
        return false;
    }

    if (num_args == 1)
    {
        return result == args[num_args - 1];
    }

    if (result % args[num_args - 1] == 0)
    {
        m = pm_match(target, result / args[num_args - 1], args, num_args -1);
    } 

    p = pm_match(target, result - args[num_args - 1], args, num_args - 1);

    return m || p;
}

static bool cpm_match(long target, long result, int *args, int num_args)
{
    int last = args[num_args - 1];
    int mag = 1;
    while (mag <= last)
    {
        mag *= 10;
    }

    bool m = false;
    bool p = false;
    bool c = false;

    if (result < args[0])
    {
        return false;
    }

    if (num_args == 1)
    {
        return result == last;
    }

    if (result % last == 0)
    {
        m = cpm_match(target, result / last, args, num_args -1);
    } 

    if ((result - last) % mag == 0)
    {
        c = cpm_match(target, (result - last) / mag, args, num_args -1);
    }

    p = cpm_match(target, result - last, args, num_args - 1);

    return  m || p || c;
}

int main(int argc, char **argv)
{
    clock_t tic = clock();
    if (argc < 2)
    {
        printf("Missing file\n");
        return EXIT_FAILURE;
    }

    char *filename = argv[1];
    long solution = 0;

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
    char *line_sp = NULL;
    char *arg_sp = NULL;

    char *line = strtok_r(buffer, "\n", &line_sp);
    while (line != NULL)
    {
        int args[25] = {0};
        long target = strtol(strtok_r(line, ":", &arg_sp), NULL, 10);

        int num_args = 0;
        char *arg_s = strtok_r(NULL, " ", &arg_sp);
        while (arg_s != NULL)
        {
            args[num_args] = strtol(arg_s, NULL, 10);
            num_args++;
            arg_s = strtok_r(NULL, " ", &arg_sp);
        }
        if (pm_match(target, target, (int *) args, num_args))
        {
            solution += target;
        }
        line = strtok_r(NULL, "\n", &line_sp);
    }
    clock_t toc = clock();
    printf("Part1 Solution: %ld\n", solution);
    printf("Part 1 Time: %.03f ms\n", (double)(toc - tic) / CLOCKS_PER_SEC * 1000);

    // Part 2
    solution = 0;
    free(buffer);
    buffer = NULL;
    line_sp = NULL;
    arg_sp = NULL;

    tic = clock();
    buffer = read_file(filename);

    line = strtok_r(buffer, "\n", &line_sp);
    while (line != NULL)
    {
        int args[25] = {0};
        long target = strtol(strtok_r(line, ":", &arg_sp), NULL, 10);

        int num_args = 0;
        char *arg_s = strtok_r(NULL, " ", &arg_sp);
        while (arg_s != NULL)
        {
            args[num_args] = strtol(arg_s, NULL, 10);
            num_args++;
            arg_s = strtok_r(NULL, " ", &arg_sp);
        }
        if (cpm_match(target, target, (int *) args, num_args))
        {
            solution += target;
        }
        line = strtok_r(NULL, "\n", &line_sp);
    }
    toc = clock();
    printf("Part2 Solution: %ld\n", solution);
    printf("Part 2 Time: %.03f ms\n", (double)(toc - tic) / CLOCKS_PER_SEC * 1000);

    if (buffer)
        free(buffer);

    return EXIT_SUCCESS;
}
