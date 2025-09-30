#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
// ------- 1

// ------- 2

void shuffleTable(int *table, size_t count)
// Fisher-yates: https://www.geeksforgeeks.org/dsa/shuffle-a-given-array-using-fisher-yates-shuffle-algorithm/
{
  for (size_t i = count - 1; i > 0; i--)
  {
    size_t random_idx = (size_t)(rand() % (i + 1));
    int temp = table[i];
    table[i] = table[random_idx];
    table[random_idx] = temp;
  }
}

int *createUniqueIntTable(size_t count)
{
  int *array = malloc(count * sizeof(*array));
  long long current = 0;
  for (size_t i = 0; i < count; i++)
  {
    // int random_number = (rand() % 100) + 1; // fra 1 til 1000
    // current += random_number;
    // array[i] = (int)current;
    array[i] = (int)i;
  }
  shuffleTable(array, count);
  return array;
}

// ------- 3

size_t hashFunction1(size_t key, size_t table_size)
{
  // 2^64, tablesize er primtall
  const size_t A = 11400714819323198485ull;
  return (key * A) % table_size;
  // return key % table_size;
}

size_t hashFunction2(size_t key, size_t table_size)
{
  return 1 + (key % (table_size - 1));
}

size_t probeLinear(size_t hash_val, size_t i, size_t table_size)
{
  return (hash_val + i) % table_size;
}

size_t probeDouble(size_t h1, size_t h2, size_t i, size_t table_size)
{
  // size_t pos = h1 % table_size;
  // h2 %= table_size;
  // for (size_t j = 0; j < i; j++)
  // {
  //   pos = (pos + h2) % table_size;
  // }
  // return pos;
  return (h1 + (i * h2)) % table_size;
}

int isPrime(int numb)
{
  if (numb == 2)
    return 1;
  if (numb % 2 == 0 || numb < 2)
    return 0;
  for (int i = 3; i <= numb / i; i += 2)
  {
    if (numb % i == 0)
    {
      return 0;
    }
  }
  return 1;
}

int findPrime(int tableSize)
{
  if (tableSize == 2)
    return 2;
  if (tableSize % 2 == 0)
    tableSize++;
  while (!isPrime(tableSize))
    tableSize += 2;
  return tableSize;
}

int insert(int key, size_t table_size, int hash_table[table_size], char chooseMethod, long long *insert_collision)
{
  size_t h1 = hashFunction1(key, table_size);
  size_t h2 = 0;

  for (size_t i = 0; i < table_size; i++)
  {
    size_t pos;
    if (chooseMethod == 'l')
    {
      pos = probeLinear(h1, i, table_size);
    }
    else
    {
      if (i == 0)
      {
        pos = h1;
      }
      else
      {
        if (h2 == 0)
        {
          h2 = hashFunction2(key, table_size);
        }
        pos = probeDouble(h1, h2, i, table_size);
      }
    }
    // Same key
    if (hash_table[pos] == key + 1)
      return (int)pos;

    // empty
    if (hash_table[pos] == 0)
    {
      hash_table[pos] = key + 1;
      return (int)pos;
    }
    (*insert_collision)++;
  }
  return -1;
}

// unused
int search(int key, size_t table_size, int hash_table[table_size], char chooseMethod)
{
  size_t h1 = hashFunction1(key, table_size);
  size_t h2 = ((chooseMethod == 'l') ? 0 : hashFunction2(key, table_size));
  for (size_t i = 0; i < table_size; i++)
  {
    size_t pos = ((chooseMethod == 'l') ? probeLinear(h1, i, table_size) : probeDouble(h1, h2, i, table_size));

    if (!hash_table[pos])
      return -1;
    if (hash_table[pos] == key + 1)
      return pos;
  }
  return -1;
}

double timing(void)
{
  struct timespec time_spec;
  clock_gettime(CLOCK_MONOTONIC_RAW, &time_spec);
  return (double)time_spec.tv_sec * 1e9 + (double)time_spec.tv_nsec;
}

// Hashing med ulike load factor og timing
void hashTimingLoadFactor(double load_factor, size_t m, int *table, size_t total_keys, size_t i, double *linear_times, double *double_times, double *linear_collisions, double *double_collisions)
{
  // printf("Total elements: %zu\n", total_keys);
  int *hash_table = calloc((size_t)m, sizeof *hash_table);

  // 'l' for linear, else for double
  long long insert_collision = 0;
  char chooseMethod = 'l';

  double startLinear = timing();
  for (size_t i = 0; i < total_keys; i++)
  {
    insert(table[i], m, hash_table, chooseMethod, &insert_collision);
  }
  double endLinear = timing();

  double linear_time = endLinear - startLinear;

  linear_times[i] = linear_time;

  printf("Load factor: %f\n", load_factor);
  printf("Insert collision linear: %lld\n", insert_collision);
  printf("Timing on linear probing: %.2f (ns)\n", linear_time);

  linear_collisions[i] = insert_collision;

  memset(hash_table, 0, m * sizeof *hash_table);

  insert_collision = 0;
  chooseMethod = 'd';

  double startDouble = timing();
  for (size_t i = 0; i < total_keys; i++)
  {
    insert(table[i], m, hash_table, chooseMethod, &insert_collision);
  }
  double endDouble = timing();

  double double_time = endDouble - startDouble;

  double_times[i] = double_time;

  printf("Insert collision double: %lld\n", insert_collision);
  printf("Timing on double hashing: %.2f (ns)\n", double_time);
  // int pos = search(table[500], m, hash_table, chooseMethod);
  // printf("%d\n", pos);

  double_collisions[i] = insert_collision;

  free(hash_table);
}

void printArray(double *array)
{
  printf("\n\n[");
  for (int i = 0; i < 5; i++)
    printf("%.2f, ", array[i]);
  printf("]");
}

int main()
{
  srand(0);
  size_t m = findPrime(10000000);
  int *keys = createUniqueIntTable(m);
  double load_factors[] = {0.5, 0.8, 0.9, 0.99, 1};

  double linear_times[5];
  double double_times[5];
  double linear_collisions[5];
  double double_collisions[5];

  int N = sizeof(load_factors) / sizeof(load_factors[0]);

  for (int i = 0; i < N; i++)
  {
    size_t total_keys = (size_t)(load_factors[i] * m);
    hashTimingLoadFactor(load_factors[i], m, keys, total_keys, i, linear_times, double_times, linear_collisions, double_collisions);
  }

  // For easy copy over to python
  printArray(linear_times);
  printArray(double_times);
  printArray(linear_collisions);
  printArray(double_collisions);
  printf("\n");

  free(keys);
  return 0;
}
