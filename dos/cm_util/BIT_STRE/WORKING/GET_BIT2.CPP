/*****************************************************************************
*
*  TITLE:        Get_Bit_Back
*
*  DESCRIPTION:  The class method "Get_Bit_Back"
*                of class BIT_STREAM
*                gets a single bit from the head
*                of the stream.  That is, it gets
*                the bit most recently put.
*
*  *k "%n"
*  FILE NAME:    "GET_BIT2.CPP"
*
*  *k "%v"
*  VERSION:      "1"
*
*  REFERENCE:    None.
*
*****************************************************************************/

#include "bit_stre.h"


UINT8 BIT_STREAM :: Get_Bit_Back()
{
   if (count > 0)
   {
      count--;

      if (head == 0)
      {
         head = size - 1;
      }
      else
      {
         head--;
      }

      UINT head_byte = head / 8;
      UINT8 bit_mask = ((UINT8) 1) << ((UINT8) (head % 8));

      return (base[head_byte] & bit_mask) != 0;
   }

   return 0;

}   // BIT_STREAM :: Get_Bit_Back

