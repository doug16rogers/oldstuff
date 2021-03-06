/*****************************************************************************
*
*  TITLE:        DATE Class Data
*
*  DESCRIPTION:  The module "class data"
*                contains the class variables and
*                constants for the class DATE.
*
*  *k "%n"
*  FILE NAME:    "CLASS_D1.CPP"
*
*  *k "%v"
*  VERSION:      "1"
*
*  REFERENCE:    None
*
*****************************************************************************/

#ifndef __DATE_CLASS
#include "date.h"
#endif


const UINT DATE::days_in_month[2][13] =       // month; 0 is not used
{
   { 0,   31, 28, 31,   30, 31, 30,   31, 31, 30,   31, 30, 31 },
   { 0,   31, 29, 31,   30, 31, 30,   31, 31, 30,   31, 30, 31 }
};

const UINT DATE::days_per_year[2] = { 365, 366 };

const UINT DATE::days_to_month[2][13] =       // days to start of month
{
   { 0,  0, 31, 59,  90, 120, 131,  181, 212, 243,  273, 304, 334 },
   { 0,  0, 31, 60,  91, 121, 132,  182, 213, 244,  274, 305, 335 }
};

const char* DATE::default_format = "%04y.%02m.%02d";        // 1980.01.01

char DATE::local_image[0x80] = "";  // for holding local image

const char* DATE::name_of_day[7] =  // day-of-week names, indexed by
{                                   //    Days_Of_Week
   "Sunday",
   "Monday",
   "Tuesday",
   "Wednesday",
   "Thursday",
   "Friday",
   "Saturday"
};

const char* DATE::name_of_month[13] =   // month names, indexed by
{                                       //    Months_Of_Year
   "???",
   "January",
   "February",
   "March",
   "April",
   "May",
   "June",
   "July",
   "August",
   "September",
   "October",
   "November",
   "December"
};

