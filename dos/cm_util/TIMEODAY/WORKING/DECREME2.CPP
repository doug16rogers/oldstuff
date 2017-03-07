/*****************************************************************************
*
*  TITLE:        Decrement By Milliseconds
*
*  DESCRIPTION:  The subprogram "Decrement_By_Milliseconds"
*                of class TIME_OF_DAY
*                decrements the time of day
*                by the given number of
*                milliseconds.
*
*                It returns the number of days wrapped
*                as a result of the decrementation.
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


int TIME_OF_DAY::Decrement_By_Milliseconds(

     int by_seconds)    // number of milliseconds (+/-) to decrement by
{
    return Increment_By_Milliseconds(-by_seconds);

}   // TIME_OF_DAY::Decrement_By_Milliseconds


