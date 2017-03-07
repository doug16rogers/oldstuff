/*****************************************************************************
*
*  TITLE:        Swap Units
*
*  DESCRIPTION:  The subprogram "Swap_Units"
*                swaps the internal order of the given
*                number of records which hold the given
*                number units of the specified size.
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


void Swap_Units(

     void* data,                /* data words whose bytes must be swapped */
     UINT record_count,         /* number of records */
     UINT unit_count,           /* number of units in each record */
     UINT unit_size)            /* size of each unit in the record */

{
   UINT8* byte_data;
   UINT record_size;
   UINT unit_counter;
   UINT byte_counter;
   UINT half_way;
   UINT bottom_offset;
   UINT top_offset;
   UINT8 temp;

   byte_data = (UINT8*) data;
   record_size = unit_count * unit_size;
   half_way = unit_count / 2;

   while (record_count-- > 0)
   {
      bottom_offset = 0;
      top_offset = record_size - unit_size;

      for (unit_counter = 0; unit_counter < half_way; unit_counter++)
      {
         for (byte_counter = 0; byte_counter < unit_size; byte_counter++)
         {
            temp = byte_data[bottom_offset];
            byte_data[bottom_offset] = byte_data[top_offset];
            byte_data[top_offset] = temp;
            bottom_offset++;
            top_offset++;
         }

           /* since top was incremented during swap, need to decrement */
           /* twice -- once to get back to the previous unit and once to */
           /* get ready for the next unit. */
           /* no need to move bottom offset since it was */
           /* already incremented. */

         top_offset -= (2 * unit_size);
      }

      byte_data += record_size;

   }   /* while */

}   /* Swap_Units */


