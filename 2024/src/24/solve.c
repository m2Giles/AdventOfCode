#define AOC_UTILS_IMPLEMENTATION
#include "aoc.h"

#define LETTER 26
#define SPACE (('z' - '0') * ('z' - '0'))
#define TENS ('z' - '0')

int str_compare(const void *a, const void *b)
{
    return strncmp((char *)a, (char *)b, 4);
}

enum
{
    AND,
    OR,
    XOR
};

typedef struct
{
    char input1[4];
    char input2[4];
    char output[4];
    int operation;
} gate;

typedef struct
{
    gate *items;
    int count;
    int capacity;
} da;

typedef struct
{
    char *items;
    int count;
    int capacity;
} sb;

bool isOutput(gate *g) { return *g->output == 'z'; }

bool isDirect(gate *g) { return *g->input1 == 'x' || *g->input2 == 'x'; }

bool hasInput(char input[4], gate *g)
{
    return (strncmp(input, g->input1, 4) == 0 ||
            strncmp(input, g->input2, 4) == 0);
}

bool hasOutput(char output[4], gate *g)
{
    return strncmp(output, g->output, 4) == 0;
}

int convertA(int a) { return (a - 'a'); }

int convert(int a, int b) { return (a - '0') * TENS + (b - '0'); }

int convertBack(int a) { return a / TENS * 10 + a % TENS; }

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

    int wires[LETTER][SPACE] = {0};
    memset(wires, -1, sizeof(wires));
    char buf[20];
    Parts_t puzzle = puzzle_split(buffer);
    char **cursor = &puzzle.part1;

    while (sgets(buf, 20, cursor))
    {
        wires[convertA(buf[0])][convert(buf[1], buf[2])] = buf[5] - '0';
    }

    cursor = &puzzle.part2;

    da gates = {0};
    while (sgets(buf, 20, cursor))
    {
        char code[4] = {0};
        int c = 0;
        gate tmp = {0};
        sscanf(buf, "%s %s %s -> %s ", tmp.input1, code, tmp.input2,
               tmp.output);
        if (strncmp(code, "AND", 4) == 0)
            c = AND;
        else if (strncmp(code, "OR", 4) == 0)
            c = OR;
        else
            c = XOR;
        tmp.operation = c;
        aoc_da_append(&gates, tmp);
    }

    int gate_solve = gates.count;
    while (gate_solve > 0)
        for (int i = 0; i < gates.count; i++)
        {
            gate *exp = &gates.items[i];

            int in1 = wires[convertA(exp->input1[0])]
                           [convert(exp->input1[1], exp->input1[2])];
            int in2 = wires[convertA(exp->input2[0])]
                           [convert(exp->input2[1], exp->input2[2])];
            int out = wires[convertA(exp->output[0])]
                           [convert(exp->output[1], exp->output[2])];

            if (in1 == -1 || in2 == -1 || out != -1)
            {
                continue;
            }

            switch (exp->operation)
            {
            case AND:
                wires[convertA(exp->output[0])]
                     [convert(exp->output[1], exp->output[2])] = in1 & in2;
                break;
            case OR:
                wires[convertA(exp->output[0])]
                     [convert(exp->output[1], exp->output[2])] = in1 | in2;
                break;
            case XOR:
                wires[convertA(exp->output[0])]
                     [convert(exp->output[1], exp->output[2])] = in1 ^ in2;
                break;
            }
            gate_solve--;
        }

    sb out = {0};
    for (int j = SPACE - 1; j >= 0; j--)
        if (wires['z' - 'a'][j] != -1)
            aoc_da_append(&out, wires['z' - 'a'][j] + '0');
    solution = strtol(out.items, NULL, 2);

    clock_t toc = clock();
    printf("Part1 Solution: %ld\n", solution);
    printf("Part1 Time: %.03f ms\n",
           (double)(toc - tic) / CLOCKS_PER_SEC * 1000);

    // Part 2
    solution = 0;
    tic = clock();

    sb badWires = {0};

    int highestZbit = 0;
    for (int i = SPACE - 1; i >= 0; i--)
    {
        if (wires['z' - 'a'][i] != -1)
        {
            highestZbit = convertBack(i);
            break;
        }
    }

    char highestZ[4] = {0};
    highestZ[0] = 'z';
    highestZ[1] = highestZbit / 10 + '0';
    highestZ[2] = highestZbit % 10 + '0';

    /*
    **
    ** Full Addr:
    ** Inputs: A, B, Cin
    ** Outputs: S, Cout
    **
    ** // Out Value
    ** A XOR B -> out1
    ** out1 XOR Cin -> S
    **
    ** // Carry
    ** A AND B -> Carry1
    ** out1 AND Cin -> Carry2
    ** Carry1 OR Carry2 -> Cout
    **
    */

    for (int i = 0; i < gates.count; i++)
    {
        gate *g = &gates.items[i];

        // Full Addr output gate is XOR, highest z is a Carry (OR).
        if (*g->output == 'z' && g->operation != XOR &&
            strncmp(g->output, highestZ, 4))
        {
            for (int j = 0; j < 4; j++)
                aoc_da_append(&badWires, g->output[j]);
            badWires.items[badWires.count - 1] = '\0';
        }

        // XOR Gates in Full Addr need to be tied to an input or output
        if (g->operation == XOR && !isDirect(g) && !isOutput(g))
        {
            for (int j = 0; j < 4; j++)
                aoc_da_append(&badWires, g->output[j]);
            badWires.items[badWires.count - 1] = '\0';
        }

        // And Gates in Full Addr are inputs to OR Gate
        if (g->operation == AND && !hasInput("x00", g))
        {
            for (int j = 0; j < gates.count; j++)
            {
                gate *g2 = &gates.items[j];
                if (hasInput(g->output, g2) && g2->operation != OR)
                {
                    for (int k = 0; k < 4; k++)
                        aoc_da_append(&badWires, g->output[k]);
                    badWires.items[badWires.count - 1] = '\0';
                }
            }
        }

        // XOR Gates in Full Addr are inputs to XOR/AND
        if (g->operation == XOR)
        {
            for (int j = 0; j < gates.count; j++)
            {
                gate *g2 = &gates.items[j];
                if (hasInput(g->output, g2) && g2->operation == OR)
                {
                    for (int k = 0; k < 4; k++)
                        aoc_da_append(&badWires, g->output[k]);
                    badWires.items[badWires.count - 1] = '\0';
                }
            }
        }
    }

    // Remove Duplicates
    for (int i = 0; i < badWires.count; i += 4)
    {
        for (int j = i + 4; j < badWires.count; j += 4)
        {
            if (strncmp(&badWires.items[i], &badWires.items[j], 4) == 0)
                for (int k = 0; k < 4; k++)
                    aoc_da_del(&badWires, j);
        }
    }

    // Sort
    qsort(badWires.items, badWires.count / 4, 4, str_compare);

    toc = clock();
    printf("Part2 Solution: ");
    for (int i = 0; i < badWires.count; i += 4)
    {
        printf("%s", &badWires.items[i]);
        if (i != badWires.count - 4)
            printf(",");
        else
            printf("\n");
    }

    printf("Part2 Time: %.03f ms\n",
           (double)(toc - tic) / CLOCKS_PER_SEC * 1000);

    aoc_da_free(out);
    aoc_da_free(gates);
    aoc_da_free(badWires);
    free(buffer_t.buffer);
    return EXIT_SUCCESS;
}
