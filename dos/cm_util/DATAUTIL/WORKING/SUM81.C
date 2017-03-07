/*****************************************************************************
*
*  TITLE:        Sum 8
*
*  DESCRIPTION:  The subprogram "Sum8"
*                calculates the sum of the given
*                number of 8-bit words.
*                It returns the sum.
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


UINT8 Sum8(             /* the sum */

     UINT8* buffer,     /* the data to generate sum of */
     UINT   length)     /* the number of data bytes */
{
   UINT8 sum = 0;

/*   FOR each byte in the data
        Add the byte to the sum
     NEXT byte
     Return the sum
*/

   while (length-- > 0)
   {
      sum += *buffer++;
   }

   return sum;

}   /* Sum8 */

