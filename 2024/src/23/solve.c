#define AOC_UTILS_IMPLEMENTATION
#include "aoc.h"

#define LETTER2 26 * 26
#define LETTER 26

// Graph
bool graph[LETTER2][LETTER2] = {0};

// For Part 1
int list[LETTER][LETTER2] = {0};
int list_length[LETTER] = {0};

// For Part 2 / Vertices
int connection[LETTER2][LETTER2] = {0};
int connectionLength[LETTER2] = {0};

bool _checkCombination(int *combination, int size)
{
    for (int i = 0; i < size; i++)
        for (int j = i + 1; j < size; j++)
            if (!graph[combination[i]][combination[j]])
                return false;
    return true;
}

bool _findClique(int arr[], int n, int r, int idx, int data[], int i)
{
    if (idx == r)
    {
        if (_checkCombination(data, r))
            return true;
        else
            return false;
    }

    if (i >= n)
        return false;

    data[idx] = arr[i];

    if (_findClique(arr, n, r, idx + 1, data, i + 1))
        return true;
    if (_findClique(arr, n, r, idx, data, i + 1))
        return true;

    return false;
}

int findClique(int arr[], int size, int clique[])
{
    for (int i = size; i >= 0; i--)
        if (_findClique(arr, size, i, 0, clique, 0))
            return i;
    return 0;
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
    char **cursor = &buffer;

    char buf[10];

    char x[2];
    char y[2];
    while (sgets(buf, 10, cursor))
    {
        sscanf(buf, " %2c-%2c ", (char *)&x, (char *)&y);
        int a = LETTER * (x[0] - 'a') + x[1] - 'a';
        int b = LETTER * (y[0] - 'a') + y[1] - 'a';
        graph[a][b] = 1;
        graph[b][a] = 1;

        if (x[0] == 't')
        {
            list[a % LETTER][list_length[a % LETTER]++] = b;
        }
        if (y[0] == 't')
        {
            list[b % LETTER][list_length[b % LETTER]++] = a;
        }
        // For Part 2
        connection[a][connectionLength[a]++] = b;
        connection[b][connectionLength[b]++] = a;
    }

    for (int i = 0; i < LETTER; i++)
        for (int j = 0; j < list_length[i]; j++)
            for (int k = j + 1; k < list_length[i]; k++)
            {
                int a = list[i][j];
                int b = list[i][k];
                if (a / LETTER == 't' - 'a' && a % LETTER < i)
                    continue;
                if (b / LETTER == 't' - 'a' && b % LETTER < i)
                    continue;
                if (graph[a][b])
                    solution++;
            }

    clock_t toc = clock();
    printf("Part1 Solution: %ld\n", solution);
    printf("Part1 Time: %.03f ms\n",
           (double)(toc - tic) / CLOCKS_PER_SEC * 1000);

    // Part 2
    solution = 0;
    tic = clock();

    int cliques[LETTER2][LETTER] = {0};
    int cliqueLength[LETTER2] = {0};

    int bestLength = 0;
    int bestIDX = 0;

    for (int i = 0; i < LETTER2; i++)
    {
        if (connectionLength[i])
        {
            cliqueLength[i] =
                findClique(connection[i], connectionLength[i], cliques[i]);
            cliques[i][cliqueLength[i]++] = i;
            if (cliqueLength[i] > bestLength)
            {
                bestLength = cliqueLength[i];
                bestIDX = i;
            }
        }
    }

    qsort(cliques[bestIDX], bestLength, sizeof(int), int_compare);

    toc = clock();

    printf("Part2 Solution: ");
    for (int i = 0; i < bestLength; i++)
    {
        printf("%c%c", cliques[bestIDX][i] / 26 + 'a',
               cliques[bestIDX][i] % 26 + 'a');
        if (i != bestLength - 1)
            printf(",");
    }
    puts("");

    printf("Part2 Time: %.03f ms\n",
           (double)(toc - tic) / CLOCKS_PER_SEC * 1000);

    free(buffer_t.buffer);

    return EXIT_SUCCESS;
}
