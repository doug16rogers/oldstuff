#ifndef __DATE_CLASS
#define __DATE_CLASS
/*****************************************************************************
*
*  TITLE:       Date
*
*  DESCRIPTION: The "DATE" class provides operations for dates.
*
*  *k "%n"
*  FILE NAME:   "DATE.H"
*
*  *k "%v"
*  VERSION:     "1"
*
*****************************************************************************/

#ifndef __icitypes
#include "icitypes.h"
#endif


class DATE
{

//===========================================================================
//
//  Class types
//
//---------------------------------------------------------------------------

public:

   static const UINT days_per_year[2];

   static const UINT days_in_month[2][13];

   static const UINT days_to_month[2][13];

   typedef enum
   {
      Sunday = 0,
      Monday,
      Tuesday,
      Wednesday,
      Thursday,
      Friday,
      Saturday

   } DAYS_OF_WEEK;


   typedef enum
   {
      January = 1,
      February,
      March,
      April,
      May,
      June,
      July,
      August,
      September,
      October,
      November,
      December

   } MONTHS_OF_YEAR;

//===========================================================================
//
//  Class data
//
//---------------------------------------------------------------------------

/*-------------------------------------------------------------------------
   Source: "class_d1.cpp"
  -------------------------------------------------------------------------*/

private:

   static const char* default_format;   // 1980.01.01

   static char local_image[0x80];       // for holding local image of time

public:

   const char* last_format;             // holds on to last format used

   static const char* name_of_month[13];    // month names, indexed by
                                            //    Months_Of_Year

   static const char* name_of_day[7];   // day-of-week names, indexed by
                                        //   Days_Of_Week

public:

   UINT16 year;
   UINT8 month;
   UINT8 day;

//===========================================================================
//
//  Class methods
//
//---------------------------------------------------------------------------

/*-------------------------------------------------------------------------
   Source: "DATE1.cpp"
  -------------------------------------------------------------------------*/

public:

   DATE();

/*-------------------------------------------------------------------------
   Source: "DATE2.cpp"
  -------------------------------------------------------------------------*/

public:

   DATE(

      int year,         // year to use for date
      int month,        // month for date
      int day);         // day of month for date

/*-------------------------------------------------------------------------
   Source: "Day_Of_1.cpp"
  -------------------------------------------------------------------------*/

public:

   DAYS_OF_WEEK Day_Of_Week(void);

/*-------------------------------------------------------------------------
   Source: "Decreme1.cpp"
  -------------------------------------------------------------------------*/

public:

   void Decrement(

        int by_days = 1);       // number of days to decrement by

/*-------------------------------------------------------------------------
   Source: "Get_Sys1.cpp"
  -------------------------------------------------------------------------*/

public:

   void Get_System_Date();

/*-------------------------------------------------------------------------
   Source: "Image1.cpp"
  -------------------------------------------------------------------------*/

public:

   char* Image(                 // buffer as passed in

        char* into_buffer = NULL,   // buffer for image; NULL = local
        const char* with_format = NULL);        // format string;
                                                //    NULL = last_format

/*-------------------------------------------------------------------------
   Source: "Increme1.cpp"
  -------------------------------------------------------------------------*/

public:

   void Increment(

        int by_days = 1);       // number of days to increment by

/*-------------------------------------------------------------------------
   Source: "Is_Leap1.cpp"
  -------------------------------------------------------------------------*/

public:

   BOOLEAN Is_Leap_Year(

        UINT16 for_year = 0);   // year to check; if 0, use object's date

/*-------------------------------------------------------------------------
   Source: "Is_Vali1.cpp"
  -------------------------------------------------------------------------*/

public:

   BOOLEAN Is_Valid(

        UINT16 for_year,        // year of date to check
        UINT8 for_month,        // month of date to check
        UINT8 for_day);         // day of month to check

/*-------------------------------------------------------------------------
   Source: "Julian1.cpp"
  -------------------------------------------------------------------------*/

public:

   int Julian_Day();

/*-------------------------------------------------------------------------
   Source: "op_lt1.cpp"
  -------------------------------------------------------------------------*/

public:

   BOOLEAN operator < (

        DATE& date);

/*-------------------------------------------------------------------------
   Source: "op_eq1.cpp"
  -------------------------------------------------------------------------*/

public:

   BOOLEAN operator == (

        DATE& date);

/*-------------------------------------------------------------------------
   Source: "op_gt1.cpp"
  -------------------------------------------------------------------------*/

public:

   BOOLEAN operator > (

        DATE& date);

/*-------------------------------------------------------------------------
   Source: "op_lteq1.cpp"
  -------------------------------------------------------------------------*/

public:

   BOOLEAN operator <= (

        DATE& date);

/*-------------------------------------------------------------------------
   Source: "op_neq1.cpp"
  -------------------------------------------------------------------------*/

public:

   BOOLEAN operator != (

        DATE& date);

/*-------------------------------------------------------------------------
   Source: "op_gteq1.cpp"
  -------------------------------------------------------------------------*/

public:

   BOOLEAN operator >= (

        DATE& date);

/*-------------------------------------------------------------------------
   Source: "Set1.cpp"
  -------------------------------------------------------------------------*/

public:

   void Set(

        int year,       // year of new date
        int month,      // month of new date
        int day);       // day of new date

/*-------------------------------------------------------------------------
   Source: "Set2.cpp"
  -------------------------------------------------------------------------*/

public:

   void Set(

        DATE& date);

/*-------------------------------------------------------------------------
   Source: "Set3.cpp"
  -------------------------------------------------------------------------*/

public:

   void Set(

        int day_of_year,        // Julian day to set
        int year);              // year in which to set it

/*-------------------------------------------------------------------------
   Source: "Set_Sys1.cpp"
  -------------------------------------------------------------------------*/

public:

   void Set_System_Date();

};   //class DATE


#endif

