/*****************************************************************************
*
*  TITLE:        Decrement
*
*  DESCRIPTION:  The subprogram "Decrement"
*                of class TIME_OF_DAY
*                decrements the time of day
*                by the given number of
*                seconds.
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


int TIME_OF_DAY::Decrement(     // number of days (+1/-1/0) changed after dec

     int by_seconds)            // number of seconds (+/-) to decrement by
{
    return Increment(-by_seconds);

}   // TIME_OF_DAY::Decrement

