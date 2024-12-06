#ifndef AOC_H_
#define AOC_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define GREEN "\033[42m"
#define RED "\033[41m"
#define BOLD "\033[47m"
#define NORMAL "\033[0m"

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

int int_compare(const void *a, const void *b);
char *read_file(const char *filename);
int count_lines(const char *filename);
Grid_t make_grid(const char *buffer, int lines);
void destroy_grid(Grid_t grid);

#endif // AOC_H_

#ifdef AOC_UTILS_IMPLEMENTATION

int int_compare(const void *a, const void *b)
{
    return (*(int *)a - *(int *)b);
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
        memcpy(grid[i], &buffer[i * output.height + i], (output.width + 1) * sizeof(char));
        grid[i][output.width] = '\0';
    }
end:
    output.grid = grid;
    return output;
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

#endif // AOC_UTILS_IMPLEMENTATION
