/*****************************************************************************
*
*  TITLE:        Get Write EEPROM Response
*
*  DESCRIPTION:  The subprogram "Get_Write_EEPROM_Response"
*                extracts the data from an EEPROM Write Response.
*
*                It returns whether or not the packet was
*                a valid response.
*
*  *k "%n"
*  FILE NAME:    "GET_WRI1.C"
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


BOOLEAN Get_Write_EEPROM_Response(   /* whether response was valid */

     SDLC_PACKET* packet)       /* packet from which to extract response */

{
   UINT8* byte_data = (UINT8*) packet;
   WRITE_EEPROM_RESPONSE_PACKET* response =
        (WRITE_EEPROM_RESPONSE_PACKET*) packet->data;

   return (packet != NULL) &&
          (Sum8(byte_data, packet->size) == 0) &&
          (packet->type == SDLC_COMMAND_PACKET) &&
          (response->command == WRITE_EEPROM_COMMAND) &&
          (response->code == 0);

}   /* Get_Write_EEPROM_Response */


