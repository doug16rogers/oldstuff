/*****************************************************************************
*
*  TITLE:        operator operator <
*
*  DESCRIPTION:  The subprogram "operator <"
*                of class TIME_OF_DAY
*                returns whether the object's
*                time is less than (before)
*                the given time.
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


BOOLEAN TIME_OF_DAY::operator < (

     const TIME_OF_DAY& time)
{
   if (hour < time.hour) return true;
   if (hour > time.hour) return false;

   if (minute < time.minute) return true;
   if (minute > time.minute) return false;

   if (second < time.second) return true;
   if (second > time.second) return true;

   if (millisecond < time.millisecond) return true;
   return false;

}   // TIME_OF_DAY::operator <

