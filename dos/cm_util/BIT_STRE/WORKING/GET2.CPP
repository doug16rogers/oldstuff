/*****************************************************************************
*
*  TITLE:        Get
*
*  DESCRIPTION:  The class method "Get"
*                of class BIT_STREAM
*                gets as many bits as
*                requested from the stream
*                and places them in the
*                specified buffer.
*
*  *k "%n"
*  FILE NAME:    "GET2.CPP"
*
*  *k "%v"
*  VERSION:      "1"
*
*  REFERENCE:    None.
*
*****************************************************************************/

#include "bit_stre.h"


void BIT_STREAM :: Get(

     UINT8* into_data,
     UINT number_of_bits)
{
   UINT8 value = 0;
   UINT8 bit = 1;

   while (number_of_bits-- > 0)
   {
      if (Get_Bit())
      {
         value |= bit;
      }

      if (bit == 0)
      {
         *into_data++ = value;
         bit = 1;
      }

   }   // while

}   // BIT_STREAM :: Get

