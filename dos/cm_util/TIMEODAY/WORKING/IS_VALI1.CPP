/*****************************************************************************
*
*  TITLE:        Is_Valid
*
*  DESCRIPTION:  The subprogram "Is_Valid"
*                of class TIME_OF_DAY
*                checks to see whether the given hour,
*                minute, second and millisecond
*                comprise a valid time of day.
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


BOOLEAN TIME_OF_DAY::Is_Valid(

     const UINT8 for_hour,
     const UINT8 for_minute,
     const UINT8 for_second,
     const UINT16 for_millisecond)

{
   return (BOOLEAN) ((for_hour < hours_per_day) &&
                     (for_minute < minutes_per_hour) &&
                     (for_second < seconds_per_minute) &&
                     (for_millisecond < milliseconds_per_second));

}   // TIME_OF_DAY::Is_Valid

