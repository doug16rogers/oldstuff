/*****************************************************************************
*
*  TITLE:        Write_Data
*
*  DESCRIPTION:  The subprogram "Z8530::Write_Data"
*                for the class Z8530
*                writes a value to the
*                channel's data register (for
*                transmission).
*
*  *k "%n"
*  FILE NAME:    "WRITE_D1.CPP"
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



void Z8530::Write_Data(

     Z8530::CHANNEL channel, // channel (A/B) on which to write data byte
     UINT8 value)            // value to write to data register
{
   outportb(io_base + (2 * (UINT16) channel) + data_offset, value);

}   // Z8530::Write_Data


