/*****************************************************************************
*
*  TITLE:        Get_Back
*
*  DESCRIPTION:  The class method "Get_Back"
*                of class BIT_STREAM
*                gets the specified number of
*                bits from the head of the stream.
*
*  *k "%n"
*  FILE NAME:    "GET_BAC2.CPP"
*
*  *k "%v"
*  VERSION:      "1"
*
*  REFERENCE:    None.
*
*****************************************************************************/

#include "bit_stre.h"


void BIT_STREAM :: Get_Back(

     UINT8* into_data,
     UINT number_of_bits)
{
   UINT8 value = 0;
   UINT8 bit = 1;

   while (number_of_bits-- > 0)
   {
      if (Get_Bit_Back())
      {
         value |= bit;
      }

      if (bit == 0)
      {
         *into_data++ = value;
         bit = 1;
      }

   }   // while

}   // BIT_STREAM :: Get_Back

