#define AOC_UTILS_IMPLEMENTATION
#include "../../../lib/aoc.h"

#define WIDTH 71
#define HEIGHT 71
#define SIZE 1024
#define END 70

typedef struct
{
    pos *items;
    int count;
    int capacity;
} da;

typedef struct
{
    pos pos;
    int dir;
    size_t score;
} step;

typedef struct
{
    step *items;
    int count;
    int capacity;
} Steps;

bool bounds(pos pos)
{
    return (pos.x >= 0 && pos.x < WIDTH && pos.y >= 0 && pos.y < HEIGHT);
}

ssize_t dijsktra(char grid[HEIGHT][WIDTH], pos start, pos end)
{
    ssize_t out = 0;
    ssize_t Distance[HEIGHT][WIDTH] = {0};
    memset(Distance, -1, sizeof(Distance));
    step bestStep = {.pos = start, .dir = 2, .score = 0};
    Steps steps = {0};
    aoc_da_append(&steps, bestStep);
    while (steps.count > 0)
    {
        int best = 0;
        for (int i = 0; i < steps.count; i++)
            if (steps.items[i].score < steps.items[best].score)
                best = i;
        bestStep = steps.items[best];
        aoc_da_del(&steps, best);
        step new;
        out = bestStep.score;
        if (bestStep.pos.y == end.y && bestStep.pos.x == end.x)
            break;
        if (grid[bestStep.pos.y][bestStep.pos.x] == '#')
            continue;
        if (bestStep.score >= Distance[bestStep.pos.y][bestStep.pos.x])
            continue;
        if (Distance[bestStep.pos.y][bestStep.pos.x] != -1)
            continue;
        Distance[bestStep.pos.y][bestStep.pos.x] = bestStep.score;
        pos nextPos = next(bestStep.pos, dirs[MOD(bestStep.dir + 1, 4)]);
        int append = 0;
        if (bounds(nextPos) && grid[nextPos.y][nextPos.x] != '#')
        {
            new = (step){.pos = nextPos,
                         .dir = MOD(bestStep.dir + 1, 4),
                         .score = out + 1};
            aoc_da_append(&steps, new);
            append++;
        }
        nextPos = next(bestStep.pos, dirs[MOD(bestStep.dir - 1, 4)]);
        if (bounds(nextPos) && grid[nextPos.y][nextPos.x] != '#')
        {
            new = (step){.pos = nextPos,
                         .dir = MOD(bestStep.dir - 1, 4),
                         .score = out + 1};
            aoc_da_append(&steps, new);
        }
        nextPos = next(bestStep.pos, dirs[bestStep.dir]);
        if (bounds(nextPos) && grid[nextPos.y][nextPos.x] != '#')
        {
            new = (step){.pos = nextPos, .dir = bestStep.dir, .score = out + 1};
            aoc_da_append(&steps, new);
        }
    }
    aoc_da_free(steps);
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
    da bytes = {0};

    char **cursor = &buffer;
    char buf[25] = {0};
    int a, b;
    while (sgets(buf, 25, cursor))
    {
        sscanf(buf, " %d,%d ", &a, &b);
        pos new = {.x = a, .y = b};
        aoc_da_append(&bytes, new);
    }

    char grid[HEIGHT][WIDTH];
    memset(grid, '.', sizeof(grid));
    for (int i = 0; i < SIZE && i < bytes.count; i++)
    {
        a = bytes.items[i].x;
        b = bytes.items[i].y;
        grid[b][a] = '#';
    }

    pos start = {0, 0};
    pos end = {END, END};

    solution = dijsktra(grid, start, end);

    clock_t toc = clock();
    printf("Part1 Solution: %ld\n", solution);
    printf("Part1 Time: %.03f ms\n",
           (double)(toc - tic) / CLOCKS_PER_SEC * 1000);

    // Part 2
    size_t keep = solution;
    solution = 0;
    tic = clock();

    int min = SIZE;
    int max = bytes.count;
    while (min <= max)
    {
        int mid = min + (max - min) / 2;
        memset(grid, '.', sizeof(grid));
        for (int i = 0; i < mid && i < bytes.count; i++)
        {
            a = bytes.items[i].x;
            b = bytes.items[i].y;
            grid[b][a] = '#';
        }
        if (dijsktra(grid, start, end) < keep)
        {
            max = mid - 1;
        }
        else
        {
            min = mid + 1;
        }
        solution = mid;
    }

    toc = clock();
    printf("Part2 Solution: %d,%d\n", bytes.items[solution].x,
           bytes.items[solution].y);
    printf("Part2 Time: %.03f ms\n",
           (double)(toc - tic) / CLOCKS_PER_SEC * 1000);

    aoc_da_free(bytes);
    free(buffer_t.buffer);

    return EXIT_SUCCESS;
}
