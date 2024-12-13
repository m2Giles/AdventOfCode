#define AOC_UTILS_IMPLEMENTATION
#include "../../../lib/aoc.h"

#define BLINKS 75

typedef struct
{
    uint64_t values;
    uint64_t stones;
} cache;

// How to do this without global variables?
cache caches[BLINKS][BUFSIZ];
uint32_t blink_cache[BLINKS] = {0};
 
typedef struct
{
    size_t *items;
    int count;
    int capacity;
} da;

uint8_t digits(uint64_t value)
{
    uint8_t num_digits = 1;
    uint64_t mag = 10;
    while (value / mag)
    {
        mag *= 10;
        num_digits++;
    }
    return num_digits;
}

uint64_t blink(uint64_t value, uint32_t iterations)
{
    if (iterations == 0)
    {
        return 1;
    }

    for (uint64_t i = 0; i < blink_cache[iterations - 1]; i++)
    {
        if (caches[iterations - 1][i].values == value)
            return caches[iterations - 1][i].stones;
    }

    uint64_t num = 0;
    uint64_t stones = 0;

    if (value == 0)
    {
        stones = blink(1, iterations - 1);
        caches[iterations - 1][blink_cache[iterations - 1]++] = (cache){value, stones};
    }
    else if ((num = digits(value)) % 2 == 0)
    {
        uint64_t half_num = num / 2;
        uint64_t mag = 1;
        for (uint64_t i = 0; i < half_num; i++)
        {
            mag *= 10;
        }
        stones = blink(value / mag, iterations - 1) + blink(value % mag, iterations - 1);
        caches[iterations - 1][blink_cache[iterations - 1]++] = (cache){value, stones};
    }
    else
    {
        stones = blink(value * 2024, iterations - 1);
        caches[iterations - 1][blink_cache[iterations - 1]++] = (cache){value, stones};
    }
    return stones;
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
        fprintf(stderr, RED "%s is empty\n" NORMAL, filename);
        free(buffer);
        return EXIT_FAILURE;
    }
    if (!lines)
    {
        fprintf(stderr, "%s does not have a trailing newline\n", filename);
    }

    // Part 1
    clock_t tic = clock();
    char *stone = strtok(buffer, " \n");
    da stones = {0};
    while (stone)
    {
        aoc_da_append(&stones, strtol(stone, NULL, 10));
        stone = strtok(NULL, " ");
    }

    for (int i = 0; i < stones.count; i++)
    {
        solution += blink(stones.items[i], 25);
    }

    clock_t toc = clock();
    printf("Part1 Solution: %ld\n", solution);
    printf("Part1 Time: %.03f ms\n", (double)(toc - tic) / CLOCKS_PER_SEC * 1000);

    // Part 2
    solution = 0;
    tic = clock();

    for (int i = 0; i < stones.count; i++)
    {
        solution += blink(stones.items[i], 75);
    }

    toc = clock();
    printf("Part2 Solution: %ld\n", solution);
    printf("Part2 Time: %.03f ms\n", (double)(toc - tic) / CLOCKS_PER_SEC * 1000);

    aoc_da_free(stones);
    free(buffer_t.buffer);

    return EXIT_SUCCESS;
}
