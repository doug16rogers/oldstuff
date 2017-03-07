/*-----------------------------------------------------------19910729 RWB---*/
/* msgapi.h  -- Novell message API header file prototypes                   */
/*--------------------------------------------------------------------------*/


int SendBroadcastMessage(

char *message,         /* Null terminated string containing the message to
                          send (maximum 127 characters INCLUDING the NULL. */
WORD *connectionList,  /* Contains a list of logical connection number to
                          send the message to.                             */
BYTE *resultList,      /* Receives a result code for each corresponding
                          target connection in the connectionList.         */
WORD  connectionCount  /* Number of logical connections in connectionList. */
);

/*--------------------------------------------------------------------------*/
int SendPersonalMessage(char *message,
                        WORD *connectionList,
                        BYTE *resultList,
                        WORD connectionCount);

/* char *message;          Null terminated string containing the message to */
/*                         send (maximum 127 characters INCLUDING the NULL. */
/* WORD *connectionList;   Contains a list of logical connection number to  */
/*                         send the message to.                             */
/* WORD  connectionCount;  Number of logical connections in connectionList. */
/* BYTE *resultList;       Receives a result code for each corresponding    */
/*                         target connection in the connectionList.         */
/*--------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------*/
int GetPersonalMessage(char *messageBuffer,
                       WORD *sourceConnection);

/* char *messageBuffer;    Receives oldest message from pipe message queue  */
/* WORD *sourceConnection; Receives the connection number of the client     */
/*                         that originated the message (1..100)             */
/*--------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------*/
int OpenMessagePipe (WORD *connectionList,
                     BYTE *resultList,
                     WORD connectionCount);

int CloseMessagePipe (WORD *connectionList,
                      BYTE *resultList,
                      WORD connectionCount);

int CheckPipeStatus  (WORD *connectionList,
                      BYTE *resultList,
                      WORD connectionCount);
/*--------------------------------------------------------------------------*/
/*                                                                          */
/*  connectionList   --     Contains a list of logical connection numbers   */
/*                          to connect a pipe to.                           */
/*  resultList       --     Receives a result code for each corresponding   */
/*                          target connection in the connectionList.        */
/*  connectionCount  --     Number of logical connections in connectionList */
/*                          (maximum of 100 connections).                   */
/*                                                                          */
/*  Output:  0 --  SUCCESSFUL                                               */
/*--------------------------------------------------------------------------*/



