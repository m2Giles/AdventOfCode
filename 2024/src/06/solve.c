#define AOC_UTILS_IMPLEMENTATION
#include "../../../lib/aoc.h"

typedef struct
{
    int X;
    int Y;
    int dx;
    int dy;
} guard_t;

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
    guard_t guard = {0};

    Grid_t grid = make_grid(buffer, lines);
    bool visited[grid.height][grid.width];
    memset(visited, 0, sizeof(visited));

    for (int i = 0; i < grid.height; ++i)
    {
        for (int j = 0; j < grid.width; ++j)
        {
            if (grid.grid[i][j] == '^')
            {
                guard.Y = i;
                guard.X = j;
                guard.dx = 0;
                guard.dy = -1;
            }
        }
    }
    while (guard.X >= 0 && guard.Y >= 0 && guard.X < grid.width && guard.Y < grid.height)
    {
        visited[guard.Y][guard.X] = true;
        if (grid.grid[guard.Y][guard.X] == '#')
        {
            visited[guard.Y][guard.X] = false;
            guard.X -= guard.dx;
            guard.Y -= guard.dy;
            int temp = guard.dx;
            guard.dx = -guard.dy;
            guard.dy = temp;
        }
        guard.X += guard.dx;
        guard.Y += guard.dy;
    }
    for (int i = 0; i < grid.height; ++i)
    {
        for (int j = 0; j < grid.height; ++j)
        {
            if (visited[i][j])
            {
                solution++;
                printf(BOLD GREEN "#" NORMAL);
            }
            else
            {
                printf(RED "." NORMAL);
            }
        }
        printf("\n");
    }
    printf("Part1 Solution: %d\n", solution);

    // Part 2
    solution = 0;
    for (int i = 0; i < grid.height; ++i)
    {
        for (int j = 0; j < grid.width; ++j)
        {
            if (grid.grid[i][j] == '^')
            {
                guard.Y = i;
                guard.X = j;
                guard.dx = 0;
                guard.dy = -1;
            }
        }
    }
    // Save Original
    guard_t start = guard;

    for (int i = 0; i < grid.height; ++i)
    {
        for (int j = 0; j < grid.width; ++j)
        {
            if (grid.grid[i][j] == '#' || visited[i][j] == false)
            {
                continue;
            }

            grid.grid[i][j] = '#';
            guard = start;

            int cycle_count = 0;
            while (guard.X >= 0 && guard.Y >= 0 && guard.X < grid.width && guard.Y < grid.height && cycle_count < grid.height * grid.width)
            {
                if (grid.grid[guard.Y][guard.X] == '#')
                {
                    guard.X -= guard.dx;
                    guard.Y -= guard.dy;
                    int temp = guard.dx;
                    guard.dx = -guard.dy;
                    guard.dy = temp;
                }

                guard.X += guard.dx;
                guard.Y += guard.dy;
                cycle_count++;
            }

            grid.grid[i][j] = '.';
            if (cycle_count == grid.height * grid.width)
            {
                solution++;
            }
        }
    }

    printf("Part2 Solution: %d\n", solution);

    destroy_grid(grid);
    if (buffer)
        free(buffer);

    return EXIT_SUCCESS;
}
