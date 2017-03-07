/*****************************************************************************
*
*  TITLE:        Fill Read EEPROM Packet
*
*  DESCRIPTION:  The subprogram "Fill_Read_EEPROM_Packet"
*                fills in a Read EEPROM packet for the
*                SDLC link.  The header is filled in.
*
*  *k "%n"
*  FILE NAME:    "FILL_RE1.C"
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


SDLC_PACKET* Fill_Read_EEPROM_Packet(

     SDLC_PACKET* packet,       /* packet to fill */
     UINT8 destination,         /* destination (0x32 or 0x33) */
     UINT32 address,            /* EEPROM address for read */
     UINT   word_count)         /* number of 32-bit words to read */

{
   static READ_EEPROM_PACKET eeprom_read_packet;

   eeprom_read_packet.command = READ_EEPROM_COMMAND;
   eeprom_read_packet.size = word_count;
   eeprom_read_packet.address = address;

   return Fill_SDLC_Packet(
        packet,
        destination,
        SDLC_COMMAND_PACKET,
        sizeof(READ_EEPROM_PACKET),
        &eeprom_read_packet);

}   /* Fill_Read_EEPROM_Packet */


