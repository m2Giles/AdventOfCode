#define AOC_UTILS_IMPLEMENTATION
#include "../../../lib/aoc.h"

#define MAXPRESS 100

typedef struct
{
    uint64_t x, y;
} V;

typedef struct
{
    V a, b, p;
} Claw;

typedef struct
{
    Claw *items;
    int capacity;
    int count;
} da_claw;

typedef struct
{
    char **items;
    int capacity;
    int count;
} da_games;

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
    da_claw claws = {0};
    da_games games = {0};
    char tdelim = '|';
    char *tbuf = buffer;
    while ((tbuf = strstr(tbuf, "\n\n")) != NULL)
    {
        *tbuf = tdelim;
        tbuf++;
    }

    char *game = strtok(buffer, "|");
    while (game)
    {
        aoc_da_append(&games, game);
        game = strtok(NULL, "|");
    }

    for (int i = 0; i < games.count; i++)
    {
        Claw temp = {0};
        sscanf(games.items[i], " Button A: X+%lu, Y+%lu Button B: X+%lu, Y+%lu Prize: X=%lu, Y=%lu ", &temp.a.x, &temp.a.y, &temp.b.x, &temp.b.y, &temp.p.x, &temp.p.y);
        aoc_da_append(&claws, temp);
    }

    for (int i = 0; i < claws.count; i++)
    {
        const Claw *claw = &claws.items[i];
        int64_t det = claw->a.x * claw->b.y - claw->b.x * claw->a.y;

        lldiv_t A = lldiv(claw->b.y * claw->p.x - claw->b.x * claw->p.y, det);
        lldiv_t B = lldiv(claw->a.x * claw->p.y - claw->a.y * claw->p.x, det);
        if (!A.rem && !A.rem)
        {
            solution += 3 * A.quot + B.quot;
        }
    }

    clock_t toc = clock();
    printf("Part1 Solution: %ld\n", solution);
    printf("Part1 Time: %.03f ms\n", (double)(toc - tic) / CLOCKS_PER_SEC * 1000);

    // Part 2
    solution = 0;
    tic = clock();

    for (int i = 0; i < claws.count; i++)
    {
        Claw *claw = &claws.items[i];
        claw->p.x += 10000000000000;
        claw->p.y += 10000000000000;
        int64_t det = claw->a.x * claw->b.y - claw->b.x * claw->a.y;

        lldiv_t A = lldiv(claw->b.y * claw->p.x - claw->b.x * claw->p.y, det);
        lldiv_t B = lldiv(claw->a.x * claw->p.y - claw->a.y * claw->p.x, det);
        if (!A.rem && !B.rem)
        {
            solution += 3 * A.quot + B.quot;
        }
    }

    toc = clock();
    printf("Part2 Solution: %ld\n", solution);
    printf("Part2 Time: %.03f ms\n", (double)(toc - tic) / CLOCKS_PER_SEC * 1000);

    aoc_da_free(claws);
    aoc_da_free(games);
    free(buffer_t.buffer);

    return EXIT_SUCCESS;
}
