#include <stdio.h>


#include "wutil.h"


WIND* mw;


void main(void)
{
  int i;
  mw = wopen(2,"This is the name", 1,1, 48,78, ATTR(LIGHTBLUE,BLUE));
  for (i = 0; (i < 48); i++)
    wprintf(mw,"This is line number 0x%X.\n",i);
  wgetc(mw);
  wsetcpos(mw,0,0);
  for (i = 0; (i < 20); i++)
  {
    wdelrow(mw);
//    wgetc(mw);
  }
  for (i = 0; (i < 20); i++)
  {
    winsrow(mw);
//    wgetc(mw);
  }
  wgetc(mw);
  wpurge();
}   //main
