/*****************************************************************************
*
*  TITLE:        Swap 32 Bit Bytes
*
*  DESCRIPTION:  The subprogram "Swap_32_Bit_Bytes"
*                swaps the byte order of the given
*                number of 32-bit words.
*                That is, byte 0 goes to byte 3,
*                byte 1 to byte 2, byte 2 to byte 1
*                and byte 3 to byte 0.
*
*  *k "%n"
*  FILE NAME:    ""
*
*  *k "%v"
*  VERSION:      ""
*
*  REFERENCE:    None.
*
*****************************************************************************/

#ifndef __datautil
#include "datautil.h"
#endif


void Swap_32_Bit_Bytes(

     UINT32* data,      /* 32-bit data words whose bytes must be swapped */
     UINT count)        /* number of 32-bit words to swap */

{
   UINT8* byte_data;
   UINT8 temp;

   byte_data = (UINT8*) data;

   while (count-- > 0)
   {
      temp = byte_data[0];
      byte_data[0] = byte_data[3];
      byte_data[3] = temp;

      temp = byte_data[1];
      byte_data[1] = byte_data[2];
      byte_data[2] = temp;

      byte_data += 4;
   }   /* while */

}   /* Swap_32_Bit_Bytes */

