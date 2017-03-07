#include <stdio.h>
#include <iostream.h>
#include <iomanip.h>



//===========================================================================
int  inline digval(char c)
{
  if ((c>='0')&&(c<='9')) return c-'0';
  if ((c>='A')&&(c<='Z')) return c-'A'+10;
  if ((c>='a')&&(c<='z')) return c-'a'+10;
  return 37;
}   //digval


//===========================================================================
char* strcchr(char* d,char* s)
{
  int n=0;
  int dig=0;
  int base=8;
  char c;

  c=*s++;
  if (c!='\\') {                       //if not a special char
    *d=c;                              //..just copy it
    return s;                          //..and quit
  }   //if not escape char

  c=*s++;
  switch (c) {
  case 0:    *d=0x5C; s--; return s;   //\-terminated string, leave in
  case 'a':  *d=0x07; return s;        //special chars...
  case 'b':  *d=0x08; return s;
  case 't':  *d=0x09; return s;
  case 'n':  *d=0x0A; return s;
  case 'v':  *d=0x0B; return s;
  case 'f':  *d=0x0C; return s;
  case 'r':  *d=0x0D; return s;
  case 'e':  *d=0x1B; return s;        //extension to the standard (<ESC>)
  case '\"': *d=0x22; return s;
  case '\'': *d=0x27; return s;
  case '\\': *d=0x5C; return s;
  default:
    if      ((c=='x')||(c=='X')) { n=0;     dig=0; base=16; }
    else if ((c=='d')||(c=='D')) { n=0;     dig=0; base=10; }
    else if ((c>='0')&&(c<='7')) { n=c-'0'; dig=1; base=8; }
    else { *d=c;  return s; }          //ignore ill-used \ char
  }   /*switch*/

  while (dig<3) {
    c=digval(*s++);
    if (c<base) {
      n = n*base + c;
      dig++;
    }
    else {
      s--;
      break;
    }   //if base 16
  }   //while
  if (dig)
    *d = n;
  else {
    s--;
    *d = *s++;
  }
  return s;
}   //strcchr


//===========================================================================
int strcstr(char *d,char *s,int maxn)
{
  int i=0;
  if (maxn--)                          //reduce by 1 for terminator
    while (*s && (i<maxn)) s=strcchr(&d[i++],s);
  else
    while (*s) s=strcchr(&d[i++],s);
  d[i]=0;
  return i;
}   /*strcstr*/



int main(int argc, char* argv[])
{
  typedef unsigned char byte;
  typedef unsigned long u32;
  byte huge *bp;
  byte search[0x80];
  u32 start = 0x00400000UL;
  u32 end   = 0xA0000000UL;
  int length = 0;
  int count = 1;

  if (argc < 2)
  {
    cout << "memfind <string> [start] [end]  -- string may have \\-sequences\n";
    return 1;
  }

  length = strcstr((char*)search,argv[1],sizeof(search));
  if (length <= 0)
  {
    cout << "can't search for nul string\n";
    return 2;
  }

  if (argc > 2) sscanf(argv[2],"%X",&start);
  if (argc > 3) sscanf(argv[3],"%X",&end);

  cout << setbase(16) << setfill('0');

  int i;
  cout << "Range is " << setw(8) << start << ".." << setw(8) << end << ".\n";
  cout << "Seaarching for:";
  for (i = 0; (i < length); i++) cout  << ' '<< setw(2) << (int)search[i];
  cout << ".\n";

  for (bp = (byte huge*)start; ((u32)bp < end); bp++)
  {
    for (i = 0; (i < length); i++) if (bp[i] != search[i]) break;
    if (i >= length)
      cout << setw(3) << count++ << "  "
           << setw(8) << (u32)bp << ".\n";
  }
}   //main
