#include <stdio.h>

#include "numutil.h"


#define LEN   100


byte n1[LEN];
byte n2[LEN];
byte n3[LEN];


void main(int argc,char* argv[])
{
  if (argc<4) {
    printf("number <num1> <op> <num2>\n");
    return;
  }

  asciitonum(argv[1],n1,LEN);
  asciitonum(argv[3],n2,LEN);
  switch (*argv[2]) {
  case '+':
    add(n1,n2,n3,LEN);
    break;
  case '-':
  case '*':
  case '/':
    printf("not available\n");
    return;
  }

printf("=====================================\n");
  printf("n1=%s\n",numtoascii(n1,LEN,NULL));
  printf("n2=%s\n",numtoascii(n2,LEN,NULL));
  printf("n3=%s\n",numtoascii(n3,LEN,NULL));
}   //main

