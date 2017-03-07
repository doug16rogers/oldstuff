#include <stdio.h>
#include <iostream.h>
#include <string.h>
#include <dos.h>

#include "morsutil.h"


void lighton(int f,char d);
void lightoff(void);


char str[256]="\0";
Morse mout;


int  cmdswitch(char *s)
{
  int i;
  unsigned int far *vp;

  if ((*s!='-')&&(*s!='/')) {
    if (str[0]) {
      printf("second string \"%s\" entered; use \"'s\n",s);
      return 0;
    }
    strcpy(str,s);
    return 1;
  }   /*if not switch*/
  s++;
  switch(*s++) {
  case 'f':
    if (sscanf(s,"%d",&i)!=1) {
      printf("illegal frequency \"%s\" Hz\n",s);
      return 0;
    }
    mout.frequency(i);
    break;
  case 's':
    if (sscanf(s,"%u",&i)!=1) {
      printf("illegal scale \"%s\"\n",s);
      return 0;
    }
    mout.scale(i);
    break;
  case 'S':
    mout.ON((MorseON)sound);
    mout.OFF((MorseOFF)nosound);
    mout.scale(8);
    break;
  case 'L':
    vp=(unsigned int far*)(0xB8000000UL+4096);
    for (i=0;i<2000;i++) *vp++=0x0FDB;
    mout.ON((MorseON)lighton);
    mout.OFF((MorseOFF)lightoff);
    mout.scale(20);
    break;
  case 'm':
    cout << "Morse of " << s << " is '" << mout.charof(s) << "'.\n";
    break;
  default:
    s--;
    s--;
    printf("unknown command switch \"%s\"\n",s);
    return 0;
  }   /*switch*/
  return 1;
}   /*cmdswitch*/


void lighton(int f,char d)
{
  union REGS r;
  r.x.ax=0x0501;
  int86(0x10,&r,&r);
//  *(unsigned int far*)(0xB8000800UL)=0x1F0F;
}

void lightoff(void)
{
  union REGS r;
  r.x.ax=0x0500;
  int86(0x10,&r,&r);
//  *(unsigned int far*)(0xB8000800UL)=0x0720;
}


void main(int argc,char *argv[])
{
  int i;

  if (argc<2) {
    printf(
      "morse [switches] <string>\n"
      "  -f<frequency>   frequency of sound in Hz (%u)\n"
      "  -s<scale>       scale, 1=10ms dit (%u)\n"
      "  -S              use sound (default)\n"
      "  -L              use light (also sets -s20, you may override)\n"
      "put <string> in quotes (\"\") to include whitespace\n"
      ,mout.frequency()
      ,mout.scale()
    );   /*printf*/
    return;
  }   /*if no args*/
  for (i=1;i<argc;i++) if (!cmdswitch(argv[i])) return;
  if (!str[0]) {
    printf("Enter string: ");
    gets(str);
  }
  for (i=0;str[i];i++) {
    cout << mout.morsestring(str[i]) << ' ';
    mout << str[i];
  }   /*for*/
  cout << '\n';
}   /*main*/
