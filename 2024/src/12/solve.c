#define AOC_UTILS_IMPLEMENTATION
#include "../../../lib/aoc.h"

bool bounds(int x, int y, Grid_t grid)
{
  return 0 <= y && y < grid.height && 0 <= x && x < grid.width;
}

bool check(int x, int y, int dx, int dy, Grid_t grid)
{
  int y2 = y + dy;
  int x2 = x + dx;
  return (bounds(x, y, grid) && bounds(x2, y2, grid) &&
          grid.grid[y][x] == grid.grid[y2][x2]);
}

void part1(int x, int y, int *area, int *perimeter, Grid_t grid,
           bool visited[grid.height][grid.width])
{
  visited[y][x] = true;
  (*area)++;
  for (int dx = -1; dx < 2; dx++)
    for (int dy = -1; dy < 2; dy++)
    {
      if (abs(dx) + abs(dy) == 1)
      {
        int y2 = y + dy;
        int x2 = x + dx;
        if (!bounds(x2, y2, grid))
        {
          (*perimeter)++;
        }
        else if (grid.grid[y][x] != grid.grid[y2][x2])
        {
          (*perimeter)++;
        }
        else if (grid.grid[y][x] == grid.grid[y2][x2] && !visited[y2][x2])
        {
          part1(x2, y2, area, perimeter, grid, visited);
        }
      }
    }
}

void part2(int x, int y, int *area, int *perimeter, Grid_t grid,
           bool visited[grid.height][grid.width])
{
  visited[y][x] = true;
  (*area)++;

  int dirs[4][2] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
  for (int i = 0; i < 4; i++)
  {
    int dy2 = dirs[i][0];
    int dx2 = dirs[i][1];
    int dy1 = dirs[(i + 1) % 4][0];
    int dx1 = dirs[(i + 1) % 4][1];

    if (!check(x, y, dx2, dy2, grid) && !check(x, y, dx1, dy1, grid))
    {
      (*perimeter)++;
    }
    else if (check(x, y, dx2, dy2, grid) && check(x, y, dx1, dy1, grid) &&
             !check(x, y, dx2 + dx1, dy2 + dy1, grid))
    {
      (*perimeter)++;
    }

    if (check(x, y, dx2, dy2, grid) && !visited[y + dy2][x + dx2])
    {
      part2(x + dx2, y + dy2, area, perimeter, grid, visited);
    }
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

  Grid_t grid = make_grid(buffer, lines);
  bool visited[grid.height][grid.width];
  memset(visited, 0, sizeof(visited));

  for (int i = 0; i < grid.height; i++)
  {
    for (int j = 0; j < grid.width; j++)
    {
      if (!visited[i][j])
      {
        int area = 0;
        int perimeter = 0;
        part1(j, i, &area, &perimeter, grid, visited);
        solution += area * perimeter;
      }
    }
  }

  clock_t toc = clock();
  printf("Part1 Solution: %ld\n", solution);
  printf("Part1 Time: %.03f ms\n", (double)(toc - tic) / CLOCKS_PER_SEC * 1000);

  // Part 2
  solution = 0;
  memset(visited, 0, sizeof(visited));
  tic = clock();

  for (int i = 0; i < grid.height; i++)
  {
    for (int j = 0; j < grid.width; j++)
    {
      if (!visited[i][j])
      {
        int area = 0;
        int perimeter = 0;
        part2(j, i, &area, &perimeter, grid, visited);
        printf("Plants: %c, area: %d, perimeter %d, start (%d, %d)\n",
               grid.grid[i][j], area, perimeter, j, i);
        solution += area * perimeter;
      }
    }
  }

  toc = clock();
  printf("Part2 Solution: %ld\n", solution);
  printf("Part2 Time: %.03f ms\n", (double)(toc - tic) / CLOCKS_PER_SEC * 1000);

  destroy_grid(grid);
  free(buffer_t.buffer);

  return EXIT_SUCCESS;
}
