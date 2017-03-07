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

  /* initialize graphics mode */
  initgraph(&gdriver, &gmode, "");

  /* read result of initialization */
  errorcode = graphresult();

  if (errorcode != grOk)  /* an error occurred */
  {
     printf("Graphics error: %s\n", grapherrormsg(errorcode));
     exit(1);             /* return with error code */
  }


  R250_init(0);

  maxx = getmaxx() + 1;
  maxy = getmaxy() + 1;

  while (1)
  {
    x = R250_random_in(maxx);
    y = R250_random_in(maxy);

    c = (getpixel(x,y) + 1) % maxc;

    putpixel(x, y, c);

    if (kbhit()) if (getch() == '\x1b') break;
  }

  closegraph();
}   //main
