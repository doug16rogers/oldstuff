/*****************************************************************************
*
*  TITLE:        Sum 16
*
*  DESCRIPTION:  The subprogram "Sum16"
*                calculates the sum of the given
*                number of 16-bit words.
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


UINT16 Sum16(           /* the sum */

     UINT16* buffer,    /* the buffer to perform sum of */
     UINT    length)    /* the length of the buffer */
{
   UINT16 sum = 0;

/*   FOR each 16-bit word in the data
        Add the 16-bit word to the sum
     NEXT byte
     Return the sum
*/

   while (length-- > 0)
   {
      sum += *buffer++;
   }

   return sum;

}   /* Sum16 */

