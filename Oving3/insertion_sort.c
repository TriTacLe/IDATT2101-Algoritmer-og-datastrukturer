void insertion_sort(int numbers[])
{
  int length_of_numbers = sizeof(numbers) / sizeof(numbers[0]);
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