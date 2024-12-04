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
    for (int i = 0; buffer[i] != '\0'; ++i)
    {
        if (strncmp(&buffer[i], "mul(", 4) == 0)
        {
            i+=4;
            char num_buffer[4];
            char num_buffer_2[4];
            memset(num_buffer, 0, sizeof(num_buffer));
            memset(num_buffer_2, 0, sizeof(num_buffer_2));
            if(isdigit(buffer[i]))
            {
                num_buffer[0] = buffer[i];
                num_buffer[1] = '\0';
                i++;
            }
            if(isdigit(buffer[i]))
            {
                num_buffer[1] = buffer[i];
                num_buffer[2] = '\0';
                i++;
            }
            if(isdigit(buffer[i]))
            {
                num_buffer[2] = buffer[i];
                num_buffer[3] = '\0';
                i++;
            }
            if(buffer[i] == ',')
            {
                i++;
            } else {
                continue;
            }
            if(isdigit(buffer[i]))
            {
                num_buffer_2[0] = buffer[i];
                num_buffer_2[1] = '\0';
                i++;
            }
            if(isdigit(buffer[i]))
            {
                num_buffer_2[1] = buffer[i];
                num_buffer_2[2] = '\0';
                i++;
            }
            if(isdigit(buffer[i]))
            {
                num_buffer_2[2] = buffer[i];
                num_buffer_2[3] = '\0';
                i++;
            }
            if(buffer[i] != ')')
                continue;
            // printf("mul(%s,%s%c\n", num_buffer, num_buffer_2, buffer[i]);
            solution += atoi(num_buffer) * atoi(num_buffer_2);
        }
    }
    printf("Part1 Solution: %d\n", solution);

    // Part 2
    solution = 0;
    int DO = 1;
    for (int i = 0; buffer[i] != '\0'; ++i)
    {
        if (strncmp(&buffer[i], "don't()", 7) == 0)
        {
            DO = 0;
            i+=7;
        }
        if (strncmp(&buffer[i], "do()", 4) == 0)
        {
            DO = 1;
            i+=4;
        }
        if (strncmp(&buffer[i], "mul(", 4) == 0)
        {
            i+=4;
            char num_buffer[4];
            char num_buffer_2[4];
            memset(num_buffer, 0, sizeof(num_buffer));
            memset(num_buffer_2, 0, sizeof(num_buffer_2));
            if(isdigit(buffer[i]))
            {
                num_buffer[0] = buffer[i];
                num_buffer[1] = '\0';
                i++;
            }
            if(isdigit(buffer[i]))
            {
                num_buffer[1] = buffer[i];
                num_buffer[2] = '\0';
                i++;
            }
            if(isdigit(buffer[i]))
            {
                num_buffer[2] = buffer[i];
                num_buffer[3] = '\0';
                i++;
            }
            if(buffer[i] == ',')
            {
                i++;
            } else {
                continue;
            }
            if(isdigit(buffer[i]))
            {
                num_buffer_2[0] = buffer[i];
                num_buffer_2[1] = '\0';
                i++;
            }
            if(isdigit(buffer[i]))
            {
                num_buffer_2[1] = buffer[i];
                num_buffer_2[2] = '\0';
                i++;
            }
            if(isdigit(buffer[i]))
            {
                num_buffer_2[2] = buffer[i];
                num_buffer_2[3] = '\0';
                i++;
            }
            if(buffer[i] != ')')
                continue;
            // if(DO)
            //     printf("mul(%s,%s%c\n", num_buffer, num_buffer_2, buffer[i]);
            solution += atoi(num_buffer) * atoi(num_buffer_2) * DO;
        }
    }
    printf("Part2 Solution: %d\n", solution);

    if (buffer)
        free(buffer);

    return EXIT_SUCCESS;
}
