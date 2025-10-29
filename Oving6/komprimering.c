#include <stdlib.h>
#include <stdio.h>

// Tall iht delfate algoritmen som brukes
#define WINDOW_SIZE 32768  // 32kb search buffer
#define LOOKAHEAD_SIZE 258 // max match length (derfor uint16_t)
#define MIN_MATCH 3        // minimum match lengde for å gi meninig

struct Match
{
  uint16_t length;
  uint16_t distance;
};

struct LZtoken
{
  uint8_t type; // literal 0 or match 1
  union
  {
    char literal;
    struct Match match;
  } literalOrMatch;
};

struct Match findBestMatch(uint8_t *input, uint32_t position, uint32_t inputSize)
{
  struct Match best = {0, 0};
  uint32_t searchStart = 0;
  uint32_t maxLength = 0;

  for (uint32_t i = searchStart; i < position; i++)
  {
  }
  return best;
}

/**
 * @param input: Input data å komprimere
 * @param input_size: Størrelse på input
 * @param output: Array til å lagre tokens (må være allokert)
 * @return: Antall tokens produsert
 */
size_t lzCompress(uint8_t *input, uint32_t inputSize, struct LZtoken *output)
{
  uint32_t position = 0;
  size_t outputPosition = 0;
  while (position < inputSize)
  {
    struct Match match = findBestMatch(input, position, inputSize);
    if (match.length >= MIN_MATCH)
    {
      output[outputPosition].type = 1;
      output[outputPosition].literalOrMatch.match = match;
      outputPosition++;
      position += match.length;
    }
    else
    {
      output[outputPosition].type = 0;
      output[outputPosition].literalOrMatch.literal = input[position];
      outputPosition++;
      position++;
    }
  }
}

/**
 * Use long because one of the file is 100MB
 */
long int findFileSize(char fileName[])
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
  long int res = findFileSize(fileName);
  (res != -1) ? printf("File size: %ld bytes\n", res) : printf("File is emtpy\n");
  return 0;
}