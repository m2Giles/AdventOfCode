#define AOC_UTILS_IMPLEMENTATION
#include "../../lib/aoc.h"

#define RED_CUBES 12
#define GREEN_CUBES 13
#define BLUE_CUBES 14

typedef struct
{
    int count;
    char *color;
} pair_t;

typedef struct
{
    pair_t red;
    pair_t green;
    pair_t blue;
} game_t;


int main(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("Missing file\n");
        return EXIT_FAILURE;
    }

    char *filename = argv[1];
    int solution = 0;

    // Read in File
    char *buffer = read_file(filename);
    if (!buffer)
    {
        return EXIT_FAILURE;
    }
    int lines = count_lines(filename);
    if (!lines)
    {
        fprintf(stderr, "%s is empty\n", filename);
        free(buffer);
        return EXIT_FAILURE;
    }

    // Part 1
    char *buffer_saveptr = NULL;
    char *line = strtok_r(buffer, "\n", &buffer_saveptr);
    char *line_saveptr;
    char *games;
    for (;;)
    // for(int l = 0; l < 5; l++)
    {
        line_saveptr = NULL;
        pair_t pair = {0};
        games = strtok_r(line, " ", &line_saveptr);
        int game_id = atoi(strtok_r(NULL, ":", &line_saveptr));
        games = strtok_r(NULL, ",;", &line_saveptr);
        int game_possible = 1;
        while (games != NULL)
        {
            if (isdigit(games[1]))
            {
                pair.count = games[1] - '0';
                pair.color = &games[3];
            }
            if (isdigit(games[2]))
            {
                pair.count = (games[1] - '0') * 10 + games[2] - '0';
                pair.color = &games[4];
            }

            if (strncmp(pair.color, "red", 3) == 0 && pair.count > RED_CUBES)
            {
                game_possible = 0;
                break;
            }
            else if (strncmp(pair.color, "green", 5) == 0 && pair.count > GREEN_CUBES)
            {
                game_possible = 0;
                break;
            }
            else if (strncmp(pair.color, "blue", 4) == 0 && pair.count > BLUE_CUBES)
            {
                game_possible = 0;
                break;
            }

            games = strtok_r(NULL, ",;", &line_saveptr);
        }

        if (game_possible)
            solution += game_id;

        line = strtok_r(NULL, "\n", &buffer_saveptr);

        if (!line)
            break;
    }
    printf("Part1 Solution: %d\n", solution);

    // Part 2
    solution = 0;
    buffer = read_file(filename);
    buffer_saveptr = NULL;
    line = strtok_r(buffer, "\n", &buffer_saveptr);
    for (;;)
    // for(int l = 0; l < 5; l++)
    {
        line_saveptr = NULL;
        pair_t pair = {0};
        games = strtok_r(line, " ", &line_saveptr);
        strtok_r(NULL, ":", &line_saveptr);
        games = strtok_r(NULL, ",;", &line_saveptr);

        game_t game_max;
        game_max.red.color = "red";
        game_max.red.count = 0;
        game_max.green.color = "green";
        game_max.green.count = 0;
        game_max.blue.color = "blue";
        game_max.blue.count = 0;

        while (games != NULL)
        {
            if (isdigit(games[1]))
            {
                pair.count = games[1] - '0';
                pair.color = &games[3];
            }
            if (isdigit(games[2]))
            {
                pair.count = (games[1] - '0') * 10 + games[2] - '0';
                pair.color = &games[4];
            }

            if (strncmp(pair.color, "red", 3) == 0 && pair.count > game_max.red.count)
            {
                game_max.red.count = pair.count;
            }
            else if (strncmp(pair.color, "green", 5) == 0 && pair.count > game_max.green.count)
            {
                game_max.green.count = pair.count;
            }
            else if (strncmp(pair.color, "blue", 4) == 0 && pair.count > game_max.blue.count)
            {
                game_max.blue.count = pair.count;
            }

            games = strtok_r(NULL, ",;", &line_saveptr);
        }

        solution += game_max.blue.count * game_max.red.count * game_max.green.count;

        line = strtok_r(NULL, "\n", &buffer_saveptr);

        if (!line)
            break;
    }
    printf("Part2 Solution: %d\n", solution);

    if (buffer)
        free(buffer);

    return EXIT_SUCCESS;
}