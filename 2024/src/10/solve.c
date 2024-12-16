#define AOC_UTILS_IMPLEMENTATION
#include "../../../lib/aoc.h"
#include <time.h>
#include <assert.h>

typedef struct
{
    int x;
    int y;
    int trails;
} pos_t;

typedef struct
{
    pos_t *items;
    int count;
    int capacity;
} da;

void da_append(da *array, pos_t item)
{
    if (array->capacity == 0)
    {
        array->count = 0;
        array->capacity = 4;
        array->items = malloc(sizeof(pos_t) * array->capacity);
    }
    if (array->count + 1 > array->capacity)
    {
        array->capacity <<= 1;
        array->items = realloc(array->items, sizeof(pos_t) * array->capacity);
        assert(array->items != NULL && "Failed to realloc memory!");
    }
    array->items[array->count++] = item;
}

int find_trail(pos_t start, char elevation, da *trails, Grid_t grid, bool part)
{
    if (elevation == '9')
    {
        for (int i = 0; i < trails->count; i++)
        {
            if (start.x == trails->items[i].x && start.y == trails->items[i].y && part == 0)
            {
                return 0;
            }
        }
        da_append(trails, start);
        return 1;
    }

    int output = 0;

    for (int x = -1; x < 2; x++)
        for (int y = -1; y < 2; y++)
            if(abs(x + y) == 1)
            {
                if (start.y + y > -1 && start.x + x > -1 && start.x + x < grid.width && start.y + y < grid.height && grid.grid[start.y + y][start.x + x] == elevation + 1)
                {
                    pos_t new = {.x = start.x + x, .y = start.y + y, .trails = 1};
                    output += find_trail(new, elevation + 1, trails, grid, part);
                }
            }
    return output;
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

    Grid_t grid = make_grid(buffer, lines);

    da trailheads = {0};

    for (int i = 0; i < grid.height; i++)
        for (int j = 0; j < grid.width; j++)
            if (grid.grid[i][j] == '0')
            {
                pos_t temp = {.x = j, .y = i};
                da trails = {0};
                da_append(&trailheads, temp);
                trailheads.items[trailheads.count - 1].trails = find_trail(temp, '0', &trails, grid, 0);
                free(trails.items);
                trails = (da) {0};
            }

    for (int i = 0; i < trailheads.count; i++)
    {
        solution += trailheads.items[i].trails;
    }

    free(trailheads.items);
    trailheads = (da) {0};

    clock_t toc = clock();
    printf("Part1 Solution: %ld\n", solution);
    printf("Part1 Time: %.03f ms\n", (double)(toc - tic) / CLOCKS_PER_SEC * 1000);

    // Part 2
    tic = clock();
    solution = 0;

    trailheads = (da) {0};

    for (int i = 0; i < grid.height; i++)
        for (int j = 0; j < grid.width; j++)
            if (grid.grid[i][j] == '0')
            {
                pos_t temp = {.x = j, .y = i};
                da trails = {0};
                da_append(&trailheads, temp);
                trailheads.items[trailheads.count - 1].trails = find_trail(temp, '0', &trails, grid, 1);
                free(trails.items);
                trails = (da) {0};
            }

    for (int i = 0; i < trailheads.count; i++)
    {
        solution += trailheads.items[i].trails;
    }

    free(trailheads.items);
    trailheads = (da) {0};

    toc = clock();
    printf("Part2 Solution: %ld\n", solution);
    printf("Part2 Time: %.03f ms\n", (double)(toc - tic) / CLOCKS_PER_SEC * 1000);

    free(buffer_t.buffer);

    return EXIT_SUCCESS;
}
