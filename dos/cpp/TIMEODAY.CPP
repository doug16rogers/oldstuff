#include <ctype.h>
#include <dos.h>
#include <stdio.h>

#include "timeoday.hpp"

/*****************************************************************************
*
*  TITLE:       Time Of Day
*
*  DESCRIPTION: The class "Time_Of_Day" maintains time-of-day information:
*               hours, minutes and seconds.
*
*  *k "%n"
*  FILE NAME: "TIMEODAY.CPP"
*
*  *k "%v"
*  VERSION:   "2"
*
*****************************************************************************/

//===========================================================================
//
//  Class data
//
//---------------------------------------------------------------------------

  char* Time_Of_Day:: default_format = "%02h:%02m:%02s";

  char Time_Of_Day:: local_image[0x80] = "";

/*****************************************************************************
*
*  TITLE:       Decrement
*
*****************************************************************************/

  int Time_Of_Day:: Decrement(  //number of days (+1/-1/0) changed after dec

    int by_seconds)             //number of seconds (+/-) to decrement by

  {

    return Increment(-by_seconds);

  }   //Time_Of_Day:: Decrement(int by_seconds)

/*****************************************************************************
*
*  TITLE:       Get System Time
*
*****************************************************************************/

  void Time_Of_Day:: Get_System_Time(void)

  {

    struct time system_time;

    gettime(&system_time);
    Set(system_time.ti_hour, system_time.ti_min, system_time.ti_sec);

  }   //Time_Of_Day:: Get_System_Time(void)

/*****************************************************************************
*
*  TITLE:       Image
*
*****************************************************************************/

  char* Time_Of_Day:: Image(    //buffer pointer as passed in

    char* into_buffer,                  //buffer to hold the image
    char* with_format)                  //most recently used format string

  {

    if (into_buffer == NULL)
    {
      into_buffer = local_image;
    }

    if (with_format == NULL)
    {
      with_format = last_format;
      if (with_format == NULL)
      {
        with_format = default_format;
      }
    }

    last_format = with_format;

    char* format = with_format;
    char* buffer = into_buffer;

    Boolean left_justify = False;
    Boolean zero_pad = False;
    static char* format_using[2] = { "%*u", "%0*u" };
    static char* long_format_using[2] = { "%*lu", "%0*lu" };

    int width = 0;

    while (*format != 0)
    {
      if (*format != '%')
      {
        *buffer++ = *format++;
        continue; //------------------------> continue
      }

      format++;

      left_justify = False;
      zero_pad = False;
      width = 0;

      if (*format == '-')
      {
        left_justify = True;
        format++;
      }

      if (*format == '0')
      {
        zero_pad = True;
        format++;
      }

      while (isdigit(*format))
      {
        width = (10 * width) + (*format - '0');
        format++;
      }

      if (left_justify)
      {
        width = -width;
      }

      switch (*format++)
      {
        case '%':
          *buffer++ = '%';
          break;

        case 'a':
        case 'p':
          if (hour < 12)
          {
            *buffer++ = 'a';
          }
          else
          {
            *buffer++ = 'p';
          }
          break;

        case 'A':
        case 'P':
          if (hour < 12)
          {
            *buffer++ = 'a';
          }
          else
          {
            *buffer++ = 'p';
          }
          break;

        case 'c':
          sprintf(buffer, long_format_using[zero_pad], width, Second_Count());
          while (*buffer) buffer++;
          break;

        case 'h':
          sprintf(buffer, format_using[zero_pad], width, (int) hour);
          while (*buffer) buffer++;
          break;

        case 'H':
          if (hour > 12)
          {
            sprintf(buffer, format_using[zero_pad], width, (int) hour - 12);
          }
          else
          {
            sprintf(buffer, format_using[zero_pad], width, (int) hour);
          }
          while (*buffer) buffer++;
          break;

        case 'm':
          sprintf(buffer, format_using[zero_pad], width, (int) minute);
          while (*buffer) buffer++;
          break;

        case 's':
          sprintf(buffer, format_using[zero_pad], width, (int) second);
          while (*buffer) buffer++;
          break;

        default:
          format--;
          *buffer++ = *format++;
      }   //switch
    }   //while

    *buffer = 0;

    return into_buffer;

  }   //Time_Of_Day:: Image

/*****************************************************************************
*
*  TITLE:       Increment
*
*****************************************************************************/

  int Time_Of_Day:: Increment(  //number of days (+1/-1/0) changed after inc

    int by_seconds)             //number of seconds (+/-) to increment by

  {
    int delta_day = 0;

    long seconds_into_day = Second_Count();

    seconds_into_day = seconds_into_day + (long) by_seconds;

    if (seconds_into_day < 0L)
    {
      seconds_into_day = seconds_into_day + seconds_per_day;
      delta_day = -1;
    }
    else if (seconds_into_day >= seconds_per_day)
    {
      seconds_into_day = seconds_into_day - seconds_per_day;
      delta_day = +1;
    }

    Set(seconds_into_day);

    return delta_day;

  }   //Time_Of_Day:: Increment(int by_seconds)

/*****************************************************************************
*
*  TITLE:       Is_Valid
*
*****************************************************************************/

  Boolean Time_Of_Day:: Is_Valid(

    uchar for_hour,
    uchar for_minute,
    uchar for_second)       

  {

    Boolean time_is_valid = True;

    if (time_is_valid && (for_hour > 23)) time_is_valid = False;
    if (time_is_valid && (for_minute > 59)) time_is_valid = False;
    if (time_is_valid && (for_second > 59)) time_is_valid = False;

    return time_is_valid;

  }   //Time_Of_Day:: Is_Valid

/*****************************************************************************
*
*  TITLE:       operator <
*
*****************************************************************************/

  Boolean Time_Of_Day:: operator < (Time_Of_Day& time)

  {

    if (hour < time.hour) return True;
    if (hour > time.hour) return False;

    if (minute < time.minute) return True;
    if (minute > time.minute) return False;

    if (second < time.second) return True;
    return False;

  }   //Time_Of_Day:: operator < (Time_Of_Day&)

/*****************************************************************************
*
*  TITLE:       Second_Count
*
*****************************************************************************/

  long Time_Of_Day:: Second_Count(void)

  {

    return (seconds_per_minute * minutes_per_hour * (long) hour) +
           (seconds_per_minute * (long) minute) +
           (long) second;

  }   //Time_Of_Day:: Second_Count(void)

/*****************************************************************************
*
*  TITLE:       Set
*
*****************************************************************************/

  void Time_Of_Day:: Set(

    int _hour,
    int _minute,
    int _second)

  {

    if (Is_Valid((uchar) _hour, (uchar) _minute, (uchar) _second))
    {
      hour = (uchar) _hour;
      minute = (uchar) _minute;
      second = (uchar) _second;
    }

  }   //Time_Of_Day:: Set

/*****************************************************************************
*
*  TITLE:       Set
*
*****************************************************************************/

  void Time_Of_Day:: Set(

    Time_Of_Day& time)          //time of day to set

  {

    Set(time.hour, time.minute, time.second);

  }   //Time_Of_Day:: Set(Time_Of_Day&)

/*****************************************************************************
*
*  TITLE:       Set
*
*****************************************************************************/

  void Time_Of_Day:: Set(

    long to_seconds_into_day)   //number of seconds since 00:00:00

  {

    if ((to_seconds_into_day >= 0L) &&
        (to_seconds_into_day < seconds_per_day))
    {
      second = to_seconds_into_day % seconds_per_hour;

      long minutes_into_day = to_seconds_into_day / seconds_per_hour;
      minute = minutes_into_day % minutes_per_hour;

      hour = minutes_into_day / minutes_per_hour;
    }

  }   //Time_Of_Day:: Set

/*****************************************************************************
*
*  TITLE:       Set System Time
*
*****************************************************************************/

  void Time_Of_Day:: Set_System_Time(void)

  {

    struct time system_time;

    system_time.ti_hour = hour;
    system_time.ti_min  = minute;
    system_time.ti_sec  = second;
    settime(&system_time);

  }   //Time_Of_Day:: Set_System_Time(void)

/*****************************************************************************
*
*  TITLE:       Time Of Day
*
*****************************************************************************/

  Time_Of_Day:: Time_Of_Day(void)

  {

    last_format = default_format;
    Set(0, 0, 0);

  }   //Time_Of_Day:: Time_Of_Day(void)

/*****************************************************************************
*
*  TITLE:       Time Of Day
*
*****************************************************************************/

  Time_Of_Day:: Time_Of_Day(

    int _hour,
    int _minute,
    int _second)

  {

    last_format = default_format;
    Set(0, 0, 0);
    Set(_hour, _minute, _second);

  }   //Time_Of_Day:: Time_Of_Day(int _hour,int _minute,int _second)

