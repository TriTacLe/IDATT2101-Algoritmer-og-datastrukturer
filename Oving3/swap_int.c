#include <stdio.h>

void swap_interger(int *i, int *j)
{
  int temp = *j;
  *j = *i;
  *i = temp;
}
