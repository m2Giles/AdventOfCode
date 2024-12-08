#define AOC_UTILS_IMPLEMENTATION
#include "../../../lib/aoc.h"
#include <time.h>

typedef struct
{
    char c;
    int x;
    int y;
} pos_t;

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
    char * buffer = buffer_t.buffer;
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
    clock_t toc = clock();
    Grid_t grid = make_grid(buffer, lines);
    bool antinodes[grid.height][grid.width];
    memset(antinodes, 0, sizeof(antinodes));

    pos_t antennas[grid.width * grid.height];
    int antenna_count = 0;


    for (int i = 0; i < grid.height; ++i)
    {
        for (int j = 0; j < grid.width; ++j)
        {
            if(isalnum(grid.grid[i][j]))
            {
                antennas[antenna_count++] = (pos_t){.c = grid.grid[i][j], .x = j, .y = i};
            }
        }
    }

    for (int i = 0; i < antenna_count; ++i)
    {
        for (int j = 0; j < antenna_count; ++j)
        {
            int dx = antennas[i].x - antennas[j].x;
            int dy = antennas[i].y - antennas[j].y;

            if (dx == 0 && dy == 0)
                continue;

            if (antennas[i].c == antennas[j].c)
            {
                if(antennas[i].x + dx >= 0 && antennas[i].x + dx < grid.width && antennas[i].y + dy >= 0 && antennas[i].y + dy < grid.height)
                {
                    if ( grid.grid[antennas[i].y + dy][antennas[i].x + dx] != antennas[i].c )
                    {
                        antinodes[antennas[i].y + dy][antennas[i].x + dx] = true;
                    }
                }
            }
        }
    }

    for (int i = 0; i < grid.height; ++i)
    {
        for (int j = 0; j < grid.width; ++j)
        {
            solution += antinodes[i][j];
            if (antinodes[i][j] && grid.grid[i][j] == '.')
                grid.grid[i][j] = '#';
        }
    }

    print_grid(grid);

    printf("Part1 Solution: %ld\n", solution);
    printf("Part1 Time: %.03f ms\n", (double)(toc - tic) / CLOCKS_PER_SEC * 1000);

    // Part 2
    tic = clock();
    solution = 0;
    destroy_grid(grid);
    grid = make_grid(buffer, lines);

    for (int i = 0; i < antenna_count; ++i)
    {
        for (int j = 0; j < antenna_count; ++j)
        {
            if (antennas[i].c == antennas[j].c)
            {
            int dx = antennas[i].x - antennas[j].x;
            int dy = antennas[i].y - antennas[j].y;
            if (dx == 0 && dy == 0)
            {
                antinodes[antennas[i].y][antennas[i].x] = true;
                continue;
            }

                int h = 1;
                while(antennas[i].x + dx*h >= 0 && antennas[i].x + dx*h < grid.width && antennas[i].y + dy*h >= 0 && antennas[i].y + dy*h < grid.height)
                {
                    antinodes[antennas[i].y + dy*h][antennas[i].x + dx*h] = true;
                    h++;
                }
            }
        }
    }

    for (int i = 0; i < grid.height; ++i)
    {
        for (int j = 0; j < grid.width; ++j)
        {
            solution += antinodes[i][j];
            if (antinodes[i][j] && grid.grid[i][j] == '.')
                grid.grid[i][j] = '#';
        }
    }

    print_grid(grid);

    toc = clock();
    printf("Part2 Solution: %ld\n", solution);
    printf("Part2 Time: %.03f ms\n", (double)(toc - tic) / CLOCKS_PER_SEC * 1000);

    free(buffer_t.buffer);

    return EXIT_SUCCESS;
}
