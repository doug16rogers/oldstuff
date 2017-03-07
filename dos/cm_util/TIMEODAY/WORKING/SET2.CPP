/*****************************************************************************
*
*  TITLE:        Set
*
*  DESCRIPTION:  The subprogram "Set"
*                of class TIME_OF_DAY
*                sets the time to the
*                given object's time.
*
*                It returns whether or
*                not the time was valid
*                (i.e., whether or not
*                the receiver object's time
*                was actually set).
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

     const TIME_OF_DAY& time)       // time of day to set
{
   BOOLEAN is_valid =
        Is_Valid(time.hour, time.minute, time.second, time.millisecond);

   if (is_valid)
   {
      hour = time.hour;
      minute = time.minute;
      second = time.second;
      millisecond = time.millisecond;
   }

   return is_valid;

}   // TIME_OF_DAY::Set

