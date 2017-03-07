/*****************************************************************************
*
*  TITLE:        Second_Count
*
*  DESCRIPTION:  The subprogram "Second_Count"
*                of class TIME_OF_DAY
*                returns the number of
*                seconds since midnight for
*                the object's time.
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


UINT32 TIME_OF_DAY::Second_Count()
{
   return (seconds_per_minute * minutes_per_hour * hour) +
          (seconds_per_minute * minute) +
          second;

}   // TIME_OF_DAY::Second_Count

