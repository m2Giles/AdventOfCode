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

    // Make grid
    Grid_t grid = make_grid(buffer, lines);
    if (!grid.grid)
    {
        destroy_grid(grid);
        free(buffer);
        return EXIT_FAILURE;
    }
    for (int i = 0; i < grid.height; ++i)
    {
        for (int j = 0; j < grid.width; ++j)
        {
            if (grid.grid[i][j] == 'X')
            {
                // down (+, 0)
                if (i + 3 < grid.height && grid.grid[i + 1][j] == 'M' && grid.grid[i + 2][j] == 'A' && grid.grid[i + 3][j] == 'S')
                    solution += 1;

                // right (0, +)
                if (j + 3 < grid.width && grid.grid[i][j + 1] == 'M' && grid.grid[i][j + 2] == 'A' && grid.grid[i][j + 3] == 'S')
                    solution += 1;

                // up (-, 0)
                if (i - 3 >= 0 && grid.grid[i - 1][j] == 'M' && grid.grid[i - 2][j] == 'A' && grid.grid[i - 3][j] == 'S')
                    solution += 1;

                // left (0, -)
                if (j - 3 >= 0  && grid.grid[i][j - 1] == 'M' && grid.grid[i][j - 2] == 'A' && grid.grid[i][j - 3] == 'S')
                    solution += 1;

                // SE (+, +)
                if (i + 3 < grid.height && j + 3 < grid.width && grid.grid[i + 1][j + 1] == 'M' && grid.grid[i + 2][j + 2] == 'A' && grid.grid[i + 3][j + 3] == 'S')
                    solution += 1;

                // NE (-, +)
                if (i - 3 >= 0 && j + 3 < grid.width && grid.grid[i - 1][j + 1] == 'M' && grid.grid[i - 2][j + 2] == 'A' && grid.grid[i - 3][j + 3] == 'S')
                    solution += 1;
                // NW (-, -)
                if (i -3 >= 0 && j - 3 >= 0 && grid.grid[i - 1][j - 1] == 'M' && grid.grid[i - 2][j - 2] == 'A' && grid.grid[i - 3][j - 3] == 'S')
                    solution += 1;

                // SW (+, -)
                if (i + 3 < grid.height && j - 3 >= 0 && grid.grid[i + 1][j - 1] == 'M' && grid.grid[i + 2][j - 2] == 'A' && grid.grid[i + 3][j - 3] == 'S')
                    solution += 1;
            }
        }
    }

    // Part 1
    printf("Part1 Solution: %d\n", solution);

    // Part 2
    solution = 0;
    for (int i = 1; i < grid.height - 1; ++i)
    {
        for (int j = 1; j < grid.width - 1; ++j)
        {
            if (grid.grid[i][j] == 'A')
            {
                if (grid.grid[i + 1][j + 1] == 'M' && grid.grid[i + 1][j - 1] == 'M' && grid.grid[i - 1][j + 1] == 'S' && grid.grid[i - 1][j - 1] == 'S')
                    solution += 1;
                if (grid.grid[i + 1][j + 1] == 'S' && grid.grid[i + 1][j - 1] == 'S' && grid.grid[i - 1][j + 1] == 'M' && grid.grid[i - 1][j - 1] == 'M')
                    solution += 1;
                if (grid.grid[i + 1][j + 1] == 'S' && grid.grid[i + 1][j - 1] == 'M' && grid.grid[i - 1][j + 1] == 'S' && grid.grid[i - 1][j - 1] == 'M')
                    solution += 1;
                if (grid.grid[i + 1][j + 1] == 'M' && grid.grid[i + 1][j - 1] == 'S' && grid.grid[i - 1][j + 1] == 'M' && grid.grid[i - 1][j - 1] == 'S')
                    solution += 1;
            }
        }
    }
    printf("Part2 Solution: %d\n", solution);

    if (grid.grid)
        destroy_grid(grid);
    if (buffer)
        free(buffer);

    return EXIT_SUCCESS;
}
