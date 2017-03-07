/*****************************************************************************
*
*  TITLE:        Poke
*
*  DESCRIPTION:  The class method "Poke"
*                of class BIT_STREAM
*                sets a single bit within
*                the bit stream.
*
*  *k "%n"
*  FILE NAME:    "POKE1.CPP"
*
*  *k "%v"
*  VERSION:      "1"
*
*  REFERENCE:    None.
*
*****************************************************************************/

#include "bit_stre.h"


void BIT_STREAM :: Poke(

     UINT index,
     UINT8 bit)
{
   if (index < count)
   {
      UINT bit_index = index + tail;

      if (bit_index > size)
      {
         bit_index -= size;
      }

      UINT8 bit_mask = ((UINT8) 1) << (UINT8) (bit_index % 8);
      UINT byte_index = bit_index / 8;

      if (bit == 0)
      {
         base[byte_index] &= ~bit_mask;
      }
      else
      {
         base[byte_index] |= bit_mask;
      }
   }

}   // BIT_STREAM :: Poke

