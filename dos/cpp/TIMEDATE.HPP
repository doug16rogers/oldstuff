#ifndef __timedate_hpp
#define __timedate_hpp

#include <ttypes.h>     // from Turbo Vision include files


#include "date.hpp"     // to hold date portion
#include "timeoday.hpp" // to hold time portion

/*****************************************************************************
*
*  TITLE:       Time and Date
*
*  DESCRIPTION: The "Time_Date" class provides operations for time and date.
*
*  *k "%n"
*  FILE NAME: "TIMEDATE.HPP"
*
*  *k "%v"
*  VERSION:   "2"
*
*****************************************************************************/

class Time_Date
{

//===========================================================================
//
//  Class data
//
//---------------------------------------------------------------------------

public:

  Date date;            //date component

  Time_Of_Day time;     //time component

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

  }   //Time_Date:: Decrement(int by_seconds)

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

    date.Get_System_Date();
    time.Get_System_Time();

  }   //Time_Date:: Get_System(void)

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

  }   //Time_Date:: Increment(int by_seconds)

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

  Boolean operator < (Time_Date& time_date)
  {

    return (Boolean) ((date == time_date.date) ?
                      (time < time_date.time) :
                      (date < time_date.date));

  }   //Time_Date:: operator < (Time_Date& time_date)

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

  Boolean operator == (Time_Date& time_date)
  {

    return (Boolean) ((date == time_date.date) &&
                      (time == time_date.time));

  }   //Time_Date:: operator == (Time_Date& time_date)

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

  Boolean operator > (Time_Date& time_date)
  {

    return (Boolean) (!(*this == time_date) &&
                      !(*this < time_date));

  }   //Time_Date:: operator > (Time_Date& time_date)

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

  Boolean operator <= (Time_Date& time_date)
  {

    return (Boolean) ((*this < time_date) ||
                      (*this == time_date));

  }   //Time_Date:: operator <= (Time_Date& time_date)

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

  Boolean operator != (Time_Date& time_date)
  {

    return (Boolean) (!(*this == time_date));

  }   //Time_Date:: operator != (Time_Date& time_date)

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

  Boolean operator >= (Time_Date& time_date)
  {

    return (Boolean) (!(*this < time_date));

  }   //Time_Date:: operator >= (Time_Date& time_date)

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
    Date& _date,        //date to set
    Time_Of_Day& _time) //time to set
  {

    date.Set(_date);
    time.Set(_time);

  }   //Time_Date:: Set(Date&, Time_Of_Day&)

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

    date.Set_System_Date();
    time.Set_System_Time();

  }   //Time_Date:: Set_System(void)

/*****************************************************************************
*
*  TITLE:       Time Date
*
*  DESCRIPTION: The "Time_Date" default constructor sets the date to
*               1980.01.01 and the time to 00:00:00.
*
*  REFERENCE:   None.
*
*****************************************************************************/

  Time_Date(void)
  {

    date.Set(1980, Date::January, 1);
    time.Set(0, 0, 0);

  }   //Time_Date:: Time_Date(void)

/*****************************************************************************
*
*  TITLE:       Time Date
*
*  DESCRIPTION: The "Time_Date" constructor sets the date and time to the
*               values provided.
*
*  REFERENCE:   None.
*
*****************************************************************************/

  Time_Date(
    Date& _date,        //date to set
    Time_Of_Day& _time) //time to set
  {

    date.Set(1980, Date::January, 1);
    time.Set(0, 0, 0);

    Set(_date, _time);

  }   //Time_Date:: Time_Date(Date&, Time_Of_Day&)

/*****************************************************************************
*
*  TITLE:       Time Date
*
*  DESCRIPTION: The "Time_Date" constructor sets the time and date to the
*               value provided.
*
*  REFERENCE:   None.
*
*****************************************************************************/

  Time_Date(
    Time_Date& time_date)
  {

    *this = time_date;

  }   //Time_Date:: Time_Date(Time_Date&)

};   //class Time_Date


#endif //__timedate_hpp

