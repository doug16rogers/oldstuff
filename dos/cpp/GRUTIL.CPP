#include "grutil.hpp"


static int Graph_on=0;                 //set when first initialized
int Xmax;
int Ymax;
int Cmax;


Graph::Graph(char* bgi_dir)            //turns on graphics
{
  if (Graph_on++) { err=grOk; return; }
  int gdriver = DETECT;
  int gmode;

  initgraph(&gdriver, &gmode, bgi_dir);
  err=graphresult();
  if (err!=grOk) { Graph_on=0; return; }
  Xmax=getmaxx();
  Ymax=getmaxy();
  Cmax=getmaxcolor();
  setcolor(Cmax);
}   //Graph::Graph


Graph::~Graph()
{
  Graph_on--;
  if (Graph_on<=0) {
    Graph_on=0;
    closegraph();
  }
}   //Graph::~Graph


