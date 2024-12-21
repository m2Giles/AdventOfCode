#define AOC_UTILS_IMPLEMENTATION
#include "aoc.h"

typedef struct
{
    char **items;
    int capacity;
    int count;
} da;

static size_t check(da *towels, char *pattern, int idx, ssize_t *cache)
{
    if (cache[idx] != -1)
        return cache[idx];

    char *subString = pattern + idx;
    int patternLen = strlen(subString);

    if (!patternLen)
        return 1;

    size_t score = 0;
    for (int i = 0; i < towels->count; i++)
    {
        int towelLen = strlen(towels->items[i]);
        if (towelLen <= patternLen && strncmp(subString, towels->items[i], towelLen) == 0)
            score += check(towels, pattern, idx + towelLen, cache);
    }

    cache[idx] = score;
    return score;
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
    Parts_t puzzle = puzzle_split(buffer);
    size_t solution2 = 0;
    da towels = {0};
    da patterns = {0};

    char *tmp = strtok(puzzle.part1, ", \n");
    while (tmp)
    {
        aoc_da_append(&towels, tmp);
        tmp = strtok(NULL, ", \n");
    }

    tmp = strtok(puzzle.part2, "\n");
    while (tmp)
    {
        aoc_da_append(&patterns, tmp);
        tmp = strtok(NULL, "\n");
    }

    for (int i = 0; i < patterns.count; i++)
    {
        ssize_t cache[strlen(patterns.items[i]) + 1];
        memset(cache, -1, sizeof(cache));
        size_t result = check(&towels, patterns.items[i], 0, cache);
        solution += !!result;
        solution2 += result;
    }

    clock_t toc = clock();
    printf("Part1 Solution: %ld\n", solution);
    printf("Part2 Solution: %ld\n", solution2);
    printf("Total Time: %.03f ms\n",
           (double)(toc - tic) / CLOCKS_PER_SEC * 1000);

    free_puzzle_split(puzzle);
    aoc_da_free(towels);
    aoc_da_free(patterns);
    free(buffer_t.buffer);

    return EXIT_SUCCESS;
}
