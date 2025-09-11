// Valgte oppgave 2
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

double timing(void)
{
  struct timespec time_spec;
  clock_gettime(CLOCK_MONOTONIC_RAW, &time_spec);
  return (double)time_spec.tv_sec * 1e9 + (double)time_spec.tv_nsec;
}

void swap_interger(int *numb1, int *numb2)
{
  int temp = *numb2;
  *numb2 = *numb1;
  *numb1 = temp;
}

int median3sort(int *numb, int leftIdx, int rightIdx)
{
  int median = (leftIdx + rightIdx) / 2;

  if (numb[leftIdx] > numb[median])
  {
    swap_interger(&numb[leftIdx], &numb[median]);
  }
  if (numb[leftIdx] > numb[rightIdx])
  {
    swap_interger(&numb[leftIdx], &numb[rightIdx]);
  }
  if (numb[median] > numb[rightIdx])
  {
    swap_interger(&numb[median], &numb[rightIdx]);
  }
  return median;
}

int partision(int *numb, int leftIdx, int rightIdx)
{
  int endLeft, endRight;
  int median = median3sort(numb, leftIdx, rightIdx);
  int pivot = numb[median];

  swap_interger(&numb[median], &numb[rightIdx - 1]);

  endLeft = leftIdx;
  endRight = rightIdx - 1;
  for (;;)
  {
    while (numb[++endLeft] < pivot)
      ;
    while (numb[--endRight] > pivot)
      ;
    if (endLeft >= endRight)
      break;
    swap_interger(&numb[endLeft], &numb[endRight]);
  }
  swap_interger(&numb[endLeft], &numb[rightIdx - 1]);

  return endLeft;
}

void counting_sort(int *in, int length, int lower_bound, int upper_bound)
{
  int range = upper_bound - lower_bound + 1;

  int *count = (int *)calloc(range, sizeof(int));

  for (int i = 0; i < length; i++)
    count[in[i] - lower_bound]++;

  for (int i = 1; i < range; i++)
    count[i] += count[i - 1];

  int *out = (int *)malloc(length * sizeof(int));

  for (int i = length - 1; i >= 0; --i)
  {
    int idx = in[i] - lower_bound;
    out[--count[idx]] = in[i];
  }

  for (int i = 0; i < length; i++)
    in[i] = out[i];

  free(count);
  free(out);
}

void quicksort(int *numb, int leftIdx, int rightIdx)
{
  if (rightIdx - leftIdx > 2)
  {
    int pivot = partision(numb, leftIdx, rightIdx);
    quicksort(numb, leftIdx, pivot - 1);
    quicksort(numb, pivot + 1, rightIdx);
  }
  else
  {
    median3sort(numb, leftIdx, rightIdx);
  }
}

void quicksort_optimalized(int *numb, int leftIdx, int rightIdx, int lower_bound, int upper_bound)
{
  if (rightIdx - leftIdx > 2)
  {
    int pivot_idx = partision(numb, leftIdx, rightIdx);
    int pivot_value = numb[pivot_idx];

    // Tabell 1 (Nedre)
    int interval_left = pivot_idx - leftIdx;
    int range_left = pivot_value - lower_bound + 1;
    if (interval_left > 0)
    {
      if (range_left < interval_left)
      {
        counting_sort(&numb[leftIdx], interval_left, lower_bound, pivot_value);
      }
      else
      {
        quicksort_optimalized(numb, leftIdx, pivot_idx - 1, lower_bound, pivot_value);
      }
    }

    // Tabell 2 (Ovre)
    int interval_right = rightIdx - pivot_idx;
    int range_right = upper_bound - pivot_value + 1;
    if (interval_right > 0)
    {
      if (range_right < interval_right)
      {
        counting_sort(&numb[pivot_idx + 1], interval_right, pivot_value, upper_bound);
      }
      else
      {
        quicksort_optimalized(numb, pivot_idx + 1, rightIdx, pivot_value, upper_bound);
      }
    }
  }
  else
  {
    median3sort(numb, leftIdx, rightIdx);
  }
}

int *createTable(int size, int lowerBound, int upperBound)
{
  //
  int *table = (int *)malloc(size * sizeof(int));
  for (int i = 0; i < size; i++)
  {
    int random_number = rand() % (upperBound - (lowerBound) + 1) + (lowerBound);
    table[i] = random_number;
  }
  return table;
}

void prepareForOptQuickSort(int *numb, int size)
{
  int min_idx = 0, max_idx = 0;
  int max_value = numb[0], min_value = numb[0];
  for (int i = 0; i < size; i++)
  {
    if (numb[i] > numb[max_idx])
    {
      max_idx = i;
      max_value = numb[i];
    }

    if (numb[i] < numb[min_idx])
    {
      min_idx = i;
      min_value = numb[i];
    }
  }
  if (max_idx != size - 1)
  {
    swap_interger(&numb[max_idx], &numb[size - 1]);
    if (min_idx == size - 1)
      min_idx = max_idx;
  }
  if (min_idx != 0)
  {
    swap_interger(&numb[0], &numb[min_idx]);
  }
  if (size > 2)
  {
    // Start quicksort
    quicksort_optimalized(numb, 1, size - 2, min_value, max_value);
  }
}

void testTableOrder(int *table, int size)
{
  for (int i = 1; i < size; i++)
  {
    if (table[i] < table[i - 1])
    {
      printf("Test: table[i] >= table[i-1]: failed\n");
      return;
    }
  }
  printf("Test: table[i] >= table[i-1]: success\n");
}

int getSumTable(int *table, int size)
{
  if (size == 0)
    return 0;

  return table[size - 1] + getSumTable(table, size - 1);
}

void testSumTable(int *tableBefore, int *tableAfter, int size)
{
  int sumBefore = getSumTable(tableBefore, size);
  int sumAfter = getSumTable(tableAfter, size);
  // printf("%d\n", sumAfter);
  // printf("%d\n", sumBefore);
  if (sumAfter != sumBefore)
    printf("Test sum failed\n");
  else
    printf("Test: sum: success\n");
}

void run()
{
  srand(time(NULL));
  const int size = 10000000; // n verdi
  int *newTableSpread = createTable(size, -1000, 1000);
  int *tableSpread1 = malloc(size * sizeof *tableSpread1);
  int *tableSpread2 = malloc(size * sizeof *tableSpread2);
  memcpy(tableSpread1, newTableSpread, size * sizeof *tableSpread1);
  memcpy(tableSpread2, newTableSpread, size * sizeof *tableSpread2);

  int *newTableDups = createTable(size, -9, 9);
  int *tableDups1 = malloc(size * sizeof *tableDups1);
  int *tableDups2 = malloc(size * sizeof *tableDups2);
  memcpy(tableDups1, newTableDups, size * sizeof *tableDups1);
  memcpy(tableDups2, newTableDups, size * sizeof *tableDups2);

  // Optimalisert quicksort
  // Spread
  double startOpt1 = timing();
  prepareForOptQuickSort(tableSpread1, size);
  double endOpt1 = timing();
  int resultOptSpread = endOpt1 - startOpt1;
  // Duplicates
  double startOpt2 = timing();
  prepareForOptQuickSort(tableDups1, size);
  double endOpt2 = timing();

  // Vanlig quicksort
  // Spread
  double startV1 = timing();
  quicksort(tableSpread2, 0, size - 1);
  double endV1 = timing();
  // Dups
  double startV2 = timing();
  quicksort(tableDups2, 0, size - 1);
  double endV2 = timing();
  int resultOptDups = endOpt2 - startOpt2;

  printf("\n");

  printf("Spread:\n");
  printf("Quicksort optimized, time: %d (ns)\n", resultOptSpread);
  printf("Quicksort normal   , time: %lf (ns)\n\n", endV1 - startV1);

  printf("Duplicates:\n");
  printf("Quicksort optimzed, time: %d (ns)\n", resultOptDups);
  printf("Quicksort normal  , time: %lf (ns)\n\n", endV2 - startV2);

  // Test dups

  testSumTable(newTableDups, tableDups1, size);
  testTableOrder(tableDups1, size);
  testSumTable(newTableDups, tableDups2, size);
  testTableOrder(tableDups2, size);

  // Test spread
  testSumTable(newTableSpread, tableSpread1, size);
  testTableOrder(tableSpread1, size);
  testSumTable(newTableSpread, tableSpread2, size);

  testTableOrder(tableSpread2, size);

  // Same process on already sorted table:
  double startRedoOptSpread = timing();
  prepareForOptQuickSort(tableSpread1, size);
  double endRedoOptSpread = timing();
  int resultRedoOptSpread = endRedoOptSpread - startRedoOptSpread;
  // Duplicates
  double startRedoOptDups = timing();
  prepareForOptQuickSort(tableDups1, size);
  double endRedoOptDups = timing();
  int resultRedoOptDups = endRedoOptDups - startRedoOptDups;

  printf("\nSorting a sorted table:\n");
  printf("Spread:\n");
  printf("Quicksort optimized, time: %d (ns)\n", resultRedoOptSpread);
  printf("Duplicates:\n");
  printf("Quicksort optimized, time: %d (ns)\n\n", resultRedoOptDups);

  // Test om det tar dobbelt så lang tid:
  (resultOptSpread * 2 > resultRedoOptSpread) ? printf("not n^2 problem for spread: %d, %d\n", resultOptSpread, resultRedoOptSpread) : printf("n^2 problem for spread: %d, %d\n", resultOptSpread, resultRedoOptSpread);
  (resultOptDups * 2 > resultRedoOptDups) ? printf("not n^2 problem for dups: %d, %d\n", resultOptDups, resultRedoOptDups) : printf("n^2 problem for spread: %d, %d\n", resultOptSpread, resultRedoOptSpread);

  free(newTableSpread);
  free(tableSpread1);
  free(tableSpread2);
  free(newTableDups);
  free(tableDups1);
  free(tableDups2);
}

// MAIN

int main(void)
{
  run();
  return 0;
}
