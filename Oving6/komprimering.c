#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <bool.h>

// Can maybe use later ???
/*typedef enum {
  OPG_6,
  JULES_VERNE,
  FL_TXT,
  FL_LYX,
  ENWIK8,
  TEST
} FileName;*/

const char FILE_NAME_STRING[][41] = {
  "opg6-kompr.lynx",                            // 34 kB lyx
  "Twenty_thousand_leagues_under_the_sea.txt",  // 595 kB txt
  "diverse.txt",                                // forelesning 17 kB txt
  "diverse.lyx",                                // forelesning 179 kB lyx
  "enwik8.txt",                                 // 100 MB txt
  "test.txt"
}

const char* getFileName(int t) {
  return FILE_NAME_STRING[t];
}

// Compare
bool isInArr(char* str) {
  int len = sizeof(FILE_NAME_STRING) / sizeof(FILE_NAME_STRING[0]);
  for (int i = 0; i < len; i++) {
    if (strcmp(FILE_NAME_STRING[i], str) {
      return true;
    }
  }
  return false;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    printf("Error: You did not pass in an argument/file to compress")
    return 1;
  }

  char input[41] = argv[1];
  if (!isInArr(input)) {
    printf("Error: Your argument \"%s\" is not in list of valid filenames", input);
    return 1;
  }


  FILE *filePointer;

  // Open 'forelesning.lyx'

  filePointer = fopen(input, "w");
  // filePointer = fopen("Twenty_thousand_leagues_under_the_sea.txt", "w");

  // copy contents of filePointer to new file

  if (filePointer == NULL)
    exit(0);

  fclose(filePointer);
  return 0;
}