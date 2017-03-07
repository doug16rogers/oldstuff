/*****************************************************************************
*
*  TITLE:        New Read EEPROM Packet
*
*  DESCRIPTION:  The subprogram "New_Read_EEPROM_Packet"
*                allocates an Read EEPROM packet for the
*                SDLC link.  The header is filled in.
*                The destination will be determined
*                by the address to be read.
*
*  *k "%n"
*  FILE NAME:    "NEW_REA1.C"
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


SDLC_PACKET* New_Read_EEPROM_Packet(

     UINT8 destination,         /* destination (0x32 or 0x33) */
     UINT32 address,            /* EEPROM address for read */
     UINT   word_count)         /* number of 32-bit words to read */

{
   static READ_EEPROM_PACKET eeprom_read_packet;

   eeprom_read_packet.command = READ_EEPROM_COMMAND;
   eeprom_read_packet.size = word_count;
   eeprom_read_packet.address = address;

   return New_SDLC_Packet(
        destination,
        SDLC_COMMAND_PACKET,
        sizeof(READ_EEPROM_PACKET),
        &eeprom_read_packet);

}   /* New_Read_EEPROM_Packet */


