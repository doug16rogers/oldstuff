#include <stdio.h>


typedef struct {
  unsigned long ptr;
  unsigned long siz;
} FREELIST_BLOCK;


#define FREELIST_BLOCKS 0x100

typedef struct {
  unsigned blocs;
  unsigned spacer;
  FREELIST_BLOCK bloc[FREELIST_BLOCKS];
} FREELIST;


FREELIST freelist={0,0};


unsigned merge(FREELIST* f)
{
  unsigned i=0,j=1;
  unsigned long top0,top1,bot0,bot1;
  char added;

  while (i < f->blocs) {
    bot0 = f->bloc[i].ptr;
    top0 = bot0 + f->bloc[i].siz;
    added=0;
    for (j=i+1;j<f->blocs;j++) {
      bot1 = f->bloc[j].ptr;
      top1 = bot1 + f->bloc[j].siz;
      if ((bot1>top0) || (top1<bot0)) continue;
      if (bot1<bot0) bot0 = bot1;
      if (top1>top0) top0 = top1;
      f->bloc[j] = f->bloc[f->blocs-1];
      (f->blocs)--;
      added=1;
      j--;
    }   //for
    if (!added) { i++; continue; }
    f->bloc[i].ptr = bot0;
    f->bloc[i].siz = top0 - bot0 + 1;
  }   //while
  return f->blocs;
}   //merge


void addbloc(FREELIST* f,unsigned long p,unsigned long s)
{
  if (f->blocs>=FREELIST_BLOCKS) return;
  f->bloc[f->blocs].ptr = p;
  f->bloc[f->blocs].siz = s;
  (f->blocs)++;
}   //addbloc


void showfree(FREELIST* f)
{
  unsigned k;
  for (k=0;k<f->blocs;k++)
    printf("%04X:  %08lX  %08lX\n",k,f->bloc[k].ptr,f->bloc[k].siz);
}   //showfree


void main(void)
{
  addbloc(&freelist,0x00000000UL,0x00000001UL);
  addbloc(&freelist,0x00000200UL,0x00000101UL);
  addbloc(&freelist,0x00000100UL,0x00000100UL);
  addbloc(&freelist,0x00000001UL,0x00000080UL);
  addbloc(&freelist,0x00000080UL,0x00000001UL);
  addbloc(&freelist,0x00000090UL,0x00000001UL);
  addbloc(&freelist,0x000000A0UL,0x00000001UL);
  printf("===========================================================\n");
  showfree(&freelist);
  merge(&freelist);
  printf("-----------------------------------------------------------\n");
  showfree(&freelist);
}   //main

