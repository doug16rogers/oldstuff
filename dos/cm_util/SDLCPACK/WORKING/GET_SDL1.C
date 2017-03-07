/*****************************************************************************
*
*  TITLE:        Get SDLC Packet
*
*  DESCRIPTION:  The subprogram "Get_SDLC_Packet"
*                checks that the given packet is valid
*                (i.e., it's length and checksum are ok).
*
*                It returns whether or not the packet was
*                valid.
*
*  *k "%n"
*  FILE NAME:    "GET_SDL1.C"
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

#ifndef NULL
#define NULL 0
#endif


BOOLEAN Get_SDLC_Packet(        /* whether packet is valid */

     SDLC_PACKET* packet)       /* packet to check */

{
   BOOLEAN ok = false;

   if ( (packet != NULL) &&
        (packet->size <= sizeof(SDLC_PACKET)) &&
        (Sum8((UINT8*) packet, packet->size) == 0) )

   {
      ok = true;
   }

   return ok;

}   /* Get_SDLC_Packet */


