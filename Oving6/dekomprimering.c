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




int main()
{
  FILE *filePointer;

  // oblig file
  filePointer = fopen("diverse.lyx", "w");
  // filePointer = fopen("Twenty_thousand_leagues_under_the_sea.txt", "w");

  fclose(filePointer);
  return 0;
}