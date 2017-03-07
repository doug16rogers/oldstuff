/*****************************************************************************
*
*  TITLE:        New High Speed Read Packet
*
*  DESCRIPTION:  The subprogram "New_High_Speed_Read_Packet"
*                allocates a High Speed EEPROM Read packet for the
*                SDLC link.  The header is filled in.
*                The destination will be determined
*                by the address to be read.
*
*  *k "%n"
*  FILE NAME:    "NEW_HIG1.C"
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


SDLC_PACKET* New_High_Speed_Read_Packet(

     UINT8 destination,         /* destination (0x32 or 0x33) */
     UINT32 address,            /* EEPROM address for read */
     UINT32 word_count)         /* number of 32-bit words to read */

{
   static HIGH_SPEED_READ_PACKET high_speed_read_packet;

   high_speed_read_packet.command = HIGH_SPEED_READ_COMMAND;
   high_speed_read_packet.address = address;
#ifdef __TURBOC__ /*-------------------------------------------------------*/
   high_speed_read_packet.count_lsb = word_count & 0xffff;
   high_speed_read_packet.count_msb = word_count >> 16;
#else /*-------------------------------------------------------------------*/
   high_speed_read_packet.count = word_count;
#endif /*------------------------------------------------------------------*/

   return New_SDLC_Packet(
        destination,
        SDLC_SYSTEM_STATUS_PACKET,
        sizeof(HIGH_SPEED_READ_PACKET),
        &high_speed_read_packet);

}   /* New_High_Speed_Read_Packet */

