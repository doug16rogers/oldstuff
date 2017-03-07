/*****************************************************************************
*
*  TITLE:        Put_Bit
*
*  DESCRIPTION:  The class method "Put_Bit"
*                of class BIT_STREAM
*                puts a single bit at
*                the head of the buffer.
*
*  *k "%n"
*  FILE NAME:    "PUT_BIT1.CPP"
*
*  *k "%v"
*  VERSION:      "1"
*
*  REFERENCE:    None.
*
*****************************************************************************/

#include "bit_stre.h"


void BIT_STREAM :: Put_Bit(

     UINT8 bit)
{
   if (count < size)
   {
      UINT8 bit_mask = ((UINT8) 1) << ((UINT8) (head % 8));

      if (bit == 0)
      {
         base[head / 8] &= ~bit_mask;
      }
      else
      {
         base[head / 8] |= bit_mask;
      }

      count++;
      head++;
      if (head >= size)
      {
         head = 0;
      }
   }

}   // BIT_STREAM :: Put_Bit

