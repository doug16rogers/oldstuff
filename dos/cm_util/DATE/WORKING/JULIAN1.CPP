/*****************************************************************************
*
*  TITLE:        Julian Day
*
*  DESCRIPTION:  The subprogram "DATE::Julian_Day"
*                in the class DATE
*                returns the Julian day for
*                the object's date.
*                The Julian day is the day
*                of the year, where Julian day 1
*                represents January 1.
*
*  *k "%n"
*  FILE NAME:    "JULIAN1.CPP"
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


int DATE::Julian_Day()

{
   BOOLEAN is_leap_year = Is_Leap_Year(year);

   int days_so_far = days_to_month[is_leap_year][month];
   days_so_far += day;

   return days_so_far;

}   // DATE::Julian_Day


