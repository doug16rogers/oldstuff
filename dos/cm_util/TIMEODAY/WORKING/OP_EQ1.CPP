/*****************************************************************************
*
*  TITLE:        operator ==
*
*  DESCRIPTION:  The subprogram "operator =="
*                of class TIME_OF_DAY
*                checks whether the given time
*                equals the object's time.
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


BOOLEAN TIME_OF_DAY::operator == (

     const TIME_OF_DAY& time)

{

   return (BOOLEAN) ((hour == time.hour) &&
                     (minute == time.minute) &&
                     (second == time.second) &&
                     (millisecond == time.millisecond));

}   // TIME_OF_DAY::operator ==

