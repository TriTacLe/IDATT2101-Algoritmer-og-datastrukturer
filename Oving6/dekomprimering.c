#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define LIT_COUNT 286
#define DIST_COUNT 30
#define MAX_BITS 31
#define END_SYMBOL 256

// readers
static int read_u8(FILE *fp, uint8_t *out)
{
  int c = fgetc(fp);
  if (c == EOF)
    return -1;
  *out = (uint8_t)c;
  return 0;
}
static int read_u64_le(FILE *fp, uint64_t *out)
{
  uint64_t v = 0;
  for (int i = 0; i < 8; i++)
  {
    int c = fgetc(fp);
    if (c == EOF)
      return -1;
    v |= ((uint64_t)(uint8_t)c) << (8 * i);
  }
  *out = v;
  return 0;
}

// bit reader
typedef struct
{
  FILE *fp;
  uint32_t buf;
  uint8_t bits;
} BitReader;
static void br_init(BitReader *br, FILE *fp)
{
  br->fp = fp;
  br->buf = 0;
  br->bits = 0;
}
static uint32_t br_read_bits(BitReader *br, uint8_t n)
{
  while (br->bits < n)
  {
    int c = fgetc(br->fp);
    if (c == EOF)
    {
      fprintf(stderr, "Unexpected EOF in bitstream\n");
      exit(1);
    }
    br->buf |= ((uint32_t)(uint8_t)c) << br->bits;
    br->bits += 8;
  }
  uint32_t val = br->buf & ((1u << n) - 1u);
  br->buf >>= n;
  br->bits -= n;
  return val;
}

// LSB traversal
typedef struct
{
  int left, right;
  int symbol;
} Node;
typedef struct
{
  Node *nodes;
  int size, cap;
} HuffTrie;
static void trie_init(HuffTrie *t)
{
  t->cap = 512;
  t->size = 1;
  t->nodes = (Node *)malloc(t->cap * sizeof(Node));
  t->nodes[0] = (Node){-1, -1, -1};
}
static int trie_new(HuffTrie *t)
{
  if (t->size >= t->cap)
  {
    t->cap *= 2;
    t->nodes = (Node *)realloc(t->nodes, t->cap * sizeof(Node));
  }
  int i = t->size++;
  t->nodes[i] = (Node){-1, -1, -1};
  return i;
}
static void trie_insert(HuffTrie *t, uint32_t code_lsb, uint8_t len, int sym)
{
  int cur = 0;
  for (uint8_t i = 0; i < len; i++)
  {
    int bit = (code_lsb >> i) & 1u;
    int *child = bit ? &t->nodes[cur].right : &t->nodes[cur].left;
    if (*child == -1)
      *child = trie_new(t);
    cur = *child;
  }
  t->nodes[cur].symbol = sym;
}
static int trie_decode(BitReader *br, const HuffTrie *t)
{
  int cur = 0;
  for (;;)
  {
    if (t->nodes[cur].symbol >= 0)
      return t->nodes[cur].symbol;
    uint32_t b = br_read_bits(br, 1);
    cur = b ? t->nodes[cur].right : t->nodes[cur].left;
    if (cur < 0)
    {
      fprintf(stderr, "Invalid Huffman bit sequence\n");
      exit(1);
    }
  }
}

static void build_decoder_from_lengths(const uint8_t *lengths, int n, HuffTrie *trie)
{
  int count[MAX_BITS + 1] = {0};
  uint32_t first[MAX_BITS + 1] = {0}, next[MAX_BITS + 1] = {0};
  for (int s = 0; s < n; s++)
  {
    uint8_t L = lengths[s];
    if (L)
    {
      if (L > MAX_BITS)
      {
        fprintf(stderr, "Code too long\n");
        exit(1);
      }
      count[L]++;
    }
  }
  for (int l = 1; l <= MAX_BITS; l++)
  {
    first[l] = ((first[l - 1] + count[l - 1]) << 1);
  }
  for (int l = 0; l <= MAX_BITS; l++)
    next[l] = first[l];

  trie_init(trie);
  for (int s = 0; s < n; s++)
  {
    uint8_t L = lengths[s];
    if (!L)
      continue;
    uint32_t msb = next[L]++; // MSB-ordered
    // reverse msb->lsb over L bits
    uint32_t lsb = 0;
    for (uint8_t i = 0; i < L; i++)
    {
      uint32_t bit = (msb >> (L - 1 - i)) & 1u;
      lsb |= bit << i;
    }
    trie_insert(trie, lsb, L, s);
  }
}

// length/distance tables
typedef struct
{
  uint16_t base;
  uint8_t extra;
} BaseExtra;
static const BaseExtra LEN_TBL[29] = {
    {3, 0}, {4, 0}, {5, 0}, {6, 0}, {7, 0}, {8, 0}, {9, 0}, {10, 0}, {11, 1}, {13, 1}, {15, 1}, {17, 1}, {19, 2}, {23, 2}, {27, 2}, {31, 2}, {35, 3}, {43, 3}, {51, 3}, {59, 3}, {67, 4}, {83, 4}, {99, 4}, {115, 4}, {131, 5}, {163, 5}, {195, 5}, {227, 5}, {258, 0}};
static const BaseExtra DIST_TBL[30] = {
    {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 1}, {7, 1}, {9, 2}, {13, 2}, {17, 3}, {25, 3}, {33, 4}, {49, 4}, {65, 5}, {97, 5}, {129, 6}, {193, 6}, {257, 7}, {385, 7}, {513, 8}, {769, 8}, {1025, 9}, {1537, 9}, {2049, 10}, {3073, 10}, {4097, 11}, {6145, 11}, {8193, 12}, {12289, 12}, {16385, 13}, {24577, 13}};

static void *xrealloc(void *p, size_t n)
{
  void *q = realloc(p, n);
  if (!q)
  {
    fprintf(stderr, "Out of memory\n");
    exit(1);
  }
  return q;
}

int main(int argc, char **argv)
{
  if (argc < 3)
  {
    fprintf(stderr, "Usage: %s input.deflate output\n", argv[0]);
    return 1;
  }
  const char *inpath = argv[1], *outpath = argv[2];

  FILE *fp = fopen(inpath, "rb");
  if (!fp)
  {
    fprintf(stderr, "Could not open %s: %s\n", inpath, strerror(errno));
    return 1;
  }

  uint64_t originalSize = 0;
  if (read_u64_le(fp, &originalSize) < 0)
  {
    fprintf(stderr, "Error in header (originalSize)\n");
    return 1;
  }

  uint8_t lit_len[LIT_COUNT], dist_len[DIST_COUNT];
  for (int i = 0; i < LIT_COUNT; i++)
    if (read_u8(fp, &lit_len[i]) < 0)
    {
      fprintf(stderr, "EOF while reading literal code lengths\n");
      return 1;
    }
  for (int i = 0; i < DIST_COUNT; i++)
    if (read_u8(fp, &dist_len[i]) < 0)
    {
      fprintf(stderr, "EOF while reading distance code lengths\n");
      return 1;
    }

  // --- SANITY right after header lengths are read ---
  int lit_sum = 0, dist_sum = 0;
  for (int i = 0; i < LIT_COUNT; i++)
    lit_sum += (lit_len[i] != 0);
  for (int i = 0; i < DIST_COUNT; i++)
    dist_sum += (dist_len[i] != 0);

  fprintf(stderr, "[DEBUG] lit_sum=%d, dist_sum=%d, lit_len[256]=%u\n",
          lit_sum, dist_sum, (unsigned)lit_len[256]);

  if (lit_len[256] == 0)
  {
    fprintf(stderr, "Error: END symbol (256) has code length 0 in header.\n");
    return 1;
  }
  if (lit_sum == 0)
  {
    fprintf(stderr, "Error: literal table is empty.\n");
    return 1;
  }

  HuffTrie litTrie, distTrie;
  build_decoder_from_lengths(lit_len, LIT_COUNT, &litTrie);
  build_decoder_from_lengths(dist_len, DIST_COUNT, &distTrie);

  BitReader br;
  br_init(&br, fp);

  size_t cap = originalSize ? (size_t)originalSize : 1024;
  if (cap < 1024)
    cap = 1024;
  uint8_t *out = (uint8_t *)malloc(cap);
  size_t outlen = 0;

  for (;;)
  {
    int sym = trie_decode(&br, &litTrie);
    if (sym < 256)
    {
      if (outlen + 1 > cap)
      {
        while (outlen + 1 > cap)
          cap *= 2;
        out = xrealloc(out, cap);
      }
      out[outlen++] = (uint8_t)sym;
      continue;
    }
    if (sym == END_SYMBOL)
      break;

    if (sym < 257 || sym > 285)
    {
      fprintf(stderr, "Invalid length symbol %d\n", sym);
      return 1;
    }
    uint16_t length = 0;
    if (sym == 285)
      length = 258;
    else
    {
      int idx = sym - 257;
      uint16_t base = LEN_TBL[idx].base;
      uint8_t eb = LEN_TBL[idx].extra;
      uint32_t add = eb ? br_read_bits(&br, eb) : 0;
      length = (uint16_t)(base + add);
    }

    if (dist_sum == 0)
    {
      fprintf(stderr, "Error: bitstream requests a distance, but distance table is empty (no distance codes in header).\n");
      return 1;
    }

    int dsym = trie_decode(&br, &distTrie);
    if (dsym < 0 || dsym >= DIST_COUNT)
    {
      fprintf(stderr, "Invalid distance symbol %d\n", dsym);
      return 1;
    }
    uint16_t distance = 0;
    {
      uint16_t base = DIST_TBL[dsym].base;
      uint8_t eb = DIST_TBL[dsym].extra;
      uint32_t add = eb ? br_read_bits(&br, eb) : 0;
      distance = (uint16_t)(base + add);
    }
    if (distance == 0 || distance > outlen)
    {
      fprintf(stderr, "Invalid distance %u at position %zu\n", distance, outlen);
      return 1;
    }

    if (outlen + length > cap)
    {
      while (outlen + length > cap)
        cap *= 2;
      out = xrealloc(out, cap);
    }
    size_t from = outlen - distance;
    for (uint16_t k = 0; k < length; k++)
    {
      out[outlen++] = out[from + (k % distance)];
    }
  }

  fclose(fp);

  if (originalSize && outlen != (size_t)originalSize)
  {
    fprintf(stderr, "Warning: output length (%zu) != originalSize (%llu)\n", outlen, (unsigned long long)originalSize);
  }

  FILE *fo = fopen(outpath, "wb");
  if (!fo)
  {
    fprintf(stderr, "Could not open %s: %s\n", outpath, strerror(errno));
    return 1;
  }
  fwrite(out, 1, outlen, fo);
  fclose(fo);
  free(out);
  free(litTrie.nodes);
  free(distTrie.nodes);

  printf("OK. Wrote %zu bytes to %s\n", outlen, outpath);
  return 0;
}
