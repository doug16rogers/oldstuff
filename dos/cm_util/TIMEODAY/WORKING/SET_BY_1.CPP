/*****************************************************************************
*
*  TITLE:        Set_By_Millisecond
*
*  DESCRIPTION:  The subprogram "Set_By_Millisecond"
*                of class TIME_OF_DAY
*                sets the time of day to
*                be the given number of
*                milliseconds since midnight.
*
*                It returns whether the object's
*                time was set (whether the
*                millisecond count fits in a day).
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


BOOLEAN TIME_OF_DAY::Set_By_Millisecond(

     const UINT32 to_milliseconds_into_day)  // ms since 00:00:00
{
   BOOLEAN is_valid = (BOOLEAN)
        (to_milliseconds_into_day < milliseconds_per_day);

   if (is_valid)
   {
      millisecond = to_milliseconds_into_day % milliseconds_per_second;

      UINT32 seconds_into_day = to_milliseconds_into_day /
           milliseconds_per_second;
      second = seconds_into_day % seconds_per_minute;

      UINT32 minutes_into_day = seconds_into_day / seconds_per_minute;
      minute = minutes_into_day % minutes_per_hour;

      hour = minutes_into_day / minutes_per_hour;
   }

   return is_valid;

}   // TIME_OF_DAY::Set_By_Millisecond

