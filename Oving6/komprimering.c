#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

// Tall iht delfate algoritmen som brukes
#define SEARCH_BUFFER 32768 // 32kb search buffer
#define LOOKAHEAD_SIZE 258  // max match length (derfor uint16_t)
#define MIN_MATCH 3         // minimum match lengde for å gi meninig

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

// Chatta opplegg
static void write_u16_le(FILE *filePointer, uint16_t value)
{
  uint8_t bytes[2] = {(uint8_t)(value & 0xFF), (uint8_t)((value >> 8) & 0xFF)};
  fwrite(bytes, 1, 2, filePointer);
}

// Chatta opplegg
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

void getLengthCode(uint16_t length,
                   uint16_t *outCode,
                   uint8_t *outExtraBits,
                   uint16_t *outExtraValue)
{
  if (length == 258)
  {
    *outCode = 285;
    *outExtraBits = 0;
    *outExtraValue = 0;
    return;
  }

  uint8_t extraBits;
  if (length <= 10)
  {
    extraBits = 0;
  }
  else if (length <= 18)
  {
    extraBits = 1;
  }
  else if (length <= 34)
  {
    extraBits = 2;
  }
  else if (length <= 66)
  {
    extraBits = 3;
  }
  else if (length <= 130)
  {
    extraBits = 4;
  }
  else
  {
    extraBits = 5;
  }

  uint16_t base;
  switch (extraBits)
  {
  case 0:
    base = 3;
    break;
  case 1:
    base = 11;
    break;
  case 2:
    base = 19;
    break;
  case 3:
    base = 35;
    break;
  case 4:
    base = 67;
    break;
  case 5:
    base = 131;
    break;
  default:
    base = 3;
    break;
  }

  uint16_t code;
  if (extraBits == 0)
  {
    code = 257 + (length - base);
  }
  else
  {
    uint16_t offset = length - base;
    uint16_t groupSize = (1 << extraBits);
    uint16_t codeOffset = offset / groupSize;
    code = 265 + (extraBits - 1) * 4 + codeOffset;
  }

  uint16_t extraValue = 0;
  if (extraBits > 0)
  {
    uint16_t groupSize = (1 << extraBits);
    uint16_t offset = length - base;
    extraValue = offset % groupSize;
  }

  *outCode = code;
  *outExtraBits = extraBits;
  *outExtraValue = extraValue;
}

void getDistanceCode(uint16_t distance,
                     uint16_t *outCode,
                     uint8_t *outExtraBits,
                     uint16_t *outExtraValue)
{
  uint8_t extraBits;
  if (distance <= 4)
  {
    extraBits = 0;
  }
  else if (distance <= 8)
  {
    extraBits = 1;
  }
  else if (distance <= 16)
  {
    extraBits = 2;
  }
  else if (distance <= 32)
  {
    extraBits = 3;
  }
  else if (distance <= 64)
  {
    extraBits = 4;
  }
  else if (distance <= 128)
  {
    extraBits = 5;
  }
  else if (distance <= 256)
  {
    extraBits = 6;
  }
  else if (distance <= 512)
  {
    extraBits = 7;
  }
  else if (distance <= 1024)
  {
    extraBits = 8;
  }
  else if (distance <= 2048)
  {
    extraBits = 9;
  }
  else if (distance <= 4096)
  {
    extraBits = 10;
  }
  else if (distance <= 8192)
  {
    extraBits = 11;
  }
  else if (distance <= 16384)
  {
    extraBits = 12;
  }
  else
  {
    extraBits = 13;
  }

  uint16_t base;
  switch (extraBits)
  {
  case 0:
    base = 1;
    break;
  case 1:
    base = 5;
    break;
  case 2:
    base = 9;
    break;
  case 3:
    base = 17;
    break;
  case 4:
    base = 33;
    break;
  case 5:
    base = 65;
    break;
  case 6:
    base = 129;
    break;
  case 7:
    base = 257;
    break;
  case 8:
    base = 513;
    break;
  case 9:
    base = 1025;
    break;
  case 10:
    base = 2049;
    break;
  case 11:
    base = 4097;
    break;
  case 12:
    base = 8193;
    break;
  case 13:
    base = 16385;
    break;
  default:
    base = 1;
    break;
  }

  uint16_t code;
  if (extraBits == 0)
  {
    code = distance - base; // 0-3
  }
  else
  {
    uint16_t offset = distance - base;
    uint16_t groupSize = (1 << extraBits);
    uint16_t codeOffset = offset / groupSize;
    code = 4 + (extraBits - 1) * 2 + codeOffset;
  }

  uint16_t extraValue = 0;
  if (extraBits > 0)
  {
    uint16_t groupSize = (1 << extraBits);
    uint16_t offset = distance - base;
    extraValue = offset % groupSize;
  }

  *outCode = code;
  *outExtraBits = extraBits;
  *outExtraValue = extraValue;
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
void writeDeflateAlgoCompressed(const char *filename,
                                struct LZtoken *tokens,
                                size_t tokenCount,
                                struct HuffmannCode *literalCodes,
                                struct HuffmannCode *distCodes,
                                uint64_t originalSize)
{
  FILE *filePointer = fopen(filename, "wb");
  if (!filePointer)
  {
    perror("fopen deflate output");
    return;
  }

  // 1) Header: original size
  write_u64_le(filePointer, originalSize);

  // 2) Bare kodelengdene (0..15) fra eksisterende tabeller
  uint8_t lit_len[286], dist_len[30];
  for (int i = 0; i < 286; i++)
    lit_len[i] = literalCodes[i].length;
  for (int i = 0; i < 30; i++)
    dist_len[i] = distCodes[i].length;

  // (valgfritt) verifiser maks 15
  for (int i = 0; i < 286; i++)
    if (lit_len[i] > 15)
    {
      fprintf(stderr, "Literal len>15\n");
      exit(1);
    }
  for (int i = 0; i < 30; i++)
    if (dist_len[i] > 15)
    {
      fprintf(stderr, "Dist len>15\n");
      exit(1);
    }

  // 3) Skriv kodelengder (uendret format)
  writeCodeLengths(filePointer, literalCodes, 286);
  writeCodeLengths(filePointer, distCodes, 30);

  // 4) Bygg KANONISKE MSB-ordnede koder fra lengdene (RFC1951-metoden)
  uint32_t lit_code_msb[286] = {0}, dist_code_msb[30] = {0};
  {
    int count[16] = {0};
    uint32_t first[16] = {0}, next[16] = {0};
    for (int i = 0; i < 286; i++)
      if (lit_len[i])
        count[lit_len[i]]++;
    for (int l = 1; l <= 15; l++)
    {
      first[l] = ((first[l - 1] + count[l - 1]) << 1);
    }
    for (int l = 0; l <= 15; l++)
      next[l] = first[l];
    for (int s = 0; s < 286; s++)
      if (lit_len[s])
        lit_code_msb[s] = next[lit_len[s]]++;
  }
  {
    int count[16] = {0};
    uint32_t first[16] = {0}, next[16] = {0};
    for (int i = 0; i < 30; i++)
      if (dist_len[i])
        count[dist_len[i]]++;
    for (int l = 1; l <= 15; l++)
    {
      first[l] = ((first[l - 1] + count[l - 1]) << 1);
    }
    for (int l = 0; l <= 15; l++)
      next[l] = first[l];
    for (int s = 0; s < 30; s++)
      if (dist_len[s])
        dist_code_msb[s] = next[dist_len[s]]++;
  }

  // 5) Konverter til LSB-first for writeBits (som sender LSB først)
  uint32_t lit_code_lsb[286] = {0}, dist_code_lsb[30] = {0};
  for (int s = 0; s < 286; s++)
  {
    uint8_t L = lit_len[s];
    if (!L)
      continue;
    uint32_t msb = lit_code_msb[s], lsb = 0;
    for (uint8_t i = 0; i < L; i++)
    {
      uint32_t bit = (msb >> (L - 1 - i)) & 1u;
      lsb |= bit << i;
    }
    lit_code_lsb[s] = lsb;
  }
  for (int s = 0; s < 30; s++)
  {
    uint8_t L = dist_len[s];
    if (!L)
      continue;
    uint32_t msb = dist_code_msb[s], lsb = 0;
    for (uint8_t i = 0; i < L; i++)
    {
      uint32_t bit = (msb >> (L - 1 - i)) & 1u;
      lsb |= bit << i;
    }
    dist_code_lsb[s] = lsb;
  }

  // 6) Skriv bitstrøm med KANONISKE koder
  struct BitWriter bw;
  bitWriterInit(&bw, filePointer);
  for (size_t i = 0; i < tokenCount; i++)
  {
    if (tokens[i].type == 0)
    {
      uint8_t lit = tokens[i].literalOrMatch.literal;
      writeBits(&bw, lit_code_lsb[lit], lit_len[lit]);
    }
    else
    {
      uint16_t length = tokens[i].literalOrMatch.match.length;
      uint16_t distance = tokens[i].literalOrMatch.match.distance;

      uint16_t lenCode, lenExtraVal;
      uint8_t lenExtraBits;
      getLengthCode(length, &lenCode, &lenExtraBits, &lenExtraVal);
      writeBits(&bw, lit_code_lsb[lenCode], lit_len[lenCode]);
      if (lenExtraBits)
        writeBits(&bw, lenExtraVal, lenExtraBits);

      uint16_t distCode, distExtraVal;
      uint8_t distExtraBits;
      getDistanceCode(distance, &distCode, &distExtraBits, &distExtraVal);
      writeBits(&bw, dist_code_lsb[distCode], dist_len[distCode]);
      if (distExtraBits)
        writeBits(&bw, distExtraVal, distExtraBits);
    }
  }
  // END (256)
  writeBits(&bw, lit_code_lsb[256], lit_len[256]);

  bitWriterFlush(&bw);
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

int main(int argc, char** argv) {
  if (argc != 2) {
	fprintf(stderr, "Usage %s <inputfile> \n", argv[0]);
	return 1;
  }
  const char *inputFileName = argv[1];

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
    printf("Didn't read\n");

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