#include <stdio.h>
#include <dos.h>


#define XDIGIT(c)      ( (c<'A')?c-'0':(c<'a')?c-'A'+10:c-'a'+10 )
#define ISXDIGIT(c)    ( ((c>='0')&&(c<='9'))? 1 : \
			 ((c>='A')&&(c<='F'))? 1 : \
			 ((c>='a')&&(c<='f'))? 1 : 0 )


int strcchr(unsigned int* d,char *s)
{
  if (*s!='\\') { *d=*s; return 1; }   //no conversion necessary
  s++;
  switch (*s++) {
  case 0:    *d='\\'; return 1;
  case 'a':  *d=0x07; return 2;
  case 'b':  *d=0x08; return 2;
  case 't':  *d=0x09; return 2;
  case 'n':  *d=0x0A; return 2;
  case 'v':  *d=0x0B; return 2;
  case 'f':  *d=0x0C; return 2;
  case 'r':  *d=0x0D; return 2;
  case 'e':  *d=0x1B; return 2;
  case '\"': *d=0x22; return 2;
  case '\'': *d=0x27; return 2;
  case '\\': *d=0x5C; return 2;
  case 'x':
  case 'X':
    if     (!ISXDIGIT(*s)) { *d='\\'; return 1; }
    *d=        XDIGIT(*s); s++;
    if     (!ISXDIGIT(*s)) return 3;
    *d=(*d<<4)+XDIGIT(*s); s++;
    if     (!ISXDIGIT(*s)) return 4;
    *d=(*d<<4)+XDIGIT(*s); s++;
    if     (!ISXDIGIT(*s)) return 5;
    *d=(*d<<4)+XDIGIT(*s);
    return 6;
  default:
    if ((*s<'0')&&(*s>'7')) break;
    *d=*s-'0';  s++;
    if ((*s<'0')&&(*s>'7')) return 2;
    *d=(*d<<3)+*s-'0';  s++;
    if ((*s<'0')&&(*s>'7')) return 3;
    *d=(*d<<3)+*s-'0';  s++;
    if ((*s<'0')&&(*s>'7')) return 4;
    *d=(*d<<3)+*s-'0';  s++;
    if ((*s<'0')&&(*s>'7')) return 5;
    *d=(*d<<3)+*s-'0';  s++;
    if ((*s<'0')&&(*s>'7')) return 6;
    *d=(*d<<3)+*s-'0';
    return 7;
  }   //switch
  *d='\\';
  return 1;
}   //strcchr


int strcstr(unsigned int* d,char *s,int dsize)
{
  int i=0;
  int j=0;

  dsize--;                             //leave room for terminator
  for (i=0;i<dsize;i++) {
    if (!s[j]) break;
    j+=strcchr(d++,&s[j]);             //advance pointer
  }   //for
  *d=0;
  return i;
}   //strcstr


int  writekbbuf(unsigned int k)        //returns 1 on success, not full
{
  union REGS r;
  r.h.ah=0x05;
  r.x.cx=k;
  int86(0x16,&r,&r);
  return r.h.al==0;
}   //writekbbuf


int  main(int argc,char* argv[])
{
  unsigned int key_code[80];
  int len;
  int i;
//  unsigned int far* kb_head=(unsigned int far*)0x0040001AUL;
//  unsigned int far* kb_tail=(unsigned int far*)0x0040001CUL;
//  unsigned int far* kb_base=(unsigned int far*)0x00400000UL;
//  unsigned int head;
//  unsigned int tail;

  if (argc<2) {
    printf(
      "kbbuf -- puts characters into keyboard buffer.\n"
      "Usage: kbbuf <string>\n"
      "If <string> is to contain blanks, surround it with \"'s.\n"
      "Strings may contain standard C escape sequences:\n"
      "  \\OOOOOO=1-6 Octals            \\xHHHH=1-4 Hex digits\n"
      "  \\a=\\x07   \\b=\\x08   \\t=\\x09   \\n=\\x0A   \\v=\\x0B\n"
      "  \\f=\\x0C   \\r=\\x0D   \\e=\\x1B   \\\\=\\x5C   \\\"=\\x22\n"
      "  \\\'=\\x27\n"
      "Return codes are:\n"
      "  0 -- success\n"
      "  n -- number of characters NOT loaded into buffer\n"
    );   //printf
    return 1;
  }

  len=strcstr(key_code,argv[1],sizeof(key_code)/sizeof(key_code[0]));
  for (i=0;i<len;i++) if (!writekbbuf(key_code[i])) break;
  return len-i;
}   //main


