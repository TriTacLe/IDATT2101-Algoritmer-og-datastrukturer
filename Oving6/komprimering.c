#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdbool.h>

#define SEARCH_BUFFER 32768
#define LOOKAHEAD_SIZE 258
#define MIN_MATCH 3

// Generalisering av lengde til arrays
#define ARRAY_LEN(a) (sizeof(a) / sizeof(a[0]))

// const char FILE_NAME_STRING[][41] = {
//     "opg6-kompr.lyx",                            // 34 kB lyx
//     "Twenty_thousand_leagues_under_the_sea.txt", // 595 kB txt
//     "diverse.txt",                               // forelesning 17 kB txt
//     "diverse.lyx",                               // forelesning 179 kB lyx
//     "enwik8.txt"                                 // 100 MB txt
//     "julesverne.txt"};

// const char *getFileName(int t)
// {
//   return FILE_NAME_STRING[t];
// }

// Compare Arrays of strings
// bool isInArr(const char *str)
// {
//   int len = sizeof(FILE_NAME_STRING) / sizeof(FILE_NAME_STRING[0]);
//   for (int i = 0; i < len; i++)
//   {
//     if (strcmp(FILE_NAME_STRING[i], str) == 0)
//     {
//       return true;
//     }
//   }
//   return false;
// }

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
    uint8_t literal;
    struct Match match;
  } literalOrMatch;
};

struct Match findBestMatch(uint8_t *input, uint32_t position, uint32_t inputSize)
{
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
  return outputPosition;
}

static void write_u16_le(FILE *filePointer, uint16_t value)
{
  uint8_t bytes[2] = {(uint8_t)(value & 0xFF), (uint8_t)(value >> 8 & 0xFF)};
  fwrite(bytes, 1, 2, filePointer);
}

static void write_u64_le(FILE *fp, uint64_t v)
{
  uint8_t b[8];
  for (int i = 0; i < 8; i++)
    b[i] = (uint8_t)((v >> (8 * i)) & 0xFF);
  fwrite(b, 1, 8, fp);
}

int writeTokensToFile(const char *outputFileName,
                      const struct LZtoken *tokens,
                      size_t tokenCount,
                      uint64_t originalSize)
{
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
Huffman-method
**/
struct HuffmannNode
{
  uint16_t symbol;
  uint32_t frequency;
  struct HuffmannNode *left;
  struct HuffmannNode *right;
};

struct HuffmannCode
{
  uint32_t bits;
  uint8_t length;
};

// 3-258 til (kode, ekstraBits, baseLength)
struct LengthCodeInfo
{
  uint16_t code; // 257-285
  u_int8_t extraBits;
  u_int16_t baseLength;
};
struct DistanceCodeInfo
{
  uint16_t code; // 0 - 29
  u_int8_t extraBits;
  u_int16_t baseLength;
};

static const uint16_t lenArrExtraBits[] = {
    3, 11, 19, 35, 67, 131, 3};

typedef struct
{
  uint16_t base;
  uint8_t extra;
} BaseExtra;

static const BaseExtra LEN_TBL[29] = {
    {3, 0}, {4, 0}, {5, 0}, {6, 0}, {7, 0}, {8, 0}, {9, 0}, {10, 0}, {11, 1}, {13, 1}, {15, 1}, {17, 1}, {19, 2}, {23, 2}, {27, 2}, {31, 2}, {35, 3}, {43, 3}, {51, 3}, {59, 3}, {67, 4}, {83, 4}, {99, 4}, {115, 4}, {131, 5}, {163, 5}, {195, 5}, {227, 5}, {258, 0}};

void getLengthCode(uint16_t length,
                   uint16_t *outCode,
                   uint8_t *outExtraBits,
                   uint16_t *outExtraValue)
{
  if (length < 3)
    length = 3;
  if (length > 258)
    length = 258;

  if (length == 258)
  {
    *outCode = 285;
    *outExtraBits = 0;
    *outExtraValue = 0;
    return;
  }

  // 257–285 korresponderer til indeks 0–28
  for (uint16_t idx = 0; idx < 29; idx++)
  {
    uint16_t base = LEN_TBL[idx].base;
    uint8_t eb = LEN_TBL[idx].extra;
    uint32_t span = (eb == 0) ? 1u : (1u << eb);
    uint32_t maxv = (uint32_t)base + span - 1u;

    if (length >= base && length <= maxv)
    {
      *outCode = (uint16_t)(257 + idx);
      *outExtraBits = eb;
      *outExtraValue = (uint16_t)(length - base);
      return;
    }
  }

  // Fallback (skal ikke skje)
  *outCode = 285;
  *outExtraBits = 0;
  *outExtraValue = 0;
}

/* // List of base values
static const uint16_t extraBitsArr[] = {
    1, 5, 9, 17,
    33, 65, 129, 257,
    513, 1025, 2049, 4097,
    8193, 16385, 24577}; */

static const BaseExtra DIST_TBL[30] = {
    {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 1}, {7, 1}, {9, 2}, {13, 2}, {17, 3}, {25, 3}, {33, 4}, {49, 4}, {65, 5}, {97, 5}, {129, 6}, {193, 6}, {257, 7}, {385, 7}, {513, 8}, {769, 8}, {1025, 9}, {1537, 9}, {2049, 10}, {3073, 10}, {4097, 11}, {6145, 11}, {8193, 12}, {12289, 12}, {16385, 13}, {24577, 13}};

void getDistanceCode(uint32_t distance,
                     uint16_t *outCode,
                     uint8_t *outExtraBits,
                     uint16_t *outExtraValue)
{
  if (distance < 1)
    distance = 1;
  if (distance > 32768)
    distance = 32768; // DEFLATE-vindu

  // Finn koden ved å lete i samme tabell som dekoderen bruker
  for (uint16_t code = 0; code < 30; code++)
  {
    uint16_t base = DIST_TBL[code].base;
    uint8_t eb = DIST_TBL[code].extra;
    uint32_t span = (eb == 0) ? 1u : (1u << eb);
    uint32_t maxv = (uint32_t)base + span - 1u;

    if (distance >= base && distance <= maxv)
    {
      *outCode = code;
      *outExtraBits = eb;
      *outExtraValue = (uint16_t)(distance - base);
      return;
    }
  }

  // Fallback (skal ikke skje med distance <= 32768)
  *outCode = 29; // siste kode
  *outExtraBits = 13;
  *outExtraValue = (uint16_t)(distance - 24577);
}

void countFrequencies(struct LZtoken *tokens,
                      size_t token_count,
                      uint32_t *literalFreq, // Array[286]
                      uint32_t *distFreq)    // Array[30]
{
  // Nullstill arrays
  for (int i = 0; i < 286; i++)
  {
    literalFreq[i] = 0;
  }
  for (int i = 0; i < 30; i++)
  {
    distFreq[i] = 0;
  }

  for (size_t i = 0; i < token_count; i++)
  {
    if (tokens[i].type == 0)
    {
      uint8_t literal = tokens[i].literalOrMatch.literal;
      literalFreq[literal]++;
    }
    else
    {
      uint16_t length = tokens[i].literalOrMatch.match.length;
      uint16_t distance = tokens[i].literalOrMatch.match.distance;

      uint16_t lenCode, lenExtra;
      uint8_t lenExtraBits;
      getLengthCode(length, &lenCode, &lenExtraBits, &lenExtra);

      uint16_t distCode, distExtra;
      uint8_t distExtraBits;
      getDistanceCode(distance, &distCode, &distExtraBits, &distExtra);

      // Tell opp
      literalFreq[lenCode]++;
      distFreq[distCode]++;
    }
  }

  // END marker (alltid én gang)
  literalFreq[256]++;
}

struct PriorityQueue
{
  struct HuffmannNode **nodes; // array av pekere til noder
  size_t count;
  size_t capacity;
};

struct PriorityQueue *priorityQueueInit(size_t capacity)
{
  struct PriorityQueue *priorityQueue = malloc(sizeof(struct PriorityQueue));
  priorityQueue->nodes = malloc(capacity * sizeof *priorityQueue->nodes);
  priorityQueue->capacity = capacity;
  priorityQueue->count = 0;
  return priorityQueue;
}

void priorityQueueInsert(struct PriorityQueue *priorityQueue, struct HuffmannNode *huffmannNode)
{
  if (priorityQueue->count >= priorityQueue->capacity)
  {
    printf("Priority queue full");
    return;
  }
  priorityQueue->nodes[priorityQueue->count++] = huffmannNode;
}

struct HuffmannNode *priorityQueueExtractMin(struct PriorityQueue *priorityQueue)
{
  size_t minIndx = 0;
  for (size_t i = 1; i < priorityQueue->count; i++)
  {
    if (priorityQueue->nodes[i]->frequency < priorityQueue->nodes[minIndx]->frequency)
    {
      minIndx = i;
    }
  }
  struct HuffmannNode *minNode = priorityQueue->nodes[minIndx];
  priorityQueue->nodes[minIndx] = priorityQueue->nodes[priorityQueue->count - 1];
  priorityQueue->count--;
  return minNode;
}

struct HuffmannNode *buildHuffmannTree(uint32_t *frequencies, size_t symbolCount)
{
  struct PriorityQueue *priorityQueue = priorityQueueInit(symbolCount);

  // Legg til
  for (size_t i = 0; i < symbolCount; i++)
  {
    if (frequencies[i] > 0)
    {
      struct HuffmannNode *huffmannNode = malloc(sizeof(struct HuffmannNode));
      huffmannNode->symbol = i;
      huffmannNode->frequency = frequencies[i];
      huffmannNode->left = NULL;
      huffmannNode->right = NULL;
      priorityQueueInsert(priorityQueue, huffmannNode);
    }
  }

  // Hvis bare ett symbol
  if (priorityQueue->count == 1)
  {
    struct HuffmannNode *only = priorityQueueExtractMin(priorityQueue);
    struct HuffmannNode *root = malloc(sizeof(struct HuffmannNode));
    root->symbol = 0;
    root->frequency = only->frequency;
    root->left = only;
    root->right = NULL;
    free(priorityQueue->nodes);
    free(priorityQueue);
    return root;
  }

  // Kombiner to minste noder
  while (priorityQueue->count > 1)
  {
    struct HuffmannNode *right = priorityQueueExtractMin(priorityQueue);
    struct HuffmannNode *left = priorityQueueExtractMin(priorityQueue);

    struct HuffmannNode *parent = malloc(sizeof(struct HuffmannNode));
    parent->symbol = 0;
    parent->frequency = left->frequency + right->frequency;
    parent->left = left;
    parent->right = right;

    priorityQueueInsert(priorityQueue, parent);
  }

  // Siste node er root
  struct HuffmannNode *root = priorityQueueExtractMin(priorityQueue);
  free(priorityQueue->nodes);
  free(priorityQueue);
  return root;
}

void generateBitCodes(struct HuffmannNode *node, struct HuffmannCode *codes, uint32_t code, uint8_t length)
{
  if (node->left == NULL && node->right == NULL)
  {
    codes[node->symbol].bits = code;
    codes[node->symbol].length = length;
    return;
  }

  if (node->left != NULL)
    generateBitCodes(node->left, codes, code << 1 | 0, length + 1);

  // shift alle bits en plass til venstre (gange med 2) og legg til | 0 eller | 1 på slutten

  if (node->right != NULL)
    generateBitCodes(node->right, codes, code << 1 | 1, length + 1);
}

void buildCanonicalCodes(struct HuffmannCode *codes, size_t count)
{
  uint16_t bl_count[32] = {0};
  uint16_t next_code[32] = {0};
  uint16_t code = 0;

  // Tell hvor mange symboler per lengde
  for (size_t i = 0; i < count; i++)
    if (codes[i].length > 0)
      bl_count[codes[i].length]++;

  // Finn første kode per lengde (count/first/next-metoden)
  for (int bits = 1; bits <= 31; bits++)
  {
    code = (code + bl_count[bits - 1]) << 1;
    next_code[bits] = code;
  }

  // Tildel kanoniske koder (LSB-først rekkefølge)
  for (size_t n = 0; n < count; n++)
  {
    uint8_t len = codes[n].length;
    if (len != 0)
    {
      uint32_t c = next_code[len]++;
      // snu bitrekkefølgen fordi vi skriver LSB først
      uint32_t rev = 0;
      for (int i = 0; i < len; i++)
      {
        rev = (rev << 1) | (c & 1);
        c >>= 1;
      }
      codes[n].bits = rev;
    }
  }
}

struct BitWriter
{
  FILE *filePointer;
  uint32_t buffer;
  uint8_t bitCount;
};

void bitWriterInit(struct BitWriter *bitWriter, FILE *filePointer)
{
  bitWriter->filePointer = filePointer;
  bitWriter->buffer = 0;
  bitWriter->bitCount = 0;
}

void writeBits(struct BitWriter *bitwriter, uint32_t bits, uint8_t count)
{
  bitwriter->buffer |= (bits << bitwriter->bitCount);
  bitwriter->bitCount += count;

  // FLush
  while (bitwriter->bitCount >= 8)
  {
    fputc(bitwriter->buffer & 0xFF, bitwriter->filePointer);
    bitwriter->buffer >>= 8;
    bitwriter->bitCount -= 8;
  }
}

// Trenger dette fordi å slutten av filen kan vi f.eks. ha 3 bits igjen i bufferen. Vi må skrive dem som en hel byte ellers mister vi data
void bitWriterFlush(struct BitWriter *bitWriter)
{
  if (bitWriter->bitCount > 0)
  {
    fputc(bitWriter->buffer & 0xFF, bitWriter->filePointer);
    bitWriter->buffer = 0;
    bitWriter->bitCount = 0;
  }
}

void writeCodeLengths(FILE *filePointer, struct HuffmannCode *codes, size_t count)
{
  for (size_t i = 0; i < count; i++)
  {
    fputc(codes[i].length, filePointer);
  }
}
// delfate = LZ77 + huffmann
void writeDeflateAlgoCompressed(const char *filename, struct LZtoken *tokens, size_t tokenCount, struct HuffmannCode *literalCodes, struct HuffmannCode *distCodes, uint64_t originalSize)
{
  FILE *filePointer = fopen(filename, "wb");

  write_u64_le(filePointer, originalSize);

  writeCodeLengths(filePointer, literalCodes, 286);
  writeCodeLengths(filePointer, distCodes, 30);

  struct BitWriter bitwriter;
  bitWriterInit(&bitwriter, filePointer);

  for (size_t i = 0; i < tokenCount; i++)
  {
    // Hvis literal
    if (tokens[i].type == 0)
    {
      uint8_t literal = tokens[i].literalOrMatch.literal;
      writeBits(&bitwriter, literalCodes[literal].bits, literalCodes[literal].length);
    }
    else
    {
      uint16_t length = tokens[i].literalOrMatch.match.length;
      uint16_t distance = tokens[i].literalOrMatch.match.distance;

      uint16_t lengthCode, lengthExtraValues;
      uint8_t lengthExtraBits;
      getLengthCode(length, &lengthCode, &lengthExtraBits, &lengthExtraValues);

      uint16_t distCode, distExtraValues;
      uint8_t distExtraBits;
      getDistanceCode(distance, &distCode, &distExtraBits, &distExtraValues);

      writeBits(&bitwriter, literalCodes[lengthCode].bits, literalCodes[lengthCode].length);
      if (lengthExtraBits > 0)
        writeBits(&bitwriter, lengthExtraValues, lengthExtraBits);

      writeBits(&bitwriter, distCodes[distCode].bits, distCodes[distCode].length);
      if (distExtraBits > 0)
        writeBits(&bitwriter, distExtraValues, distExtraBits);
    }
  }
  writeBits(&bitwriter, literalCodes[256].bits, literalCodes[256].length);
  bitWriterFlush(&bitwriter);
  fclose(filePointer);
}

/**
 * Use long because one of the file is 100MB
 */
static long int findFileSize(const char *fileName)
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

int main(int argc, char **argv)
{
  if (argc != 2)
  {
    printf("Error: You did not pass in an argument/file to compress");
    return 1;
  }

  const char *inputFileName = argv[1];
  // if (!isInArr(inputFileName))
  // {
  //   printf("Error: Your argument \"%s\" is not in list of valid filenames", inputFileName);
  //   return 1;
  // }

  const char *outputFileName = "output.komprimert.lz";

  printf("Input file: %s\n", inputFileName);
  long int inputFileSize = findFileSize(inputFileName);
  // (inputFileSize != -1) ? printf("File size: %ld bytes\n", inputFileSize) : printf("File is emtpy\n");

  FILE *filePointer = fopen(inputFileName, "rb");
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
  // printf("Produced %zu tokens\n", tokenCount);

  writeTokensToFile(outputFileName, tokens, tokenCount, (uint64_t)inputFileSize);

  // printf("Wrote LZ compressed file to: %s\n", outputFileName);
  long int outputFileSize = findFileSize(outputFileName);
  // (outputFileSize != -1) ? printf("File size: %ld bytes\n", outputFileSize) : printf("File is emtpy\n");

  uint32_t literalFreq[286] = {0};
  uint32_t distFreq[30] = {0};
  countFrequencies(tokens, tokenCount, literalFreq, distFreq);

  // Literal tre
  struct HuffmannNode *literalTree = buildHuffmannTree(literalFreq, 286);
  // Bygg distance tre
  struct HuffmannNode *distTree = buildHuffmannTree(distFreq, 30);

  struct HuffmannCode literalCodes[286] = {0};
  struct HuffmannCode distCodes[30] = {0};
  generateBitCodes(literalTree, literalCodes, 0, 0);
  generateBitCodes(distTree, distCodes, 0, 0);

  buildCanonicalCodes(literalCodes, 286);
  buildCanonicalCodes(distCodes, 30);

  const char *deflateOutputFile = "output.komprimert.deflate";

  writeDeflateAlgoCompressed(deflateOutputFile,
                             tokens,
                             tokenCount,
                             literalCodes,
                             distCodes,
                             (uint64_t)inputFileSize);

  long int deflateSize = findFileSize(deflateOutputFile);
  printf("Compressed file: %s\n", deflateOutputFile);
  printf("Compressed file size: %ld bytes\n", deflateSize);

  if (deflateSize > 0)
  {
    double ratio = 100.0 * (1.0 - (double)deflateSize / inputFileSize);
    printf("Compression: %.1f%%\n\n", ratio);

    printf("Comparison:\n");
    printf("- Original: %ld bytes\n", inputFileSize);
    printf("- LZ77: %ld bytes\n", outputFileSize);
    printf("- LZ77+Huffman: %ld bytes\n", deflateSize);
  }

  free(tokens);
  free(inputBuffer);

  return 0;
}