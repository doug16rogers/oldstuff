/*****************************************************************************
*
*  TITLE:        BIT_STREAM
*
*  DESCRIPTION:  The class method "BIT_STREAM"
*                of class BIT_STREAM
*                constructs a bitstream that is
*                mapped on top of the given memory.
*
*  *k "%n"
*  FILE NAME:    "BIT_STR2.CPP"
*
*  *k "%v"
*  VERSION:      "1"
*
*  REFERENCE:    None.
*
*****************************************************************************/

#include <stdio.h>

#include "bit_stre.h"


BIT_STREAM :: BIT_STREAM(

     void *memory_for_mapping,
     UINT size_in_bits)
{
   is_mapped = false;
   size = 0;

   if (memory_for_mapping != NULL)
   {
      is_mapped = true;
      base = (UINT8*) memory_for_mapping;
      size = size_in_bits;
   }

   Reset();

}   // BIT_STREAM :: BIT_STREAM

