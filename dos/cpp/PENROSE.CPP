#include <stdiostr.h>
#include <conio.h>
#include <graphics.h>
#include <math.h>

#include "turtlegr.hpp"


#define _36_DEG  (M_PI/5.0)
#define PHI      (1.618033988749894903)
#define SCALE    40.0

typedef struct { int x; int y; } iPoint;
typedef struct { double x; double y; } dPoint;
typedef struct { double a; double r; } RadPoint;


TurtleGraphics t;


//
//
//                  72
//                  /\
//                 /  \
//            phi /    \ phi
//               / dart \
//              /   108  \
//             /   ----   \
//            /36--    --36\
//        108/---        ---\108
//           \72  1    1  72/
//            \            /
//             \          /
//              \  kite  /
//           phi \      / phi
//                \    /
//                 \  /
//                  \/
//                  72
//
//

double scale=SCALE;
double one=1.0*scale;
double phi=PHI*scale;


void DrawKite(void)
{
  double x=t.X();
  double y=t.Y();
  double d=t.Dir();

  t.Turn(-36.0);
  t.Draw(phi);
  t.Turn(108.0);
  t.Draw(one);
  t.Turn(36.0);
  t.Draw(one);
  t.Turn(108.0);
  t.Draw(phi);

  t.Goto(x,y);
  t.Dir(d);
  t.Move(one/2);
  t.Fill();
  t.Goto(x,y);
}   //DrawKite





void main(void) {
  int col=Cmax;
  char alive=1;
  int c;
  int a=0;

  while (alive) {
    t.Dir((double)a*36.0+90.0);
    DrawKite();
    c=getch();
    col=t.Color();
    t.Pen(BLACK);
    DrawKite();
    t.Pen(col);
    switch (c) {
    case 'i': t.Goto(t.X(),t.Y()-1); break;
    case 'k': t.Goto(t.X(),t.Y()+1); break;
    case 'j': t.Goto(t.X()-1,t.Y()); break;
    case 'l': t.Goto(t.X()+1,t.Y()); break;
    case 's': a++; if (a>=10) a=0; break;
    case 'f': a--; if (a<0) a=10; break;
    case 'd': if (t.Color()<=1) t.Pen(Cmax); else t.Pen(t.Color()-1); break;
    case 'e': if (t.Color()>=Cmax) t.Pen(1); else t.Pen(t.Color()+1); break;
    case 'D':
      if (scale>3.0) scale -= 1.0;
      one=1.0*scale;
      phi=PHI*scale;
      break;
    case 'E':
      scale += 1.0;
      one=1.0*scale;
      phi=PHI*scale;
      break;
    case ' ':
      DrawKite();
      a=0;
      t.Goto(Xmax/4*3,Ymax);
      break;
    case 0x1B: alive=0; break;
    }   //switch
  }   //while
}   //main


