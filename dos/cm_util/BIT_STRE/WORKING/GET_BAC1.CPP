/*****************************************************************************
*
*  TITLE:        Get_Back
*
*  DESCRIPTION:  The class method "Get_Back"
*                of class BIT_STREAM
*                removes bits from the head
*                of the stream.
*
*  *k "%n"
*  FILE NAME:    "GET_BAC1.CPP"
*
*  *k "%v"
*  VERSION:      "1"
*
*  REFERENCE:    None.
*
*****************************************************************************/

#include "bit_stre.h"


UINT32 BIT_STREAM :: Get_Back(

     UINT number_of_bits)
{
   UINT32 bit = 1;
   UINT32 value = 0;

   if (number_of_bits > 32)
   {
      number_of_bits = 32;
   }

   while (number_of_bits-- > 0)
   {
      if (Get_Bit_Back())
      {
         value |= bit;
      }
      bit <<= 1;
   }

   return value;

}   // BIT_STREAM :: Get_Back

