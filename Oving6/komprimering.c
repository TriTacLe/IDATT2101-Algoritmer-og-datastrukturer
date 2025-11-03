#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdbool.h>

// Tall iht delfate algoritmen som brukes
#define SEARCH_BUFFER 32768 // 32kb search buffer
#define LOOKAHEAD_SIZE 258  // max match length (derfor uint16_t)
#define MIN_MATCH 3         // minimum match lengde for å gi meninig

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
};

const char* getFileName(int t) {
  return FILE_NAME_STRING[t];
}

// Compare Arrays of strings
bool isInArr(const char* str) {
  int len = sizeof(FILE_NAME_STRING) / sizeof(FILE_NAME_STRING[0]);
  for (int i = 0; i < len; i++) {
    if (strcmp(FILE_NAME_STRING[i], str)) {
      return true;
    }
  }
  return false;
}

struct Match {
  uint16_t length;
  uint16_t distance;
};

struct LZtoken {
  uint8_t type; // literal 0 or match 1
  union
  {
    uint8_t literal;
    struct Match match;
  } literalOrMatch;
};

struct Match findBestMatch(uint8_t *input, uint32_t position, uint32_t inputSize) {
  struct Match best = {0, 0};
  uint32_t searchStart = (position > SEARCH_BUFFER) ? position - SEARCH_BUFFER : 0;
  uint32_t maxLength = (LOOKAHEAD_SIZE > (inputSize - position)) ? inputSize - position : LOOKAHEAD_SIZE;

  for (uint32_t i = searchStart; i < position; i++)
  {
    uint16_t matchLength = 0;
    while (matchLength < maxLength && input[i + matchLength] == input[position + matchLength])
    {
      matchLength++;
    }
    if (matchLength > best.length)
    {
      best.length = matchLength;
      best.distance = position - i;
      if (best.length == maxLength)
        break;
    }
  }
  return best;
}

size_t lzCompress(uint8_t *input, uint32_t inputSize, struct LZtoken *output) {
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
  return outputPosition;
}

static void write_u16_le(FILE *filePointer, uint16_t value) {
  uint8_t bytes[2] = {(uint8_t)(value & 0xFF), (uint8_t)((value >> 8) & 0xFF)};
  fwrite(bytes, 1, 2, filePointer);
}

static void write_u64_le(FILE *fp, uint64_t v) {
  uint8_t b[8];
  for (int i = 0; i < 8; i++)
    b[i] = (uint8_t)((v >> (8 * i)) & 0xFF);
  fwrite(b, 1, 8, fp);
}

int writeTokensToFile(const char *outputFileName,
                      const struct LZtoken *tokens,
                      size_t tokenCount,
                      uint64_t originalSize) {
  FILE *filePointer = fopen(outputFileName, "wb");

  write_u64_le(filePointer, originalSize);
  write_u64_le(filePointer, (uint64_t)tokenCount);

  for (size_t i = 0; i < tokenCount; i++)
  {
    fputc(tokens[i].type, filePointer);
    if (tokens[i].type == 0)
    {
      fputc(tokens[i].literalOrMatch.literal, filePointer);
    }
    else
    {
      write_u16_le(filePointer, tokens[i].literalOrMatch.match.length);
      write_u16_le(filePointer, tokens[i].literalOrMatch.match.distance);
    }
  }

  fclose(filePointer);
  return 0;
}

/**
 * Use long because one of the file is 100MB
 */
static long int findFileSize(const char *fileName) {
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

int main(int argc, char** argv) {
  if (argc != 2) {
    printf("Error: You did not pass in an argument/file to compress");
    return 1;
  }

  const char* inputFileName = argv[1];
  if (!isInArr(inputFileName)) {
    printf("Error: Your argument \"%s\" is not in list of valid filenames", inputFileName);
    return 1;
  }

  // oblig file
  const char *outputFileName = "output.lz";

  long int inputFileSize = findFileSize(inputFileName);
  (inputFileSize != -1) ? printf("File size: %ld bytes\n", inputFileSize) : printf("File is emtpy\n");

  FILE* filePointer = fopen(inputFileName, "rb");
  if (!filePointer)
    perror("fopen input\n");

  uint8_t *inputBuffer = (uint8_t *)malloc((size_t)inputFileSize);

  size_t readCount = fread(inputBuffer, 1, (size_t)inputFileSize, filePointer);
  if (readCount != (size_t)inputFileSize)
    printf("Didnt read\n");

  fclose(filePointer);

  struct LZtoken *tokens = (struct LZtoken *)malloc((size_t)inputFileSize * sizeof(struct LZtoken));
  if (!tokens)
    fprintf(stderr, "malloc tokens failed\n");

  size_t tokenCount = lzCompress(inputBuffer, (uint32_t)inputFileSize, tokens);
  printf("Produced %zu tokens\n", tokenCount);

  writeTokensToFile(outputFileName, tokens, tokenCount, (uint64_t)inputFileSize);

  printf("Wrote compressed file to: %s\n", outputFileName);
  long int outputFileSize = findFileSize(outputFileName);
  (outputFileSize != -1) ? printf("File size: %ld bytes\n", outputFileSize) : printf("File is emtpy\n");
  free(tokens);
  free(inputBuffer);
  return 0;
}