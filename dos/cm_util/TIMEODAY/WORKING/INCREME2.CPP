/*****************************************************************************
*
*  TITLE:        Increment_By_Milliseconds
*
*  DESCRIPTION:  The subprogram "Increment_By_Milliseconds"
*                of class TIME_OF_DAY
*                increments the date by
*                the given number of
*                milliseconds.
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

#include "timeoday.h"


int TIME_OF_DAY::Increment_By_Milliseconds(     // days wrapped

     int by_milliseconds)       // ms by which to increment

{
   int delta_day = 0;

   INT32 milliseconds_into_day = (INT32) Millisecond_Count();

   milliseconds_into_day += (INT32) by_milliseconds;

   while (milliseconds_into_day < 0L)
   {
      milliseconds_into_day += milliseconds_per_day;
      delta_day -= 1;
   }

   while (milliseconds_into_day >= milliseconds_per_day)
   {
      milliseconds_into_day -= milliseconds_per_day;
      delta_day += 1;
   }

   Set_By_Millisecond((UINT32) milliseconds_into_day);

   return delta_day;

}   // TIME_OF_DAY::Increment_By_Milliseconds

