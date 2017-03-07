/*****************************************************************************
*
*  TITLE:        Set
*
*  DESCRIPTION:  The subprogram "Set"
*                of class TIME_OF_DAY
*                sets the time to be
*                the given number of
*                seconds and milliseconds
*                into the day.
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


BOOLEAN TIME_OF_DAY::Set(

     const UINT32 to_seconds_into_day,  // seconds since 00:00:00
     const UINT32 with_milliseconds)

{
   return Set_By_Millisecond(
        (to_seconds_into_day * milliseconds_per_second) + with_milliseconds);

}   // TIME_OF_DAY::Set

