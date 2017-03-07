/*****************************************************************************
*
*  TITLE:        Is Valid
*
*  DESCRIPTION:  The subprogram "DATE::Is_Valid"
*                in the class DATE
*                returns whether the given
*                year, month and day of month
*                comprise a valid date.
*
*  *k "%n"
*  FILE NAME:    "IS_VALI1.CPP"
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


BOOLEAN DATE::Is_Valid(

     UINT16 for_year,   // year of date to check
     UINT8 for_month,   // month of date to check
     UINT8 for_day)     // day of month to check

{
   BOOLEAN date_is_valid = false;
   BOOLEAN in_leap_year = Is_Leap_Year(for_year);

   if ( (for_month >= January) &&
        (for_month <= December) &&
        (for_day >= 1) &&
        (for_day <= days_in_month[in_leap_year][for_month]) )
   {
      date_is_valid = true;
   }

   return date_is_valid;

}   // DATE::Is_Valid

