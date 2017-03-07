/*****************************************************************************
*
*  TITLE:        Get
*
*  DESCRIPTION:  The class method "Get"
*                of class BIT_STREAM
*                gets up to 32 bits from
*                the tail of the stream.
*
*  *k "%n"
*  FILE NAME:    "GET1.CPP"
*
*  *k "%v"
*  VERSION:      "1"
*
*  REFERENCE:    None.
*
*****************************************************************************/

#include "bit_stre.h"


UINT32 BIT_STREAM :: Get(

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
      if (Get_Bit())
      {
         value |= bit;
      }
      bit <<= 1;
   }

   return value;

}   // BIT_STREAM :: Get

