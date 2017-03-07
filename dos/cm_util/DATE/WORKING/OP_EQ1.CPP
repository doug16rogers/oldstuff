/*****************************************************************************
*
*  TITLE:        operator ==
*
*  DESCRIPTION:  The subprogram "DATE::operator =="
*                in the class DATE
*                returns whether the
*                given date is the same
*                as the object's.
*
*  *k "%n"
*  FILE NAME:    "OP_EQ1.CPP"
*
*  *k "%v"
*  VERSION:      "1"
*
*  REFERENCE:    None.
*
*****************************************************************************/

#ifndef __DATE_CLASS
#include "date.h"
#endif


BOOLEAN DATE::operator == (

     DATE& date)

{

   return (BOOLEAN) ((year == date.year) &&
                     (month == date.month) &&
                     (day == date.day));

}   // DATE::operator ==

