/*****************************************************************************
*
*  TITLE:        Get Read EEPROM Response
*
*  DESCRIPTION:  The subprogram "Get_EEPROM_Read_Response"
*                extracts the data from a Read EEPROM Response packet.
*                The header and checksum are validated.
*                The various response data elements are
*                extracted into the given parameters.
*
*                It returns whether or not the packet was
*                a valid response.
*
*  *k "%n"
*  FILE NAME:    "GET_REA1.C"
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


BOOLEAN Get_Read_EEPROM_Response(       /* whether response was valid */

     SDLC_PACKET* packet,       /* packet from which to extract response */
     UINT32*  address,          /* EEPROM address from which data was read */
     UINT32** data,             /* pointer to words read (inside packet) */
     UINT*    word_count)       /* number of 32-bit words read */

{
   UINT8* byte_data = (UINT8*) packet;
   READ_EEPROM_RESPONSE_PACKET* response =
        (READ_EEPROM_RESPONSE_PACKET*) packet->data;
   BOOLEAN ok = false;
   UINT32 sum;

   if ( (packet != NULL) &&
        (Sum8(byte_data, packet->size) == 0) &&
        (packet->type == SDLC_COMMAND_PACKET) &&
        (response->command == READ_EEPROM_COMMAND) )
   {
      if (response->size == 0)
      {
         sum = Sum32(response->data, 0x100);
      }
      else
      {
         sum = Sum32(response->data, response->size);
      }

      if ((response->checksum + sum) == CHECKSUM_CONSTANT)
      {
         *address = response->address;
         *data = response->data;
         if (response->size == 0x00)
         {
            *word_count = 0x100;
         }
         else
         {
            *word_count = response->size;
         }
         ok = true;
      }
   }

   return ok;

}   /* Get_Read_EEPROM_Response */

