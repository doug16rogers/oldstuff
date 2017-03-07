#include <stdlib.h>
#include <time.h>

#include "r250.hpp"

#define LOWORD(x)      (*((WORD*)&(x)))
#define HIWORD(x)      (*(((WORD*)&(x))+1))


R250::R250(void)
{
  int i;
  DWORD k=1;

  randomize();
  for (i=0;i<SIZE;i++) {               //put in initial random numbers
    HIWORD(r[i])=(rand()<<1) ^ rand(); //..initialize MSW, rand onl
    LOWORD(r[i])=(rand()<<1) ^ rand(); //..initialize LSW
  }   //for initial values
  for (i=0;i<32;i++) {                 //make sure we have a 32-bit basis..
    r[i] |= k;                         //..by setting each bit
    r[i+1] &= ~k;                      //..and resetting it, too
    k<<=1;                             //..go to next bit
  }   //for basis-ensuring
  index=0;
  for (i=0;i<32;i++) rnd();
}   //R250::R250(void)


R250::R250(int seed)
{
  int i;
  DWORD k=1;

  srand(seed);
  for (i=0;i<SIZE;i++) {               //put in initial random numbers
    HIWORD(r[i])=(rand()<<1) ^ rand(); //..initialize MSW, rand onl
    LOWORD(r[i])=(rand()<<1) ^ rand(); //..initialize LSW
  }   //for initial values
  for (i=0;i<32;i++) {                 //make sure we have a 32-bit basis..
    r[i] |= k;                         //..by setting each bit
    r[i+1] &= ~k;                      //..and resetting it, too
    k<<=1;                             //..go to next bit
  }   //for basis-ensuring
  index=0;
}   //R250::R250(int seed)


DWORD R250::rnd(void)
{
  register DWORD v;

  if (index>=PREV)
    v=r[index] ^ r[index-PREV];
  else
    v=r[index] ^ r[index+SIZE-PREV];
  r[index++]=v;
  if (index>=SIZE) index=0;
  return v;
}   //DWORD R250::rnd(void)


