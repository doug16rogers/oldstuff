/*****************************************************************************
*
*  TITLE:        Read
*
*  DESCRIPTION:  The subprogram "Z8530::Read"
*                for the class Z8530
*                reads the register on the given channel.
*
*  *k "%n"
*  FILE NAME:    "READ1.CPP"
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


UINT8 Z8530::Read(              // value read from the given register

     Z8530::CHANNEL channel,            // channel (A/B) from which to read
     Z8530::READ_REGISTER _register)    // register from which to read
{
   UINT16 control_port = io_base + (2 * (UINT16) channel) + control_offset;

   outportb(control_port, (UINT8) _register);
   goto next_line;
next_line:
   return (UINT8) inportb(control_port);

}   // Z8530::Read


