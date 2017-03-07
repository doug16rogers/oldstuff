#include <stdio.h>
#include <conio.h>


char s[0x100];


void main(int argc,char* argv[])
{
  int i;
  if (argc<2) {
    printf("scrmsg <msg>\n");
    return;
  }
  strcpy(s,argv[i=1]);
  for (i=2;i<argc;i++) { strcat(s," "); strcat(s,argv[i]); }
  clrscr();
  gotoxy(40-strlen(s)/2,12);
  cprintf("%s",s);
  while (getch()!=0x1B);
  clrscr();
}   //main
