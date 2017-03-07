/*----------------------------------------------------------10010729 RWB--*/
/* msgapi.c -- novell message passing api function calls                  */
/*------------------------------------------------------------------------*/
/* These functions call interrupt 0x21 with novell defined AH values      */
/*                                                                        */
/* SendPersonalMessage  -- send a personal message                        */
/* GetPersonalMessage   -- recv a personal message                        */
/* OpenMessagePipe      -- open a message pipe                            */
/* CloseMessagePipe     -- close a message pipe                           */
/* CheckPipeStatus      -- check the status of a message pipe             */
/*                                                                        */
/*------------------------------------------------------------------------*/
#include <string.h>
#include <dos.h>
#include "typdef.h"
#include "msgapi.h"





byte ShellRequest(BYTE FunctionNumber,
                  BYTE *request,
                  BYTE *reply)
{
  union  REGS  dregs;
  struct SREGS sregs;
  dregs.h.ah = FunctionNumber;        /* Novell Shell Message Function */
  dregs.h.al = 0x01;
  dregs.x.bx = 0x00;
  dregs.x.dx = 0x00;
  sregs.ds   = FP_SEG(request);
  dregs.x.si = FP_OFF(request);
  sregs.es   = FP_SEG(reply);
  dregs.x.di = FP_OFF(reply);
  int86x(0x21,&dregs,&dregs,&sregs);
  return(dregs.h.al);               /* return error status */
}




/*___________________________________________________________________________
|
|     Output:  0 --  SUCCESSFUL
|__________________________________________________________________________*/

int SendBroadcastMessage(message, connectionList, resultList, connectionCount)

char *message;         /* Null terminated string containing the message to
                          send (maximum 127 characters INCLUDING the NULL. */
WORD *connectionList;  /* Contains a list of logical connection number to
                          send the message to.                             */
WORD  connectionCount; /* Number of logical connections in connectionList. */
BYTE *resultList;      /* Receives a result code for each corresponding
                          target connection in the connectionList.         */
{
int     ccode, offset;
char    len, sendBuf[231], replyBuf[103];

    len = (char)strlen(message);
    if (len > 126)
        len = 126;
    *((int *)sendBuf) = connectionCount + len + 3;
    sendBuf[2] = (char)0;        /*broadcast*/
    sendBuf[3] = (BYTE)connectionCount;
    for( offset =0;offset<connectionCount;offset++)
         sendBuf[4+offset] = ((BYTE)connectionList[offset]);
    sendBuf[connectionCount + 4] = len;
    memmove((sendBuf + connectionCount + 5), message, (int)len);
    *((int *)replyBuf) = connectionCount + 1;
    replyBuf[2] = (char)connectionCount;
    ccode = ShellRequest((BYTE)0xE1, (BYTE *)sendBuf, (BYTE *)replyBuf);
    if (ccode)
        return (ccode);
    memmove(resultList, replyBuf + 3, connectionCount);
    resultList[connectionCount] = 0;
    return (0);
}

/*___________________________________________________________________________
|
|     Output:  0 --  SUCCESSFUL
|__________________________________________________________________________*/

int SendPersonalMessage(message, connectionList, resultList, connectionCount)

char *message;         /* Null terminated string containing the message to
                          send (maximum 127 characters INCLUDING the NULL. */
WORD *connectionList;  /* Contains a list of logical connection number to
                          send the message to.                             */
WORD  connectionCount; /* Number of logical connections in connectionList. */
BYTE *resultList;      /* Receives a result code for each corresponding
                          target connection in the connectionList.         */
{
int     ccode, offset;
char    len, sendBuf[231], replyBuf[103];

    len = (char)strlen(message);
    if (len > 126)
        len = 126;
    *((int *)sendBuf) = connectionCount + len + 3;
    sendBuf[2] = (char)4;
    sendBuf[3] = (BYTE)connectionCount;
    for( offset =0;offset<connectionCount;offset++)
         sendBuf[4+offset] = ((BYTE)connectionList[offset]);
    sendBuf[connectionCount + 4] = len;
    memmove((sendBuf + connectionCount + 5), message, (int)len);
    *((int *)replyBuf) = connectionCount + 1;
    replyBuf[2] = (char)connectionCount;
    ccode = ShellRequest((BYTE)0xE1, (BYTE *)sendBuf, (BYTE *)replyBuf);
    if (ccode)
        return (ccode);
    memmove(resultList, replyBuf + 3, connectionCount);
    resultList[connectionCount] = 0;
    return (0);
}

 /***************************************************************************/

int GetPersonalMessage(messageBuffer, sourceConnection)

char *messageBuffer;    /* Receives oldest message from pipe message queue */
WORD *sourceConnection; /* Receives the connection number of the client that
                           originated the message (1..100)                 */
{
    char sendBuf[3], replyBuf[132], len;
    int ccode;

    *((int *)sendBuf) = 1;
    sendBuf[2] = (char)5;
    *((int *)replyBuf) = 130;
    ccode = ShellRequest((BYTE)0xE1, (BYTE *)sendBuf, (BYTE *)replyBuf);
    if (ccode)
        return (ccode);
    *sourceConnection = (WORD)replyBuf[2];
    len = replyBuf[3];
    memmove(messageBuffer, replyBuf + 4, len);
    messageBuffer[len] = 0;
    return (0);
}


/*------------------------------PIPE FUNCTIONS----------------------------*/

/*__________________________________________________________________________
|                                                                          |
|  connectionList   --     Contains a list of logical connection numbers   |
|                          to connect a pipe to.                           |
|  resultList       --     Receives a result code for each corresponding   |
|                          target connection in the connectionList.        |
|  connectionCount  --     Number of logical connections in connectionList |
|                          (maximum of 100 connections).                   |
|                                                                          |
|  Output:  0 --  SUCCESSFUL                                              |
|_________________________________________________________________________*/

int OpenMessagePipe(connectionList, resultList, connectionCount)
WORD *connectionList, connectionCount;
BYTE *resultList;
{
    char sendBuf[104], replyBuf[103];
    int ccode;
    int offset;

    *((int *)sendBuf) = connectionCount + 2;
    sendBuf[2] = (char)6;
    sendBuf[3] = (BYTE)connectionCount;
    for (offset = -1; ++offset < connectionCount;
         sendBuf[4+offset] = (BYTE)(connectionList[offset]) );
    *((int *)replyBuf) = connectionCount + 1;
    ccode = ShellRequest((BYTE)0xE1, (BYTE *)sendBuf, (BYTE *)replyBuf);
    if (ccode)
        return (ccode);
    memmove(resultList, replyBuf + 3, (int)connectionCount);
    resultList[connectionCount] = 0;
    return (0);
}






int CloseMessagePipe(connectionList, resultList, connectionCount)
WORD *connectionList, connectionCount;
BYTE *resultList;
{
    char sendBuf[104], replyBuf[103];
    int ccode, offset;

    *((int *)sendBuf) = connectionCount + 2;
    sendBuf[2] = (char)7;
    sendBuf[3] = (char)connectionCount;
    for (offset = -1; ++offset < connectionCount;
         sendBuf[4+offset] = (BYTE)(connectionList[offset]) );
    *((int *)replyBuf) = connectionCount + 1;
    ccode = ShellRequest((BYTE)0xE1, (BYTE *)sendBuf, (BYTE *)replyBuf);
    if (ccode)
        return (ccode);
    memmove(resultList, replyBuf + 3, (int)connectionCount);
    resultList[connectionCount] = 0;
    return (0);
}








int CheckPipeStatus(connectionList, resultList, connectionCount)
WORD *connectionList, connectionCount;
BYTE *resultList;
{
    char sendBuf[104], replyBuf[103];
    int ccode, offset;

    *((int *)sendBuf) = connectionCount + 2;
    sendBuf[2] = (char)8;
    sendBuf[3] = (char)connectionCount;
    for (offset = -1; ++offset < connectionCount;
         sendBuf[4+offset] = (BYTE)(connectionList[offset]) );
    *((int *)replyBuf) = connectionCount + 1;
    ccode = ShellRequest((BYTE)0xE1, (BYTE *)sendBuf, (BYTE *)replyBuf);
    if (ccode)
        return (ccode);
    memmove(resultList, replyBuf + 3, (int)connectionCount);
    resultList[connectionCount] = 0;
    return (0);
}
