#ifndef __timeoday_hpp
#define __timeoday_hpp
/*****************************************************************************
*
*  TITLE:        Time Of Day
*
*  DESCRIPTION:  The class "TIME_OF_DAY"
*                maintains time-of-day information.
*
*  *k "%n"
*  FILE NAME:    ""
*
*  *k "%v"
*  VERSION:      ""
*
*  REFERENCE:    None.
*
*****************************************************************************/

#ifndef __icitypes
#include "icitypes.h"
#endif


class TIME_OF_DAY
{

//===========================================================================
//
//  Class data
//
//---------------------------------------------------------------------------

/*-------------------------------------------------------------------------
   Source: "class_d1.cpp"
  -------------------------------------------------------------------------*/

private:

   static char* default_format;         //00:00:00 .. 23:59:59

   static char local_image[0x80];       //for holding direct image

public:

   static const UINT32 milliseconds_per_second;
   static const UINT32 seconds_per_minute;
   static const UINT32 minutes_per_hour;
   static const UINT32 hours_per_day;
   static const UINT32 minutes_per_day;
   static const UINT32 seconds_per_hour;
   static const UINT32 seconds_per_day;
   static const UINT32 milliseconds_per_minute;
   static const UINT32 milliseconds_per_hour;
   static const UINT32 milliseconds_per_day;

public:

   char* last_format;           // holds on to last format used

public:

   UINT8 hour;                  // this is the time of day...
   UINT8 minute;
   UINT8 second;
   UINT16 millisecond;

//===========================================================================
//
//  Class methods
//
//---------------------------------------------------------------------------

/*-------------------------------------------------------------------------
   Source: "Decreme1.cpp"
  -------------------------------------------------------------------------*/

public:

   int Decrement(               // number of days (+/-/0) changed after dec

        int by_seconds = 1);    // number of seconds (+/-) to decrement by

/*-------------------------------------------------------------------------
   Source: "Decreme2.cpp"
  -------------------------------------------------------------------------*/

public:

   int Decrement_By_Milliseconds(       // days wrapped

        int by_milliseconds = 1);       // ms by which to decrement

/*-------------------------------------------------------------------------
   Source: "Get_Sys1.cpp"
  -------------------------------------------------------------------------*/

public:

   virtual void Get_System_Time();

/*-------------------------------------------------------------------------
   Source: "Image1.cpp"
  -------------------------------------------------------------------------*/

public:

   char* Image(                  //buffer as passed in

        char* into_buffer = NULL,       // buffer to hold the image
        char* with_format = NULL);      // NULL = last format used for object

/*-------------------------------------------------------------------------
   Source: "Increme1.cpp"
  -------------------------------------------------------------------------*/

public:

   int Increment(               // number of days (+/-/0) changed after inc

        int by_seconds = 1);    // number of seconds (+/-) to increment by

/*-------------------------------------------------------------------------
   Source: "Increme2.cpp"
  -------------------------------------------------------------------------*/

public:

   int Increment_By_Milliseconds(       // days wrapped

        int by_milliseconds = 1);       // ms by which to increment

/*-------------------------------------------------------------------------
   Source: "Is_Vali1.cpp"
  -------------------------------------------------------------------------*/

public:

   BOOLEAN Is_Valid(

        const UINT8 for_hour,
        const UINT8 for_minute,
        const UINT8 for_second = 0,
        const UINT16 millisecond = 0);

/*-------------------------------------------------------------------------
   Source: "Millise1.cpp"
  -------------------------------------------------------------------------*/

public:

   UINT32 Millisecond_Count();

/*-------------------------------------------------------------------------
   Source: "op_eq1.cpp"
  -------------------------------------------------------------------------*/

public:

   BOOLEAN operator == (

        const TIME_OF_DAY& time);

/*-------------------------------------------------------------------------
   Source: "op_gt1.cpp"
  -------------------------------------------------------------------------*/

public:

   BOOLEAN operator > (

        const TIME_OF_DAY& time);

/*-------------------------------------------------------------------------
   Source: "op_gteq1.cpp"
  -------------------------------------------------------------------------*/

public:

   BOOLEAN operator >= (

        const TIME_OF_DAY& time);

/*-------------------------------------------------------------------------
   Source: "op_lt1.cpp"
  -------------------------------------------------------------------------*/

public:

   BOOLEAN operator < (

        const TIME_OF_DAY& time);

/*-------------------------------------------------------------------------
   Source: "op_lteq1.cpp"
  -------------------------------------------------------------------------*/

public:

   BOOLEAN operator <= (

        const TIME_OF_DAY& time);

/*-------------------------------------------------------------------------
   Source: "op_neq1.cpp"
  -------------------------------------------------------------------------*/

public:

   BOOLEAN operator != (

        const TIME_OF_DAY& time);

/*-------------------------------------------------------------------------
   Source: "Second1.cpp"
  -------------------------------------------------------------------------*/

public:

   UINT32 Second_Count();

/*-------------------------------------------------------------------------
   Source: "Set1.cpp"
  -------------------------------------------------------------------------*/

public:

   BOOLEAN Set(

        const int hour,
        const int minute,
        const int second = 0,
        const int millisecond = 0);

/*-------------------------------------------------------------------------
   Source: "Set2.cpp"
  -------------------------------------------------------------------------*/

public:

   BOOLEAN Set(

        const TIME_OF_DAY& time);       // time of day to set

/*-------------------------------------------------------------------------
   Source: "Set3.cpp"
  -------------------------------------------------------------------------*/

public:

   BOOLEAN Set(

        const UINT32 to_seconds_into_day,  // seconds since 00:00:00
        const UINT32 with_milliseconds = 0);

/*-------------------------------------------------------------------------
   Source: "Set_By_1.cpp"
  -------------------------------------------------------------------------*/

public:

   BOOLEAN Set_By_Millisecond(

        const UINT32 to_milliseconds_into_day);  // ms since 00:00:00

/*-------------------------------------------------------------------------
   Source: "Set_Sys1.cpp"
  -------------------------------------------------------------------------*/

public:

   void Set_System_Time();

/*-------------------------------------------------------------------------
   Source: "TIME_OF1.cpp"
  -------------------------------------------------------------------------*/

public:

   TIME_OF_DAY();

/*-------------------------------------------------------------------------
   Source: "TIME_OF2.cpp"
  -------------------------------------------------------------------------*/

public:

   TIME_OF_DAY(

        const int hour,
        const int minute,
        const int second = 0,
        const int millisecond = 0);

};   //class TIME_OF_DAY


#endif
