#include <stdio.h>
#include <string.h>

#include "numutil.h"

int   asciitonum(char* s,byte* n,int len)
{
  int l;
  int i=0;
  int k;
  char h[3]="00";

  l=strlen(s);
  while (((l-2)>=0)&&(i<len)) {
    h[0]=s[l-2];
    h[1]=s[l-1];
    sscanf(h,"%x",&k);
    n[i++]=k;
    l-=2;
  }
  if ((l==1)&&(i<len)) {
    h[0]='0';
    h[1]=s[0];
    sscanf(h,"%x",&k);
    n[i++]=k;
  }
  for (k=i;k<len;k++) n[k]=0;
  return i;
}   //asciitonum


char* numtoascii(byte *n,int len,char *s)
{
  static char ours[0x100];
  int i,j=0;

  if (s==NULL) s=ours;

  for (i=len-1;i>=0;i--) if (n[i]) break;
  for (;i>=0;i--) {
    sprintf(&s[j],"%02X",n[i]);
    j+=2;
  }

  return s;
}   //numtoascii



byte *add(byte *n1,byte *n2,byte *n3,int len)
{
  int i;
  int k=0;
  int carry=0;

  for (i=0;i<len;i++) {
    k=n1[i]+n2[i]+carry;
    n3[i]=k;
    if (k>0xFF) carry=1; else carry=0;
  }
  return n3;
}   //add


byte *sub(byte *n1,byte *n2,byte *n3,int len);
byte *mul(byte *n1,byte *n2,byte *n3,int len);
byte *div(byte *n1,byte *n2,byte *n3,int len);