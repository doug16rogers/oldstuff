/*****************************************************************************
*
*  TITLE:        Insert Checksum
*
*  DESCRIPTION:  The subprogram "Insert_Checksum"
*                inserts the appropriate link layer
*                checksum for the given SDLC packet.
*                The SDLC packet length should include
*                the checksum.
*
*  *k "%n"
*  FILE NAME:    "INSERT_1.C"
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


void Insert_Checksum(

     SDLC_PACKET* packet)       /* packet into which to insert the checksum */

{
   UINT8* byte_data = (UINT8*) packet;

   if ( (packet != NULL) &&
        (packet->size > 0) )
   {
      byte_data[packet->size - 1] = -Sum8(byte_data, packet->size - 1);
   }

}   /* Insert_Checksum */


