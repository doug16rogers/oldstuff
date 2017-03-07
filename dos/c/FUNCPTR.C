#include <stdio.h>


void val(int k)
{
  printf("VALUE\t%u\n",k);
}   //val


void valsqrd(int k)
{
  printf("SQUARE\t%u\n",k*k);
}   //valsqrd


void valcubd(int k)
{
  printf("CUBE\t%u\n",k*k*k);
}   //valcubd




typedef void (*funcptr)(int k);


void main(void)
{
  funcptr func;

  func = val;
  func(3);

  func = valsqrd;
  func(4);

  func = valcubd;
  func(5);
}   //main

VALUE	3
SQUARE	16
CUBE	125
