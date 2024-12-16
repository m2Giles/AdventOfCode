#define AOC_UTILS_IMPLEMENTATION
#include "../../../lib/aoc.h"

// Puzzle Input
#define WIDTH 101
#define HEIGHT 103

typedef struct
{
    int x, y, dx, dy;
} pos_t;

typedef struct
{
    pos_t *items;
    int count;
    int capacity;
} da;

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
    char ptemp[100] = {0};
    da robots = {0};
    pos_t robot = {0};
    char **parse = &buffer;

    while (sgets(ptemp, 100, parse) != NULL)
    {
        sscanf(ptemp, " p=%d,%d v=%d,%d ", &robot.x, &robot.y, &robot.dx,
               &robot.dy);
        aoc_da_append(&robots, robot);
    }

    int q[4] = {0};

    for (int i = 0; i < robots.count; i++)
    {
        int y = 0;
        int x = 0;
        pos_t *r = &robots.items[i];
        {
            y = (r->y + (r->dy + HEIGHT) * 100) % HEIGHT;
            x = (r->x + (r->dx + WIDTH) * 100) % WIDTH;
        }
        if (y < HEIGHT / 2 && x < WIDTH / 2)
            q[0]++;
        else if (y < HEIGHT / 2 && x > WIDTH / 2)
            q[1]++;
        else if (y > HEIGHT / 2 && x < WIDTH / 2)
            q[2]++;
        else if (y > HEIGHT / 2 && x > WIDTH / 2)
            q[3]++;
    }

    solution = q[0] * q[1] * q[2] * q[3];

    clock_t toc = clock();
    printf("Part1 Solution: %ld\n", solution);
    printf("Part1 Time: %.03f ms\n",
           (double)(toc - tic) / CLOCKS_PER_SEC * 1000);

    // Part 2
    solution = 0;
    tic = clock();

    int max_robot = 0;
    int seconds = 0;

    while (seconds < HEIGHT * WIDTH && max_robot != 1)
    {
        max_robot = 0;
        int grid[HEIGHT][WIDTH] = {0};
        for (int i = 0; i < robots.count; i++)
        {
            pos_t *r = &robots.items[i];
            r->y = (r->y + r->dy + HEIGHT) % HEIGHT;
            r->x = (r->x + r->dx + WIDTH) % WIDTH;
            grid[r->y][r->x]++;
            if (max_robot < grid[r->y][r->x])
                max_robot = grid[r->y][r->x];
        }
        seconds++;
    }

    bool grid[HEIGHT][WIDTH] = {0};
    for (int i = 0; i < robots.count; i++)
        {
            pos_t *r = &robots.items[i];
            grid[r->y][r->x] = true;
        }

    for (int i = 0; i < HEIGHT; i++)
        {
            for (int j = 0; j < WIDTH; j++)
                if (grid[i][j])
                    printf(GREEN " " NORMAL);
                else
                    printf(RED " " NORMAL);
            printf("\n");
        }

    solution = seconds;

    toc = clock();
    printf("Part2 Solution: %ld\n", solution);
    printf("Part2 Time: %.03f ms\n",
           (double)(toc - tic) / CLOCKS_PER_SEC * 1000);

    aoc_da_free(robots);
    free(buffer_t.buffer);

    return EXIT_SUCCESS;
}
