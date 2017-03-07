/*****************************************************************************
*
*  TITLE:        Get_Bit
*
*  DESCRIPTION:  The class method "Get_Bit"
*                of class BIT_STREAM
*                gets a single bit from the
*                tail end of the stream.
*
*  *k "%n"
*  FILE NAME:    "GET_BIT1.CPP"
*
*  *k "%v"
*  VERSION:      "1"
*
*  REFERENCE:    None.
*
*****************************************************************************/

#include "bit_stre.h"


UINT8 BIT_STREAM :: Get_Bit()
{
   if (count > 0)
   {
      UINT tail_byte = tail / 8;
      UINT8 bit_mask = ((UINT8) 1) << ((UINT8) (tail % 8));

      count--;
      tail++;
      if (tail >= size)
      {
         tail = 0;
      }

      return (base[tail_byte] & bit_mask) != 0;
   }

   return 0;

}   // BIT_STREAM :: Get_Bit

