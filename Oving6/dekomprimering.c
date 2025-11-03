#include <stdlib.h>
#include <stdio.h>

int main()
{
  FILE *filePointer;

  // oblig file
  filePointer = fopen("diverse.lyx", "w");
  // filePointer = fopen("Twenty_thousand_leagues_under_the_sea.txt", "w");

  fclose(filePointer);
  return 0;
}