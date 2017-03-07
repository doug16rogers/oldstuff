#include <stdio.h>
#include <graphics.h>
#include <conio.h>
#include <stdlib.h>


#include "r250.hpp"
#include "scrutil.hpp"
#include "grutil.hpp"

typedef unsigned char byte;
typedef unsigned int  word;

R250 R;



class Guy:Graph {
private:
  int x;
  int y;
  int c;
  int oldc;
  int un;
  int xoff;
  int xmax;
  int yoff;
  int ymax;
public:
  Guy(void);
  Guy(char* bgi_dir);
  Guy(int x0,int y0,int c0,char* bgi_dir="");
  int  Get(void)   { return getpixel(x,y); }
  void Put(void)   {
    putpixel(x,y,c);
    if (x>0) putpixel(x-1,y,c);
    if (x<xmax) putpixel(x+1,y,c);
    if (y>0) putpixel(x,y-1,c);
    if (y<ymax) putpixel(x,y+1,c);
  }
  void UnPut(void) {
    if (!un) return;
    putpixel(x,y,0/*oldc*/);
    if (x>0) putpixel(x-1,y,0/*oldc*/);
    if (x<xmax) putpixel(x+1,y,0/*oldc*/);
    if (y>0) putpixel(x,y-1,0/*oldc*/);
    if (y<ymax) putpixel(x,y+1,0/*oldc*/);
  }
  int  Un(void) { return un; }
  void Un(int u) { un=u; }
  int  Swap(void)  { oldc=getpixel(x,y); Put(); return oldc; }
  virtual int  Left(int n=1);
  virtual int  Right(int n=1);
  virtual int  Down(int n=1);
  virtual int  Up(int n=1);
  void Off(int xo=0,int yo=0) {
    if ((xo>=0)&&(xo<=Xmax)) xoff=xo;
    if ((yo>=0)&&(yo<=Xmax)) yoff=xo;
  }
  int  XOff(void) { return xoff; }
  int  YOff(void) { return yoff; }
  void Max(int xm=0,int ym=0) {
    if ((xm>=0)&&(xm<=Xmax)) xmax=xm;
    if ((ym>=0)&&(ym<=Xmax)) ymax=xm;
  }
  int XMax(void) { return xmax; }
  int YMax(void) { return ymax; }
  void Set(int x0,int y0,int c0) { UnPut(); x=x0; y=y0; c=c0; Swap(); }
  operator ++() { if (c>=Cmax) c=0; else c++; Put(); return c; }
  operator --() { if (c<=0) c=Cmax; else c--; Put(); return c; }
};   //class Guy


Guy::Guy(char* bgi_dir)
{
  Graph(bgi_dir);
  x=0;
  y=0;
  c=Cmax;
  un=1;
  xoff=0;
  xmax=Xmax;
  yoff=0;
  ymax=Ymax;
  Swap();
}   //Guy::Guy


Guy::Guy()
{
  Graph();
  x=0;
  y=0;
  c=Cmax;
  un=1;
  xoff=0;
  xmax=Xmax;
  yoff=0;
  ymax=Ymax;
  Swap();
}   //Guy::Guy


Guy::Guy(int x0,int y0,int c0,char* bgi_dir)
{
  Graph(bgi_dir);
  x=x0;
  y=y0;
  c=c0;
  un=1;
  xoff=0;
  xmax=Xmax;
  yoff=0;
  ymax=Ymax;
  Swap();
}   //Guy::Guy

int  Guy::Left(int n)  {
  UnPut();
  x-=n;
  if (x<0) x+=xmax;
  Swap();
  return x;
}   //Guy::Left

int  Guy::Right(int n)
{
  UnPut();
  x+=n;
  if (x>xmax) x-=xmax;
  Swap();
  return x;
}   //Guy::Right

int  Guy::Down(int n)
{
  UnPut();
  y+=n;
  if (y>ymax) y-=ymax;
  Swap();
  return y;
}   //Guy::Down

int  Guy::Up(int n)
{
  UnPut();
  y-=n;
  if (y<0) y+=ymax;
  Swap();
  return y;
}   //Guy::Up



#define VMAX  8

class vGuy:public Guy {                //Guy's, but with velocities
private:
  int vx,vy;                           //velocity in x- and y-directions
public:
  vGuy();
  vGuy(char* bgi_dir="");
  inline void UpDate(int show=1);
  virtual int  Left(int n=1,int show=1);
  virtual int  Right(int n=1,int show=1);
  virtual int  Down(int n=1,int show=1);
  virtual int  Up(int n=1,int show=1);
  void vSet(int vx0,int vy0) { vx=vx0; vy=vy0; }
};   //class vGuy


vGuy::vGuy(char* bgi_dir)
{
  Guy(bgi_dir);
  vx=vy=0;
}   //vGuy::vGuy(char*)


vGuy::vGuy()
{
  Guy("k:\\bc\\bgi");
  vx=vy=0;
}   //vGuy::vGuy()

void vGuy::UpDate(int show)
{
  if (show) UnPut();
  if (vx) if (vx>0) Guy::Right(vx); else Guy::Left(-vx);
  if (vy) if (vy>0) Guy::Down(vy); else Guy::Up(-vy);
  if (show) Put();
}   //UpDate


int  vGuy::Left(int n,int show)
{
  vx-=n;
  if (vx<-VMAX) vx=-VMAX; else if (vx>VMAX) vx=VMAX;
  UpDate(show);
  return vx;
}   //vGuy::Left

int  vGuy::Right(int n,int show)
{
  vx+=n;
  if (vx<-VMAX) vx=-VMAX; else if (vx>VMAX) vx=VMAX;
  UpDate(show);
  return vx;
}   //vGuy::Right

int  vGuy::Down(int n,int show)
{
  vy+=n;
  if (vy<-VMAX) vy=-VMAX; else if (vy>VMAX) vy=VMAX;
  UpDate(show);
  return vy;
}   //vGuy::Down

int  vGuy::Up(int n,int show)
{
  vy-=n;
  if (vy<-VMAX) vy=-VMAX; else if (vy>VMAX) vy=VMAX;
  UpDate(show);
  return vy;
}   //vGuy::Up



#define AMAX  1

class aGuy:public vGuy {               //Guy's, but with velocities
private:
  int ax,ay;                           //velocity in x- and y-directions
public:
  aGuy();
  aGuy(char* bgi_dir="");
  inline void UpDate(void);
  virtual int  Left(int n=1);
  virtual int  Right(int n=1);
  virtual int  Down(int n=1);
  virtual int  Up(int n=1);
  void aSet(int ax0,int ay0) { ax=ax0; ay=ay0; }
};   //class vGuy


aGuy::aGuy(char* bgi_dir) : vGuy(bgi_dir)
{
  ax=ay=0;
}   //aGuy::aGuy(char*)


aGuy::aGuy() : vGuy("k:\\bc\\bgi")
{
  ax=ay=0;
}   //aGuy::aGuy()

void aGuy::UpDate(void)
{
  UnPut();
  if (ax || ay) {
    if (ax>0) vGuy::Right(ax,0); else vGuy::Left(-ax,0);
    if (ay>0) vGuy::Down(ay,0); else vGuy::Up(-ay,0);
  }
  vGuy::UpDate(0);
  Put();
}   //aGuy::UpDate


int  aGuy::Left(int n)
{
  ax-=n;
  if (ax<-AMAX) ax=-AMAX; else if (ax>AMAX) ax=AMAX;
  UpDate();
  return ax;
}   //vGuy::Left

int  aGuy::Right(int n)
{
  ax+=n;
  if (ax<-AMAX) ax=-VMAX; else if (ax>VMAX) ax=VMAX;
  UpDate();
  return ax;
}   //aGuy::Right

int  aGuy::Down(int n)
{
  ay+=n;
  if (ay<-VMAX) ay=-VMAX; else if (ay>VMAX) ay=VMAX;
  UpDate();
  return ay;
}   //aGuy::Down

int  aGuy::Up(int n)
{
  ay-=n;
  if (ay<-VMAX) ay=-VMAX; else if (ay>VMAX) ay=VMAX;
  UpDate();
  return ay;
}   //aGuy::Up




#define GNUM 40
vGuy g[GNUM];




void centerrand(void)
{
  for (int i=0;i<GNUM;i++) {
    int x4=g[i].XMax()/4;
    int y4=g[i].YMax()/4;
    g[i].Set(R.rnd(x4)+x4+x4/2,R.rnd(y4)+y4+y4/2,R.rnd(Cmax/2)+9);
  }   //for
}   //centerrand



//=========================================================================
void main (void)
{
  centerrand();

  int i,goin=1;
  int move=1;
  while (goin) {
    while (kbhit()) {
      switch (getkey()) {
      case '+':
      case _PGUP:
        for (i=0;i<GNUM;i++) g[i]++;
        break;
      case '-':
      case _PGDN:
        for (i=0;i<GNUM;i++) g[i]--;
        break;
      case 'i':
      case _UPAR:
        for (i=0;i<GNUM;i++) g[i].Guy::Up();
        break;
      case 'k':
      case _DNAR:
        for (i=0;i<GNUM;i++) g[i].Guy::Down();
        break;
      case 'j':
      case _LTAR:
        for (i=0;i<GNUM;i++) g[i].Guy::Left();
        break;
      case 'l':
      case _RTAR:
        for (i=0;i<GNUM;i++) g[i].Guy::Right();
        break;
      case 'I':
      case '8':
        for (i=0;i<GNUM;i++) g[i].Guy::Up(8);
        break;
      case 'K':
      case '2':
        for (i=0;i<GNUM;i++) g[i].Guy::Down(8);
        break;
      case 'J':
      case '4':
        for (i=0;i<GNUM;i++) g[i].Guy::Left(8);
        break;
      case 'L':
      case '6':
        for (i=0;i<GNUM;i++) g[i].Guy::Right(8);
        break;
      case 'r':
        for (i=0;i<GNUM;i++) {
          g[i].Set(R.rnd(Xmax)+1,R.rnd(Ymax)+1,R.rnd(Cmax)+1);
//          g[i].vSet(R.rnd(VMAX*2+1)-VMAX,R.rnd(VMAX*2+1)-VMAX);
        }   //for
        break;
      case 'R':
        centerrand();
        break;
      case 'm':
        move=!move;
        break;
      case 'u':
        for (i=0;i<GNUM;i++) g[i].Un(!g[i].Un());
        break;
      case 'c':
        setcolor(0);
        setwritemode(0);
        for (i=0;i<=Ymax;i++) line(0,i,Xmax,i);
        break;
      case 'e':
        for (i=0;i<GNUM;i++) g[i].Up();
        break;
      case 'd':
        for (i=0;i<GNUM;i++) g[i].Down();
        break;
      case 's':
        for (i=0;i<GNUM;i++) g[i].Left();
        break;
#if 0
      case '0':
        for (i=0;i<GNUM;i++) { g[i].vSet(0,0); g[i].aSet(0,0); }
        break;
#endif
      case 'f':
        for (i=0;i<GNUM;i++) g[i].Right();
        break;
      case ESC: goin=0; break;
      }   //switch on keypress
    }   //if key hit

    if (move) {
      for (i=0;i<GNUM;i++) {
        switch (R.rnd(4)) {
        case 0: g[i].Up(); break;
        case 1: g[i].Down(); break;
        case 2: g[i].Left(); break;
        case 3: g[i].Right(); break;
        }   //switch
      }   //for
    }   //if moving

  }   //while (main loop)
}   //main
