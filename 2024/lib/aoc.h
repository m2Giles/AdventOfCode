#ifndef AOC_H_
#define AOC_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int int_compare(const void *a, const void *b);
char *read_file(const char *filename);
size_t count_lines(const char *filename);

#endif // AOC_H_

#ifdef AOC_UTILS_IMPLEMENTATION
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int int_compare(const void *a, const void *b)
{
    return (*(int *)a - *(int *)b);
}

size_t count_lines(const char *filename)
{
    FILE *fp;
    size_t lines = 0;

    fp = fopen(filename, "r");
    if (!fp)
    {
        perror("Error opening file.");
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
    size_t file_size;

    fp = fopen(filename, "r");
    if (!fp)
    {
        perror("Error opening file.");
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

#endif // AOC_UTILS_IMPLEMENTATION