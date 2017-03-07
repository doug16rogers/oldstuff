#include <conio.h>
#include <stdio.h>


char scratch[0x200];

int main(void)
{
  int x,y;
  int moved = 0;

  x = wherex();
  y = wherey();

  gotoxy(70,25);
  gettext(70, 25, 80, 25, scratch);
  printf("\x1b[H");
  puttext(70, 25, 80, 25, scratch);

  if (wherex() == 1) moved = 1;

  gotoxy(x, y);

  return !moved;
}   //main
