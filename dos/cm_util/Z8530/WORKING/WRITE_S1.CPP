/*****************************************************************************
*
*  TITLE:        Write_Set
*
*  DESCRIPTION:  The subprogram "Z8530::Write_Set"
*                for the class Z8530
*                writes a sequence of the given
*                number of values values
*                to the registers specified in
*                the WRITE_VALUE pair.
*
*                If the count is given as 0
*                then the sequence is terminated
*                when a register of value WREND is
*                encountered.
*
*  *k "%n"
*  FILE NAME:    "WRITE_S1.CPP"
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



void Z8530::Write_Set(

     Z8530::CHANNEL channel,         // channel on which to write data byte
     Z8530::WRITE_VALUE* write_set,  // Z8530::WREND-terminated list
     UINT count)                     // count of values to write, 0 =
                                     //    terminate when WREND is register
{
   UINT16 control_port = io_base + (2 * (UINT16) channel) + control_offset;

   if (count == 0)
   {
      count = (UINT) -1;
   }

   while ( (count-- > 0) &&
           (write_set->_register != WREND) )
   {
      outportb(control_port, (UINT8) write_set->_register);

      goto next_line;
next_line:

      outportb(control_port, write_set->value);

      write_set++;

   }   // while

}   // Z8530::Write_Set


