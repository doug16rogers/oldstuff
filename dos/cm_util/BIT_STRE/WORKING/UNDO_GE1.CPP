/*****************************************************************************
*
*  TITLE:        Undo_Get
*
*  DESCRIPTION:  The class method "Undo_Get"
*                of class BIT_STREAM
*                ungets the given number
*                of bits by moving back the
*                tail of the bit stream.
*
*  *k "%n"
*  FILE NAME:    "UNDO_GE1.CPP"
*
*  *k "%v"
*  VERSION:      "1"
*
*  REFERENCE:    None.
*
*****************************************************************************/

#include "bit_stre.h"


void BIT_STREAM :: Undo_Get(

     UINT number_of_bits)
{
   if ((number_of_bits + count) > size)
   {
      number_of_bits = size - count;
   }

   if (tail < number_of_bits)
   {
      tail += size;
   }

   tail -= number_of_bits;

}   // BIT_STREAM :: Undo_Get

