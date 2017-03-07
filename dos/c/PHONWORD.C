#include <stdio.h>

char *numlet[]= {
  /*0*/ "o\0",
  /*1*/ "il\0",
  /*2*/ "abc",
  /*3*/ "def",
  /*4*/ "ghi",
  /*5*/ "jkl",
  /*6*/ "mno",
  /*7*/ "prs",
  /*8*/ "tuv",
  /*9*/ "wxy",
};

int nums=0;
unsigned char num[20];
char prt[sizeof(num)];

void showdigits(int n)
{
  char* s;

  if (n>=nums) { printf("%s\n",prt); return; }
  s = numlet[num[n]];
  while (*s) {
    prt[n] = *s;
    showdigits(n+1);
    s++;
  }   //while
}   //showdigits


void main(int argc,char* argv[])
{
  char *s;

  if (argc<2) { printf("phonword <phone-number>\n"); return; }

  s=argv[1];
  while (*s && (nums<sizeof(num))) {
    if ((*s>='0')&&(*s<='9')) num[nums++] = *s - '0';
    s++;
  }

  showdigits(0);
}   //main

