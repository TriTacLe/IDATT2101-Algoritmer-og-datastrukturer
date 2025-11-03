#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>


// functions to read bytes from a binaryfile 

static int read_u8(FILE *fp, uint8_t *out){
  int c = fgetc(fp); 
  if (c == EOF){
    return -1; 
  }
  *out = (uint8_t)c; 
  return 0; 
}

static int read_u16_le(FILE*fp, uint16_t *out){
  uint8_t b0, b1; 
  if(read_u8(fp,&b0) < 0){
    return -1; 
  }
  if(read_u8(fp,&b1) < 0){
    return -1;
  }
  *out = (uint16_t) (b0 | ((uint16_t)b1 << 8)); 
  return 0; 
}

static int read_u64_le(FILE *fp, uint64_t *out)
{
  uint8_t b[8];
  if (fread(b, 1, 8, fp) != 8)
    return -1;
  *out = 0;
  for (int i = 7; i >= 0; --i)
    *out = (*out << 8) | b[i];
  return 0;
}

// backwards copy of the data... 

static void lz_overlap(uint8_t *dst,size_t *pos, uint16_t dist, uint16_t len){


  size_t writePos = *pos; 
  size_t readPos = writePos-dist;

  // copying byte for byte 
  for (uint16_t i = 0; i < len; i++)
  {
    dst[writePos] = dst[readPos];
    writePos++;
    readPos++;
  }

  *pos = writePos; 
}

static int decompress_file(const char *in_path, const char *out_path)
{

  //opening the compressed file
  FILE *in = fopen(in_path, "rb"); 
  if(!in){
    fprintf(stderr, "Kunne ikke åpne %s: %s\n", in_path, strerror(errno));
    return 1;
  }
  
  //reading the header
  uint64_t originalSize = 0, tokenCount = 0;
  if (read_u64_le(in, &originalSize) < 0 || read_u64_le(in, &tokenCount) < 0)
  {
    fprintf(stderr, "Korrupt/ufullstendig header\n");
    fclose(in);
    return 1;
  }

  //allocating buffer
  uint8_t *outbuf = NULL;
  if (originalSize > 0)
  {
    outbuf = (uint8_t *)malloc((size_t)originalSize);
    if (!outbuf)
    {
      fprintf(stderr, "malloc(%zu) feilet\n", (size_t)originalSize);
      fclose(in);
      return 1;
    }
  }


  //reading tokens
  size_t out_pos = 0;
  for (uint64_t t = 0; t < tokenCount && out_pos < (size_t)originalSize; ++t)
  {
    uint8_t type = 0;
    if (read_u8(in, &type) < 0)
    {
      fprintf(stderr, "EOF ved token %llu\n", (unsigned long long)t);
      free(outbuf);
      fclose(in);
      return 1;
    }

    if (type == 0)
    {
    
      uint8_t lit;
      if (read_u8(in, &lit) < 0)
      {
        fprintf(stderr, "EOF literal\n");
        free(outbuf);
        fclose(in);
        return 1;
      }
      if (out_pos >= (size_t)originalSize)
      {
        fprintf(stderr, "Overflow literal\n");
        free(outbuf);
        fclose(in);
        return 1;
      }
      outbuf[out_pos++] = lit;
    }
    else if (type == 1)
    {
      /* match (length, distance) */
      uint16_t length = 0, distance = 0;
      if (read_u16_le(in, &length) < 0 || read_u16_le(in, &distance) < 0)
      {
        fprintf(stderr, "EOF match\n");
        free(outbuf);
        fclose(in);
        return 1;
      }
      if (length == 0)
        continue; 
      if (distance == 0 || distance > out_pos)
      { /* kan ikke peke før start */
        fprintf(stderr, "Ugyldig distance=%u ved out_pos=%zu (token %llu)\n",
                distance, out_pos, (unsigned long long)t);
        free(outbuf);
        fclose(in);
        return 1;
      }
      if ((size_t)length > (size_t)originalSize - out_pos)
      {
        fprintf(stderr, "Overflow match len=%u\n", length);
        free(outbuf);
        fclose(in);
        return 1;
      }
      lz_overlap(outbuf, &out_pos, distance, length);
    }
    else
    {
      fprintf(stderr, "Ukjent token type=%u (token %llu)\n", type, (unsigned long long)t);
      free(outbuf);
      fclose(in);
      return 1;
    }
  }

  if (out_pos != (size_t)originalSize)
  {
    fprintf(stderr, "Størrelsesfeil: produserte %zu, forventet %llu\n",
            out_pos, (unsigned long long)originalSize);
    free(outbuf);
    fclose(in);
    return 1;
  }


//writing out to the file
  FILE *out = fopen(out_path, "wb");
  if (!out)
  {
    fprintf(stderr, "Kunne ikke åpne %s: %s\n", out_path, strerror(errno));
    free(outbuf);
    fclose(in);
    return 1;
  }
  if (originalSize && fwrite(outbuf, 1, (size_t)originalSize, out) != (size_t)originalSize)
  {
    fprintf(stderr, "Skrivefeil\n");
    free(outbuf);
    fclose(out);
    fclose(in);
    return 1;
  }

  fclose(out);
  free(outbuf);
  fclose(in);
  return 0;
}

int main(int argc, char **argv)
{
  if (argc < 3)
  {
    fprintf(stderr, "Bruk: %s <input.lz> <output>\n", argv[0]);
    return 2;
  }
  return decompress_file(argv[1], argv[2]);
}
/*   // oblig file
  filePointer = fopen("diverse.lyx", "w");
  // filePointer = fopen("Twenty_thousand_leagues_under_the_sea.txt", "w");

  fclose(filePointer);
  return 0; */