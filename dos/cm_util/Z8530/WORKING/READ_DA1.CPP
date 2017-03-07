/*****************************************************************************
*
*  TITLE:        Read_Data
*
*  DESCRIPTION:  The subprogram "Z8530::Read_Data"
*                for the class Z8530
*                reads the data register for
*                the given channel.
*
*  *k "%n"
*  FILE NAME:    "READ_DA1.CPP"
*
*  *k "%v"
*  VERSION:      "2"
*
*  REFERENCE:    Zilog 8530 programmer's reference.
*
*****************************************************************************/

#ifndef __z8530
#include "z8530.h"
#endif


UINT8 Z8530::Read_Data(         // data register (RR8) value for channel

     Z8530::CHANNEL channel)    // channel (A/B) for read
{
   return (UINT8) inportb(io_base + (2 * (UINT16) channel) + data_offset);

}   // Z8530::Read_Data


