#define AOC_UTILS_IMPLEMENTATION
#include "../../../lib/aoc.h"
#include <time.h>
#include <assert.h>

typedef struct
{
    int *items;
    int count;
    int capacity;
} da;

typedef struct file_node file_node;

struct file_node
{
    int id;
    int size;
    int freeSpace;
    file_node *next;
    file_node *prev;
};

void da_append(da *array, const int item)
{
    if (array->capacity == 0)
    {
        array->count = 0;
        array->capacity = 4;
        array->items = malloc(sizeof(int) * array->capacity);
    }
    if (array->count + 1 > array->capacity)
    {
        array->capacity <<= 1;
        array->items = realloc(array->items, sizeof(int) * array->capacity);
        assert(array->items != NULL && "Failed to realloc memory!");
    }
    array->items[array->count++] = item;
}

void da_swap(da *array, int dst, int src)
{
    unsigned int temp = array->items[dst];
    array->items[dst] = array->items[src];
    array->items[src] = temp;
}

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
    clock_t toc = clock();
    da memory = {0};

    int holes = 0;
    for (int i = 0; i < count; ++i)
    {
        if (!(i & 1))
        {
            for (int j = 0; j < buffer[i] - '0'; ++j)
            {
                da_append(&memory, i / 2);
            }
        }
        else
        {
            for (int j = 0; j < buffer[i] - '0'; ++j)
            {
                da_append(&memory, -1);
                holes++;
            }
        }
    }
    int len = memory.count;
    len--;
    for (int i = 0; i < memory.count - holes; ++i)
    {
        while (memory.items[i] == -1)
        {
            da_swap(&memory, i, len);
            len--;
        }
    }

    for (int i = 0; i < memory.count - holes; ++i)
    {
        solution += i * memory.items[i];
    }

    printf("Part1 Solution: %ld\n", solution);
    printf("Part1 Time: %.03f ms\n", (double)(toc - tic) / CLOCKS_PER_SEC * 1000);
    free(memory.items);
    memory = (da){0};

    // Part 2
    tic = clock();
    solution = 0;

    file_node head = {.id = 0, .size = buffer[0] - '0', .freeSpace = buffer[1] - '0', .next = NULL, .prev = NULL};
    file_node *tail = &head;

    for (int i = 2; i < count; i += 2)
    {
        file_node *new = calloc(1, sizeof(file_node));
        new->id = i / 2;
        new->size = buffer[i] - '0';
        if (i + 1 < count)
        {
            new->freeSpace = buffer[i + 1] - '0';
        }
        tail->next = new;
        new->prev = tail;
        tail = new;
    }

    file_node *tail_cursor = tail;
    while (tail_cursor != NULL)
    {
        file_node *store = tail_cursor->prev;
        for (file_node *head_cursor = &head; tail_cursor != head_cursor && head_cursor != NULL; head_cursor = head_cursor->next)
        {
            if (head_cursor->freeSpace >= tail_cursor->size)
            {
                if (tail_cursor->prev != NULL)
                {
                    tail_cursor->prev->next = tail_cursor->next;
                    tail_cursor->prev->freeSpace += tail_cursor->size + tail_cursor->freeSpace;
                }
                if (tail_cursor->next != NULL)
                {
                    tail_cursor->next->prev = tail_cursor->prev;
                }
                if (head_cursor->next != NULL)
                {
                    head_cursor->next->prev = tail_cursor;
                    tail_cursor->next = head_cursor->next;
                }
                tail_cursor->freeSpace = head_cursor->freeSpace - tail_cursor->size;
                head_cursor->freeSpace = 0;
                head_cursor->next = tail_cursor;
                tail_cursor->prev = head_cursor;
                break;
            }
        }
        tail_cursor = store;
    }

    solution = 0;
    int position = 0;
    for (file_node *cursor = &head; cursor != NULL; cursor = cursor->next)
    {
        for (int i = 0; i < cursor->size; i++)
        {
            solution += cursor->id * position;
            position++;
        }
        for (int i = 0; i < cursor->freeSpace; i++)
        {
            position++;
        }
    }

    file_node *cursor = head.next;
    while (cursor != NULL)
    {
        file_node *temp = cursor;
        cursor = cursor->next;
        free(temp);
    }

    toc = clock();
    printf("Part2 Solution: %ld\n", solution);
    printf("Part2 Time: %.03f ms\n", (double)(toc - tic) / CLOCKS_PER_SEC * 1000);

    free(buffer_t.buffer);

    return EXIT_SUCCESS;
}
