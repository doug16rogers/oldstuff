/*****************************************************************************
*
*  TITLE:        Increment
*
*  DESCRIPTION:  The subprogram "Increment"
*                of class TIME_OF_DAY
*                increments the time
*                by the given (possibly negative)
*                number of seconds.
*
*                It returns the number of days
*                wrapped (either forward or
*                backward, if negative) because
*                of the incrementation.
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


int TIME_OF_DAY::Increment(     // number of days changed after inc

     int by_seconds)            // number of seconds (+/-) to increment by

{
   int delta_day = 0;

   INT32 seconds_into_day = (INT32) Second_Count();

   seconds_into_day = seconds_into_day + (INT32) by_seconds;

   while (seconds_into_day < 0L)
   {
      seconds_into_day += seconds_per_day;
      delta_day -= 1;
   }

   while (seconds_into_day >= seconds_per_day)
   {
      seconds_into_day -= seconds_per_day;
      delta_day += 1;
   }

   Set((UINT32) seconds_into_day, millisecond);

   return delta_day;

}   // TIME_OF_DAY::Increment

