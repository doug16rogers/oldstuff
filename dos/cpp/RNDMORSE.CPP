#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <dos.h>
#include <time.h>
#include <stdlib.h>
#include <ctype.h>
#include <iostream.h>

#include "morsutil.h"


#define DEFN           1
#define MAXN           80

char clist[]="ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
char slist[MAXN]="\0";
int  n=DEFN;
char disp=0;

void lighton(int f,char d);
void lightoff(void);


Morse mout;


int  cmdswitch(char *s)
{
  int i;
  unsigned int far *vp;

  if ((*s!='-')&&(*s!='/')) {
    printf("all commandline entries must be switches \"%s\"\n",s);
    return 0;
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
  case 'n':
    if ((sscanf(s,"%u",&n)!=1)||(n>=MAXN)||(n<1)) {
      printf("illegal number of letters \"%s\"\n");
      return 0;
    }
    break;
  case 'd':
    disp=1;
    break;
  case 'D':
    disp=0;
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
  char c;
  char s[256];

  for (i=1;i<argc;i++) {
    if (!cmdswitch(argv[i])) {
      printf(
        "rndmorse random morse code generator\n"
        "Useage: rndmorse [switches]\n"
        "Listen/watch, press correct key\n"
        "  -h/H/?          help\n"
        "  -f<frequency>   frequency of sound in Hz (%u)\n"
        "  -s<scale>       scale, 1=10ms dit (%u)\n"
        "  -S              use sound (default)\n"
        "  -L              use light (also sets -s20, you may override)\n"
        "  -d              display dits and dahs (default is D, off)\n"
        "  -n<number>      number of letters in each word (%u)\n"
        ,mout.frequency()
        ,mout.scale()
        ,DEFN
      );   /*printf*/
    }   /*error!*/
  }   /*for each switch*/
  randomize();

  i=-1;
  while (1) {
    if (i<0) {
      for (i=0;i<n;i++) slist[i]=clist[random(sizeof(clist)-1)];
      slist[i]=0;
    }   //if getting new string
    mout << slist;
    if (disp) for (i=0;i<n;i++) cout << mout.morsestring(slist[i]) << ' ';
    if (n==1) {
      c=toupper(getch());
      if (c==0x1b) break;
      if (c==slist[0]) {
        cout << "great: " << slist[0] << '\n';
        i=-1;
        delay(100);
      }   //if ok
      else {
        sound(100);
        delay(200);
        nosound();
        delay(100);
        cout << "nope: " << slist[0] << '\n';
      }   //else wrong char
    }   //if one char
    else {
      printf("What is it?: ");
      gets(s);
      strupr(s);
      if (!s[0]) break;
      if (strcmp(s,slist)==0) {
        cout << "great: " << slist << '\n';
        i=-1;
        delay(100);
      }   //if ok
      else {
        sound(100);
        delay(200);
        nosound();
        delay(100);
        cout << "nope: " << slist << '\n';
      }   //else wrong string
    }   //if multi-char string
  }   //while
  cout << '\n';
}   //main
