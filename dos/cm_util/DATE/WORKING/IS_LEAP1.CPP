/*****************************************************************************
*
*  TITLE:        Is Leap Year
*
*  DESCRIPTION:  The subprogram "DATE::Is_Leap_Year"
*                in the class DATE
*                returns whether or not the
*                given year (or, if the given year
*                is 0, the object's year) is a
*                leap year.
*
*  *k "%n"
*  FILE NAME:    "IS_LEAP1.CPP"
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


BOOLEAN DATE::Is_Leap_Year(

     UINT16 for_year)           // year to check; if 0, use object's date

{

   BOOLEAN is_leap_year = true;

   if (for_year == 0)
   {
      for_year = this->year;
   }

   if      ((for_year %   4) != 0) is_leap_year = false;
   else if ((for_year % 100) != 0) is_leap_year = true;
   else if ((for_year % 400) != 0) is_leap_year = false;

   return is_leap_year;

}   // DATE::Is_Leap_Year

