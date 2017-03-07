/*****************************************************************************
*
*  TITLE:        Undo_Put
*
*  DESCRIPTION:  The class method "Undo_Put"
*                of class BIT_STREAM
*                unputs the given number of
*                bits by moving back the
*                head of the bit stream.
*
*  *k "%n"
*  FILE NAME:    "UNDO_PU1.CPP"
*
*  *k "%v"
*  VERSION:      "1"
*
*  REFERENCE:    None.
*
*****************************************************************************/

#include "bit_stre.h"


void BIT_STREAM :: Undo_Put(

     UINT number_of_bits)
{
   if (number_of_bits > count)
   {
      number_of_bits = count;
   }

   if (head < number_of_bits)
   {
      head += size;
   }

   head -= number_of_bits;

}   // BIT_STREAM :: Undo_Put

