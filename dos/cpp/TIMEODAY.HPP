#ifndef __timeoday_hpp
#define __timeoday_hpp

#include <ttypes.h>     // from Turbo Vision include files


//===========================================================================
//
//  Helpful constants
//
//---------------------------------------------------------------------------

const long hours_per_day = 24L;
const long minutes_per_hour = 60L;
const long seconds_per_minute = 60L;
const long seconds_per_hour = seconds_per_minute * minutes_per_hour;
const long minutes_per_day = minutes_per_hour * hours_per_day;
const long seconds_per_day = seconds_per_minute * minutes_per_day;

/*****************************************************************************
*
*  TITLE:       Time Of Day
*
*  DESCRIPTION: The class "Time_Of_Day" maintains time-of-day information:
*               hours, minutes and seconds.
*
*  *k "%n"
*  FILE NAME: "TIMEODAY.HPP"
*
*  *k "%v"
*  VERSION:   "2"
*
*****************************************************************************/

class Time_Of_Day
{

//===========================================================================
//
//  Class data
//
//---------------------------------------------------------------------------


private:

  static char* default_format;          //00:00:00 .. 23:59:59

  static char local_image[0x80];        //for holding direct image

public:

  char* last_format;            //holds on to last format used

public:

  uchar hour;                   //this is the time of day...
  uchar minute;
  uchar second;

//===========================================================================
//
//  Class methods
//
//---------------------------------------------------------------------------

/*****************************************************************************
*
*  TITLE:       Decrement
*
*  DESCRIPTION: The "Time_Of_Day:: Decrement" procedure decrements the time of
*               day by the indicated number of seconds.  It returns the change
*               in days created by the decrementation (-1 if the new time
*               dropped below 00:00:00, 0 if the time did not wrap, and +1 if
*               the time went over 23:59:59).
*
*  REFERENCE:   None.
*
*****************************************************************************/

public:

  int Decrement(                //number of days (+1/-1/0) changed after dec

    int by_seconds = 1);        //number of seconds (+/-) to decrement by

/*****************************************************************************
*
*  TITLE:       Get System Time
*
*  DESCRIPTION: The procedure "Get_System_Time" loads the time with the system
*               time.
*
*  REFERENCE:   None.
*
*****************************************************************************/

public:

  virtual void Get_System_Time(void);

/*****************************************************************************
*
*  TITLE:       Image
*
*  DESCRIPTION: The function "Time_Of_Day:: Image" formats the time according
*               to the format string provided.  It returns the buffer provided
*               to it for storing the formatted time.
*
*               The format string contains '%'-sequences:
*
*                 %[-][0][<width>]<control>
*
*               The optional [-] causes left justification.
*               The optional [0] causes zero-padding of the field.
*               The optional [<width>] specifies the width of the field.
*               The <control> character may be:
*
*                 '%'          insert the '%' character
*                 'a' or 'p'   insert 'a'/'p' according to am/pm time
*                 'A' or 'P'   insert 'A'/'P' according to am/pm time
*                 'c'          insert the second count since 00:00:00
*                 'h'          insert the hour
*                 'H'          insert the hour according to am/pm time
*                 'm'          insert the minute
*                 's'          insert the second
*
*               The default format string is "%02h:%02m:%02s".
*               For am/pm time use "%H:%02m:%02s %am".
*
*  REFERENCE:   None.
*
*****************************************************************************/

public:

  char* Image(                  //buffer as passed in

    char* into_buffer = NULL,   //buffer to hold the image
    char* with_format = NULL);  //NULL means last format used for object

/*****************************************************************************
*
*  TITLE:       Increment
*
*  DESCRIPTION: The "Time_Of_Day:: Increment" procedure increments the time of
*               day by the indicated number of seconds.  It returns the change
*               in days created by the incrementation (-1 if the new time
*               dropped below 00:00:00, 0 if the time did not wrap, and +1 if
*               the time went over 23:59:59).
*
*  REFERENCE:   None.
*
*****************************************************************************/

public:

  int Increment(                //number of days (+1/-1/0) changed after inc

    int by_seconds = 1);        //number of seconds (+/-) to increment by

/*****************************************************************************
*
*  TITLE:       Is_Valid
*
*  DESCRIPTION: The function "Time_Of_Day:: Is_Valid" checks the given
*               hour, minute and second to check that they comprise a valid
*               time of day.
*
*  REFERENCE:   None.
*
*****************************************************************************/

public:

  Boolean Is_Valid(

    uchar for_hour,
    uchar for_minute,
    uchar for_second = 0);

/*****************************************************************************
*
*  TITLE:       operator <
*
*  DESCRIPTION: The "operator <" function returns True if the left time is
*               less than the right time; otherwise, it returns False.
*
*  REFERENCE:   None.
*
*****************************************************************************/

public:

  Boolean operator < (Time_Of_Day& time);

/*****************************************************************************
*
*  TITLE:       operator ==
*
*  DESCRIPTION: The "operator ==" function returns True if the left time is
*               equal to the right time; otherwise, it returns False.
*
*  REFERENCE:   None.
*
*****************************************************************************/

public:

  Boolean operator == (Time_Of_Day& time)
  {

    return (Boolean) ((hour == time.hour) &&
                      (minute == time.minute) &&
                      (second == time.second));

  }   //Time_Of_Day:: operator == (Time_Of_Day& time)

/*****************************************************************************
*
*  TITLE:       operator >
*
*  DESCRIPTION: The "operator >" function returns True if the left time is
*               greater than the right time; otherwise, it returns False.
*
*  REFERENCE:   None.
*
*****************************************************************************/

public:

  Boolean operator > (Time_Of_Day& time)
  {

    return (Boolean) (!(*this == time) &&
                      !(*this < time));

  }   //Time_Of_Day:: operator > (Time_Of_Day& time)

/*****************************************************************************
*
*  TITLE:       operator <=
*
*  DESCRIPTION: The "operator <=" function returns True if the left time is
*               less than or equal to the right time; otherwise, it returns
*               False.
*
*  REFERENCE:   None.
*
*****************************************************************************/

public:

  Boolean operator <= (Time_Of_Day& time)
  {

    return (Boolean) ((*this < time) ||
                      (*this == time));

  }   //Time_Of_Day:: operator <= (Time_Of_Day& time)

/*****************************************************************************
*
*  TITLE:       operator !=
*
*  DESCRIPTION: The "operator !=" function returns True if the left time is
*               not equal to the right time; otherwise, it returns False.
*
*  REFERENCE:   None.
*
*****************************************************************************/

public:

  Boolean operator != (Time_Of_Day& time)
  {

    return (Boolean) !(*this == time);

  }   //Time_Of_Day:: operator != (Time_Of_Day& time)

/*****************************************************************************
*
*  TITLE:       operator >=
*
*  DESCRIPTION: The "operator >=" function returns True if the left time is
*               greater than or equal to the right time; otherwise, it returns
*               False.
*
*  REFERENCE:   None.
*
*****************************************************************************/

public:

  Boolean operator >= (Time_Of_Day& time)
  {

    return (Boolean) !(*this < time);

  }   //Time_Of_Day:: operator >= (Time_Of_Day& time)

/*****************************************************************************
*
*  TITLE:       Second_Count
*
*  DESCRIPTION: The function "Time_Of_Day:: Second_Count" returns the number
*               of seconds since 00:00:00.
*
*  REFERENCE:   None.
*
*****************************************************************************/

public:

  long Second_Count(void);

/*****************************************************************************
*
*  TITLE:       Set
*
*  DESCRIPTION: This "Set" procedure sets the full time.
*
*  REFERENCE:   None.
*
*****************************************************************************/

public:

  void Set(

    int _hour,
    int _minute,
    int _second = 0);

/*****************************************************************************
*
*  TITLE:       Set
*
*  DESCRIPTION: This "Set" procedure sets the time according to the given
*               time-of-day.
*
*  REFERENCE:   None.
*
*****************************************************************************/

public:

  void Set(

    Time_Of_Day& time);         //time of day to set

/*****************************************************************************
*
*  TITLE:       Set
*
*  DESCRIPTION: This "Set" procedure sets the time according to the given
*               number of seconds into the day (since 00:00:00).
*
*  REFERENCE:   None.
*
*****************************************************************************/

public:

  void Set(

    long to_seconds_into_day);  //number of seconds since 00:00:00

/*****************************************************************************
*
*  TITLE:       Set System Time
*
*  DESCRIPTION: The procedure "Set_System_Time" sets the system time to the
*               object's time.
*
*  REFERENCE:   None.
*
*****************************************************************************/

public:

  void Set_System_Time(void);

/*****************************************************************************
*
*  TITLE:       Time Of Day
*
*  DESCRIPTION: The default constructor "Time_Of_Day:: Time_Of_Day"
*               initializes the time to 00:00:00.
*
*  REFERENCE:   None.
*
*****************************************************************************/

public:

  Time_Of_Day(void);

/*****************************************************************************
*
*  TITLE:       Time Of Day
*
*  DESCRIPTION: The constructor "Time_Of_Day:: Time_Of_Day" initializes the
*               time to hour, minute and second provided.
*
*  REFERENCE:   None.
*
*****************************************************************************/

public:

  Time_Of_Day(

    int _hour,
    int _minute,
    int _second = 0);

};   //class Time_Of_Day


#endif
