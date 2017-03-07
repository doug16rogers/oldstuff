#include <stdio.h>
#include <conio.h>

#include "scrutil.hpp"
#include "r250.hpp"


#ifndef BYTE
  #define BYTE unsigned char
#endif
#ifndef WORD
  #define WORD unsigned int
#endif
#ifndef DWORD
  #define DWORD unsigned long int
#endif


#define TMASK          0xC0
#define RMASK          0x30
#define BMASK          0x0C
#define LMASK          0x03


typedef struct {                       //bitfields, why not?
  WORD t:2;                            //number on top
  WORD r:2;                            //number on right
  WORD b:2;                            //number on bottom
  WORD l:2;                            //number on left
} Border;


typedef struct {                       //chars/descriptions
  BYTE c;                              //character, ASCII
  Border b;                            //how does it border?
} Chardef;


Chardef cdef[]={
  { 0xb3, {1,0,1,0} },
  { 0xb4, {1,0,1,1} },
  { 0xb5, {1,0,1,2} },
  { 0xb6, {2,0,2,1} },
  { 0xb7, {0,0,2,1} },
  { 0xb8, {0,0,1,2} },
  { 0xb9, {2,0,2,2} },
  { 0xba, {2,0,2,0} },
  { 0xbb, {0,0,2,2} },
  { 0xbc, {2,0,0,2} },
  { 0xbd, {2,0,0,1} },
  { 0xbe, {1,0,0,2} },
  { 0xbf, {0,0,1,1} },
  { 0xc0, {1,1,0,0} },
  { 0xc1, {1,1,0,1} },
  { 0xc2, {0,1,1,1} },
  { 0xc3, {1,1,1,0} },
  { 0xc4, {0,1,0,1} },
  { 0xc5, {1,1,1,1} },
  { 0xc6, {1,2,1,0} },
  { 0xc7, {2,1,2,0} },
  { 0xc8, {2,2,0,0} },
  { 0xc9, {0,2,2,0} },
  { 0xca, {2,2,0,2} },
  { 0xcb, {0,2,2,2} },
  { 0xcc, {2,2,2,0} },
  { 0xcd, {0,2,0,2} },
  { 0xce, {2,2,2,2} },
  { 0xcf, {1,2,0,2} },
  { 0xd0, {2,1,0,1} },
  { 0xd1, {0,2,1,2} },
  { 0xd2, {0,1,2,1} },
  { 0xd3, {2,1,0,0} },
  { 0xd4, {1,2,0,0} },
  { 0xd5, {0,2,1,0} },
  { 0xd6, {0,1,2,0} },
  { 0xd7, {2,1,2,1} },
  { 0xd8, {1,2,1,2} },
  { 0xd9, {1,0,0,1} },
  { 0xda, {0,1,1,0} }
};   //cdef array
#define CDEF_SIZE      (sizeof(cdef)/sizeof(Chardef))

int  rows;                             //rows available on screen
int  cols;                             //columns available on screen


#define ATTRBYTE       0xFF00
#define FILL           0x47B0


int match(WORD ch,int r,int c)
{
  register char cc;
  register WORD k;

  k=r*cols+c;
  if (r>0) {
    cc=(char)scrseg[k-cols]-cdef[0].c;
    if (cc>=0) if (cdef[cc].b.b!=cdef[ch].b.t) return 0;
  }
  if (r<(rows-1)) {
    cc=(char)scrseg[k+cols]-cdef[0].c;
    if (cc>=0) if (cdef[cc].b.t!=cdef[ch].b.b) return 0;
  }
  if (c>0) {
    cc=(char)scrseg[k-1]-cdef[0].c;
    if (cc>=0) if (cdef[cc].b.r!=cdef[ch].b.l) return 0;
  }
  if (c<(cols-1)) {
    cc=(char)scrseg[k+1]-cdef[0].c;
    if (cc>=0) if (cdef[cc].b.l!=cdef[ch].b.r) return 0;
  }
  return 1;

}


void main(void)
{
  if (!setscrseg()) return;            //make sure text mode is ok
  rows=getsrows();                     //rows available on screen
  cols=getscols();                     //columns available on screen
  R250 R;                              //set up random number generator
  BYTE done=0;                         //not done yet
  int  roco=rows*cols;                 //product of the two
  WORD* save=new WORD[roco];           //save location for the screen
  WORD far* wf=(WORD far*)scrseg;      //word far pointer for transfers
  WORD* sp=save;                       //pointer to our save section
  WORD i;
  for (i=0;i<roco;i++) { *sp++=*wf; *wf++=FILL; }

  while (!done) {
    int r=R.rnd(rows);
    int c=R.rnd(cols);
    WORD ch=R.rnd(CDEF_SIZE/*+1*/);
//  if (ch==CDEF_SIZE)
//    scrseg[r*cols+c]=FILL;
//  else
      if (match(ch,r,c)) scrseg[r*cols+c]=(WORD)cdef[ch].c+0x1700;
    if (kbhit()) {
      int c=getkey();
      switch (c) {
      case ESC: done=1; break;
      default:
        ;
      }   //switch on input char
    }   //if kb
  }   //while

  wf=scrseg;
  sp=save;
  for (i=0;i<roco;i++) *wf++=*sp++;
}   //main
