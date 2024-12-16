#ifndef AOC_H_
#define AOC_H_

#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define AOC_ARRAY_H_IMPLEMENTATION_
#include "aoc_array.h"

#define GREEN "\033[42m"
#define RED "\033[41m"
#define BOLD "\033[01m"
#define NORMAL "\033[0m"

#define MOD(a, b)                                                              \
    ({                                                                         \
        typeof(a) _a = (a);                                                    \
        typeof(b) _b = (b);                                                    \
        assert(_b != 0 && "Divide by Zero");                                   \
        (_a % _b + _b) % b;                                                    \
    })

enum
{
    NORTH = 0,
    EAST = 1,
    SOUTH = 2,
    WEST = 3
};

typedef struct
{
    int x, y;
} pos;

const pos dirs[4] = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}};

typedef struct
{
    char **grid;
    int height;
    int width;
} Grid_t;

typedef struct
{
    char *part1;
    int part1_len;
    int part1_lines;
    char *part2;
    int part2_len;
    int part2_lines;
} Parts_t;

typedef struct
{
    char *buffer;
    int count;
    int lines;
} Buffer_t;

Buffer_t read_file_buffer(const char *filename);
char *sgets(char *str, int n, char **input);
int int_compare(const void *a, const void *b);
char *read_file(const char *filename);
int count_lines(const char *filename);
Grid_t make_grid(const char *buffer, int lines);
void print_grid(Grid_t grid);
void destroy_grid(Grid_t grid);
pos next(pos start, pos direction);

#endif // AOC_H_

#ifdef AOC_UTILS_IMPLEMENTATION

pos next(pos start, pos direction)
{
    return (pos){.x = start.x + direction.x, .y = start.y + direction.y};
}

int int_compare(const void *a, const void *b)
{
    int A = *(int *)a;
    int B = *(int *)b;
    return (A > B) - (B > A);
}

int uint64_compare(const void *a, const void *b)
{
    uint64_t A = *(uint64_t *)a;
    uint64_t B = *(uint64_t *)b;
    return (A > B) - (B > A);
}

int count_lines(const char *filename)
{
    FILE *fp;
    int lines = 0;

    fp = fopen(filename, "r");
    if (!fp)
    {
        perror("fopen failed");
        goto defer;
    }

    while (!feof(fp))
    {
        char c = fgetc(fp);
        if (c == '\n')
            lines++;
    }

defer:
    if (fp)
        fclose(fp);
    return lines;
}

char *read_file(const char *filename)
{
    FILE *fp;
    char *buffer = NULL;
    int file_size;

    fp = fopen(filename, "r");
    if (!fp)
    {
        perror("fopen failed");
        goto defer;
    }

    fseek(fp, 0, SEEK_END);
    file_size = ftell(fp);
    rewind(fp);

    buffer = (char *)malloc(file_size + 1);
    if (!buffer)
    {
        perror("Memory allocation error");
        goto defer;
    }

    fread(buffer, file_size, 1, fp);
    buffer[file_size] = '\0';

defer:
    if (fp)
        fclose(fp);
    return buffer;
}

Buffer_t read_file_buffer(const char *filename)
{
    Buffer_t buffer_t = {0};
    FILE *fp = NULL;
    fp = fopen(filename, "r");
    if (!fp)
    {
        perror("fopen failed");
        goto defer;
    }

    fseek(fp, 0, SEEK_END);
    buffer_t.count = ftell(fp);
    rewind(fp);

    buffer_t.buffer = (char *)malloc(buffer_t.count + 1);
    if (!buffer_t.buffer)
    {
        perror("Memory allocation error");
        goto defer;
    }

    fread(buffer_t.buffer, buffer_t.count, 1, fp);
    buffer_t.buffer[buffer_t.count] = '\0';

    rewind(fp);
    while (!feof(fp))
    {
        char c = fgetc(fp);
        if (c == '\n')
            buffer_t.lines++;
    }

defer:
    if (fp)
        fclose(fp);
    return buffer_t;
}

Grid_t make_grid(const char *buffer, int lines)
{
    Grid_t output = {0};
    output.height = lines;
    while (buffer[output.width] != '\n')
    {
        output.width++;
    }

    // Allocate and Populate Grid
    char **grid = (char **)malloc((output.height) * sizeof(char *));
    if (!grid)
    {
        perror("Failed to allocate memory...");
        output.grid = NULL;
        goto end;
    }
    for (int i = 0; i < output.height; ++i)
    {
        grid[i] = (char *)malloc((output.width + 1) * sizeof(char));
        if (!grid[i])
        {
            perror("Failed to allocate memory...");
            output.grid = NULL;
            goto end;
        }
        memcpy(grid[i], &buffer[i * output.height + i],
               (output.width + 1) * sizeof(char));
        grid[i][output.width] = '\0';
    }
end:
    output.grid = grid;
    return output;
}

void print_grid(Grid_t grid)
{
    for (int i = 0; i < grid.height; ++i)
    {
        for (int j = 0; j < grid.width; ++j)
        {
            printf("%c", grid.grid[i][j]);
        }
        printf("\n");
    }
}

void destroy_grid(Grid_t grid)
{
    for (int i = 0; i < grid.height; ++i)
    {
        free(grid.grid[i]);
    }
    free(grid.grid);
    grid.grid = NULL;
}

Parts_t puzzle_split(const char *buffer)
{
    Parts_t puzzle = {0};
    char *split = strstr(buffer, "\n\n");
    split++;
    puzzle.part1_len = split - buffer;
    puzzle.part1 = malloc(sizeof(char) * (puzzle.part1_len));
    if (!puzzle.part1)
        return puzzle;
    strncpy(puzzle.part1, buffer, split - buffer);
    puzzle.part1[puzzle.part1_len] = '\0';
    split++;

    for (int i = 0; puzzle.part1[i] != '\0'; ++i)
    {
        if (puzzle.part1[i] == '\n')
            puzzle.part1_lines++;
    }

    while (buffer[split - buffer + puzzle.part2_len] != '\0')
    {
        puzzle.part2_len++;
    }
    puzzle.part2_len++;
    puzzle.part2 = malloc(sizeof(char) * (puzzle.part2_len));
    if (!puzzle.part2)
    {
        free(puzzle.part1);
        puzzle.part1 = NULL;
        return puzzle;
    }
    strncpy(puzzle.part2, &buffer[puzzle.part1_len + 1], puzzle.part2_len);
    puzzle.part2[puzzle.part2_len] = '\0';

    for (int i = 0; puzzle.part2[i] != '\0'; ++i)
    {
        if (puzzle.part2[i] == '\n')
            puzzle.part2_lines++;
    }
    return puzzle;
}

void free_puzzle_split(Parts_t puzzle)
{
    free(puzzle.part1);
    free(puzzle.part2);
}

char *sgets(char *str, int num, char **input)
{
    char *next = *input;
    int nread = 0;

    while (nread + 1 < num && *next)
    {
        int isnewline = (*next == '\n');
        *str++ = *next++;
        nread++;
        if (isnewline)
            break;
    }

    if (nread == 0)
        return NULL; // "eof"

    *str = '\0';
    // set up input for next call
    *input = next;
    return str;
}

#endif // AOC_UTILS_IMPLEMENTATION
