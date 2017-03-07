/*****************************************************************************
*
*  TITLE:        New SDLC Packet
*
*  DESCRIPTION:  The subprogram "New_SDLC_Packet"
*                allocates a generic SDLC packet.
*
*  *k "%n"
*  FILE NAME:    "NEW_SDL1.C"
*
*  *k "%v"
*  VERSION:      "1"
*
*  REFERENCE:    See CSC reference.
*
*****************************************************************************/

#ifdef __TURBOC__
#include <alloc.h>
#include <mem.h>
#define Allocate        malloc
#endif

#ifndef __sdlcpack
#include "sdlcpack.h"
#endif

#include "datautil.h"


SDLC_PACKET* New_SDLC_Packet(

     UINT8 destination,         /* to whom the packet goes (DSPx_ADDRESS) */
     UINT  packet_type,         /* type of packet (command, etc) */
     UINT  data_size,           /* count of bytes in data portion of packet */
     void* data)                /* packet's data portion; NULL = do not use */

{
   SDLC_PACKET* packet;

   if (data_size > SDLC_DATA_SIZE)
   {
      return NULL; /*-----------------------------------------> return! */
   }

   packet = (SDLC_PACKET*) Allocate(sizeof(SDLC_PACKET));

   if (packet != NULL)
   {
      packet->destination = destination;
      packet->source = local_sdlc_address;
      packet->type = packet_type;
      packet->size = SDLC_HEADER_SIZE + data_size;

      if (data != NULL)
      {
         Move_Data(data, packet->data, data_size);
      }

      packet->data[data_size] = -Sum8((UINT8*) packet, packet->size - 1);
   }

   return packet;

}   /* New_SDLC_Packet */


