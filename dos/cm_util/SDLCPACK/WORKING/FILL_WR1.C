/*****************************************************************************
*
*  TITLE:        Fill Write EEPROM Packet
*
*  DESCRIPTION:  The subprogram "Fill_Write_EEPROM_Packet"
*                allocates an EEPROM Write packet for the
*                SDLC link.  The header is filled in.
*
*  *k "%n"
*  FILE NAME:    "FILL_WR1.C"
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


SDLC_PACKET* Fill_Write_EEPROM_Packet(

     SDLC_PACKET* packet,       /* packet to fill */
     UINT8 destination,         /* destination (0x32 or 0x33) */
     UINT32  address,           /* EEPROM address for write, unswapped */
     UINT32* data,              /* the list of 32-bit words to write */
     UINT    word_count)        /* number of 32-bit words to write */

{
   WRITE_EEPROM_PACKET* write_eeprom_packet;
   UINT  packet_data_size;

   packet_data_size = sizeof(WRITE_EEPROM_PACKET) -
                      sizeof(write_eeprom_packet->data) +
                      (word_count * sizeof(UINT32));

   if (Fill_SDLC_Packet(
        packet,
        destination,
        SDLC_COMMAND_PACKET,
        packet_data_size,
        NULL) != NULL)
   {
      write_eeprom_packet = (WRITE_EEPROM_PACKET*) packet->data;

      write_eeprom_packet->command = WRITE_EEPROM_COMMAND;
      write_eeprom_packet->size = word_count;
      write_eeprom_packet->address = address;
      write_eeprom_packet->checksum =
           CHECKSUM_CONSTANT - Sum32(data, word_count);
      if (data != NULL)
      {
         Move_Data(data,
                   write_eeprom_packet->data,
                   word_count * sizeof(UINT32));
      }
      packet->data[packet_data_size] = -Sum8((UINT8*) packet,
           packet->size - 1);
   }

   return packet;

}   /* Fill_Write_EEPROM_Packet */


