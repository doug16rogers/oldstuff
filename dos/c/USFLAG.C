#include <graphics.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <math.h>


typedef struct { double x,y; }    Point;
typedef struct { int    x,y; } intPoint;


//===========================================================================
double away0(double x) {
#if 1
  return x;
#else
  if (x>=0.0)
    return (x+0.5);
  else
    return (x-0.5);
#endif
}   //away0


//===========================================================================
int  GenerateNStar(double outer_r,double inner_r,Point p[],int n) {
  double a,d_a;
  int i;

  d_a=M_PI/((double)n);                //angle between each successive point
  if ((inner_r<=0.0)&&(n>4))           //make level top line
    inner_r=outer_r*sin(M_PI_2-2*d_a)/sin(M_PI_2+d_a);

  for (i=0,a=-M_PI_2;i<2*n;i+=2) {     //for each spot, start at pi/2 (90o)
    p[i+0].x=away0(outer_r*cos(a));
    p[i+0].y=away0(outer_r*sin(a));
    a+=d_a;
    p[i+1].x=away0(inner_r*cos(a));
    p[i+1].y=away0(inner_r*sin(a));
    a+=d_a;
  }
  return i;
}   //GenerateNStar


//===========================================================================
void ShowRelativePoly(int x,int y,double scale,Point rela_p[],int n) {
  intPoint* p;
  int i;

  p=malloc(n*sizeof(intPoint));
  if (!p) return;
  if (scale<=0) scale=1;
  for (i=0;i<n;i++) {
    p[i].x=(double)x+scale*rela_p[i].x;
    p[i].y=(double)y+scale*rela_p[i].y;
  }   //for
  fillpoly(n,(int*)p);
}   //ShowRelativePoly


//===========================================================================
void FillRectangle(int x0,int y0,int x1,int y1,int color) {
  intPoint p[4];
  p[0].x=x0;  p[0].y=y0;
  p[1].x=x1;  p[1].y=y0;
  p[2].x=x1;  p[2].y=y1;
  p[3].x=x0;  p[3].y=y1;
  setcolor(color);
  setfillstyle(SOLID_FILL,color);
  fillpoly(4,(int*)p);
}   //FillRectangle


//===========================================================================
void DrawUSFlag(double x0,double y0,double x1,double y1) {
  Point star[10];
  double x_2,y_13,y_10,x_7;
  double x,y;
  int i,j;

  x_2=(x1-x0)/2.0;
  y_13=(y1-y0)/13.0;
  FillRectangle(x0,     y0+ 0*y_13, x0+x_2, y0+ 7*y_13, BLUE);
  x=x0+x_2;
  y=y0;
  for (i=0;i<7;i++) {
    FillRectangle(x, y, x1, y+y_13, (i&1) ? WHITE : RED);
    y=y+y_13;
  }   //for top bars
  x=x0;
  for (;i<13;i++) {
    FillRectangle(x, y, x1, y+y_13, (i&1) ? WHITE : RED);
    y=y+y_13;
  }   //for bottom bars
  y_10=7.0*y_13/10.0;
  x_7=x_2/6.0;
  GenerateNStar(y_10/2,0.0,star,5);
  setcolor(WHITE);
  setfillstyle(SOLID_FILL,WHITE);

  y=y0+y_10;
  for (j=0;j<5;j++) {
    x=x0+x_7/2.0;
    for (i=0;i<6;i++) {
      ShowRelativePoly(x,y,1.0,star,10);
      x=x+x_7;
    }   //for i
    y=y+2*y_10;
  }   //for j

  y=y0+y_10+y_10;
  for (j=0;j<4;j++) {
    x=x0+x_7;
    for (i=0;i<5;i++) {
      ShowRelativePoly(x,y,1.0,star,10);
      x=x+x_7;
    }   //for i
    y=y+2*y_10;
  }   //for j
}   //DrawUSFlag


//===========================================================================
int main(void)
{
   int gdriver = DETECT, gmode, errorcode;

   initgraph(&gdriver, &gmode, "");
   errorcode = graphresult();

   if (errorcode != grOk)  /* an error occurred */
   {
      printf("Graphics error: %s\n", grapherrormsg(errorcode));
      return 1;
   }

   /* draw a line */
   DrawUSFlag(0,0,getmaxx(),getmaxy());
   getch();

   closegraph();
   return 0;
}   //main


