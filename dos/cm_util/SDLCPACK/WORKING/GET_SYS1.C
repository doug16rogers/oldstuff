/*****************************************************************************
*
*  TITLE:        Get System Status Response
*
*  DESCRIPTION:  The subprogram "Get_System_Status_Response"
*                extracts the data from a System Status
*                response packet.
*
*                It returns whether or not the packet was
*                a valid response.
*
*  *k "%n"
*  FILE NAME:    "GET_SYS1.C"
*
*  *k "%v"
*  VERSION:      "1"
*
*  REFERENCE:    See CSC reference.
*
*****************************************************************************/

#ifndef __sdlcpack
#include "sdlcpack.h"
#endif

#include "datautil.h"


BOOLEAN Get_System_Status_Response(     /* whether response was valid */

     SDLC_PACKET* packet,       /* packet from which to extract response */
     UINT8* status_code)        /* status code from DSP */

{
   SYSTEM_STATUS_RESPONSE_PACKET* response =
        (SYSTEM_STATUS_RESPONSE_PACKET*) packet->data;
   BOOLEAN ok = false;

   if ( (packet != NULL) &&
        (packet->size ==
             (SDLC_HEADER_SIZE + sizeof(SYSTEM_STATUS_RESPONSE_PACKET))) &&
        (Sum8((UINT8*) packet, packet->size) == 0) &&
        (packet->type == SDLC_SYSTEM_STATUS_PACKET) &&
        (response->command == SYSTEM_STATUS_COMMAND) )
   {
      *status_code = response->code;
      ok = true;
   }

   return ok;

}   /* Get_System_Status_Response */


