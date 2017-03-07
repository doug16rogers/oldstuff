/*---------------------------------------------------------19910726 RWB-----*/
/*  jobser.c  --  recv messages passed over the network                     */
/*--------------------------------------------------------------------------*/
#include <stdio.h>
#include <dos.h>
#include <conio.h>
#include <process.h>
#include <stdlib.h>
#include <string.h>
#include "typdef.h"
#include "msglib.h"
#include "msgapi.h"



/*---------------------------------------------------------------------*/
/* run_command -- run a dos command via command.com                    */
/*---------------------------------------------------------------------*/
int run_command( char command[] )
{
  char args[25][30];                     /* allocate space for arguments  */
  char *argp[25];
  int  err=0;
  word laststart=0;
  word parmno=2;
  word chn=0;
  word endstr=0;
  word pn=0;

  for (pn=0; pn<25; pn++) argp[pn]=args[pn];
  strcpy(args[0],"command.com");
  strcpy(args[1],"/c");
  while (!endstr)
  {
    if (command[chn]==0) endstr=1;
    if ((command[chn]==' ') || (command[chn]==',') || (endstr))
    {
      strncpy(args[parmno],command+laststart,chn-laststart);
      args[parmno][chn-laststart]=0;
//      printf(" Parm %2i: [%s] \n",parmno,args[parmno]);
      laststart=chn+1;
      parmno++;
    }
    chn++;
  }
  argp[parmno]=NULL;
  err=spawnvp(P_WAIT,args[0],argp);
  return(err);
}


/*--------------------------------------------------------------------------*/
/* try_pipe_all -- try to message pipe to all stations to advertise         */
/*--------------------------------------------------------------------------*/
void try_pipe_all(void)
{
  int sn=0;
  word connection_list[150];
  byte result_list[150];
  for (sn=0; sn<105; sn++) { connection_list[sn]=sn+1; }
  OpenMessagePipe(connection_list,result_list,90);
}


//--------------------------------------------------------------------------
// sit around and wait for messages to come
//--------------------------------------------------------------------------
main(void)
{
#if 0
  unsigned int  ss[120]={ 0x3132, 0, 0, 0 };
  char          mm[300]="tester";
  unsigned char rr[300];
  printf("Return code: %u\n",SendPersonalMessage(mm,ss,rr,1));
  return 0;
#else
  char mess[400];
  int  err=0;
  word station=0;
  char command[200];
  union REGS r;
  word our_station=0;
  byte our_station_byte=0;

  r.h.ah=0xDC;
  int86(0x21,&r,&r);
  our_station_byte=r.h.al;
  our_station=r.x.cx;

  printf("Station=%02X, (word=%04X)\n",our_station_byte,our_station);
  if (!our_station_byte) {
    printf("not connected to net -- no station ID.\n");
    return 1;
  }

  delay(0);
  printf("Awaiting commands \n");
  while (!kbhit())
  {
    try_pipe_all();
    station=recv_message(command);
    if (station)
    {
      printf(" From station [%i] Command: \"%s\" \n",station,command);
      err=run_command(command);
      printf(" Command Error Level= %i \n",err);
      delay(500);
      if (station!=our_station_byte) {
        sprintf(mess,"JOBSER: \"%s\" complete (%u)",command,err);
        broadcast_message(station,mess);
      }
    }
  }
  return(0);
#endif
}   //main


