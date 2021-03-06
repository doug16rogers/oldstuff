/*****************************************************************************
*
*  TITLE:        Set_Baud_Rate
*
*  DESCRIPTION:  The subprogram "Z8530::Set_Baud_Rate"
*                for the class Z8530
*                sets the baud rate by
*                writing an appropriate time constant
*                into register WR12 and WR13
*                of the given channel.
*
*                The class member "clock_speed" is
*                used in the calculation.
*
*                The divisor is assumed to be 1.
*
*  *k "%n"
*  FILE NAME:    "SET_BAU1.CPP"
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


UINT32 Z8530::Set_Baud_Rate(        // baud rate actually set

     Z8530::CHANNEL channel,     // channel to change
     UINT32 baud_rate)           // new baud rate for channel
{
   UINT32 time_constant;

   time_constant = (clock_speed / (2 * baud_rate)) - 2;

   if (time_constant <= 0x00010000UL)
   {
      Write(channel, WR12, (UINT8) time_constant);
      Write(channel, WR13, (UINT8) ((UINT32) time_constant >> 8));
   }

   return Baud_Rate(channel);

}   // Z8530::Set_Baud_Rate


