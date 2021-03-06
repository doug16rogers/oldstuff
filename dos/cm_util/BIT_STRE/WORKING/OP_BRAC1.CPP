/*****************************************************************************
*
*  TITLE:        operator [] 
*
*  DESCRIPTION:  The class method "operator []"
*                of class BIT_STREAM
*                returns the bit at the
*                given index.
*
*  *k "%n"
*  FILE NAME:    "OP_BRAC1.CPP"
*
*  *k "%v"
*  VERSION:      "1"
*
*  REFERENCE:    None.
*
*****************************************************************************/

#include "bit_stre.h"


UINT8 BIT_STREAM :: operator [] (

     UINT index)
{
   if (index < count)
   {
      index += tail;

      UINT index_byte = index / 8;
      UINT8 bit_mask = ((UINT8) 1) << (UINT8) (index % 8);

      return (UINT8) (base[index_byte] & bit_mask) != 0;
   }

   return 0;

}   // BIT_STREAM :: [] 

