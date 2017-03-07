/*****************************************************************************
*
*  TITLE:        Day_Of_Week
*
*  DESCRIPTION:  The subprogram "DATE::Day_Of_Week"
*                in the class DATE
*                calculates and returns the
*                day of week for the object's date.
*
*  *k "%n"
*  FILE NAME:    "DAY_OF_1.CPP"
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


DATE::DAYS_OF_WEEK DATE::Day_Of_Week()

{
   static const int offset_into_week_for[13] =  //offset of month into week
   {
      0,                // this month (0) not used
      0, 3, 3, 6,       // January, February, March, April
      1, 4, 6, 2,       // May, June, July, August
      5, 0, 3, 5        // September, October, November, December
   };

   int day_of_week;     // day of week -- algorithm will keep it >= 0 thru-out
   int year_mod_400;

   year_mod_400 = year % 400;          // year into 400yr cycle

   day_of_week  = (int) Saturday;      // 400 yr calendar starts on Saturday
   day_of_week += year_mod_400;        // add in 1 day of week per year
   day_of_week += year_mod_400/4;      // add in an extra for leap years
   day_of_week -= year_mod_400/100;    // no leap year on centuries
   day_of_week += offset_into_week_for[month];  // add in offset for month
   day_of_week += day;                 // add in day of month

   if ( Is_Leap_Year(year) &&
        (month <= February) )
   {
      day_of_week--;                   // sub extra leapyear added
   }

   day_of_week %= 7;                   // get day of week

   return (DAYS_OF_WEEK) day_of_week;

}   // DATE::Day_Of_Week


