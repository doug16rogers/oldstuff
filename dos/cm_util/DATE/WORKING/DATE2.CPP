/*****************************************************************************
*
*  TITLE:        DATE
*
*  DESCRIPTION:  The subprogram "DATE::DATE"
*                in the class DATE
*                sets the date to the given
*                year month and day.
*
*  *k "%n"
*  FILE NAME:    "DATE2.CPP"
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


DATE::DATE(

     int year,
     int month,
     int day)

{

   last_format = default_format;
   this->year = 1980;
   this->month = January;
   this->day = 1;
   Set(year, month, day);

}   // DATE::DATE


