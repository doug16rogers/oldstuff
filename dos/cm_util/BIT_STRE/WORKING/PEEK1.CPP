/*****************************************************************************
*
*  TITLE:        Peek
*
*  DESCRIPTION:  The class method "Peek"
*                of class BIT_STREAM
*                gets the number of bits
*                specified (up to 32) from the
*                given index within the bit stream.
*                The bits are not removed.
*
*  *k "%n"
*  FILE NAME:    "PEEK1.CPP"
*
*  *k "%v"
*  VERSION:      "1"
*
*  REFERENCE:    None.
*
*****************************************************************************/

#include "bit_stre.h"


UINT32 BIT_STREAM :: Peek(

     UINT index,
     UINT number_of_bits)
{
   if (number_of_bits > 32)
   {
      number_of_bits = 32;
   }


   UINT32 value = 0;
   UINT32 bit = 1;
   UINT bit_index = index + tail;

   if (bit_index > size)
   {
      bit_index -= size;
   }

   while ( (number_of_bits-- > 0) &&
           (index++ < count) )
   {
      UINT8 bit_mask = ((UINT8) 1) << (UINT8) (bit_index % 8);
      if ((base[bit_index / 8] & bit_mask) != 0)
      {
         value |= bit;
      }

      bit <<= 1;

      bit_index++;
      if (bit_index >= size)
      {
         bit_index = 0;
      }

   }   // while

   return value;

}   // BIT_STREAM :: Peek

