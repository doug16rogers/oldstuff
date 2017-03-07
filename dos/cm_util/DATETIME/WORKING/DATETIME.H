#ifndef DATE_TIME_CLASS
#define DATE_TIME_CLASS
/*****************************************************************************
*
*  TITLE:       Date and Time
*
*  DESCRIPTION: The "DATE_TIME" class provides operations for time and date.
*
*  *k "%n"
*  FILE NAME:   ""
*
*  *k "%v"
*  VERSION:     ""
*
*****************************************************************************/

#include "date.h"       // to hold date portion
#include "timeoday.h"   // to hold time portion


class DATE_TIME
{

//===========================================================================
//
//  Class data
//
//---------------------------------------------------------------------------

public:

  DATE date;            //date component

  TIME_OF_DAY time;     //time component

/*****************************************************************************
*
*  TITLE:       Decrement
*
*  DESCRIPTION: The "Decrement" procedure decrements the time of day by the
*               given number of seconds, and adjusts the date if necessary.
*
*  REFERENCE:   None.
*
*****************************************************************************/

  void Decrement(

    int by_seconds)

  {

    Increment(-by_seconds);

  }   //DATE_TIME:: Decrement(int by_seconds)

/*****************************************************************************
*
*  TITLE:       Get System
*
*  DESCRIPTION: The "Get_System" procedure gets the time and date from the
*               system.
*
*  REFERENCE:   None.
*
*****************************************************************************/

  void Get_System(void)
  {

    date.Get_System_DATE();
    time.Get_System_Time();

  }   //DATE_TIME:: Get_System(void)

/*****************************************************************************
*
*  TITLE:       Increment
*
*  DESCRIPTION: The "Increment" procedure increments the time of day by the
*               given number of seconds, and adjusts the date if necessary.
*
*  REFERENCE:   None.
*
*****************************************************************************/

  void Increment(

    int by_seconds)

  {

    int days_wrapped;

    days_wrapped = time.Increment(by_seconds);

    if (days_wrapped != 0)
    {
      date.Increment(days_wrapped);
    }

  }   //DATE_TIME:: Increment(int by_seconds)

/*****************************************************************************
*
*  TITLE:       operator <
*
*  DESCRIPTION: The "operator <" function returns true if the left time is
*               less than the right time; otherwise, it returns false.
*
*  REFERENCE:   None.
*
*****************************************************************************/

  BOOLEAN operator < (DATE_TIME& date_time)
  {

    return (BOOLEAN) ((date == date_time.date) ?
                      (time < date_time.time) :
                      (date < date_time.date));

  }   //DATE_TIME:: operator < (DATE_TIME& date_time)

/*****************************************************************************
*
*  TITLE:       operator ==
*
*  DESCRIPTION: The "operator ==" function returns true if the left time is
*               equal to the right time; otherwise, it returns false.
*
*  REFERENCE:   None.
*
*****************************************************************************/

  BOOLEAN operator == (DATE_TIME& date_time)
  {

    return (BOOLEAN) ((date == date_time.date) &&
                      (time == date_time.time));

  }   //DATE_TIME:: operator == (DATE_TIME& date_time)

/*****************************************************************************
*
*  TITLE:       operator >
*
*  DESCRIPTION: The "operator >" function returns true if the left time is
*               greater than the right time; otherwise, it returns false.
*
*  REFERENCE:   None.
*
*****************************************************************************/

  BOOLEAN operator > (DATE_TIME& date_time)
  {

    return (BOOLEAN) (!(*this == date_time) &&
                      !(*this < date_time));

  }   //DATE_TIME:: operator > (DATE_TIME& date_time)

/*****************************************************************************
*
*  TITLE:       operator <=
*
*  DESCRIPTION: The "operator <=" function returns true if the left time is
*               less than or equal to the right time; otherwise, it returns
*               false.
*
*  REFERENCE:   None.
*
*****************************************************************************/

  BOOLEAN operator <= (DATE_TIME& date_time)
  {

    return (BOOLEAN) ((*this < date_time) ||
                      (*this == date_time));

  }   //DATE_TIME:: operator <= (DATE_TIME& date_time)

/*****************************************************************************
*
*  TITLE:       operator !=
*
*  DESCRIPTION: The "operator !=" function returns true if the left time is
*               not equal to the right time; otherwise, it returns false.
*
*  REFERENCE:   None.
*
*****************************************************************************/

  BOOLEAN operator != (DATE_TIME& date_time)
  {

    return (BOOLEAN) (!(*this == date_time));

  }   //DATE_TIME:: operator != (DATE_TIME& date_time)

/*****************************************************************************
*
*  TITLE:       operator >=
*
*  DESCRIPTION: The "operator >=" function returns true if the left time is
*               greater than or equal to the right time; otherwise, it returns
*               false.
*
*  REFERENCE:   None.
*
*****************************************************************************/

  BOOLEAN operator >= (DATE_TIME& date_time)
  {

    return (BOOLEAN) (!(*this < date_time));

  }   //DATE_TIME:: operator >= (DATE_TIME& date_time)

/*****************************************************************************
*
*  TITLE:       Set
*
*  DESCRIPTION: The "Set" procedure sets the date and time to the
*               values provided.
*
*  REFERENCE:   None.
*
*****************************************************************************/

  void Set(
    DATE& _date,        //date to set
    TIME_OF_DAY& _time) //time to set
  {

    date.Set(_date);
    time.Set(_time);

  }   //DATE_TIME:: Set(DATE&, TIME_OF_DAY&)

/*****************************************************************************
*
*  TITLE:       Set System
*
*  DESCRIPTION: The "Set_System" procedure sets the time and date of the
*               system to those in the object.
*
*  REFERENCE:   None.
*
*****************************************************************************/

  void Set_System(void)
  {

    date.Set_System_DATE();
    time.Set_System_Time();

  }   //DATE_TIME:: Set_System(void)

/*****************************************************************************
*
*  TITLE:       Time DATE
*
*  DESCRIPTION: The "DATE_TIME" default constructor sets the date to
*               1980.01.01 and the time to 00:00:00.
*
*  REFERENCE:   None.
*
*****************************************************************************/

  DATE_TIME(void)
  {

    date.Set(1980, DATE::January, 1);
    time.Set(0, 0, 0);

  }   //DATE_TIME:: DATE_TIME(void)

/*****************************************************************************
*
*  TITLE:       Time DATE
*
*  DESCRIPTION: The "DATE_TIME" constructor sets the date and time to the
*               values provided.
*
*  REFERENCE:   None.
*
*****************************************************************************/

  DATE_TIME(
    DATE& _date,        //date to set
    TIME_OF_DAY& _time) //time to set
  {

    date.Set(1980, DATE::January, 1);
    time.Set(0, 0, 0);

    Set(_date, _time);

  }   //DATE_TIME:: DATE_TIME(DATE&, TIME_OF_DAY&)

/*****************************************************************************
*
*  TITLE:       Time DATE
*
*  DESCRIPTION: The "DATE_TIME" constructor sets the time and date to the
*               value provided.
*
*  REFERENCE:   None.
*
*****************************************************************************/

  DATE_TIME(
    DATE_TIME& date_time)
  {

    *this = date_time;

  }   //DATE_TIME:: DATE_TIME(DATE_TIME&)

};   //class DATE_TIME


#endif //__timedate_hpp

