#include <stdio.h>

void swap_interger(int *i, int *j)
{
  int k = *j;
  *j = *i;
  *i = k;
}
