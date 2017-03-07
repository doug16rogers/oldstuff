/*****************************************************************************
*
*  TITLE:        Set
*
*  DESCRIPTION:  The subprogram "Set"
*                of class TIME_OF_DAY
*                sets the object's time
*                to the given hour, minute,
*                second and millisecond
*                (if they are valid).
*
*                It returns whether or not the
*                given elements comprise a valid
*                time (i.e., whether the time
*                was set).
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

     const int hour,
     const int minute,
     const int second,
     const int millisecond)
{
   BOOLEAN is_valid = Is_Valid(hour, minute, second, millisecond);

   if (is_valid)
   {
      this->hour = hour;
      this->minute = minute;
      this->second = second;
      this->millisecond = millisecond;
   }

   return is_valid;

}   // TIME_OF_DAY::Set

