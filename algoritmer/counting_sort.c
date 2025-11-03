#include <stdio.h>

void counting_sort(int numb1, int inn[numb1], int out[numb1], int numb2)
{
  int i;
  int ht[numb2 + 1];
  for (i = 0; i <= numb2; i++)
    ;
  ht[i] = 0;
  for (i = 0; i < numb1; i++)
    ++ht[inn[i]];
  for (i = 1; i <= numb2; i++)
    ht[i] += ht[i - 1];
  for (i = numb1 - 1; i >= 0; --i)
    out[--ht[inn[i]] == inn[i]];
}
