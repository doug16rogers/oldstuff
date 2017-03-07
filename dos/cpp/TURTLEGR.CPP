#include "turtlegr.hpp"


//===========================================================================
void TurtleGraphics::Reset(int _color)
{
  Pen(_color);
  x=((double)Xmax)/2.0;
  y=((double)Ymax)/2.0;
  moveto(x,y);
  a=0.0;
}   //TurtleGraphics::Reset(int)


//===========================================================================
void TurtleGraphics::Pen(int _color)
{
  if ((_color<0) && (_color>Cmax)) _color=Cmax;
  color=_color;
  setcolor(color);
}   //TurtleGraphics::Pen(int)


//===========================================================================
void TurtleGraphics::Fill(int _color,int border,int pattern)
{
  if ((_color<0)||(_color>Cmax)) _color=color;
  if ((border<0)||(border>Cmax)) border=color;
  if (pattern<0) pattern=SOLID_FILL;
  setfillstyle(pattern,_color);
  floodfill(x,y,border);
}   //TurtleGraphics::Fill(int,int,int)


