#define AOC_UTILS_IMPLEMENTATION
#include "../../../lib/aoc.h"

enum
{
    UP = 0,
    RIGHT = 1,
    DOWN = 2,
    LEFT = 3
} decode;

typedef struct
{
    int x, y;
} pos;

const pos dirs[4] = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}};

pos next(pos position, pos direction)
{
    return (pos){.x = position.x + direction.x, .y = position.y + direction.y};
}

typedef struct
{
    int *items;
    int capacity;
    int count;
} da;

Grid_t make_double_grid(const char *buffer, int lines)
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
        grid[i] = (char *)malloc((output.width * 2) * sizeof(char));
        if (!grid[i])
        {
            perror("Failed to allocate memory...");
            output.grid = NULL;
            goto end;
        }
        for (int j = 0; j < output.width; j++)
        {
            switch (buffer[i * output.height + i + j])
            {
            case '#':
                grid[i][j * 2] = '#';
                grid[i][j * 2 + 1] = '#';
                break;
            case '.':
                grid[i][j * 2] = '.';
                grid[i][j * 2 + 1] = '.';
                break;
            case '@':
                grid[i][j * 2] = '@';
                grid[i][j * 2 + 1] = '.';
                break;
            case 'O':
                grid[i][j * 2] = '[';
                grid[i][j * 2 + 1] = ']';
                break;
            case '\n':
                grid[i][j * 2] = '\n';
                grid[i][j * 2 + 1] = '\0';
                break;
            default:
                break;
            }
        }
    }
end:
    output.width *= 2;
    output.grid = grid;
    return output;
}

size_t calculate_value(Grid_t grid)
{
    size_t output = 0;
    for (int i = 0; i < grid.height; i++)
        for (int j = 0; j < grid.width; j++)
            if (grid.grid[i][j] == 'O' || grid.grid[i][j] == '[')
                output += 100 * i + j;
    return output;
}

bool check(Grid_t grid, pos start, pos dir)
{
    if (!dir.y)
        return true;

    pos new = next(start, dir);
    char nexttile = grid.grid[new.y][new.x];
    if (nexttile == '#')
        return false;
    if (nexttile == '.')
        return true;

    pos right = {.x = start.x + 1, .y = start.y + dir.y};
    pos left = {.x = start.x - 1, .y = start.y + dir.y};
    bool check1, check2;
    check1 = check(grid, new, dir);
    if (nexttile == '[')
        check2 = check(grid, right, dir);
    else
        check2 = check(grid, left, dir);
    return (check1 && check2);
}

void move(Grid_t grid, pos start, pos dir)
{
    pos new = next(start, dir);
    char nexttile = grid.grid[new.y][new.x];

    // Part 1
    if (nexttile == 'O')
    {
        move(grid, new, dir);
        goto end;
    }

    // Part 2
    pos right = {.x = start.x + 1, .y = start.y + dir.y};
    pos left = {.x = start.x - 1, .y = start.y + dir.y};
    // Horizontal Move
    if (!dir.y && (nexttile == '[' || nexttile == ']'))
    {
        move(grid, new, dir);
        goto end;
    }
    // Vertical move
    else if (nexttile == '[')
    {
        move(grid, new, dir);
        move(grid, right, dir);
        goto end;
    }
    else if (nexttile == ']')
    {
        move(grid, new, dir);
        move(grid, left, dir);
        goto end;
    }

end:
    if (grid.grid[new.y][new.x] == '.')
    {
        grid.grid[new.y][new.x] = grid.grid[start.y][start.x];
        grid.grid[start.y][start.x] = '.';
    }
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

    pos robot = {0};
    da instructions = {0};
    Parts_t puzzle = puzzle_split(buffer);
    Grid_t grid = make_grid(puzzle.part1, puzzle.part1_lines);

    for (int i = 0; i < grid.height; i++)
        for (int j = 0; j < grid.width; j++)
            if (grid.grid[i][j] == '@')
                robot = (pos){.x = j, .y = i};

    for (int i = 0; i < puzzle.part2_len; i++)
    {
        switch (puzzle.part2[i])
        {
        case '^':
            aoc_da_append(&instructions, UP);
            break;
        case '>':
            aoc_da_append(&instructions, RIGHT);
            break;
        case 'v':
            aoc_da_append(&instructions, DOWN);
            break;
        case '<':
            aoc_da_append(&instructions, LEFT);
            break;
        default:
            break;
        }
    }

    for (int i = 0; i < instructions.count; i++)
    {
        pos dir = dirs[instructions.items[i]];
        move(grid, robot, dir);
        if (grid.grid[robot.y][robot.x] != '@')
            robot = next(robot, dir);
    }
    solution = calculate_value(grid);

    clock_t toc = clock();
    destroy_grid(grid);
    printf("Part1 Solution: %ld\n", solution);
    printf("Part1 Time: %.03f ms\n",
           (double)(toc - tic) / CLOCKS_PER_SEC * 1000);

    // Part 2
    solution = 0;
    tic = clock();

    grid = make_double_grid(puzzle.part1, puzzle.part1_lines);

    for (int i = 0; i < grid.height; i++)
        for (int j = 0; j < grid.width; j++)
            if (grid.grid[i][j] == '@')
                robot = (pos){.x = j, .y = i};

    for (int i = 0; i < instructions.count; i++)
    {
        pos dir = dirs[instructions.items[i]];
        if (check(grid, robot, dir))
        {
            move(grid, robot, dir);
            if (grid.grid[robot.y][robot.x] != '@')
                robot = next(robot, dir);
        }
    }
    solution = calculate_value(grid);

    toc = clock();
    printf("Part2 Solution: %ld\n", solution);
    printf("Part2 Time: %.03f ms\n",
           (double)(toc - tic) / CLOCKS_PER_SEC * 1000);

    destroy_grid(grid);
    free_puzzle_split(puzzle);
    aoc_da_free(instructions);
    free(buffer_t.buffer);

    return EXIT_SUCCESS;
}
