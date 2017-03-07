#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <math.h>


#include "grutil.hpp"
#include "r250.hpp"
#include "scrutil.hpp"
#include "vecutil.hpp"


#define G          6.67e-11            //gravitational constant
#define C          2.9975e+08          //speed of light

typedef unsigned char byte;
typedef unsigned int  word;

double inline deg(double a) { return a*(180.0/M_PI); }


int grGraphon=0;                       //tells that grGraph is on
double xang=M_PI/4.0;                  //angles for display
double yang=M_PI/4.0;                  //angles for display
double xmid,ymid;                      //middle of screen
Vec o;                                 //origin
Vec basis[3];                          //basis for looking

class grGraph: public Graph {          //ok, but reduce Ymax
public:
  grGraph() : Graph() {                //construct, but reduce Ymax
    if (grGraphon++) return;           //see if already reduced
    Ymax-=16;                          //reducing!
    xmid=Xmax/2;                       //get middle of screen for x direction
    ymid=Ymax/2;                       //..for y direction
    o<<0.0<<0.0<<0.0;                  //start origin at 0,0,0
    basis[0]<<1.0<<0.0<<0.0;           //x direction
    basis[1]<<0.0<<1.0<<0.0;           //y direction
    basis[2]<<0.0<<0.0<<1.0;           //z direction
  }   //grGraph::grGraph
  ~grGraph() { grGraphon--; }          //reduce reduction count
};   //class grGraph


class Star {                           //for stars/planets, etc
public:
  double mass;                         //object's mass
  Vec x,v;                             //position, velocity

  Star() { x.Fill(); v.Fill(); mass=0; }         //default constructor
  virtual Vec& Update(void) { x+=v; return x; }  //updates the position
};   //class Star


class ScrStar: public Star {           //graphics star
public:                                //all's public
  int sx,sy,sc,onscreen;               //screen coordinates
  ScrStar();                           //call default Star
  void Put(void)   {                   //puts the star on the screen
    putpixel(sx,sy,sc);
    if (sx>0) putpixel(sx-1,sy,sc);
    if (sx<Xmax) putpixel(sx+1,sy,sc);
    if (sy>0) putpixel(sx,sy-1,sc);
    if (sy<Ymax) putpixel(sx,sy+1,sc);
  }
  void UnPut(void) {                   //deletes from the screen
    putpixel(sx,sy,0);
    if (sx>0) putpixel(sx-1,sy,0);
    if (sx<Xmax) putpixel(sx+1,sy,0);
    if (sy>0) putpixel(sx,sy-1,0);
    if (sy<Ymax) putpixel(sx,sy+1,0);
  }
  Vec& Update(void);
};   //class ScrStar

ScrStar::ScrStar() : Star() { sc=Cmax; onscreen=0; }

Vec& ScrStar::Update(void)             //checks for location on screen
{
  Star::Update();                      //find new position
  double dot2=x*basis[2];              //dot product with up-axis
  double dot1=x*basis[1];              //dot product with left-axis
  double angx=((x-basis[2]*dot2) >> basis[0])/xang;  //angle left
  double angy=((x-basis[1]*dot1) >> basis[0])/yang;  //angle up

  if ((angx<1.0) && (angy<1.0)) {       //check angles
    if (onscreen) UnPut();             //if was on screen, take it off
    if (dot2<0.0) angy=-angy;          //below plane
    if (dot1<0.0) angx=-angx;          //right of plane
    sx=xmid*(1.0-angx);                //get x-coordinate
    sy=ymid*(1.0-angy);                //get y-coordinate
    Put();                             //put it on the screen
    onscreen=1;                        //it's now on the screen
  }
  else {   //else not on screen now
    if (onscreen) UnPut();             //if was on screen, take it off
    onscreen=0;                        //mark as not on screen
  }   //else not on screen
  return x;                            //return the new position
}   //ScrStar::Check(void)



#if 0
void main(void)
{
  Vec a,b,c,d;

  a<<1<<0<<0;
  b<<0<<1<<0;
  c=3.0;
  d=a+b+c;

  cout<<"=============================================================\n";
  cout << "a=" << a <<"\n";
  cout << "b=" << b <<"\n";
  cout << "c=" << c <<"\n";
  cout << "d=" << d <<"  =a+b+c\n";
}   //temp main
#else


grGraph gr;                            //initialize graphics and constants
R250 R;

#define NUM_STARS 40
ScrStar star[NUM_STARS];


//===========================================================================


void main(void)
{
  int i;
  double sp=0.2;
  double speed=10.0;

  for (i=0;i<NUM_STARS;i++) {
    star[i].x <<100.0<<R.frnd(-50,50)<<R.frnd(-50,50);  //put it out there
    star[i].v <<0.0<<0.0<<0.0;
    star[i].sc=R.rnd(7)+9;
    star[i].Update();                     //put it on the screen
    star[i].v << R.frnd(-sp,sp) << R.frnd(-sp,sp) << sp*R.frnd(-sp,sp);
  }   //for

  int done=0;
  while (!done) {
    if (kbhit()) {
      switch(getkey()) {
      case 'i':
        o=o+speed*basis[0];
        break;
      case 'k':
        o=o-speed*basis[0];
        break;
      case 'j':
        o=o+speed*basis[1];
        break;
      case 'l':
        o=o-speed*basis[1];
        break;
      case 'I':
        o=o+speed*basis[2];
        break;
      case 'K':
        o=o-speed*basis[2];
        break;
      case 'u':
        for (i=0;i<NUM_STARS;i++) star[i].Update();
        circle(xmid,ymid,4);
        break;
      case ESC:
        done=1;
        break;
      default: ;
      }   //switch
    }   //servicing kb
  }   //while
}   //main(void)


#endif
