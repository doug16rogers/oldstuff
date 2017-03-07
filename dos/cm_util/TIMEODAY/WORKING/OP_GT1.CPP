/*****************************************************************************
*
*  TITLE:        operator > 
*
*  DESCRIPTION:  The subprogram "operator > "
*                of class TIME_OF_DAY
*                returns whether the object's
*                time is greater (later) than the
*                given time.
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


BOOLEAN TIME_OF_DAY::operator > (

     const TIME_OF_DAY& time)

{
   return (BOOLEAN) (!(*this == time) &&
                     !(*this < time));

}   // TIME_OF_DAY::operator >

