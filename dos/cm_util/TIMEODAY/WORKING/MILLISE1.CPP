/*****************************************************************************
*
*  TITLE:        Millisecond Count
*
*  DESCRIPTION:  The subprogram "Millisecond_Count"
*                of class TIME_OF_DAY
*                returns the number of milliseconds
*                that have expired for the object's time
*                since midnight.
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


UINT32 TIME_OF_DAY::Millisecond_Count()
{
   return ((UINT32) hour * milliseconds_per_hour) +
          ((UINT32) minute * milliseconds_per_minute) +
          ((UINT32) second * milliseconds_per_second) +
          (UINT32) millisecond;

}   // TIME_OF_DAY::Millisecond_Count


