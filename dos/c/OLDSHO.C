#include <stdio.h>
#include <conio.h>
#include <dos.h>
#include <dir.h>
#include <fcntl.h>
#include <io.h>


#include "scrutil.h"
#include "strutil.h"
#include "wutil.h"


#define STATTR     ATTR(BLACK,LIGHTGRAY)
#define BKATTR     ATTR(LIGHTGRAY,BLACK)
#define TXATTR     ATTR(YELLOW,BLUE)

typedef struct { long off; unsigned len; } LineID;

int rows,cols;
char far* tp=NULL;
int   th=0;
long  tsize=0;
long  tline=0;
long  tlines=0;
long  toffs=0;
LineID far* tlidptr=NULL;
unsigned tbufseg;
unsigned tlidseg;
char* tname=NULL;
WIND* tw;
WIND* sw;


//===========================================================================
void far* Inc(void far* ptr)
{
  _AX=((unsigned*)&ptr)[0];
  _DX=((unsigned*)&ptr)[1];
  _AX+=1;
  if (_AX==0) _DX+=0x1000;
  _BX=_AX;
  _BX>>=4;
  _DX+=_BX;
  _AX&=0x000F;
  return MK_FP(_DX,_AX);
}   //Inc


//===========================================================================
void far* Ptr(void far* ptr)
{
  _AX=((unsigned*)&ptr)[0];
  _DX=((unsigned*)&ptr)[1];
  _BX=_AX;
  _BX>>=4;
  _DX+=_BX;
  _AX&=0x000F;
  return MK_FP(_DX,_AX);
}   //Ptr


//===========================================================================
void far* Add(void far* ptr,unsigned long offset)
{
  _DX=((unsigned*)&offset)[1];
  _DX<<=12;
  _DX+=((unsigned*)&ptr)[1];
  _AX=((unsigned*)&offset)[0];
  _AX+=((unsigned*)&ptr)[0];
  if (_AX < ((unsigned*)&ptr)[0]) _DX+=0x1000;
  _BX=_AX;
  _BX>>=4;
  _DX+=_BX;
  _AX&=0x000F;
  return MK_FP(_DX,_AX);
}   //Add


//===========================================================================
void statline(void)
{
  wclr(sw);
  wprintf(sw,
    "File=%s  Size=%lu  Lines=%lu  Line=%lu  Offset=%lu"
    ,tname
    ,tsize
    ,tlines
    ,tline+1
    ,toffs
  );   //wprintf
}   //statline


//===========================================================================
long linecount(char far* b,long len)
{
  long lin=0;
  long i=0;

  if (len==0) return 0;
  while (1) {
    if (*b=='\n') lin++;
    i++;
    if (i>=len) { if (*b!='\n') lin++; break; }
    b=Inc(b);
  }
  return lin;
}   //linecount


//===========================================================================
int  displaybuffer(void)
{
  int line;
  unsigned i;
  LineID far* lidp;
  char far* txt;

  while (1) {
    wupd=0;
    wsetattr(tw,BKATTR);
    wclr(tw);
    wsetattr(tw,TXATTR);
    if (tline<0) tline=0;
    if (tline>=tlines) tline=tlines-1;
    lidp=Add(tlidptr,tline*sizeof(LineID));
    toffs=lidp->off;
    for (line=0;(line<tw->rows)&&((tline+line)<tlines);line++) {
      txt=Add(tp,lidp->off);
      for (i=0;i<lidp->len;i++) wputc(tw,txt[i]);
      lidp=Add(lidp,sizeof(LineID));
    }
    if ((line<tw->rows)&&((tline+line)>=tline)) {
      wsetattr(tw,BKATTR);
      wputs(tw,"[EOF]");
      wsetattr(tw,TXATTR);
    }
    wupdate(tw);
    statline();

    switch (wgetc(tw)) {
    case _PGUP:
      tline-=(tw->rows-1);
      break;
    case _PGDN:
      tline+=(tw->rows-1);
      break;
    case _UPAR:
      tline--;
      break;
    case _DNAR:
      tline++;
      break;
    case ESC:
      return 0;
    case A_X:
      return 1;
    }   //switch
  }   //forever
}   //displaybuffer


//===========================================================================
int  maketlids(void)
{
  LineID far* p;
  char far* ptr=tp;
  long i=0;
  long offs=0;

  if (allocmem((tlines*sizeof(LineID)+15)>>4,&tlidseg)!=-1) return 0;
  tlidptr=MK_FP(tlidseg,0);
  p=Ptr(tlidptr);

  while (i<tsize) {
    ptr=Add(tp,offs);
    p->off=offs;
    p->len=0;
    while ((i<tsize)&&(*ptr!='\n')) {
      i++;
      offs++;
      ptr++;
      p->len++;
    }
    if (i<tsize) p->len++;
    p=Add(p,sizeof(LineID));
  }   //while
  return 1;
}   //maketlids


//===========================================================================
void loadfile(int handle,long size,char far* buffer)
{
  long i;
  int n;

  i=0;
  while (i<size) {
    n=0x4000;
    if ((i+0x4000L)>size) n=(int)size&0x3FFF;
    i+=(long)read(handle,buffer,n);
    buffer=Add(buffer,0x4000L);
  }   //while
}   //loadfile


//===========================================================================
int  main(int argc,char* argv[])
{
  int i;

  cols=getscols();
  rows=getsrows();
  wupd=0;
  sw=wopen(0,NULL, 0,0,      1,cols, STATTR);
  sw->fl.wrap=0;
  sw->fl.scroll=0;
  tw=wopen(0,NULL, 1,0, rows-1,cols, TXATTR);
  tw->fchr=0xB1;
  tw->fl.wrap=0;
  tw->fl.scroll=0;
  wupdate(NULL);
  for (i=1;i<argc;i++) {
    tname=argv[i];
    th=open(tname,O_RDONLY|O_BINARY);
    if (th==-1) continue;
    tsize=filelength(th);
    if (allocmem((tsize+16)>>4,&tbufseg)!=-1) {
      wsetattr(sw,wgetattr(sw)|BLINK);
      wprintf(sw,"Not enough memory to load \"%s\"!",tname);
      wgetc(sw);
      wsetattr(sw,wgetattr(sw)&~BLINK);
      continue;
    }
    tp=MK_FP(tbufseg,0);
    loadfile(th,tsize,tp);
    close(th);
    tlines=linecount(tp,tsize);
    toffs=0;
    tline=0;
    if (!maketlids()) {
      freemem(tbufseg);
      wsetattr(sw,wgetattr(sw)|BLINK);
      wprintf(sw,"Not enough memory to load \"%s\"!",tname);
      wgetc(sw);
      wsetattr(sw,wgetattr(sw)&~BLINK);
      continue;
    }
    wsetattr(tw,TXATTR);
    if (displaybuffer()) i=argc;
    freemem(tbufseg);
    freemem(tlidseg);
  }
  wpurge();
  return 0;
}   //main

