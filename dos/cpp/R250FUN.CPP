#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <graphics.h>
#include <alloc.h>

#include "strutil.h"
#include "w:\\rogers\\doug\\cpp\\scrutil.h"

#define SIZE           250
#define PREV           103

#ifndef WORD
  typedef unsigned int WORD;
#endif
#ifndef DWORD
  typedef unsigned long int DWORD;
#endif

#define LOWORD(x)      (*((WORD*)&(x)))
#define HIWORD(x)      (*(((WORD*)&(x))+1))



class PRand {                          //pseudorandom number generator
private:
  int  index;                          //index of second array element
  DWORD r[SIZE];                       //pointer to array
public:
  PRand(void);                         //fill randomly lau time
  PRand(int seed);                     //seed for initialization
//~PRand();                            //destructor doesn't do anything
  DWORD rnd(void);                     //returns random on [0,0xFFFFFFFF]
  WORD rnd(WORD topval) {              //returns random on [0,topval-1]
    if (topval>0)
      return ((WORD)rnd()) % topval;
    else
      return ((WORD)rnd()) & 1;
  };
  double frnd(void) {                  //doublizer, random on [0.0,1.0]
    return ((double)(DWORD)rnd())/((double)0xFFFFFFFFUL);
  };
};   //class PRand


PRand::PRand(void)
{
  int i;
  DWORD k=1;

  randomize();
  for (i=0;i<SIZE;i++) {               //put in initial random numbers
    HIWORD(r[i])=(rand()<<1) ^ rand(); //..initialize MSW, rand onl
    LOWORD(r[i])=(rand()<<1) ^ rand(); //..initialize LSW
  }   //for initial values
  for (i=0;i<32;i++) {                 //make sure we have a 32-bit basis..
    r[i] |= k;                         //..by setting each bit
    r[i+1] &= ~k;                      //..and resetting it, too
    k<<=1;                             //..go to next bit
  }   //for basis-ensuring
  index=0;
  for (i=0;i<32;i++) rnd();
}   //PRand::PRand(void)


PRand::PRand(int seed)
{
  int i;
  DWORD k=1;

  srand(seed);
  for (i=0;i<SIZE;i++) {               //put in initial random numbers
    HIWORD(r[i])=(rand()<<1) ^ rand(); //..initialize MSW, rand onl
    LOWORD(r[i])=(rand()<<1) ^ rand(); //..initialize LSW
  }   //for initial values
  for (i=0;i<32;i++) {                 //make sure we have a 32-bit basis..
    r[i] |= k;                         //..by setting each bit
    r[i+1] &= ~k;                      //..and resetting it, too
    k<<=1;                             //..go to next bit
  }   //for basis-ensuring
  index=0;
}   //PRand::PRand(int seed)


DWORD PRand::rnd(void)
{
  DWORD v;

  if (index>=PREV)
    v=r[index] ^ r[index-PREV];
  else
    v=r[index] ^ r[index+SIZE-PREV];
  r[index++]=v;
  if (index>=SIZE) index=0;
  return v;
}   //DWORD PRand::rnd(void)





void main(int argc,char* argv[])
{
  PRand R;

  if (argc<2) {
    int r,c;
    int rows;
    int cols;
    WORD screen[50][80];
    WORD *wn;
    WORD far *wf;
    if (!setscrseg()) return;          //set the scrseg variable
    rows=getsrows();                   //get number of rows on screen
    cols=getscols();                   //columns
    wf=scrseg;                         //point to screen
    wn=&screen[0][0];                  //point to our copy of screen
    for (r=0;r<rows;r++) for (c=0;c<cols;c++) {
      *wn++=*wf;
      *wf++=0x00FE;
    }
    while (1) {
      r=R.rnd(rows)*cols + R.rnd(cols);
      scrseg[r]+=0x0100;
      scrseg[r]&=0x0FFF;
      if (kbhit()) {
        r=getkey();
        if (r==0x0d) break;
        if (r==ESC) {
          wf=scrseg;
          wn=&screen[0][0];
          for (r=0;r<rows;r++) for (c=0;c<cols;c++) *wf++=*wn++;
          break;
        }   //if ESC
        if (r==' ') getkey();
      }   //if key pressed
    }   //while
  }
  else if (argv[1][0]) {
    int gdriver = DETECT;
    int gmode, errorcode;
    int xmax,ymax,cmax;
    int x,y,c;
    int siz;
    char goin=1;
    BYTE far* img;

    initgraph(&gdriver, &gmode, "");
    errorcode = graphresult();

    if (errorcode != grOk) {
       printf("Graphics error: %s\n", grapherrormsg(errorcode));
       printf("Press any key to halt:");
       getch();
       exit(1);             /* return with error code */
    }

    xmax=getmaxx();
    ymax=getmaxy();
    cmax=getmaxcolor();

    while (goin) {
      x=R.rnd(xmax);
      y=R.rnd(ymax);
      c=getpixel(x,y)+1;
      if (c>cmax) c=1;
      putpixel(x,y,c);
      if (kbhit()) {
        c=getkey();
        switch (c) {
        case ESC: goin=0; break;
        case ' ':  getkey(); break;
        case 'b':
          img=(BYTE far*)farmalloc(siz=imagesize(0,0,0x1f,0x1f));
          if (!img) break;
          getimage(x=0,y=0,0x1f,0x1f,img);
          do {
            putimage(x,y,img,XOR_PUT);
            c=getkey();
            putimage(x,y,img,XOR_PUT);
            switch(c) {
            case _LTAR: if (x) x--; break;
            case _RTAR: if (x<(xmax-0x20)) x++; break;
            case _UPAR: if (y) y--; break;
            case _DNAR: if (y<(ymax-0x20)) y++; break;
            case 's': {
                FILE* f;
                f=fopen("fun.dat","wb");
                if (!f) break;
                fwrite(img,siz,1,f);
                fclose(f);
              }
              break;
            }   //switch
          } while (c!=ESC);
          farfree(img);
          break;
        }   //switch
      }   //if key
    }   //while

    closegraph();
  }   //else graphics mode
}   //main

