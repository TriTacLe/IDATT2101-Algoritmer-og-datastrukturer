// Hvis intervallet til deltabellen spenner et mindre området enn størrelsen/lengden til deltabellen kan den sorteres kjapt med tellesortering
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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

  // Delingsposisjon
  return endLeft;
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
  int numbers[10] = {};
  // int length = sizeof(numbers) / sizeof(numbers[0]);

  for (int i = 0; i <= list_length; i++)
  {
    int random_number = (rand() % 21) - 10;
    // printf("%d ", random_number);
    numbers[i] = random_number;
    // printf("%d ", numbers[i]);
  }
  quicksort(numbers, 0, list_length - 1);

  for (int i = 0; i < list_length; i++)
  {
    printf("%d ", numbers[i]);
  }
  printf("\n");

  return 0;
}
