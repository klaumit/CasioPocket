#include "mem.h"

void memneartofarcpy(const void *source,void far *dest,word len)
{
    word i;

  for(i = 0; i < len; i++) {
    ((byte far*)dest)[i] = ((byte *)source)[i]; 
  }
}

void memfartonearcpy(const void far *source,void *dest,word len)
{
    word i;

  for(i = 0; i < len; i++) {
    ((byte *)dest)[i] = ((byte far*)source)[i]; 
  }
}

void memfarcpy(const void far *source,void far *dest,word len)
{
    word i;

  for(i = 0; i < len; i++) {
    ((byte far*)dest)[i] = ((byte far*)source)[i]; 
  }
}

void memfarfill(const void far *ptr,word len, byte value)
{
  word i;
  for(i = 0; i < len; i++) {
    ((byte far*)ptr)[i] = value;
  }
}