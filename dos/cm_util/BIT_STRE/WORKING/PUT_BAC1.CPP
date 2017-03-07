/*****************************************************************************
*
*  TITLE:        Put_Back
*
*  DESCRIPTION:  The class method "Put_Back"
*                of class BIT_STREAM
*                puts the specified number of
*                bits from the given data
*                value, lsb first, at the
*                tail of the bit stream.
*                It returns the number
*                of bits successfully put.
*
*  *k "%n"
*  FILE NAME:    "PUT_BAC1.CPP"
*
*  *k "%v"
*  VERSION:      "1"
*
*  REFERENCE:    None.
*
*****************************************************************************/

#include "bit_stre.h"


UINT BIT_STREAM :: Put_Back(

     UINT32 data,
     UINT number_of_bits)
{
   if (number_of_bits > 32)
   {
      number_of_bits = 32;
   }

   UINT bits_put = 0;

   while ( (number_of_bits-- > 0) &&
           (count < size) )
   {
      Put_Bit_Back((UINT8) data & 1);
      bits_put++;

      data >>= 1;

   }   // while

   return bits_put;

}   // BIT_STREAM :: Put_Back

