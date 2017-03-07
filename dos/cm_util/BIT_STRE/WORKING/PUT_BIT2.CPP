/*****************************************************************************
*
*  TITLE:        Put_Bit_Back
*
*  DESCRIPTION:  The class method "Put_Bit_Back"
*                of class BIT_STREAM
*                puts a single bit at the tail
*                of the bit stream.
*
*  *k "%n"
*  FILE NAME:    "PUT_BIT2.CPP"
*
*  *k "%v"
*  VERSION:      "1"
*
*  REFERENCE:    None.
*
*****************************************************************************/

#include "bit_stre.h"


void BIT_STREAM :: Put_Bit_Back(

     UINT8 bit)
{
   if (count < size)
   {
      count++;

      if (tail > 0)
      {
         tail--;
      }
      else
      {
         tail = size - 1;
      }

      UINT8 bit_mask = ((UINT8) 1) << ((UINT8) (tail % 8));

      if (bit == 0)
      {
         base[tail / 8] &= ~bit_mask;
      }
      else
      {
         base[tail / 8] |= bit_mask;
      }
   }

}   // BIT_STREAM :: Put_Bit_Back

