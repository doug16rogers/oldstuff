#include <stdio.h>
#include <string.h>

char str[20]="1234";

typedef void (*funcptr)(void);
void (*execute_func)(void)=NULL;


void permute0(char* s,int n)           //(n:0)
{
  execute_func();
}   //permute0


void permute1(char* s,int n)           //(n:1)
{
  char c0,c1;
  if (n<2) {
    execute_func();
  }
  else {
    c0=s[0];
    c1=s[1];
    permute1(&s[1],n-1);
    s[1]=c0;
    s[0]=c1;
    permute1(&s[2],n-2);
    s[0]=c0;
    s[1]=c1;
  }
}   //permute1


void permute2(char* s,int n)           //(n:2) --- still incorrect
{
  char c0,c1,c2;
  if (n<3) {
    if (n<2) execute_func();
    else {
      c0=s[0]; c1=s[1];
      execute_func();
      s[0]=c1; s[1]=c0;
      execute_func();
      s[0]=c0; s[1]=c1;
    }
  }
  else {
    c0=s[0];
    c1=s[1];
    c2=s[2];
    permute2(&s[1],n-1);
    s[0]=c1;
    s[1]=c0;
    s[2]=c2;
    permute2(&s[3],n-3);
    s[0]=c2;
    s[1]=c0;
    s[2]=c1;
    permute2(&s[3],n-3);
    s[0]=c1;
    s[1]=c2;
    s[2]=c0;
    permute2(&s[3],n-3);
    s[0]=c2;
    s[1]=c1;
    s[2]=c0;
    permute2(&s[3],n-3);
    s[0]=c0;
    s[1]=c1;
    s[2]=c2;
  }
}   //permute2


typedef void (*permfunc)(char*,int);
permfunc permutek[]={ permute0,permute1,permute2 };

int  permute(char* s,int n,int k,funcptr fp)      //(n:k)
{
  if (k>2) return 0;
  if (fp == NULL) return 0;
  execute_func = fp;
  permutek[k](s,n);
  return 1;
}   //permute


void printstr(void)
{
  printf("%s\n",str);
}   //printstr


void main(void)
{
  int i;
  printf("========================\n");
  for (i=0;i<=2;i++) {
    permute(str,strlen(str),i,printstr);
    printf("------------------------\n");
  }   //for
}   //main

