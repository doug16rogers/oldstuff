/*--------------------------------------------------------19910729 RWB-----*/
/* msglib.c  -- functions to talk to a single workstation per message      */
/*-------------------------------------------------------------------------*/
#include "typdef.h"
#include "msgapi.h"
#include "msglib.h"


/*-------------------------------------------------------------------------*/
/* open_message_pipe -- try to get pipe and return error                   */
/*-------------------------------------------------------------------------*/
byte open_message_pipe(int station)
{
  word connection_list[100];
  char result_list[100];
  connection_list[0]=station;
  OpenMessagePipe(connection_list,result_list,1);
  return(result_list[0]);                                /* 0 = success */
}

/*-------------------------------------------------------------------------*/
/* close_message_pipe -- close message pipe and return error               */
/*-------------------------------------------------------------------------*/
byte close_message_pipe(int station)
{
  word connection_list[100];
  char result_list[100];
  connection_list[0]=station;
  CloseMessagePipe(connection_list,result_list,1);
  return(result_list[0]);                                /* 0 = success */
}


/*--------------------------------------------------------------------------*/
/* send_message -- send a message to a single station                       */
/*--------------------------------------------------------------------------*/
byte send_message( byte station, char message[] )
{
  word connection_list[100];
  byte result_list[100];
  connection_list[0]=station;
  SendPersonalMessage(message,connection_list,result_list,1);
  return(result_list[0]);
}

/*--------------------------------------------------------------------------*/
/* broadcast_message -- sends a broadcast message to a single station                       */
/*--------------------------------------------------------------------------*/
byte broadcast_message( byte station, char message[] )
{
  word connection_list[100];
  byte result_list[100];
  connection_list[0]=station;
  SendBroadcastMessage(message,connection_list,result_list,1);
  return(result_list[0]);
}

/*--------------------------------------------------------------------------*/
/* recv_message -- get a message from the shell message queue               */
/*--------------------------------------------------------------------------*/
byte recv_message( char *message )
{
  word station=0;
  GetPersonalMessage( message, &station );
  return(station);
}
