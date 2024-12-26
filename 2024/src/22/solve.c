#include <stdint.h>
#define AOC_UTILS_IMPLEMENTATION
#include "aoc.h"

#define ITERATIONS 2000
#define SPACE 19 * 19 * 19 * 19
#define PRUNE ((1 << 24) - 1)

size_t newSecret(size_t input)
{
    size_t out = input;
    out ^= out << 6 & PRUNE;
    out ^= out >> 5 & PRUNE;
    out ^= out << 11 & PRUNE;
    return out;
}

int convert(int a, int b, int c, int d)
{
    return a * 19 * 19 * 19 + b * 19 * 19 + c * 19 + d;
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("Missing file\n");
        return EXIT_FAILURE;
    }

    char *filename = argv[1];

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
    size_t part1 = 0;
    size_t part2 = 0;

    char **cursor = &buffer;
    char buf[100] = {0};

    size_t secret;
    int seqs[SPACE] = {0};

    while (sgets(buf, 100, cursor))
    {
        sscanf(buf, "%ld ", &secret);
        char prev = secret % 10;
        char a = -1, b = -1, c = -1, d = -1;
        char seen[19 * 19 * 19 * 19] = {0};
        for (int i = 0; i < ITERATIONS; i++)
        {
            secret = newSecret(secret);
            a = b;
            b = c;
            c = d;
            d = secret % 10 - prev + 9;
            int idx = convert(a, b, c, d);
            if (a != -1 && !seen[idx])
            {
                seen[idx] = true;
                seqs[idx] += secret % 10;
            }
            prev = secret % 10;
        }
        // Part 1
        part1 += secret;
    }

    for (int i = 0; i < SPACE; i++)
        if (seqs[i] > part2)
            part2 = seqs[i];

    clock_t toc = clock();
    printf("Part1 Solution: %ld\n", part1);
    printf("Part2 Solution: %ld\n", part2);
    printf("Time: %.03f ms\n", (double)(toc - tic) / CLOCKS_PER_SEC * 1000);

    free(buffer_t.buffer);

    return EXIT_SUCCESS;
}
