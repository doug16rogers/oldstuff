/*****************************************************************************
*
*  TITLE:        Put
*
*  DESCRIPTION:  The class method "Put"
*                of class BIT_STREAM
*                puts the specified number of
*                bits from the given data bytes,
*                lsb first, to the head of the
*                bit stream.
*                It returns the number of bits
*                successfully put.
*
*  *k "%n"
*  FILE NAME:    "PUT2.CPP"
*
*  *k "%v"
*  VERSION:      "1"
*
*  REFERENCE:    None.
*
*****************************************************************************/

#include "bit_stre.h"


UINT BIT_STREAM :: Put(

     UINT8* out_of_data,
     UINT number_of_bits)
{
   UINT8 value = *out_of_data;
   UINT8 bit = 1;
   UINT bits_put = 0;

   while ( (number_of_bits-- > 0) &&
           (count < size) )
   {
      Put_Bit(value & bit);
      bits_put++;

      bit <<= 1;

      if (bit == 0)
      {
         value = *out_of_data++;
         bit = 1;
      }

   }   // while

   return bits_put;

}   // BIT_STREAM :: Put

