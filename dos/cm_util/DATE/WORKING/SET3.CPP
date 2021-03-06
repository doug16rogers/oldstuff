/*****************************************************************************
*
*  TITLE:        Set
*
*  DESCRIPTION:  The subprogram "DATE::Set"
*                in the class DATE
*                sets the object's date to
*                the given day of the year
*                and the given year.
*
*  *k "%n"
*  FILE NAME:    "SET3.CPP"
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


void DATE::Set(

     int day_of_year,   // Julian day to set
     int year)          // year in which to set it

{
    BOOLEAN on_leap_year = Is_Leap_Year(year);

    if ((day_of_year < 1) ||
        (day_of_year > days_per_year[on_leap_year]))
    {
      return; //----------------------------------------------> return!
    }

    this->year = year;

    int for_month;

    for (for_month = February; for_month <= December; for_month++)
    {
      if (day_of_year < days_to_month[on_leap_year][for_month]) break;
    }

    this->month = for_month - 1;
    this->day = day_of_year - days_to_month[on_leap_year][month];

}   // DATE::Set

