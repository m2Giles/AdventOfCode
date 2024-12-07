#define AOC_UTILS_IMPLEMENTATION
#include "../../../lib/aoc.h"

#include <time.h>

#define DELIM " \n"

static long cat(long a, long b)
{
    int mag = 1;
    while (mag <= b)
    {
        mag *= 10;
    }
    return mag * a + b;
}

static bool pm_match(long target, long result, int *args, int num_args)
{
    if (result > target)
    {
        return false;
    }
    if (num_args == 0)
    {
        return result == target;
    }

    if (pm_match(target, result + args[0], &args[1], num_args - 1))
    {
        return true;
    }
    if (pm_match(target, result * args[0], &args[1], num_args - 1))
    {
        return true;
    }
    return false;
}

static bool cpm_match(long target, long result, int *args, int num_args)
{
    if (result > target)
    {
        return false;
    }
    if (num_args == 0)
    {
        return result == target;
    }

    if (cpm_match(target, result + args[0], &args[1], num_args - 1))
    {
        return true;
    }
    if (cpm_match(target, result * args[0], &args[1], num_args - 1))
    {
        return true;
    }
    if (cpm_match(target, cat(result, args[0]), &args[1], num_args - 1))
    {
        return true;
    }
    return false;
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
        if (pm_match(target, args[0], &args[1], num_args - 1))
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
        if (cpm_match(target, args[0], &args[1], num_args - 1))
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
