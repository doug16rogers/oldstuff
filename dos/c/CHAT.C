#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <dos.h>

#include "wutil.h"
#include "scrutil.h"

#undef BYTE
#undef WORD

#include "typdef.h"
#include "msgapi.h"
#include "msglib.h"




int  main(int argc,char* argv[])
{
  word remsta;
  word locsta;
  word sta;
  WIND* remw;
  WIND* locw;
  union REGS r;
  char locnam[80];
  char remnam[80];
  int cols,rows;
  char going=1;
  int c;
  char msg[200];

  delay(0);

  if (argc!=2) {
    printf("specify station number\n");
    return 1;
  }
  if (!sscanf(argv[1],"%u",&remsta)) {
    printf("bad station number\n");
    return 1;
  }
  r.h.ah=0xDC;
  int86(0x21,&r,&r);
  locsta=r.h.al;
  if (!locsta) {
    printf("no net connection\n");
    return 2;
  }

  printf("attempting/awaiting contact with %u...\n",remsta);
  while (!kbhit()) if (!open_message_pipe(remsta)) break; else delay(200);
  if (kbhit()) { if (!getch()) getch(); return 3; }

  sprintf(locnam," Local - %u ",locsta);
  sprintf(remnam," Remote - %u ",remsta);
  rows=getsrows();
  cols=getscols();
  locw=wopen(1,locnam, 1,1, (rows-4)/2,cols-2, ATTR(WHITE,BLUE));
  remw=wopen(1,remnam, rows/2+1,1, (rows-5)/2,cols-2, ATTR(BLACK,LIGHTGRAY));

  while (going) {
    if (kbhit()) {
      c=wgetc(locw);
      if (c>0x100)
        going=(c!=A_X)&&(c!=_F10);
      else {
        if (c==CR) c=LF;
        wputc(locw,c);
        msg[0]=c;
        msg[1]=0;
        send_message(remsta,msg);
      }
    }
    while ((sta=recv_message(msg))!=0) {
      if (sta==remsta) {
        wfront(remw);
        wputc(remw,msg[0]);
      }
      else {
        wprintf(remw,"\n*** MESSAGE FROM %u: \"%s\" ***\n",sta,msg);
      }
    }
  }   //while
  wpurge();
  return 0;
}   //main



