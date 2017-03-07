/*---------------------------------------------------------19910726 RWB-----*/
/*  runser.c  --  send messages over the network to run command             */
/*--------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dos.h>
#include <conio.h>
#include "typdef.h"
#include "msglib.h"


/*--------------------------------------------------------------------------*/
/* wait_for_pipe -- wait till pipe established or timeout                   */
/*--------------------------------------------------------------------------*/
byte wait_for_pipe( byte station, word timeout )
{
  byte got_pipe=0;
  int  err=0;
  word timecnt=0;
  cprintf(" Attempting to connect to station %i \n\r",station);
  while ((!got_pipe) && (timecnt++ < timeout))
  {
    err=open_message_pipe(station);
    if (err)
    {
       switch (err)
       {
         case 0xFF : cprintf(" FF -- No station %i on server.\r");
                     break;
         case 0xFE : cprintf(" FE -- Station not responding.\r");
                     break;
         default   : cprintf(" ERROR %02x on get pipe command.\r",err);
                     break;
       }
       delay(200);
    }
    else
    {
      clreol();
      cprintf("Pipe open and ready. ");
      got_pipe=1;
    }
  }
  cprintf("\n\r");
  return(got_pipe);
}


main(int argc, char *argv[])
{
  word station=0;
  word got_pipe=0;
  word cmd=0;
  char command[200];

//  clrscr();
  station=atoi(argv[1]);
  got_pipe=wait_for_pipe(station,50);
  if (got_pipe)
  {
    command[0]=0;
    for (cmd=2; cmd<argc; cmd++)
    {
      if (command[0]) strcat(command," ");
      strcat(command,argv[cmd]);
    }
    cprintf(" Sending command %s\n\r",command);
    send_message(station,command);
    close_message_pipe(station);
  }
  else
  {
    cprintf(" Connection could not be made. Aborting. \n\r");
  }
  return(got_pipe);
}

