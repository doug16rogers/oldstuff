/*****************************************************************************
*
*  TITLE:        Fill High Speed Read Packet
*
*  DESCRIPTION:  The subprogram "Fill_High_Speed_Read_Packet"
*                fills in a High Speed EEPROM Read packet for the
*                SDLC link.  The header is filled in.
*
*  *k "%n"
*  FILE NAME:    "FILL_HI1.C"
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


SDLC_PACKET* Fill_High_Speed_Read_Packet(

     SDLC_PACKET* packet,       /* packet to fill */
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

   return Fill_SDLC_Packet(
        packet,
        destination,
        SDLC_SYSTEM_STATUS_PACKET,
        sizeof(HIGH_SPEED_READ_PACKET),
        &high_speed_read_packet);

}   /* Fill_High_Speed_Read_Packet */

