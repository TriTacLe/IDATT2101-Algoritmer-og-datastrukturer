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

int decompress_file(int argc, char **argv){
  if (argc < 3){
    fprintf(stderr, "Bruk: %s <input.lz> <output> \n", argv[0]); 
    return 2; 
  }
  const char *in_path = argv[1], *out_path = argv[2];


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

  //allocating buffer for output


  //mainloop for building the decompressed file

  


  //writing results to the file

}


int main()
{
  FILE *filePointer;

  // oblig file
  filePointer = fopen("diverse.lyx", "w");
  // filePointer = fopen("Twenty_thousand_leagues_under_the_sea.txt", "w");

  fclose(filePointer);
  return 0;
}