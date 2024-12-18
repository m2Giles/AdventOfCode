#define AOC_UTILS_IMPLEMENTATION
#include "../../../lib/aoc.h"

typedef struct
{
    uint64_t *items;
    uint64_t count;
    uint64_t capacity;
} da;

void print_out(da solution)
{
    if (!solution.count)
        return;
    for (int i = 0; i < solution.count - 1; i++)
        printf("%ld,", solution.items[i]);
    printf("%ld\n", solution.items[solution.count - 1]);
}

uint64_t combooperand(uint64_t operand, uint64_t A, uint64_t B, uint64_t C)
{
    assert(operand < 7 && operand >= 0);
    switch (operand)
    {
    case 4:
        return A;
    case 5:
        return B;
    case 6:
        return C;
    }
    return operand;
}

void solve(da *program, da *solution, uint64_t A, uint64_t B, uint64_t C)
{
    uint64_t out;
    int instruction_pointer = 0;
    while (instruction_pointer < program->count)
    {
        assert(instruction_pointer + 1 < program->count);
        uint64_t operand = program->items[instruction_pointer + 1];
        switch (program->items[instruction_pointer])
        {
        case 0:
            A >>= combooperand(operand, A, B, C);
            instruction_pointer += 2;
            break;
        case 1:
            B ^= operand;
            instruction_pointer += 2;
            break;
        case 2:
            B = combooperand(operand, A, B, C) % 8;
            instruction_pointer += 2;
            break;
        case 3:
            if (!(A))
            {
                instruction_pointer += 2;
                break;
            }
            else
            {
                instruction_pointer = operand;
                break;
            }
        case 4:
            B ^= C;
            instruction_pointer += 2;
            break;
        case 5:
            out = combooperand(operand, A, B, C) % 8;
            aoc_da_append(solution, out);
            instruction_pointer += 2;
            break;
        case 6:
            B = A >> combooperand(operand, A, B, C);
            instruction_pointer += 2;
            break;
        case 7:
            C = A >> combooperand(operand, A, B, C);
            instruction_pointer += 2;
            break;
        }
    }
}

void reverseEngineer(da program, da *out, int offset, uint64_t input)
{
    uint64_t A;
    da output = {0};
    int length = program.count;
    if (offset == length)
    {
        solve(&program, &output, input, 0, 0);
        if (output.items[0] == program.items[0])
            aoc_da_append(out, input);
    }

    for (int i = 0; i < 8; i++)
    {
        A = (input << 3) + i;
        solve(&program, &output, A, 0, 0);
        if (program.count - 1 - offset < program.count)
            if (output.items[output.count - 1 - offset] ==
                program.items[program.count - 1 - offset])
                reverseEngineer(program, out, offset + 1, A);
    }
    aoc_da_free(output);
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("Missing file\n");
        return EXIT_FAILURE;
    }

    char *filename = argv[1];

    da solution = {0};

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

    char **cursor = &buffer;
    char buf[100] = {0};
    uint64_t A, B, C;
    da program = {0};
    sgets(buf, 100, cursor);
    sscanf(buf, " Register A: %ld ", &A);
    sgets(buf, 100, cursor);
    sscanf(buf, " Register B: %ld ", &B);
    sgets(buf, 100, cursor);
    sscanf(buf, " Register C: %ld ", &C);
    sgets(buf, 100, cursor);
    sgets(buf, 100, cursor);
    sscanf(buf, "Program: %s", buf);
    char *tmp = strtok(buf, ",");
    while (tmp)
    {
        aoc_da_append(&program, strtol(tmp, NULL, 10));
        tmp = strtok(NULL, ",");
    }

    solve(&program, &solution, A, B, C);

    printf("A = %ld, B = %ld, C = %ld\n", A, B, C);
    printf("Part1 Solution: ");
    print_out(solution);

    clock_t toc = clock();
    // printf("Part1 Solution: %s\n", solution);
    printf("Part1 Time: %.03f ms\n",
           (double)(toc - tic) / CLOCKS_PER_SEC * 1000);

    // Part 2
    tic = clock();

    da output = {0};
    reverseEngineer(program, &output, 0, 0);
    aoc_da_qsort(&output, uint64_compare);

    toc = clock();
    printf("Part2 Solution: %ld\n", output.items[0]);
    printf("Part2 Time: %.03f ms\n",
           (double)(toc - tic) / CLOCKS_PER_SEC * 1000);

    aoc_da_free(program);
    aoc_da_free(solution);
    aoc_da_free(output);
    free(buffer_t.buffer);

    return EXIT_SUCCESS;
}
