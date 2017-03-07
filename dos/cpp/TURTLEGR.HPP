#ifndef __TURTLEGR_HPP
#define __TURTLEGR_HPP


#include <math.h>
#include <graphics.h>

#include "grutil.hpp"


class TurtleGraphics : Graph {
private:
  double x,y;
  double xmid,ymid;
  double a;
  int color;
public:
  double X(void) { return x; }
  double Y(void) { return y; }
  double Dir(void) { return a*180.0/M_PI; }
  int Color(void) { return color; }
  void Reset(int _color=-1);           //color==-1 == use max color
  TurtleGraphics(void) : Graph() { Reset(); return; }
  void Draw(double len) { lineto(x+=len*cos(a),y-=len*sin(a)); }
  void Move(double len) { moveto(x+=len*cos(a),y-=len*sin(a)); }
  void Turn(double deg) { a += deg*M_PI/180.0; }
  void Fill(int _color=-1,int border=-1,int pattern=SOLID_FILL);
  void Goto(double _x,double _y) { moveto(x=_x,y=_y); }
  void Dir(double deg) { a = deg*M_PI/180.0; }
  void Pen(int _color);
};


#endif
