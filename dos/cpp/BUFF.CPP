#include <stdio.h>
#include <string.h>

#include "buff.hpp"


ByteBuffer::ByteBuffer(unsigned int _size) {
  buff = new BYTE[_size];
  if (buff) size=_size; else size=0;
  count=0;
  different_bytes=0;
  sort_performed=0;
  memset(bytecount,0,sizeof(bytecount));
}   //ByteBuffer::ByteBuffer(unsigned)


void ByteBuffer::Add(char*s)
{
  while (*s) Add(*(BYTE*)s++);
}   //ByteBuffer::Add(char*s)


void ByteBuffer::Add(Alphabet& a,char* s)
{
  while (*s) Add(a,*(BYTE*)s++);
}   //ByteBuffer::Add(Alphabet&,char*)


int  ByteBuffer::Load(Alphabet& a,char* fn)
{
  FILE* f = fopen(fn,"rb");
  if (f == NULL) return 0;

  BYTE b;
  while (fread(&b,1,1,f)) if (a.Inv(b)>=0) Add(b);
  fclose(f);
  return 1;
}   //void ByteBuffer::Load(Alphabet&,char*)


unsigned int ByteBuffer::Sort(void)
{
  unsigned int diff=0;
  sort_performed=1;
  for (unsigned int i=0;i<0x100;i++) if (bytecount[i]) sorted[diff++]=i;
  different_bytes=diff;
  if (diff<2) return different_bytes;
  diff--;
  i=0;
  while (i<diff) {
    if (bytecount[sorted[i]]<bytecount[sorted[i+1]]) {
      BYTE b=sorted[i];
      sorted[i]=sorted[i+1];
      sorted[i+1]=b;
      if (i) i--; else i++;
    }   //if need to swap
    else
      i++;
  }   //while
  return different_bytes;
}   //ByteBuffer::Sort(void)


