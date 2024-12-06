#define AOC_UTILS_IMPLEMENTATION
#include "../../../lib/aoc.h"

#include <time.h>

typedef struct
{
    int X;
    int Y;
    int dx;
    int dy;
    char dir;
} guard_t;

int main(int argc, char **argv)
{
    clock_t toc = clock();

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
    guard_t guard = {0};
    guard_t start = {0};

    Grid_t grid = make_grid(buffer, lines);

    for (int i = 0; i < grid.height; ++i)
    {
        for (int j = 0; j < grid.width; ++j)
        {
            if (grid.grid[i][j] == '^')
            {
                start = guard = (guard_t){.Y = i, .X = j, .dx = 0, .dy = -1, .dir = 1};
            }
        }
    }
    while (guard.X >= 0 && guard.Y >= 0 && guard.X < grid.width && guard.Y < grid.height)
    {
        if (grid.grid[guard.Y][guard.X] == '#')
        {
            guard.X -= guard.dx;
            guard.Y -= guard.dy;
            int temp = guard.dx;
            guard.dx = -guard.dy;
            guard.dy = temp;
        }
        grid.grid[guard.Y][guard.X] = 'X';
        guard.X += guard.dx;
        guard.Y += guard.dy;
    }
    for (int i = 0; i < grid.height; ++i)
    {
        for (int j = 0; j < grid.height; ++j)
        {
            if (grid.grid[i][j] == 'X')
            {
                solution++;
                printf(RED " " NORMAL);
            }
            else if (grid.grid[i][j] == '#')
            {
                printf(BOLD " " NORMAL);
            }
            else
            {
                printf(GREEN " " NORMAL);
            }
        }
        printf("\n");
    }
    printf("Part1 Solution: %d\n", solution);

    // Part 2
    solution = 0;
    char state[grid.height][grid.width];

    for (int i = 0; i < grid.height; ++i)
    {
        for (int j = 0; j < grid.width; ++j)
        {
            if (grid.grid[i][j] != 'X')
            {
                continue;
            }
            memset(state, 0, sizeof(state));
            grid.grid[i][j] = '#';
            guard = start;

            bool inLoop = false;
            while (guard.X >= 0 && guard.Y >= 0 && guard.X < grid.width && guard.Y < grid.height)
            {
                if (grid.grid[guard.Y][guard.X] == '#')
                {
                    if (state[guard.Y][guard.X] & guard.dir)
                    {
                        inLoop = true;
                        break;
                    } else {
                        state[guard.Y][guard.X] = guard.dir | state[guard.Y][guard.X];
                    }
                    guard.X -= guard.dx;
                    guard.Y -= guard.dy;
                    int temp = guard.dx;
                    guard.dx = -guard.dy;
                    guard.dy = temp;
                    guard.dir = guard.dir << 1;
                    if (guard.dir > 15)
                    {
                        guard.dir = 1;
                    }
                }

                guard.X += guard.dx;
                guard.Y += guard.dy;
            }

            grid.grid[i][j] = 'X';
            if (inLoop)
            {
                solution++;
            }
        }
    }

    clock_t tic = clock();

    printf("Part2 Solution: %d\n", solution);
    printf("Part2 Time: %.03f ms\n", (double)(tic - toc) / CLOCKS_PER_SEC * 1000);

    destroy_grid(grid);
    if (buffer)
        free(buffer);

    return EXIT_SUCCESS;
}
