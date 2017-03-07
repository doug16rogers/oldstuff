/*****************************************************************************
*
*  TITLE:        Set
*
*  DESCRIPTION:  The subprogram "DATE::Set"
*                in the class DATE
*                sets the object's date to
*                the given year, month and day.
*
*  *k "%n"
*  FILE NAME:    "SET1.CPP"
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

     int year,          // year of new date
     int month,         // month of new date
     int day)           // day of new date

{
   if (Is_Valid((UINT16) year, (UINT8) month, (UINT8) day))
   {
      this->year = (UINT16) year;
      this->month = (UINT8) month;
      this->day = (UINT8) day;
   }

}   // DATE::Set


