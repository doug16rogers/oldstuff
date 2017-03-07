#include <stdio.h>

#include "serutil.h"





#define BUFFSIZE 1024

PORT *p;                           /*handle, pointer to serial port*/

void main(void)                    /*loopback test program*/
{
  int c;                           /*keyboard character*/

  p=seropen("c%d b2400 pNd8s1 r%d t%d",COM1,BUFFSIZE,BUFFSIZE);
  if (p==NULL) {                   /*check that port initialized ok*/
    printf("Error on open: %s\n",sererrmsg(sererr));
    return;
  }   /*if error*/

  printf("Connect lines 2 and 3 of COM1 for a loopback test.\n");
  printf("Press <ESC> to quit.\n");
  while (1) {
    while (kbhit()) {              /*while keyboard input..*/
      c=getch();                   /*..get input*/
      if (c==0x1B) {               /*..if <ESC>..*/
        serclose(p);               /*....close port*/
        return;                    /*....quit*/
      }   /*if <ESC>*/
      serputc(p,c);                /*..else put char out serial port*/
    }   /*while kb input*/
    while (!bufempty(p->rx))       /*if char received*/
      putchar(sergetc(p));         /*..echo it to screen*/
  }   /*while forever*/
}   /*main*/


