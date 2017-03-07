#include <stdio.h>
#include <conio.h>

#include "scrutil.hpp"
#include "r250.hpp"
#include "textscr.hpp"


#ifndef BYTE
  #define BYTE unsigned char
#endif
#ifndef WORD
  #define WORD unsigned int
#endif
#ifndef DWORD
  #define DWORD unsigned long int
#endif


enum {                                 //names for border field values
  BORDER_0,                            //forces to no appendages match
  BORDER_1,                            //forces to 1 appendage
  BORDER_2,                            //forces to 2 appendages
  BORDER_X                             //don't care what touches it
};

typedef struct {                       //bitfields, why not?
  BYTE t;                              //number on top
  BYTE r;                              //number on right
  BYTE b;                              //number on bottom
  BYTE l;                              //number on left
} Border;


typedef struct {                       //chars/descriptions
  BYTE c;                              //character, ASCII
  Border b;                            //how does it border?
} Chardef;

enum {
  ___=1,
  ONE=2,
  TWO=4,
  SET=ONE|TWO
};

Chardef cdef[]={
//single-liners...
  { 0xbf, {___,___,ONE,ONE} },
  { 0xc4, {___,ONE,___,ONE} },
  { 0xda, {___,ONE,ONE,___} },
  { 0xc2, {___,ONE,ONE,ONE} },
  { 0xd9, {ONE,___,___,ONE} },
  { 0xb3, {ONE,___,ONE,___} },
  { 0xb4, {ONE,___,ONE,ONE} },
  { 0xc0, {ONE,ONE,___,___} },
  { 0xc1, {ONE,ONE,___,ONE} },
  { 0xc3, {ONE,ONE,ONE,___} },
  { 0xc5, {ONE,ONE,ONE,ONE} },
//double-liners...
  { 0xbb, {___,___,TWO,TWO} },
  { 0xcd, {___,TWO,___,TWO} },
  { 0xc9, {___,TWO,TWO,___} },
  { 0xcb, {___,TWO,TWO,TWO} },
  { 0xbc, {TWO,___,___,TWO} },
  { 0xba, {TWO,___,TWO,___} },
  { 0xb9, {TWO,___,TWO,TWO} },
  { 0xc8, {TWO,TWO,___,___} },
  { 0xca, {TWO,TWO,___,TWO} },
  { 0xcc, {TWO,TWO,TWO,___} },
  { 0xce, {TWO,TWO,TWO,TWO} },
//single-verts/double-hors...
  { 0xb8, {___,___,ONE,TWO} },
  { 0xd5, {___,TWO,ONE,___} },
  { 0xd1, {___,TWO,ONE,TWO} },
  { 0xbe, {ONE,___,___,TWO} },
  { 0xb5, {ONE,___,ONE,TWO} },
  { 0xd4, {ONE,TWO,___,___} },
  { 0xcf, {ONE,TWO,___,TWO} },
  { 0xc6, {ONE,TWO,ONE,___} },
  { 0xd8, {ONE,TWO,ONE,TWO} },
//double-verts/single-hors...
  { 0xb7, {___,___,TWO,ONE} },
  { 0xd6, {___,ONE,TWO,___} },
  { 0xd2, {___,ONE,TWO,ONE} },
  { 0xbd, {TWO,___,___,ONE} },
  { 0xb6, {TWO,___,TWO,ONE} },
  { 0xd3, {TWO,ONE,___,___} },
  { 0xd0, {TWO,ONE,___,ONE} },
  { 0xc7, {TWO,ONE,TWO,___} },
  { 0xd7, {TWO,ONE,TWO,ONE} },
//...test...
  { 0xdf, {SET,___,___,___} },
  { 0xde, {___,SET,___,___} },
  { 0xdc, {___,___,SET,___} },
  { 0xdd, {___,___,___,SET} },
  { ' ',  {___,___,___,___} },
};   //cdef array
#define CDEF_SIZE      (sizeof(cdef)/sizeof(Chardef))

typedef struct {
  BYTE c;
  BYTE f;
  int  l;
} Atom;

#define ATTRBYTE       0xFF00
#define FILL           0x40B0
#define CATTR          0x1700

R250 R;                                //set up random number generator

BYTE guy[80];
int  lookup[0x100];
TextScreen scr(FILL);
Atom atom[0x100];


int openend(int r,int c,WORD ch)
{
  register WORD k;

  k = r*scr.cols + c;
  if (r>0)            if ((cdef[ch].b.t&SET) && (scrseg[k-scr.cols]==FILL)) return 1;
  if (r<(scr.rows-1)) if ((cdef[ch].b.b&SET) && (scrseg[k+scr.cols]==FILL)) return 1;
  if (c>0)            if ((cdef[ch].b.l&SET) && (scrseg[k-1]   ==FILL)) return 1;
  if (c<(scr.cols-1)) if ((cdef[ch].b.r&SET) && (scrseg[k+1]   ==FILL)) return 1;
  return 0;
}   //openend


int match(WORD ch,int r,int c)
{
  register char cc;
  register WORD k;

  k = CDEF_SIZE;
  k = r*scr.cols + c;
  if (r>0) {
    cc = lookup[(BYTE)scrseg[k-scr.cols]];
    if (cc >= 0) if (!(cdef[cc].b.b & cdef[ch].b.t)) return 0;
  }
  if (r < (scr.rows-1)) {
    cc = lookup[(BYTE)scrseg[k+scr.cols]];
    if (cc >= 0) if (!(cdef[cc].b.t & cdef[ch].b.b)) return 0;
  }
  if (c > 0) {
    cc = lookup[(BYTE)scrseg[k-1]];
    if (cc >= 0) if (!(cdef[cc].b.r & cdef[ch].b.l)) return 0;
  }
  if (c < (scr.cols-1)) {
    cc = lookup[(BYTE)scrseg[k+1]];
    if (cc >= 0) if (!(cdef[cc].b.l & cdef[ch].b.r)) return 0;
  }
  return 1;
}   //match


void drawguy(BYTE* g,int n,int r,int c)
{
  register WORD ch;
  while (n--) {
    ch = *g & 0x3f;
    if (ch >= CDEF_SIZE) break;
    if (!match(ch,r,c)) { *g = 0xFF; break; }
    scrseg[r*scr.cols+c] = CATTR+cdef[ch].c;
    ch = *g & 0xC0;
    switch (ch) {
    case 0x00:
      r--;
      if (r < 0) r = scr.rows - 1;
      break;
    case 0x40:
      r++;
      if (r >= scr.rows) r = 0;
      break;
    case 0x80:
      c--;
      if (c < 0) c = scr.cols - 1;
      break;
    default  :
      c++;
      if (c >= scr.cols) c = 0;
      break;
    }  //switch
    g++;
  }   //while
}   //drawguy


void randomguy(BYTE* g,int n)
{
#define TRIES 5
  int i;
  while (n--)
  {
    for (i = 0; (i < TRIES); i++)
    {
      *g = (BYTE)R.rnd();
      if ((*g&0x3f) < CDEF_SIZE) break;
    }   //for
    if (i >= TRIES) return;
    g++;
  }   //while
}   //randomguy


/*
void randomstart(void)
{
  for (int i = 0; (i < sizeof(atom)); i++) {
    atom[i].c = R.rnd(CDEF_SIZE);
    atom[i].f = 0;
    atom[i].l = i;
    scr.video[i] = CATTR + cdef[atom[i].c].c;
  }
  for (i=0;i<sizeof(atom);i++) {
    int r=R.rnd(roco);
    ????????????
}   //randomstart
*/



void main(void)
{
  if (!scr.rows) return;
  int i;
  for (i=0;i<0x100;i++) lookup[i]=-1;  //init lookup table
  for (i=0;i<CDEF_SIZE;i++) lookup[(BYTE)cdef[i].c]=i;  //EZ lookup for type

  BYTE done=0;                         //not done yet
  BYTE on=1;                           //set when doing global randoming

//  randomstart();

  while (!done) {
    if (on) {
      int r=R.rnd(scr.rows);
      int c=R.rnd(scr.cols);
      WORD ch=R.rnd(CDEF_SIZE+1);
      int k=r*scr.cols+c;
      if (ch==CDEF_SIZE) {
        char cc=lookup[(BYTE)scrseg[k]];
        if (cc>=0)
          if (openend(r,c,(WORD)cc)) scrseg[k]=FILL;
      }
      else {
        if (scrseg[k]==FILL)
          if (match(ch,r,c)) scrseg[k]=(WORD)cdef[ch].c+CATTR;
      }
    }   //if on
    if (kbhit()) {
      int c=getkey();
      switch (c) {
      case ESC:
        done=1;
        break;
      case 'c':
        scr.Fill(FILL);
        break;
      case 'r':
        randomguy(guy,sizeof(guy));
        drawguy(guy,sizeof(guy),scr.rows/2,scr.cols/2);
        break;
      case ' ': on=!on; break;
      default:
        ;
      }   //switch on input char
    }   //if kb
  }   //while
}   //main
