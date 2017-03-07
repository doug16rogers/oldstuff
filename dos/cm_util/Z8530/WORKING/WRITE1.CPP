/*****************************************************************************
*
*  TITLE:        Write
*
*  DESCRIPTION:  The subprogram "Z8530::Write"
*                for the class Z8530
*                writes a value to the specified
*                register on the given channel.
*
*  *k "%n"
*  FILE NAME:    "WRITE1.CPP"
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


void Z8530::Write(

     Z8530::CHANNEL channel,              // channel (A/B) for write
     Z8530::WRITE_REGISTER _register,     // register for write
     UINT8 value)                         // value to write to register
{
   UINT16 control_port = io_base + (2 * (UINT16) channel) + control_offset;

   outportb(control_port, (UINT8) _register);
   goto next_line;
next_line:
   outportb(control_port, value);

}   // Z8530::Write


