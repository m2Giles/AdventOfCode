#define AOC_UTILS_IMPLEMENTATION
#include "../../lib/aoc.h"

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
    char *buffer_saveptr = NULL;
    char *line = strtok_r(buffer, "\n", &buffer_saveptr);
    char *line_saveptr = NULL;
    while (line != NULL)
    {
        int count = 0;
        int length = strlen(line);
        int array[length];
        memset(array, 0, sizeof(array));

        char *token = strtok_r(line, " ", &line_saveptr);
        while (token != NULL)
        {
            array[count] = atoi(token);
            count++;
            token = strtok_r(NULL, " ", &line_saveptr);
        }
        int safe = 1;
        int diff = 0;
        int pos = 0;
        int neg = 0;
        for (int i = 1; i < length; ++i)
        {
            if (neg && pos)
            {
                safe = 0;
            }
            if (array[i] == 0)
                break;
            diff = array[i] - array[i - 1];
            if (diff > 3 || diff < -3 || diff == 0)
            {
                safe = 0;
            }

            if (diff < 0)
            {
                neg = 1;
            }
            else
            {
                pos = 1;
            }
        }
        if (safe)
        {
            solution++;
        }
        line = strtok_r(NULL, "\n", &buffer_saveptr);
    }
    printf("Part1 Solution: %d\n", solution);

    // Part 2
    solution = 0;
    buffer = read_file(filename);
    buffer_saveptr = NULL;
    line = strtok_r(buffer, "\n", &buffer_saveptr);
    line_saveptr = NULL;
    for(;;)
    {
        int count = 0;
        int safe = 1;
        int length = strlen(line);
        int array[length];
        memset(array, 0, sizeof(array));

        char *token = strtok_r(line, " ", &line_saveptr);
        while (token != NULL)
        {
            array[count] = atoi(token);
            count++;
            token = strtok_r(NULL, " ", &line_saveptr);
        }
        int copy[count];
        for(int i = 0; i < count; i++)
            {
                int pos = 0;
                int neg = 0;
                for (int j = 0; j < count - 1; j++)
                {
                    if(j < i)
                        copy[j] = array[j];
                    else
                        copy[j] = array[j+1];
                }

                for (int j = 0; j < count - 2; j++)
                {
                    int diff = copy[j] - copy[j+1];

                    if(diff < 0)
                    {
                        neg = 1;
                    } else {
                        pos = 1;
                    }

                    if (abs(diff) > 3 || diff == 0 || (neg && pos))
                    {
                        safe = 0;
                        break;
                    } else {
                        safe = 1;
                    }
                }
                if(safe)
                    break;
            }
        if(safe)
        {
            solution++;
        }
        line = strtok_r(NULL, "\n", &buffer_saveptr);
        if (!line)
            break;
    }
    printf("Part2 Solution: %d\n", solution);

    if (buffer)
        free(buffer);

    return EXIT_SUCCESS;
}
