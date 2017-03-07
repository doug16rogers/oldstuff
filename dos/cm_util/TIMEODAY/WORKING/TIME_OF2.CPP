/*****************************************************************************
*
*  TITLE:        TIME_OF_DAY
*
*  DESCRIPTION:  The subprogram "TIME_OF_DAY"
*                of class TIME_OF_DAY
*                creates a time with the
*                given hour, minute, second
*                and millisecond.
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


TIME_OF_DAY::TIME_OF_DAY(

     const int hour,
     const int minute,
     const int second,
     const int millisecond)
{
    Set(0, 0, 0);
    Set(hour, minute, second, millisecond);

}   // TIME_OF_DAY::TIME_OF_DAY

