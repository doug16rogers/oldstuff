#include <stdio.h>

#include "r250.hpp"

R250 r;

unsigned bad[9]={                      //by car,guess
  3, 2, 1,                             //car==0
  2, 3, 0,                             //car==1
  1, 0, 3                              //car==2
};


unsigned stay=0,swap=0;

int  main(int argc,char *argv[])
{
  unsigned car;
  unsigned guess;
  unsigned monte;
  if (argc!=2) {
    printf("monte <iterations>\n");
    return 1;
  }
  unsigned n=1000;
  sscanf(argv[1],"%u",&n);
  for (unsigned i=0;i<n;i++) {
    car=r.rnd(3);
    guess=r.rnd(3);
    if (car==guess)
      monte=(car+r.rnd(2)+1)%3;
    else
      monte=bad[car*3+guess];
    if (guess==car) stay++; else swap++;

    if ((i%1000)==0) printf("car  guess  monte  Stay  Swap  Total\n");
    if ((i%100)==0)
      printf(" %c     %c      %c    %04u  %04u  %04u\n",
	car+'0',guess+'0',monte+'0',stay,swap,i+1);
  }   //for
  printf(" %c     %c      %c    %04u  %04u  %04u\n",
    car+'0',guess+'0',monte+'0',stay,swap,n);
}   //main

