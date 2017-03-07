/*****************************************************************************
*
*  TITLE:        New Write EEPROM Packet
*
*  DESCRIPTION:  The subprogram "New_Write_EEPROM_Packet"
*                allocates an EEPROM Write packet for the
*                SDLC link.  The header is filled in.
*
*  *k "%n"
*  FILE NAME:    "NEW_WRI1.C"
*
*  *k "%v"
*  VERSION:      "2"
*
*  REFERENCE:    See CSC reference.
*
*****************************************************************************/

#ifndef __sdlcpack
#include "sdlcpack.h"
#endif

#include "datautil.h"


SDLC_PACKET* New_Write_EEPROM_Packet(

     UINT8 destination,         /* destination (0x32 or 0x33) */
     UINT32  address,           /* EEPROM address for write, unswapped */
     UINT32* data,              /* the list of 32-bit words to write */
     UINT    word_count)        /* number of 32-bit words to write */

{
   WRITE_EEPROM_PACKET* eeprom_write_packet;
   SDLC_PACKET* packet;
   UINT  packet_data_size;

   packet_data_size = sizeof(WRITE_EEPROM_PACKET) -
                      sizeof(eeprom_write_packet->data) +
                      (word_count * sizeof(UINT32));

   packet = New_SDLC_Packet(
        destination,
        SDLC_COMMAND_PACKET,
        packet_data_size,
        NULL);

   if (packet != NULL)
   {
      eeprom_write_packet = (WRITE_EEPROM_PACKET*) packet->data;

      eeprom_write_packet->command = WRITE_EEPROM_COMMAND;
      eeprom_write_packet->size = word_count;
      eeprom_write_packet->address = address;
      eeprom_write_packet->checksum =
           CHECKSUM_CONSTANT - Sum32(data, word_count);
      Move_Data(data, eeprom_write_packet->data, word_count * sizeof(UINT32));
   }

   return packet;

}   /* New_Write_EEPROM_Packet */


