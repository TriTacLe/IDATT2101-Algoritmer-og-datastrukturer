// Hvis intervallet til deltabellen spenner et mindre området enn størrelsen/lengden til deltabellen kan den sorteres kjapt med tellesortering
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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
void counting_sort(int in[], int length)
{
  int max = 0;
  for (int i = 0; i < length; i++)
    if (in[i] > max)
      max = in[i];

  int *count = (int *)calloc(max + 1, sizeof(int));

  for (int i = 0; i < length; i++)
    count[in[i]]++;

  for (int i = 1; i <= max; i++)
    count[i] += count[i - 1];

  int *out = (int *)malloc(length * sizeof(int));
  for (int i = length - 1; i >= 0; i--)
  {
    out[count[in[i]] - 1] = in[i];
    count[in[i]]--;
  }
  for (int i = 0; i < length; i++)
    in[i] = out[i];
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

int main(void)
{
  srand(time(NULL));

  int list_length = 10;
  int numb[10] = {};
  int numb2[10] = {};
  int numb3[10] = {};
  int length = sizeof(numb) / sizeof(numb[0]);

  for (int i = 0; i < list_length; i++)
  {
    int random_number = (rand() % 21) - 10;
    numb[i] = random_number;
    numb2[i] = random_number;
    // printf("%d ", numb[i]);
  }
  // printf("\n");

  // Optimalisert quicksort
  double start = timing();

  // Finn max og min før sorteringen begynner
  int min_idx = 0, max_idx = 0;
  for (int i = 0; i < length; i++)
  {
    if (numb[i] > numb[max_idx])
      max_idx = i;
    if (numb[i] < numb[min_idx])
      min_idx = i;
  }
  if (max_idx != length - 1)
  {
    swap_interger(&numb[max_idx], &numb[length - 1]);
    if (min_idx == length - 1)
      min_idx = max_idx;
  }
  if (min_idx != 0)
  {
    swap_interger(&numb[0], &numb[min_idx]);
  }
  if (length > 2)
    // Start quicksort
    quicksort(numb, 1, list_length - 2);

  double end = timing();

  // Vanlig quicksort
  double startV = timing();
  quicksort(numb2, 0, list_length - 1);
  double endV = timing();

  // Tellesortering
  double startT = timing();
  counting_sort(numb3, length);
  double endT = timing();

  for (int i = 0; i < list_length; i++)
  {
    printf("%d ", numb[i]);
  }

  printf("\n");
  printf("Quicksort optimisert, tid: %lf (ns)", end - start);
  printf("\n");
  printf("Quicksort vanlig, tid: %lf (ns)", endV - startV);
  printf("\n");
  printf("Tellesort, tid: %lf (ns)", endT - startT);
  return 0;
}
