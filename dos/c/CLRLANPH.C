/********************************************************************
*
*  CSC TITLE:       CLRLAN Packet Handler
*  CSC FILE NAME:   CLRLANPH.C
*  LANGUAGE:        Intel 960 C
*  DESCRIPTION:     This contains all of the routines necessary
*                   to arbitrate CLRLAN usage.  This oversees the possibility
*                   of multiple tasks of varying priority vying for
*                   CLRLAN service.
* *k"%v"
*  VERSION NO:      ""
*
*
*********************************************************************/

#include "iopsys.h"          /* system/kernel declarations */
#include "1553bios.h"        /* 1553 BCRT BIOS declarations */
#include "dtatyp53.h"        /* data types for 1553 */
#include "dtatypcl.h"        /* data types for CLRLAN */    
#include "1553bint.h"        /* 1553 interface declarations */
#include "clrlanph.h"        /* CLRLAN packet handler declarations */

#include "dtypes.h"          /* CLRLAN BIOS data types */
#include "lan_cfg.h"         /* CLRLAN link-level defines */
#include "i82592.h"         
#include "clrlan.h"          /* CLRLAN BIOS routine declarations and types */

      /**************************************************************
      *    The macro MessageQueueStatus gives a boolean value that is 
      *  true if the packet type is a System Status Packet.
      *    It MUST be invoked with a variable of type 
      *  (CLRLAN_PAKCET *).                                    
      **************************************************************/
#define    SystemStatusPacket(a)              \
             ((a) -> udp_destination_address == CLRLAN_SYSTEM_STATUS_PACKET)

      /**************************************************************
      *    The macro MessageQueueStatus gives a boolean value that is 
      *  true if the command is Message Queue Status.
      *    It MUST be invoked with a variable of type 
      *  (CLRLAN_PAKCET *).                                    
      **************************************************************/
#define    MessageQueueStatus(a)              \
             (((MESSAGE_QUEUE_HEADER *)((a) -> CLRLAN_data)) -> command == \
                  CLRLAN_MESSAGE_QUEUE_STATUS)

                /* This is the new packet type which tells the GPP to return
                  one of its task control blocks.  It is sent in transparent
                  mode.  This definition should be moved to iopsys.h. TBD */

#define    CLRLAN_GPP_MEMORY_PACKET             6

/*****************************************************************************
*
*  CSU NAME:       CLRLANTask
*  DESCRIPTION:    This task selects the highest priority event on the
*                  queue an sends the CLRLAN packet which it points to.
*                  An event is generated for the calling task upon either
*                  the receipt of a packet of the specified type or upon
*                  completion of packet transmission.  If the calling event
*                  is the result of a message queue status packet, the Link
*                  Message task is called.
*
*  CSU's CALLed                  CSC Name
*  ------------                  --------
*  EventQueueClear               Kernel
*  EventQueueWrite               Kernel
*  TaskBlock                     Kernel
*  CLRLANCommandResponse         CLRLAN Packet Handler
*  WatchdogTimerSet              Watchdog Timer
*  MessageQueueRead              Kernel
*  PollSystemStatus              CLRLAN Packet Handler
*  ParseMessageQueuePacket       CLRLAN Packet Handler
*  MemoryFree                    Memory Manager
*
*****************************************************************************/

/* 

BEGIN
  DO forever
    CALL TaskBlock on CLRLAN packet received OR CLRLAN service requested 
            OR watchdog OR time for a periodic GPP status poll 

    IF watchdog event THEN
      CALL EventQueueClear to clear the watchdog event
      CALL WatchdogTimerSet to reset watchdog
    ENDIF

    IF CLRLAN packet received THEN
      DO
        CALL MessageQueueRead to obtain the CLRLAN packet 
        IF the message is a Message Queue Status packet THEN
          CALL ParseMessageQueuePacket to determine if the message received 
                   event should be set in the link message task
        ELSE
          CALL MemoryFree to release the unexpected packet
        ENDIF
      UNTIL a NULL event message is read
    ENDIF

    IF CLRLAN service required at any priority THEN
      determine the highest priority event
      CALL MessageQueueRead for the CLRLAN packet for the event with the
              highest priority
      CALL CLRLANCommandResponse to handle the CLRLAN interface processing
      IF CLRLAN service was determined to be faulty THEN
        CALL PollSystemStatus to check up on the CLRLAN communication
        clear the event to check system status
      ENDIF
    ENDIF

    IF the poll system status event is set THEN
      CALL EventQueueClear to clear the packet received event
      CALL PollSystemStatus to create the command and send it on the CLRLAN
    ENDIF

  ENDDO
END

*/

void CLRLANTask(void)
{
register   SYSTEM_EVENTS       wake_up_events;
register   CLRLAN_PACKET       *CLRLAN_packet;
register   CLRLAN_TASK_MESSAGE *CLRLAN_message_block;
register   PASS_FAIL           result;

  while( TRUE )
    {
          /* wait for one of the specified wake-up events */
    wake_up_events = TaskBlock( CLRLAN_task, 
                                CLRLAN_packet_received_event |
                                  CLRLAN_service_events_mask |
                                  CLRLAN_poll_system_status_event |
                                  watchdog_timer_event );

          /* check for watchdog service */
    if( wake_up_events & watchdog_timer_event ) 
      {
      EventQueueClear( CLRLAN_task, watchdog_timer_event );
      WatchdogTimerSet( CLRLAN_task );
      }

            /* read the first message for this event */

    if( wake_up_events & CLRLAN_packet_received_event )
      {
      CLRLAN_packet = (CLRLAN_PACKET *) MessageQueueRead
                         ( CLRLAN_task, CLRLAN_packet_received_event );
      while( CLRLAN_packet != NULL )
        {     /* set an event in link message task if necessary */
        if( SystemStatusPacket(CLRLAN_packet) && 
            MessageQueueStatus(CLRLAN_packet) )
           {     /* the following function will also free the memory */
           ParseMessageQueuePacket( CLRLAN_packet );
           }
        else
           {     /* do nothing: unexpected packet */
           MemoryFree( CLRLAN_packet );
           }
        CLRLAN_packet = (CLRLAN_PACKET *) MessageQueueRead
                           ( CLRLAN_task, CLRLAN_packet_received_event );
        }
            /* if MessageQueueRead returns NULL, it will clear the event */
      }
    
          /* check for CLRLAN service requested */
    if( wake_up_events & CLRLAN_service_events_mask )
      {
            /* get the highest priority message first (lowest sig. bit) */
      if( wake_up_events & CLRLAN_requested_BIT_event )
        {
        CLRLAN_message_block = (CLRLAN_TASK_MESSAGE *) MessageQueueRead
                         ( CLRLAN_task, CLRLAN_requested_BIT_event );
              /* if there are no more messages for this event */
        if( CLRLAN_message_block == NULL )
          {
          EventQueueClear( CLRLAN_task, CLRLAN_requested_BIT_event );
          }
        }
      else
        {     /* check the next highest priority event */
        if( wake_up_events & CLRLAN_link_message_event )
          {
          CLRLAN_message_block = (CLRLAN_TASK_MESSAGE *) MessageQueueRead
                           ( CLRLAN_task, CLRLAN_link_message_event );
              /* if there are no more messages for this event */
          if( CLRLAN_message_block == NULL )
            {
            EventQueueClear( CLRLAN_task, CLRLAN_link_message_event );
            }
          }
        else
          {     /* check for a requested parameter event */
          if( wake_up_events & CLRLAN_requested_parameter_event )
            {
            CLRLAN_message_block = (CLRLAN_TASK_MESSAGE *) MessageQueueRead
                             ( CLRLAN_task, CLRLAN_requested_parameter_event );
            if( CLRLAN_message_block == NULL )
              {
              EventQueueClear( CLRLAN_task, CLRLAN_requested_parameter_event );
              }
            }
          else    /* must be transparent mode */
            {
            CLRLAN_message_block = (CLRLAN_TASK_MESSAGE *) MessageQueueRead
                             ( CLRLAN_task, CLRLAN_transparent_mode_event );
            if( CLRLAN_message_block == NULL )
              {
              EventQueueClear( CLRLAN_task, CLRLAN_transparent_mode_event );
              }
            }
          }
        }

            /* do the CLRLAN interface processing for the selected event */
      if( CLRLAN_message_block != NULL )
        {
        result = CLRLANCommandResponse( CLRLAN_message_block );
        if( result != pass )
          {     /* CLRLAN failed, try to pick up the pieces */
          PollSystemStatus();
          wake_up_events &= ~CLRLAN_poll_system_status_event;
          }
        }
      }  

          /* check for another task requesting system status */
    if( wake_up_events & CLRLAN_poll_system_status_event )
      {
      EventQueueClear( CLRLAN_task, CLRLAN_poll_system_status_event );
      PollSystemStatus();
      }
    }      /* while (forever) */
}


/*****************************************************************************
*
*  CSU NAME:       CLRLANCommandResponse
*  DESCRIPTION:    This routine performs the processing inherent in sending
*                  a CLRLAN command and waiting for a response.
*
*  CSU's CALLed                  CSC Name
*  ------------                  --------
*  EventQueueClear               Kernel
*  EventQueueWrite               Kernel
*  TaskBlock                     Kernel
*  MessageQueueRead              Kernel
*  EventTimerStart               Event Timer Handler
*  EventTimerStop                Event Timer Handler
*  CLRLANTransmit                CLRLAN BIOS
*  ConvertPacketTypeToFlag       CLRLAN Packet Handler
*  WatchdogTimerSet              Watchdog Timer
*  ParseMessageQueuePacket       CLRLAN Packet Handler
*  MemoryFree                    Memory Manager
*
*****************************************************************************/

/* 

BEGIN
  save the calling task since the information may be released back into
            memory
  complete the CLRLAN packet header
  CALL EventTimerStart to wait before transmitting to avoid LAN
            controller problems
  CALL TaskBlock to wait for the time to elapse
  CALL EventTimerStop
  CALL EventQueueClear to clear the timer event

  DO until the packet was transmitted successfully OR the retry count expired
    CALL CLRLANTransmit to send the command on the CLRLAN
    IF there was a failure THEN
      increment the retry attempt count
      CALL EventTimerStart to wait before transmitting to avoid LAN
                controller problems
      CALL TaskBlock to wait for the time to elapse
      CALL EventTimerStop
      CALL EventQueueClear to clear the timer event
    ENDIF
  ENDDO    
  
  IF CLRLANTransmit was successful THEN
    IF calling message indicates that no response is expected THEN    
      CALL EventQueueWrite to set event in calling that tx is complete    
    ELSE
      CALL EventQueueClear to clear the calling task timeout event
      DO
        CALL TaskBlock on CLRLAN response received or timeout or
                  watchdog events
        CALL EventQueueClear to clear the wake-up event(s)

        IF watchdog timer event THEN
          clear flag from wake-up events
          CALL WatchdogTimerSet to reset the watchdog
        ENDIF

        IF calling task timeout event THEN
          clear the event locally in case a different task timed out
          IF the CLRLAN task is the one requesting THEN
            clear the response valid flag
          ELSE
            WHILE the message queue is not empty DO
              CALL MessageQueueRead to get the timeout event's task
              IF it is the same as the current task in service THEN
                clear the response valid flag
                set the event locally to signify a timeout has been ordered
              ENDIF
            ENDDO
          ENDIF
        ENDIF

        IF CLRLAN packet received event THEN
          CALL MessageQueueRead to get the packet
          parse out the UDP destination address
          CALL ConvertPacketTypeToFlag to check received packet type
          compare this to the packet type(s) requested by the calling task
          IF packet type does not match any of the expected responses THEN
            clear flag from wake-up events
            IF the packet is an unsolicited system status packet THEN
              CALL ParseMessageQueuePacket to determine the appropriate action
            ELSE
              CALL MemoryFree to release the unexpected buffer to memory
            ENDIF
          ENDIF
        ENDIF
      WHILE there are no outstanding wake-up events
  
      IF a valid response was received THEN
        CALL EventQueueWrite to set the event with the CLRLAN response 
                  received for the calling task    
      ENDIF    
    ENDIF    
    return pass (calling task will initiate error recovery if necessary)
  ELSE
    CALL EventQueueWrite to set an event in the calling task indicating
              we were unable to transmit 
    return fail
  ENDIF
END

*/

PASS_FAIL CLRLANCommandResponse(                /* whether or not a response
                                                 was received */
            CLRLAN_TASK_MESSAGE *message_info ) /* the event message */
{

#define    CLRLAN_TX_ATTEMPTS    2
#define    CLRLAN_TX_RETRY_DELAY 20           /* milliseconds */
#define    CLRLAN_INTERMESSAGE_GAP 15         /* milliseconds */
                  /* this must be longer the longest timeout */
register   CLRLANTransmit_RETURN result;               /* of PASS_FAILroutine */
register   uint                  attempts;             /* CLRLAN transmit
                                                        attempt counter */
register   uint                  wake_up_events;       /* from TaskBlock */
register   PASS_FAIL             return_value = pass;  /* value returned */
register   CLRLAN_PACKET         *CLRLAN_response;     /* pointer to response
                                                        packet from CLRLAN */
register   uint                  response_packet_type; /* temp variable */
register   uint                  response_packet_flag; /* flag version of the
                                                        CLRLAN response type */ 
register   uint                  correct_type;         /* boolean result of
                                                         the response packet-
                                                         type check */
register   CLRLAN_PACKET         *CLRLAN_command;      /* temp variable */
register   uint                  timeout_event_task;   /* the task which said
                                                         it timed out waiting
                                                         for CLRLAN service */
register   uint                  task_getting_service;

        /* Save this now.  If the calling task times out, it will release
          this buffer back to memory.  */
  task_getting_service = message_info -> task;  

  CLRLAN_command = message_info -> packet;
        /* just in case */
  if( CLRLAN_command -> udp_length < 5 )
    CLRLAN_command -> udp_length = 5;

  CLRLAN_command -> udp_source_address = IOP_ADDRESS;
  CLRLAN_command -> udp_protocol = UDP_PROTOCOL;
  CLRLAN_command -> udp_frame_check = FRAME_CHECK_SEQUENCE;
  CLRLAN_command -> destination_address = GPP_LAN_ADDRESS;             
        /* byte count is udp length * 2 + 12 header words */
  CLRLAN_command -> byte_count =
     (CLRLAN_command -> udp_length) * sizeof(WORD16) + 2;
        /* clear out the message chaining word (the word after the last data */
  ((word *)CLRLAN_command -> CLRLAN_data)
      [CLRLAN_command -> udp_length - 5] = 0;

        /* Make sure that there is at least 10 mSec. between messages on
          the link. */
  EventTimerStart( CLRLAN_task, event_timer_event, CLRLAN_INTERMESSAGE_GAP ); 
  wake_up_events = TaskBlock( CLRLAN_task, event_timer_event );
  EventTimerStop( CLRLAN_task, event_timer_event );
  EventQueueClear( CLRLAN_task, event_timer_event);

  attempts = 0;
  do{     /* send the packet to the BIOS */
    result = CLRLANTransmit( (BUFFER *)message_info -> packet, CLRLAN_task );
    if (result != packet_transmitted)
      {
      attempts++;
            /* wait a while, then try again */
      EventTimerStart( CLRLAN_task, event_timer_event, CLRLAN_TX_RETRY_DELAY ); 
      wake_up_events = TaskBlock( CLRLAN_task, event_timer_event );
      EventTimerStop( CLRLAN_task, event_timer_event );
      EventQueueClear( CLRLAN_task, event_timer_event);
      }
    } while( (result != packet_transmitted) && (attempts < CLRLAN_TX_ATTEMPTS) );

  if( result == packet_transmitted )
    {
    if( message_info -> response_types != NULL )
      {     /* wait for resopnse */
      correct_type = 0;      /* initialize packet type check to false */
      EventQueueClear( CLRLAN_task, calling_task_timeout_event );                                      
      do {      /* wait for good response or calling task times out */
        wake_up_events = TaskBlock( CLRLAN_task,
                                      CLRLAN_packet_received_event |
                                      watchdog_timer_event |
                                      calling_task_timeout_event );
        EventQueueClear( CLRLAN_task, wake_up_events );

        if (wake_up_events & watchdog_timer_event)
          {
                /* clear this event; leave any others active */
          wake_up_events &= ~watchdog_timer_event;
          WatchdogTimerSet( CLRLAN_task );
          }

              /* If calling task has timed out, do NOT send a packet to
                task.  If both these events are set, it means that a
                packet was received just as the calling task gave up */
        if (wake_up_events & calling_task_timeout_event)
          {
                /* First, if this was a poll system status request (the
                  only packet generated by the CLRLAN task), then give up. */
          if( task_getting_service == CLRLAN_task )
            {
                  /* The calling task did in fact signal timeout.  Do what
                    is necessary give up on receiving a CLRLAN packet */
            correct_type = 0;
            }
                /* Otherwise, some other (non-zero) task said give up  */
          else
            {
                  /* Initialize this just to get the loop started.  */
            timeout_event_task = !NULL;
                  /* Assume that a different task timed out unless it is shown
                    that the task in service did.  */
            wake_up_events &= ~calling_task_timeout_event;
                  /* Read all messages pending for this event.  */
            while( timeout_event_task != NULL )
              {
              timeout_event_task = (uint)
                  MessageQueueRead( CLRLAN_task, calling_task_timeout_event );
              if( timeout_event_task == task_getting_service )
                {
                      /* The calling task did in fact signal timeout.  Do what
                        is necessary give up on receiving a CLRLAN packet */
                correct_type = 0;
                wake_up_events |= calling_task_timeout_event;
                }
              }
            }
          }

        if (wake_up_events & CLRLAN_packet_received_event)
          {     /* check against expected response types */
          CLRLAN_response = (CLRLAN_PACKET *)
              MessageQueueRead( CLRLAN_task, CLRLAN_packet_received_event );
                /* get the packet type */
          response_packet_type = 
              (uint)CLRLAN_response -> udp_destination_address;
                /* convert it to flag form */
          response_packet_flag = 
              ConvertPacketTypeToFlag( response_packet_type );
                /* check against the type(s) of response packet which
                 the calling task is expecting */
          correct_type = (message_info -> response_types) & 
                         response_packet_flag;

                /* no match */
          if (correct_type == 0) 
            {     /* clear event flag, check for unsolicited system status */
            wake_up_events &= ~CLRLAN_packet_received_event;
                  /* check for message queue packet type (with macros) */
            if( SystemStatusPacket(CLRLAN_response) && 
                MessageQueueStatus(CLRLAN_response) )
              {     /* the following function will also free the memory */
              ParseMessageQueuePacket( CLRLAN_response );
              }
            else
              {     /* do nothing: unexpected packet */
              MemoryFree( CLRLAN_response );
              }
            }
          }
            
        }

      while (wake_up_events == NULL);

            /* set the proper message for the calling task */

      if( correct_type != 0 )
        {     /* successful response; send CLRLAN packet to calling task */
        EventQueueWrite( task_getting_service,
                         CLRLAN_packet_received_event, 
                         CLRLAN_response );
        }
      else
        {
              /* do nothing; calling task has already timed out */
        }                            
      }
    else
      {     /* if no response is expected, set event in the calling task */
      EventQueueWrite( task_getting_service, 
                       CLRLAN_transmit_complete_event,
                       NULL );
      }
    }
  else
    {     /* tell calling task of failure to transmit */
    EventQueueWrite( task_getting_service, 
                     CLRLAN_transmit_failed_event, 
                     NULL );
    return_value = fail;
    }
        /* release memory buffers after they have been used */
  FreeCLRLANTaskMessage( message_info );
  return( return_value );
}

/*****************************************************************************
*
*  CSU NAME:       ConvertPacketTypeToFlag
*  DESCRIPTION:    This converts the value in the CLRLAN packet type field
*                  to a flag.  This flag is used for the validation check
*                  against the types expected, which was sent as part of the
*                  event message.
*
*  CSU's CALLed                  CSC Name
*  ------------                  --------
*  None                          None
*
*****************************************************************************/

/* 

BEGIN
  find the value based on the look-up table
  IF the value is found
    return the value
  ELSE
    return 0 (this will cause the check at the level of the calling routine
              to fail)
  ENDIF
END

*/

uint ConvertPacketTypeToFlag(       /* the flag corresponding to the packet
                                     type passed to the function */
       uint response_packet_type )  /* CLRLAN-format destination address
                                     (packet type) value */
{
      /* the data structure that is for the look-up table */
typedef struct {
  uint                    CLRLAN_packet_type;     /* CLRLAN value */
  uint                    packet_type_flag;       /* packet type check flag */
} PACKET_CONVERSION;

      /* the actual look-up table to convert for CLRLAN packet type value to 
       a packet type flag used for the validation check */
const PACKET_CONVERSION packet_table[] = {        
  { CLRLAN_COMMAND_PACKET,         command_packet_flag          },
  { CLRLAN_STATUS_PACKET,          param_status_packet_flag     },
  { CLRLAN_SYSTEM_STATUS_PACKET,   system_status_packet_flag    },
  { CLRLAN_ECHO_PACKET,            echo_packet_flag             },
  { CLRLAN_SDLC_PORT_PACKET,       SDLC_port_packet_flag        },
  { CLRLAN_MESSAGE_REQUEST_PACKET, request_data_packet_flag     },
  { CLRLAN_DATA_MESSAGE_PACKET,    data_message_packet_flag     },
  { CLRLAN_MESSAGE_ACK_PACKET,     data_ack_packet_flag         },
  { CLRLAN_GPP_MEMORY_PACKET,      GPP_memory_packet_flag       }
};

register   uint              index;              /* loop counter */
register   uint              temp;               /* temp. for clarity */  
register   uint              table_size;         /* number of entries */
register   uint              return_value;       /* response */

        /* initialize response to 0 (value not found) */
  return_value = 0;
  table_size = sizeof(packet_table) / sizeof(PACKET_CONVERSION);
  for( index=0; index < table_size; index++)
    {
          /* get type from table */
    temp = packet_table[index].CLRLAN_packet_type;
          /* compare against the type from the received packet */
    if( temp == response_packet_type )
      {
            /* if a match is found, return the corresponding flag */
      return_value = packet_table[index].packet_type_flag;
      break;
      }
    }
  return( return_value );
}

/*****************************************************************************
*
      ELSE    
      ELSE    
*  CSU NAME:       PollSystemStatus
*  DESCRIPTION:    This routine creates a system status packet and 
*                  sends it to the GPP on the CLRLAN.
*
*  CSU's CALLed                  CSC Name
*  ------------                  --------
*  CLRLANCommandResponse         CLRLAN Packet Handler
*  UpdateIDMStatusBits           IDM BIT/Status
*  AllocateCLRLANTaskMessage     Local Utility
*  FreeCLRLANTaskMessage         Local Utility
*  MemoryFree                    Memory Manager
*  EventTimerStart               Event Timer Handler
*  EventTimerStop                Event Timer Handler
*  EventQueueRead                Kernel
*  EventQueueClear               Kernel
*  MessageQueueRead              Kernel
*
*****************************************************************************/

/* 

BEGIN
  CALL AllocateCLRLANTaskMessage to get a CLRLAN packet buffer
  create a System Status CLRLAN packet
  CALL EventTimerStart to set up a timeout
  CALL CLRLANCommandResponse to send the packet
  CALL EventTimerStop to remove the timer
  CALL EventQueueRead to get the current events for this task
  CALL EventQueueClear to clear all non-CLRLAN service related tasks
  IF a response is available THEN
    CALL MessageQueueRead to get the message
    IF packet is good THEN
      parse the response to set the local flags appropriately
      CALL MemoryFree to release the packet to available memory
    ELSE
      set all local flags to fail
    ENDIF
  ELSE
    set all local flags to fail
  ENDIF
  CALL UpdateIDMStatusBits to update the IDM status bits based on the response
  CALL FreeCLRLANTaskMessage to release the packet
END

*/

void PollSystemStatus(void)
{
register   CLRLAN_PACKET           *CLRLAN_command;  /* packet to be sent */
register   CLRLAN_PACKET           *CLRLAN_response; /* packet to be sent */
register   PASS_FAIL               result;           /* pass or fail */
register   SYSTEM_EVENTS           events_set;       /* current events */
static     IDM_STATUS              status_bits;      /* new IDM status bits */
IDM_STATUS                         status_mask;      /* bits updated */
register   CLRLAN_TASK_MESSAGE     *poll_status_packet;
register   REQUEST_STATUS_CMD      *response_data;   /* address of the data
                                                      portion of the packet */

  poll_status_packet = (CLRLAN_TASK_MESSAGE *)AllocateCLRLANTaskMessage();
        /* complete CLRLAN packet header */
  CLRLAN_command = (CLRLAN_PACKET *)poll_status_packet -> packet;
  CLRLAN_command -> udp_destination_address = CLRLAN_SYSTEM_STATUS_PACKET;
  CLRLAN_command -> udp_length = 5 + 1;     /* udp header + data */
  ((REQUEST_STATUS_CMD *)CLRLAN_command -> CLRLAN_data) -> command =
      CLRLAN_REQUEST_SYSTEM_STATUS;
        /* complete CLRLAN event message */
  poll_status_packet -> task = CLRLAN_task;
  poll_status_packet -> response_types = system_status_packet_flag;

        /* send the command, wait for the response */
        /* NOTE: this will cause the task to give up waiting for a response */
  EventTimerStart( CLRLAN_task,
                   calling_task_timeout_event,
                   1000 );     /* mSec timeout time */
        /* this will set an event in THIS task (CLRLAN task) */
  CLRLANCommandResponse( poll_status_packet );
  EventTimerStop( CLRLAN_task,
                  calling_task_timeout_event);

        /* get the event(s) set */
  events_set = EventQueueRead( CLRLAN_task );

        /* clear all non-service request events */
  EventQueueClear( CLRLAN_task, ~CLRLAN_service_events_mask );

        /* response will be in this task's own event queue */
  if( events_set & CLRLAN_packet_received_event )
    {
    CLRLAN_response = MessageQueueRead( CLRLAN_task, 
                                      CLRLAN_packet_received_event );
    if( CLRLAN_response != NULL )
      {     /* response is valid */
      response_data = (REQUEST_STATUS_CMD *)CLRLAN_response -> CLRLAN_data;
      status_bits.GPP_fail = 
          !( response_data -> GPP_active ); 
      status_bits.DSP1_fail = 
          !( (response_data -> DSP_1_active) &
             (response_data -> DSP_1_respond) ); 
      status_bits.DSP2_fail = 
          !( (response_data -> DSP_2_active) &
             (response_data -> DSP_2_respond) ); 

      MemoryFree( CLRLAN_response );
      }
    else
      {     /* CLRLAN_timeout -- not operational */
      status_bits.GPP_fail = TRUE;
      status_bits.DSP1_fail = TRUE;
      status_bits.DSP2_fail = TRUE;
      }
    }
  else
    {     /* invalid or no response */
    status_bits.GPP_fail = TRUE;
    status_bits.DSP1_fail = TRUE;
    status_bits.DSP2_fail = TRUE;
    }
        /* update IDM status subaddress buffer */
  UpdateIDMStatusBits( *(uint *)&status_bits, NON_IOP_STATUS_MASK );
}

/*****************************************************************************
*
*  CSU NAME:       ParseMessageQueuePacket
*  DESCRIPTION:    This routine reads each message corresponding to the 
*                  CLRLAN packet received event.  It is assumed that it has
*                  already been determined that this packet is indeed from
*                  an unsolicited message queue update packet.  This routine
*                  assures that only new messages are put on the link message
*                  task's event queue.
*
*  CSU's CALLed                  CSC Name
*  ------------                  --------
*  EventQueueWrite               Kernel
*  MemoryAllocate                Memory Manager
*  MemoryFree                    Memory Manager
*
*****************************************************************************/

/* 

BEGIN
  read the new message information out of the received packet
  FOR each of the four channels DO
    get the most recent message ID for this channel
    IF the message is new THEN
      save the message ID as the most recently received message
      CALL MemoryAllocateLarge to get a buffer for the event message
      complete the task message information
      CALL EventQueueWrite to set the message received event in the 
                link message task
    ENDIF
  ENDDO
  CALL MemoryFree to release the CLRLAN packet
END

*/

void ParseMessageQueuePacket( 
       CLRLAN_PACKET *CLRLAN_packet )            /* packet to be parsed */
{
register   uint              index;              /* loop counter */
register   uint              message_ID;         /* parsed from packet */
register   uint              message_count;      /* parsed from packet */
register   word              *data_offset;       /* pointer to a data word
                                                  of the CLRLAN packet corres-
                                                  ponding a channel */
LINK_TASK_MESSAGE            *event_message;     /* to be sent to link task */

      /* current message being processed by each channel (initialized with 
       invalid values so that the first valid one will be sent) */
static     uint              current_message[TOTAL_CHANNELS] 
                   = { 0xFF, 0xFF, 0xFF, 0xFF }; 

        /* get the address of the first data word (after the command word) */
  data_offset = &((word *)(CLRLAN_packet -> CLRLAN_data))[1];
  for( index=0; index < TOTAL_CHANNELS; index++ )
    {
          /* get current message available for channel [index] */
    message_ID = ((MESSAGE_QUEUE_DESC *)data_offset) -> latest_receive;
    message_count = ((MESSAGE_QUEUE_DESC *)data_offset) -> receive_count;
    if( (message_ID != current_message[index]) && (message_count != 0) )
      {     /* inform the link message task of a new message */
            /* save this as last received */
      current_message[index] = message_ID;
      event_message = (LINK_TASK_MESSAGE *)MemoryAllocateLarge();
      event_message -> channel = index;
      event_message -> message_ID = message_ID;
      EventQueueWrite( link_message_task, new_message_event, event_message );
      }
          /* check the next word for the next channel (this assumes that the
           queue statuses are stored in the packet in ascending channel
           order) */
    data_offset++;
    }
  MemoryFree( CLRLAN_packet );
}

/*****************************************************************************
*
*  CSU NAME:       CLRLANService
*  DESCRIPTION:    This is a general-purpose CLRLAN service routine.  It
*                  requires the calling task, the event to set in the CLRLAN
*                  task, the expected CLRLAN packet types, the number of
*                  retries allowed, the timeout time, and a pointer to a
*                  response data validation routine.  It will return the 
*                  response or a NULL if a valid response could not be 
*                  obtained.
*
*  CSU's CALLed                  CSC Name
*  ------------                  --------
*  TaskBlock                     Kernel
*  EventQueueWrite               Kernel
*  EventQueueClear               Kernel
*  MessageQueueRead              Kernel
*  EventTimerStart               Event Timer Handler
*  EventTimerStop                Event Timer Handler
*  MemoryFree                    Memory Manager
*  WatchdogTimerSet              Watchdog Timer
*
*****************************************************************************/

/* 

BEGIN
  IF no response is expected THEN
    indicate the unblocking event is transmit complete
  ELSE
    indicate that the unblocking event is packet received
  ENDIF
  complete the CLRLAN task event information

  DO until appropriate response event is received OR retries are exhausted
    CALL EventQueueWrite to set the service requested event in the CLRLAN
      task
    CALL EventTimerStart to time sending the calling task timeout event
    WHILE watchdog timer event ONLY DO
      CALL TaskBlock until CLRLAN reponse OR timeout OR watchdog events
      CALL EventQueueClear to clear the wake-up events
      IF watchdog timer event THEN
        CALL WatchdogTimerSet to reset the watchdog
      ENDIF
    ENDDO

    CALL EventTimerStop to free the timer
    IF a packet response was received THEN
      CALL MessageQueueRead to get the packet
      IF no packet is found THEN
        clear loop-test flag
      ELSE
        if the validation routine is no NULL THEN
        CALL the supplied validation routine
        IF the response fails THEN
          clear the loop_test flag
          CALL MemoryFree to release the packet
        ENDIF
      ENDIF
    ELSE
      IF the timer expired THEN
        CALL EventQueueWrite to set the calling task timeout event in the
                  CLRLAN task (tell it to give up)
      ENDIF
    ENDIF
  ENDDO

  IF valid response THEN
    return the CLRLAN response packet
  ELSE
    return NULL
  ENDIF
END

*/

CLRLAN_PACKET      *CLRLANService(      /* CLRLAN response packet (or NULL) */
  CLRLAN_TASK_MESSAGE  *CLRLAN_message, /* CLRLAN message to be put on the
                                         CLRLAN event queue */
  SYSTEM_EVENTS        calling_event,   /* event to set in the CLRLAN task */
  uint                 retries_allowed, /* retries permissable before failure
                                         (0 - n) */
  uint                 timeout,         /* milliseconds before failure */
  void                 *validate )      /* validation function address (or
                                         NULL if none) */
{
register   uint                wake_up_events;   /* reason for wake-up */
register   uint                retry_count = 0;  
register   SYSTEM_TASK         calling_task;     
register   CLRLAN_PACKET       *CLRLAN_response;
register   uint                result;
register   uint                unblock_event;    /* CLRLAN event to unblock */

  calling_task = CLRLAN_message -> task;
  if (CLRLAN_message -> response_types != 0)
    {     /* response packet received will unblock */
    unblock_event = CLRLAN_packet_received_event;
    }
  else
    {     /* transmit complete will unblock */
    unblock_event = CLRLAN_transmit_complete_event;
    }
  do {
          /* instruct the CLRLAN interface to service this request */
    EventQueueWrite( CLRLAN_task,
                     calling_event,
                     CLRLAN_message );
    EventTimerStart( calling_task,
                     event_timer_event,
                     timeout );
    do
      {
      wake_up_events = TaskBlock( calling_task,
                                  unblock_event |
                                    watchdog_timer_event |
                                    event_timer_event );
      EventQueueClear( calling_task, wake_up_events );
            /* if calling task's watchdog comes around, take care of it */
      if (wake_up_events & watchdog_timer_event)
        {
        WatchdogTimerSet( calling_task );
              /* no go on like nothing happened */
        wake_up_events &= ~watchdog_timer_event;
        }
      }
    while (wake_up_events == NULL);
    EventTimerStop( calling_task,
                    event_timer_event );

          /* if response expected and received, check for validity */                    
    if( wake_up_events & CLRLAN_packet_received_event )
      {
      CLRLAN_response = 
        (CLRLAN_PACKET *) MessageQueueRead( calling_task,
                                            CLRLAN_packet_received_event );
            /* check for timeout detected in CLRLAN task */
      if( CLRLAN_response == NULL )
        {
        wake_up_events = 0;      /* make sure loop test fails */
        }
      else    /* do validation */
        {
        if( validate != NULL )
          {     /* call the validation routine */
          result = (*(VALIDATION_FUNCTION *)validate)( CLRLAN_response );
          if( result == fail )
            {     /* make sure we retry */
            wake_up_events = 0;
            MemoryFree( CLRLAN_response );
            }
          }
        }
      }
    else
      {
            /* if we gave up, notify the CLRLAN task so it can go on */
      if( wake_up_events & event_timer_event )
         {
         EventQueueWrite( CLRLAN_task, calling_task_timeout_event,
                          (uint *)calling_task );
         }
      }
          /* until too many retries or anticipated event is set */
    } while( (retry_count++ < retries_allowed) &&  
             !(wake_up_events & unblock_event) );

       /* return result */
  if( wake_up_events & CLRLAN_packet_received_event )
    {
    return( CLRLAN_response );
    }
  else
    {
    return( NULL );
    }
}

