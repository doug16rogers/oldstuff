/*****************************************************************************
*
*  TITLE:        Fill System Status Packet
*
*  DESCRIPTION:  The subprogram "Fill_System_Status_Packet"
*                allocates a system status packet for
*                the given DSP.
*                The destination is used
*                to determine the unit field.
*
*  *k "%n"
*  FILE NAME:    "FILL_SY1.C"
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


SDLC_PACKET* Fill_System_Status_Packet(

     SDLC_PACKET* packet,       /* packet to fill */
     UINT8 destination)         /* to whom the packet goes (DSPx_ADDRESS) */

{
   static SYSTEM_STATUS_PACKET system_status_packet =
   {
      SYSTEM_STATUS_COMMAND,
      0
   };

   return Fill_SDLC_Packet(
        packet,
        destination,
        SDLC_SYSTEM_STATUS_PACKET,
        sizeof(SYSTEM_STATUS_PACKET),
        &system_status_packet);

}   /* Fill_System_Status_Packet */


