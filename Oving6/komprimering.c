#include <stdlib.h>
#include <stdio.h>

long int findSize(char fileName[])
{
  FILE *filePointer;

  filePointer = fopen(fileName, "rb");

  if (filePointer == NULL)
  {
    printf("File does not exist");
    return -1;
  }

  fseek(filePointer, 0, SEEK_END);

  long int fileSize = ftell(filePointer);
  fclose(filePointer);
  return fileSize;
}

int main()
{
  // oblig file
  char fileName[] = {"diverse.lyx"};
  long int res = findSize(fileName);
  (res != -1) ? printf("File size: %ld bytes\n", res) : printf("File is emtpy\n");
  return 0;
}