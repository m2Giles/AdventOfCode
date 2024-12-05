#define AOC_UTILS_IMPLEMENTATION
#include "../../../lib/aoc.h"

char *update_sp = NULL;
char *update_s = NULL;
char *page_sp = NULL;
char *page = NULL;

typedef struct
{
    int X;
    int Y;
} rule_t;

rule_t rules[2000] = {0};
int rule_count = 0;

int rule_compare(const void *a, const void *b)
{
    rule_t *ra = (rule_t *)a;
    rule_t *rb = (rule_t *)b;
    int diff = (ra->X - rb->X);
    if (!diff)
        diff = (ra->Y - rb->Y);
    return (diff);
}

int compare(const void *a, const void *b)
{
    int pageX = *(int *)a;
    int pageY = *(int *)b;

    for (int i = 0; i < rule_count; ++i)
    {
        if(rules[i].Y == pageX && rules[i].X == pageY){
            return -1;
        }
        if (rules[i].Y == pageY && rules[i].X == pageX)
        {
            return 1;
        }
    }
    return 0;
}

rule_t * get_rules(Parts_t puzzle)
{
    char *rules_input = puzzle.part1;
    rule_t * rules = malloc(sizeof(rule_t) * puzzle.part1_lines);
    for (int i = 0; i < puzzle.part1_lines; ++i)
    {
        int line = i * puzzle.part1_len / puzzle.part1_lines;
        rules[i].X = (rules_input[line] - '0') * 10 + rules_input[line + 1] - '0';
        rules[i].Y = (rules_input[line + 3] - '0') * 10 + rules_input[line + 4] - '0';
    }
    return rules;
}

int check(rule_t *rules, int rules_count, int *pages, int page_count)
{
    for (int i = 0; i < rules_count; ++i)
    {
        int X_pos = -1;
        int Y_pos = 100;
        for (int j = 0; j < page_count; ++j)
        {
            if(pages[j] == rules[i].X){
                X_pos = j;
            }
            if(pages[j] == rules[i].Y)
            {
                Y_pos = j;
            }
            if(X_pos > Y_pos)
            {
                printf("Broke Rule = %d|%d at position %d", rules[i].X, rules[i].Y, j);
                return 0;
            }
        }
    }
    return 1;
}

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

    Parts_t puzzle = puzzle_split(buffer);

    if (!puzzle.part1)
    {
        fprintf(stderr, "Failed to split puzzle into parts");
        free(buffer);
        return EXIT_FAILURE;
    }

    rule_t * temp_rules = get_rules(puzzle);
    if (!temp_rules)
    {
        fprintf(stderr, "Unable to allocate memory for rulesets");
        free(buffer);
        free_puzzle_split(puzzle);
        return EXIT_FAILURE;
    }
    rule_count = puzzle.part1_lines;
    memcpy(rules, temp_rules, sizeof(rule_t) * rule_count);
    free(temp_rules);
    temp_rules = NULL;


    // Sort
    qsort(rules, puzzle.part1_lines, sizeof(rule_t), rule_compare);

    update_s = strtok_r(puzzle.part2, "\n", &update_sp);
    while (update_s != NULL)
    {
        int page_count = 0;
        int pages[100] = {0};
        page = strtok_r(update_s, ", ", &page_sp);
        while (page != NULL)
        {
            pages[page_count] = strtol(page, NULL, 10);
            printf("%d ", pages[page_count]);
            page_count++;
            page = strtok_r(NULL, ", ", &page_sp);
        }
        if(check(rules, puzzle.part1_lines, pages, page_count))
        {
            printf("Midpoint = %d", pages[page_count/2]);
            solution += pages[page_count/2];
        }
        update_s = strtok_r(NULL, "\n", &update_sp);
        printf("\n");
    }

    // Part 1
    printf("Part1 Solution: %d\n", solution);

    // Part 2
    solution = 0;
    free_puzzle_split(puzzle);

    puzzle = puzzle_split(buffer);

    // Sort
    qsort(rules, puzzle.part1_lines, sizeof(rule_t), rule_compare);

    update_s = strtok_r(puzzle.part2, "\n", &update_sp);
    while (update_s != NULL)
    {
        int page_count = 0;
        int pages[100] = {0};
        page = strtok_r(update_s, ", ", &page_sp);
        while (page != NULL)
        {
            pages[page_count] = strtol(page, NULL, 10);
            printf("%d ", pages[page_count]);
            page_count++;
            page = strtok_r(NULL, ", ", &page_sp);
        }
        if(!check(rules, rule_count, pages, page_count))
        {
            int sort_pages[page_count];
            memcpy(sort_pages, pages, page_count * sizeof(int));
            qsort(sort_pages, page_count, sizeof(int), compare);
            solution += sort_pages[page_count/2];
        }
        update_s = strtok_r(NULL, "\n", &update_sp);
        printf("\n");
    }
    
    printf("Part2 Solution: %d\n", solution);

    free_puzzle_split(puzzle);
    if (buffer)
        free(buffer);

    return EXIT_SUCCESS;
}
