#include <stdio.h>
#include <conio.h>
#include <graphics.h>


#include "r250.h"


void main(void)
{
  /* request auto detection */
  int gdriver = DETECT, gmode, errorcode;
  int x, y, c;
  int maxx, maxy, maxc;

  double x_mu, x_sigma;
  double y_mu, y_sigma;

  R250_GENERATOR rng;

  /* initialize graphics mode */
  initgraph(&gdriver, &gmode, "");

  /* read result of initialization */
  errorcode = graphresult();

  if (errorcode != grOk)  /* an error occurred */
  {
     printf("Graphics error: %s\n", grapherrormsg(errorcode));
     exit(1);             /* return with error code */
  }


  rng = R250_Create(0);

  maxx = getmaxx() + 1;
  maxy = getmaxy() + 1;

  x_mu = maxx / 2;
  x_sigma = maxx / 8;
  y_mu = maxy / 2;
  y_sigma = maxy / 8;

  while (1)
  {
    x = R250_Normal_Random(rng, x_mu, x_sigma);
    y = R250_Normal_Random(rng, y_mu, y_sigma);

    c = (getpixel(x,y) + 1) % maxc;

    putpixel(x, y, c);

    if (kbhit()) if (getch() == '\x1b') break;
  }

  closegraph();
}   //main
