/*****************************************************************************
*
*  TITLE:        BIT_STREAM
*
*  DESCRIPTION:  The subprogram "BIT_STREAM :: BIT_STREAM"
*                constructs a bit stream of the
*                given size.
*
*  *k "%n"
*  FILE NAME:    "BIT_STR1.CPP"
*
*  *k "%v"
*  VERSION:      "1"
*
*  REFERENCE:    None.
*
*****************************************************************************/

#include <stdio.h>

#include "bit_stre.h"


BIT_STREAM :: BIT_STREAM(                  // source in "BIT_STR1.cpp"

     UINT size_in_bits)
{
   UINT size_in_bytes;

   size_in_bytes = (size_in_bits + 7) / 8;
   is_mapped = false;

   base = new UINT8[size_in_bytes];

   size = 0;
   if (base != NULL)
   {
      size = size_in_bits;
   }

   Reset();

}   // BIT_STREAM :: BIT_STREAM

