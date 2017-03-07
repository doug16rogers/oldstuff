/*****************************************************************************
*
*  TITLE:        operator !=
*
*  DESCRIPTION:  The subprogram "operator !="
*                of class TIME_OF_DAY
*                returns whether the object's
*                time does not equal (is not simultaneous
*                with) the given time.
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


BOOLEAN TIME_OF_DAY::operator != (

     const TIME_OF_DAY& time)

{

   return (BOOLEAN) (!(*this == time));

}   // TIME_OF_DAY::operator !=

