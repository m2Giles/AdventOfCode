#define AOC_UTILS_IMPLEMENTATION
#include "aoc.h"

#define MINSAVE 100

typedef struct
{
    pos position;
    int save;
} Cheat;

typedef struct
{
    Cheat *items;
    int count;
    int capacity;
} da;

int manDistance(pos a, pos b) { return abs(a.x - b.x) + abs(a.y - b.y); }

bool bounds(pos a, const Grid_t *grid)
{
    return a.x >= 0 && a.x < grid->width && a.y >= 0 && a.y < grid->height;
}

void findPath(const pos a, const Grid_t grid, int path[grid.height][grid.width])
{
    for (int i = 0; i < 4; i++)
    {
        pos new = next(a, dirs[i]);
        if (bounds(new, &grid) && grid.grid[new.y][new.x] != '#' &&
            path[new.y][new.x] == -1 && path[a.y][a.x] + 1 > path[new.y][new.x])
        {
            path[new.y][new.x] = path[a.y][a.x] + 1;
            findPath(new, grid, path);
        }
    }
}

size_t cheat(int maxcheat, pos cursor, const pos end, const Grid_t grid,
             const int path[grid.height][grid.width])
{
    size_t out = 0;
    int d;

    while (cursor.x != end.x || cursor.y != end.y)
    {
        for (int i = -maxcheat; i <= maxcheat; i++)
        {
            for (int j = -maxcheat; j <= maxcheat; j++)
            {
                pos cheat = {.x = cursor.x + j, .y = cursor.y + i};
                if ((d = manDistance(cursor, cheat)) > maxcheat)
                    continue;
                if (!bounds(cheat, &grid))
                    continue;
                if (grid.grid[cheat.y][cheat.x] == '#')
                    continue;
                if (path[cheat.y][cheat.x] - path[cursor.y][cursor.x] - d <
                    MINSAVE)
                    continue;

                out++;
            }
        }
        for (int i = 0; i < 4; i++)
        {
            pos new = next(cursor, dirs[i]);
            if (path[cursor.y][cursor.x] + 1 == path[new.y][new.x])
            {
                cursor = new;
                break;
            }
        }
    }
    return out;
}

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
    char *buffer = buffer_t.buffer;
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
    Grid_t grid = make_grid(buffer, lines);

    int path[grid.height][grid.width];
    memset(path, -1, sizeof(path));

    pos start, end;

    for (int i = 0; i < grid.height; i++)
        for (int j = 0; j < grid.width; j++)
        {
            if (grid.grid[i][j] == 'S')
                start = (pos){.x = j, .y = i};
            if (grid.grid[i][j] == 'E')
                end = (pos){.x = j, .y = i};
        }

    path[start.y][start.x] = 0;
    findPath(start, grid, path);

    solution = cheat(2, start, end, grid, path);

    clock_t toc = clock();
    printf("Part1 Solution: %ld\n", solution);
    printf("Part1 Time: %.03f ms\n",
           (double)(toc - tic) / CLOCKS_PER_SEC * 1000);

    // Part 2
    solution = 0;
    tic = clock();

    solution = cheat(20, start, end, grid, path);

    toc = clock();
    printf("Part2 Solution: %ld\n", solution);
    printf("Part2 Time: %.03f ms\n",
           (double)(toc - tic) / CLOCKS_PER_SEC * 1000);

    destroy_grid(grid);
    free(buffer_t.buffer);

    return EXIT_SUCCESS;
}
