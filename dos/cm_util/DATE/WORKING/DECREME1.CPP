/*****************************************************************************
*
*  TITLE:        Decrement
*
*  DESCRIPTION:  The subprogram "DATE::Decrement"
*                in the class DATE
*                decrements the date by the
*                given number of days.
*
*  *k "%n"
*  FILE NAME:    "DECREME1.CPP"
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


void DATE::Decrement(

     int by_days)       // number of days by which to decrement date

{

   Increment(-by_days);

}   // DATE::Decrement

