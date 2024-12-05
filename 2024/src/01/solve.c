#define AOC_UTILS_IMPLEMENTATION
#include "../../../lib/aoc.h"

#define DELIM " \n"

int main(int argc, char **argv)
{
    int exit_code = EXIT_FAILURE;
    if (argc < 2)
    {
        fprintf(stderr, "Missing file\n");
        return exit_code;
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
    int diff[lines];
    int column1[lines];
    int column2[lines];

    column1[0] = atoi(strtok(buffer, DELIM));
    column2[0] = atoi(strtok(NULL, DELIM));

    for (int i = 1; i < lines; i++)
    {
        column1[i] = atoi(strtok(NULL, DELIM));
        column2[i] = atoi(strtok(NULL, DELIM));
    }

    qsort(column1, lines, sizeof(int), int_compare);
    qsort(column2, lines, sizeof(int), int_compare);

    for (int i = 0; i < lines; i++)
    {
        // printf("%d   %d\n", column1[i], column2[i]);
        diff[i] = (column1[i] > column2[i]) ? column1[i] - column2[i] : column2[i] - column1[i];
    }

    for (int i = 0; i < lines; i++)
    {
        // printf("%d\n", diff[i]);
        solution += diff[i];
    }

    printf("Part1 Solution: %d\n", solution);

    // Part 2
    solution = 0;
    for (int i = 0; i < lines; i++)
    {
        int count = 0;
        for (int j = 0; j < lines; j++)
        {
            if (column1[i] == column2[j])
            {
                count++;
            }
        }
        solution += column1[i] * count;
    }

    printf("Part2 Solution: %d\n", solution);

    free(buffer);
    return EXIT_SUCCESS;
}
