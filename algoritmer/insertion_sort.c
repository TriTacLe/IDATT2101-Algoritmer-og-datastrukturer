#include <stdio.h>

void insertion_sort(int numbers[], size_t length_of_numbers)
{
  for (int position = 1; position < length_of_numbers; position++)
  {
    int newValue = numbers[position];
    int leftIdx = position - 1;

    while (leftIdx >= 0 && numbers[leftIdx] > newValue)
    {
      numbers[leftIdx + 1] = numbers[leftIdx];
      leftIdx--;
    }
    numbers[leftIdx + 1] = newValue;
  }
}

int main()
{
  int numbs[] = {8, 7, 6, 5, 4};
  int length_of_numbers = sizeof(numbs) / sizeof(numbs[0]);
  insertion_sort(numbs, length_of_numbers);
  for (int i = 0; i < length_of_numbers; i++)
  {
    printf("%d", numbs[i]);
  }

  return 0;
}