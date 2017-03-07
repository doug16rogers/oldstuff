/*****************************************************************************
*
*  TITLE:        Put
*
*  DESCRIPTION:  The class method "Put"
*                of class BIT_STREAM
*                adds the given number of
*                bits of the given data
*                value, lsb first, at the
*                head of the
*                bit stream.
*                It returns the number of
*                bits successfully put.
*
*  *k "%n"
*  FILE NAME:    "PUT1.CPP"
*
*  *k "%v"
*  VERSION:      "1"
*
*  REFERENCE:    None.
*
*****************************************************************************/

#include "bit_stre.h"


UINT BIT_STREAM :: Put(

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
      Put_Bit((UINT8) data & 1);
      bits_put++;

      data >>= 1;

   }   // while

   return bits_put;

}   // BIT_STREAM :: Put

