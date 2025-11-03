#include <stdio.h>
#include <stdlib.h>

int main()
{
  // malloc() = a function that dynamically allocates a specific number of bytes in memory
  // reserving space in the heap
  // Must free space and avoid dangling pointer
  // Accepts size of something as a argument

  int number = 5; // number of elements in cars

  char *cars = malloc(number * sizeof(char));

  free(cars);
  cars = NULL;
  return 0;
}