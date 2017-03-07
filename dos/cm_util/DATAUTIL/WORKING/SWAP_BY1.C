/*****************************************************************************
*
*  TITLE:        Swap Bytes
*
*  DESCRIPTION:  The subprogram "Swap_Bytes"
*                swaps the byte order of the given
*                number of 16-bit words.
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


void Swap_Bytes(

     UINT16* data,      /* data words whose bytes must be swapped */
     UINT count)        /* number of 16-bit words to swap */

{
   UINT8* byte_data;
   UINT8 temp;

   byte_data = (UINT8*) data;

   while (count-- > 0)
   {
      temp = byte_data[0];
      byte_data[0] = byte_data[1];
      byte_data[1] = temp;
      byte_data += 2;
   }   /* while */

}   /* Swap_Bytes */

