#include <stdio.h>
#define AOC_UTILS_IMPLEMENTATION
#include "../../../lib/aoc.h"

typedef struct
{
    pos pos;
    int dir;
    size_t score;
} step_t;

typedef struct
{
    step_t *items;
    int count;
    int capacity;
} Steps;

bool markSeats(step_t step, size_t target, Grid_t grid,
               ssize_t Distances[grid.height][grid.width][4],
               bool seats[grid.height][grid.width])
{
    int x = step.pos.x;
    int y = step.pos.y;
    int dir = step.dir;
    if (step.score == target && grid.grid[y][x] == 'E')
    {
        seats[y][x] = true;
        return true;
    }
    if (step.score >= target)
        return false;
    if (grid.grid[y][x] == '#')
        return false;
    if (step.score > Distances[y][x][dir])
        return false;

    Distances[y][x][dir] = step.score;
    step_t new = (step_t){
        .dir = dir, .pos = next(step.pos, dirs[dir]), .score = step.score + 1};
    bool best = markSeats(new, target, grid, Distances, seats);

    new = (step_t){
        .dir = MOD(dir + 1, 4), .pos = step.pos, .score = step.score + 1000};
    if (markSeats(new, target, grid, Distances, seats))
        best = true;

    new = (step_t){
        .dir = MOD(dir - 1, 4), .pos = step.pos, .score = step.score + 1000};
    if (markSeats(new, target, grid, Distances, seats))
        best = true;

    if (best)
        seats[y][x] = true;

    return best;
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

    Steps steps = {0};
    pos start = {0};
    Grid_t grid = make_grid(buffer, lines);

    ssize_t Distance[grid.height][grid.width][4];

    for (int i = 0; i < grid.height; i++)
        for (int j = 0; j < grid.width; j++)
        {
            for (int k = 0; k < 4; k++)
                Distance[i][j][k] = -1;
            if (grid.grid[i][j] == 'S')
                start = (pos){.x = j, .y = i};
        }

    step_t bestStep = {.pos = start, .dir = EAST, .score = 0};
    aoc_da_append(&steps, bestStep);

    while (steps.count != 0)
    {
        int best = 0;
        for (int i = 0; i < steps.count; i++)
        {
            if (steps.items[i].score < steps.items[best].score)
                best = i;
        }
        bestStep = steps.items[best];
        aoc_da_del(&steps, best);
        step_t new;
        solution = bestStep.score;
        if (grid.grid[bestStep.pos.y][bestStep.pos.x] == 'E')
            break;
        if (grid.grid[bestStep.pos.y][bestStep.pos.x] == '#')
            continue;
        if (bestStep.score >=
            Distance[bestStep.pos.y][bestStep.pos.x][bestStep.dir])
            continue;
        if (Distance[bestStep.pos.y][bestStep.pos.x][bestStep.dir] != -1)
            continue;
        Distance[bestStep.pos.y][bestStep.pos.x][bestStep.dir] = bestStep.score;
        pos nextPos = next(bestStep.pos, dirs[MOD(bestStep.dir + 1, 4)]);
        if (grid.grid[nextPos.y][nextPos.x] != '#')
            new = (step_t){.pos = nextPos,
                           .dir = MOD(bestStep.dir + 1, 4),
                           .score = solution + 1001};
        aoc_da_append(&steps, new);
        nextPos = next(bestStep.pos, dirs[MOD(bestStep.dir - 1, 4)]);
        if (grid.grid[nextPos.y][nextPos.x] != '#')
            new = (step_t){.pos = nextPos,
                           .dir = MOD(bestStep.dir - 1, 4),
                           .score = solution + 1001};
        aoc_da_append(&steps, new);
        new = (step_t){.pos = next(bestStep.pos, dirs[bestStep.dir]),
                       .dir = bestStep.dir,
                       .score = solution + 1};
        aoc_da_append(&steps, new);
    }

    clock_t toc = clock();
    printf("Part1 Solution: %ld\n", solution);
    printf("Part1 Time: %.03f ms\n",
           (double)(toc - tic) / CLOCKS_PER_SEC * 1000);

    // Part 2
    size_t score = solution;
    solution = 0;
    tic = clock();

    bool seats[grid.height][grid.width];
    memset(seats, 0, sizeof(seats));

    bestStep = (step_t){.pos = start, .dir = EAST, .score = 0};
    markSeats(bestStep, score, grid, Distance, seats);

    for (int i = 0; i < grid.height; i++)
        for (int j = 0; j < grid.width; j++)
            if (seats[i][j])
                solution++;

    toc = clock();
    printf("Part2 Solution: %ld\n", solution);
    printf("Part2 Time: %.03f ms\n",
           (double)(toc - tic) / CLOCKS_PER_SEC * 1000);

    aoc_da_free(steps);
    destroy_grid(grid);
    free(buffer_t.buffer);

    return EXIT_SUCCESS;
}
