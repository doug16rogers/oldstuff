/*****************************************************************************
*
*  TITLE:        Increment
*
*  DESCRIPTION:  The subprogram "DATE::Increment"
*                in the class DATE
*                increments the object's date by
*                the given number of days.
*                Negative numbers may be used for
*                the parameter.
*                The number of days should not be
*                more than 365 or less than -365.
*
*  *k "%n"
*  FILE NAME:    "INCREME1.CPP"
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


void DATE::Increment(

     int by_days)       // number of days by which to increment

{
   if ((by_days >  days_per_year[0]) ||
       (by_days < -days_per_year[0]))
   {
      return; //----------------------------------------------> return!
   }

   int day_of_year = Julian_Day() + by_days;

   if (day_of_year < 1)
   {
      year--;
      day_of_year = day_of_year + days_per_year[Is_Leap_Year(year)];
   }
   else
   {
      int days_this_year = days_per_year[Is_Leap_Year(year)];

      if (day_of_year > days_this_year)
      {
         day_of_year = day_of_year - days_this_year;
         year++;
      }
   }

   Set(day_of_year, year);

}   // DATE::Increment

