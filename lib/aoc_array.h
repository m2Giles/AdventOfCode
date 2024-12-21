#ifndef AOC_ARRAY_H_
#define AOC_ARRAY_H_

#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

extern void *aoc_da_find(void *items, size_t item_size, size_t count, const void *item);
extern void *aoc_da_bsearch(void *items, size_t item_size, size_t count,
                     const void *item);

#define aoc_da_resize(da_array_p, new)                                         \
    do                                                                         \
    {                                                                          \
        if ((da_array_p)->capacity == 0)                                       \
        {                                                                      \
            (da_array_p)->count = 0;                                           \
            (da_array_p)->capacity = 4;                                        \
            (da_array_p)->items =                                              \
                malloc(sizeof(new) * (da_array_p)->capacity);                  \
            assert((da_array_p)->items != NULL && "Failed to Allocate!");      \
        }                                                                      \
        if ((da_array_p)->count == (da_array_p)->capacity)                     \
        {                                                                      \
            (da_array_p)->capacity <<= 1;                                      \
            (da_array_p)->items = realloc(                                     \
                (da_array_p)->items, sizeof(new) * (da_array_p)->capacity);    \
            assert((da_array_p)->items != NULL && "Failed to Realloc!");       \
        }                                                                      \
    } while (0)

#define aoc_da_append(da_array_p, new)                                         \
    do                                                                         \
    {                                                                          \
        aoc_da_resize((da_array_p), new);                                      \
        (da_array_p)->items[(da_array_p)->count++] = new;                      \
    } while (0)

#define aoc_da_insert(da_array_p, new, index)                                  \
    do                                                                         \
    {                                                                          \
        assert(index < (da_array_p)->capacity && "Index too Large!");          \
        assert(index > -1 && "Index too Small!");                              \
        aoc_da_resize((da_array_p), new);                                      \
        memmove(&(da_array_p)->items[index + 1], &(da_array_p)->items[index],  \
                sizeof(new) * ((da_array_p)->count - index));                  \
        (da_array_p)->count++;                                                 \
        (da_array_p)->items[index] = new;                                      \
    } while (0)

#define aoc_da_del(da_array_p, index)                                          \
    do                                                                         \
    {                                                                          \
        assert(index < (da_array_p)->capacity && "Index too Large!");          \
        assert(index > -1 && "Index too Small!");                              \
        memmove(&(da_array_p)->items[index], &(da_array_p)->items[index + 1],  \
                sizeof((da_array_p)->items[index]) *                           \
                    ((da_array_p)->count - index - 1));                        \
        (da_array_p)->count--;                                                 \
    } while (0)

#define aoc_da_free(da_array)                                                  \
    do                                                                         \
    {                                                                          \
        free(da_array.items);                                                  \
        da_array = (typeof(da_array)){0};                                      \
    } while (0)

#define aoc_da_qsort(da_array_p, compare)                                      \
    do                                                                         \
    {                                                                          \
        qsort((da_array_p)->items, (da_array_p)->count,                        \
              sizeof((da_array_p)->items[0]), compare);                        \
    } while (0)

#define aoc_da_swap(da_array_p, dst, src)                                      \
    do                                                                         \
    {                                                                          \
        typeof((da_array_p)->items[src]) _aoc_tmp_swap =                       \
            (da_array_p)->items[src];                                          \
        (da_array_p)->items[src] = (da_array_p)->items[dst];                   \
        (da_array_p)->items[dst] = _aoc_tmp_swap;                              \
    } while (0)

#define aoc_da_pushback(da_array_p, data)                                      \
    do                                                                         \
    {                                                                          \
        int i;                                                                 \
        for (int i = 0; i < (da_array_p)->count; i++)                          \
        {                                                                      \
            if ((da_array_p)->items[i] == data)                                \
                break;                                                         \
        }                                                                      \
        assert(i < (da_array_p)->count && "Not in Array!");                    \
        aoc_da_del(da_array_p, i);                                             \
        aoc_da_append(da_array_p, data)                                        \
    } while (0)

#define aoc_da_free_and_destroy(da_array_p)                                    \
    do                                                                         \
    {                                                                          \
        for (int i = 0; i < (da_array_p)->count; ++i)                          \
        {                                                                      \
            free((da_array_p)->items[i]);                                      \
        }                                                                      \
        aoc_da_free(*da_array_p)                                               \
    } while (0)

#endif // AOC_ARRAY_H_

#ifdef AOC_ARRAY_H_IMPLEMENTATION_
#include <stdlib.h>
#include <string.h>
void *aoc_da_find(void *items, size_t item_size, size_t count, const void *item)
{
    for (size_t i = 0; i < count; i++)
    {
        void *data = (char *)items + i * item_size;
        if (memcmp(data, item, item_size) == 0)
            return data;
    }
    return NULL;
}

void *aoc_da_bsearch(void *items, size_t item_size, size_t count,
                     const void *item)
{
    size_t low = 0;
    size_t high = count;
    while (low < high)
    {
        size_t mid = low + (high - low) / 2;
        void *data = (char *) items + mid * item_size;
        int c = (memcmp(data, item, item_size));
        if (c == 0)
            return data;
        if (c < 0)
            low = mid + 1;
        else
            high = mid;
    }
    return NULL;
}
#endif // AOC_ARRAY_H_IMPLEMENTATION_
