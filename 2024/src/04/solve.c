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

    // Make grid
    int height = lines;
    int width = 0;
    while (buffer[width] != '\n')
    {
        width++;
    }

    char grid[height][width];

    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            grid[j][i] = buffer[i * height + j + i];
        }
    }

    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            // right (+, 0)
            if (j + 3 < width && grid[j][i] == 'X' && grid[j + 1][i] == 'M' && grid[j + 2][i] == 'A' && grid[j + 3][i] == 'S')
                solution += 1;

            // down (0, +)
            if (i + 3 < height && grid[j][i] == 'X' && grid[j][i + 1] == 'M' && grid[j][i + 2] == 'A' && grid[j][i + 3] == 'S')
                solution += 1;

            // left (-, 0)
            if (j - 3 >= 0 && grid[j][i] == 'X' && grid[j - 1][i] == 'M' && grid[j - 2][i] == 'A' && grid[j - 3][i] == 'S')
                solution += 1;

            // up (0, -)
            if (i - 3 >= 0 && grid[j][i] == 'X' && grid[j][i - 1] == 'M' && grid[j][i - 2] == 'A' && grid[j][i - 3] == 'S')
                solution += 1;

            // SE (+, +)
            if (j + 3 < width && i + 3 < height && grid[j][i] == 'X' && grid[j + 1][i + 1] == 'M' && grid[j + 2][i + 2] == 'A' && grid[j + 3][i + 3] == 'S')
                solution += 1;

            // SW (-, +)
            if (j - 3 >= 0 && i + 3 < height && grid[j][i] == 'X' && grid[j - 1][i + 1] == 'M' && grid[j - 2][i + 2] == 'A' && grid[j - 3][i + 3] == 'S')
                solution += 1;
            // NW (-, -)
            if (j - 3 >= 0 && i - 3 >= 0 && grid[j][i] == 'X' && grid[j - 1][i - 1] == 'M' && grid[j - 2][i - 2] == 'A' && grid[j - 3][i - 3] == 'S')
                solution += 1;

            // NE (+, -)
            if (j + 3 < width && i - 3 >= 0 && grid[j][i] == 'X' && grid[j + 1][i - 1] == 'M' && grid[j + 2][i - 2] == 'A' && grid[j + 3][i - 3] == 'S')
                solution += 1;
        }
    }

    // Part 1
    printf("Part1 Solution: %d\n", solution);

    // Part 2
    solution = 0;
    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            if (j + 1 < width && i + 1 < height && j - 1 >= 0 && i - 1 >= 0)
            {
                if(grid[j][i] == 'A' && grid[j+1][i+1] == 'M' && grid[j+1][i-1] == 'M' && grid[j-1][i+1] == 'S' && grid[j-1][i-1] == 'S')
                    solution += 1;
                if(grid[j][i] == 'A' && grid[j+1][i+1] == 'S' && grid[j+1][i-1] == 'S' && grid[j-1][i+1] == 'M' && grid[j-1][i-1] == 'M')
                    solution += 1;
                if(grid[j][i] == 'A' && grid[j+1][i+1] == 'S' && grid[j+1][i-1] == 'M' && grid[j-1][i+1] == 'S' && grid[j-1][i-1] == 'M')
                    solution += 1;
                if(grid[j][i] == 'A' && grid[j+1][i+1] == 'M' && grid[j+1][i-1] == 'S' && grid[j-1][i+1] == 'M' && grid[j-1][i-1] == 'S')
                    solution += 1;
            }
        }
    }
    printf("Part2 Solution: %d\n", solution);

    if (buffer)
        free(buffer);

    return EXIT_SUCCESS;
}
