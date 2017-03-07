#include <ctype.h>
#include <dos.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#include "date.hpp"


/*****************************************************************************
*
*  TITLE:       Date
*
*  DESCRIPTION: The "Date" class provides operations for dates.
*
*  *k "%n"
*  FILE NAME: "DATE.CPP"
*
*  *k "%v"
*  VERSION:   "2"
*
*****************************************************************************/


//===========================================================================
//
//  Constants
//
//---------------------------------------------------------------------------

static const int days_per_year[2] = { 365, 366 };

static const uchar days_in_month[2][13] =   //month 0 is not used
     {
       { 0,   31, 28, 31,   30, 31, 30,   31, 31, 30,   31, 30, 31 },
       { 0,   31, 29, 31,   30, 31, 30,   31, 31, 30,   31, 30, 31 }
     };

static const ushort days_to_month[2][13] =   //days in year to start of month
     {
       { 0,  0, 31, 59,  90, 120, 131,  181, 212, 243,  273, 304, 334 },
       { 0,  0, 31, 60,  91, 121, 132,  182, 213, 244,  274, 305, 335 }
     };

//===========================================================================
//
//  Class data
//
//---------------------------------------------------------------------------

  char* Date:: default_format = "%04y.%02m.%02d";       //1980.01.01

  char Date:: local_image[0x80] = "";   //for holding local image

  char* Date:: name_of_month[13] =      //month names, indexed by
  {                                     //  Months_Of_Year
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

  char* Date:: name_of_day[7] =         //day-of-week names, indexed by
  {                                     //  Days_Of_Week
    "Sunday",
    "Monday",
    "Tuesday",
    "Wednesday",
    "Thursday",
    "Friday",
    "Saturday"
  };

//===========================================================================
//
//  Class methods
//
//---------------------------------------------------------------------------

/*****************************************************************************
*
*  TITLE:       Date
*
*****************************************************************************/

  Date:: Date(void)

  {

    last_format = default_format;
    Set(1980, January, 1);

  }   //Date:: Date(void)

/*****************************************************************************
*
*  TITLE:       Date
*
*****************************************************************************/

  Date:: Date(

    int _year,
    int _month,
    int _day)

  {

    last_format = default_format;
    Set(1980, January, 1);
    Set(_year, _month, _day);

  }   //Date:: Date(void)

/*****************************************************************************
*
*  TITLE:       Day Of Week
*
*****************************************************************************/

  Date::Days_Of_Week Date:: Day_Of_Week(void)
  {
    static const int offset_into_week_for[13] =  //offset of month into week
    {
      0,                //this month (0) not used
      0, 3, 3, 6,       //January, February, March, April
      1, 4, 6, 2,       //May, June, July, August
      5, 0, 3, 5        //September, October, November, December
    };

    int day_of_week;    //day of week -- algorithm will keep it >= 0 thru-out
    int year_mod_400;

    year_mod_400 = year % 400;          //year into 400yr cycle

    day_of_week  = (int) Saturday;      //400 yr calendar starts on Saturday
    day_of_week += year_mod_400;        //add in 1 day of week per year
    day_of_week += year_mod_400/4;      //add in an extra for leap years
    day_of_week -= year_mod_400/100;    //no leap year on centuries
    day_of_week += offset_into_week_for[month];  //add in offset for month
    day_of_week += day;                 //add in day of month

    if (Is_Leap_Year(year) && (month <= February))
    {
      day_of_week--;                    //sub extra leapyear added
    }

    day_of_week %= 7;                   //get day of week

    return (Days_Of_Week) day_of_week;  //yep, all ok

  }   //Date:: Day_Of_Week(void)

/*****************************************************************************
*
*  TITLE:       Decrement
*
*****************************************************************************/

  void Date:: Decrement(

    int by_days)                //number of days to decrement by

  {

    Increment(-by_days);

  }   //Date:: Decrement

/*****************************************************************************
*
*  TITLE:       Get System Date
*
*****************************************************************************/

  void Date:: Get_System_Date(void)
  {
    date dos_date;

    getdate(&dos_date);

    year = dos_date.da_year;
    month = dos_date.da_mon;
    day = dos_date.da_day;

  }   //Date:: Get_System_Date

/*****************************************************************************
*
*  TITLE:       Image
*
*****************************************************************************/

  char* Date:: Image(                   //buffer as passed in

    char* into_buffer,                  //buffer to hold the image
    char* with_format)                  //format string

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

  //forward declaration of function...

    void Write_String_To_Output(
      char** output,            //output string pointer
      char* string_to_write,    //string to write to output
      int width,                //desired width (negative for left-justify)
      Boolean to_upper_case,    //force string to upper case
      Boolean to_lower_case);   //force string to lower case

    char* output = into_buffer;         //our copy of output buffer

    Boolean left_justify = False;
    Boolean zero_pad = False;
    Boolean force_upper_case = False;
    Boolean force_lower_case = False;
    static char* format_using[2] = { "%*u", "%0*u" };

    int width = 0;

    while (*with_format)
    {
      left_justify = False;
      zero_pad = False;
      force_upper_case = False;
      force_lower_case = False;
      width = 0;

      if (*with_format != '%')
      {
        *output++ = *with_format++;
        continue; //--------------------------> continue
      }
      with_format++;

      if (*with_format == '-')
      {
        left_justify = True;
        with_format++;
      }

      if (*with_format == '0')
      {
        zero_pad = True;
        with_format++;
      }

      while (isdigit(*with_format))
      {
        width = (10 * width) + (*with_format - '0');
        with_format++;
      }

      if (left_justify)
      {
        width = -width;
      }

      if (*with_format == 'u')
      {
        force_upper_case = True;
        with_format++;
      }
      else if (*with_format == 'l')
      {
        force_lower_case = True;
        with_format++;
      }

      switch (*with_format++)
      {
        case '%':
          *output++ = '%';
          break;

        case 'd':
          sprintf(output, format_using[zero_pad], width, (int) day);
          while (*output) output++;
          break;

        case 'j':
          sprintf(output, format_using[zero_pad], width, Julian_Day());
          while (*output) output++;
          break;

        case 'm':
          sprintf(output, format_using[zero_pad], width, (int) month);
          while (*output) output++;
          break;

        case 'M':
          Write_String_To_Output(&output, name_of_month[month], width,
                                 force_upper_case, force_lower_case);
          break;

        case 'w':
          sprintf(output, format_using[zero_pad], width, (int) Day_Of_Week());
          while (*output) output++;
          break;

        case 'W':
          Write_String_To_Output(&output, name_of_day[Day_Of_Week()], width,
                                 force_upper_case, force_lower_case);
          break;

        case 'y':
          sprintf(output, format_using[zero_pad], width, year);
          while (*output) output++;
          break;

        case 'Y':
          sprintf(output, format_using[zero_pad], width, year % 100);
          while (*output) output++;
          break;

        default:
          with_format--;
          *output++ = *with_format++;
          break;

      }   //switch
    }

    *output = 0;                //terminate output string

    return into_buffer;         //return output string

  }   //Date:: Image(char* into_buffer,char* with_format)

/*****************************************************************************
*
*  TITLE:       Increment
*
*****************************************************************************/

  void Date:: Increment(

    int by_days)                //number of days to increment by

  {

    if ((by_days >  days_per_year[0]) ||
        (by_days < -days_per_year[0]))
    {
      return; //------------------------------> return!
    }

    int day_of_year = Julian_Day() + by_days;

    if (day_of_year < 1)
    {
      year--;
      day_of_year = day_of_year + days_per_year[Is_Leap_Year(year)];
    }
    else
    {
      int days_this_year = days_per_year[Is_Leap_Year(year)];

      if (day_of_year > days_this_year)
      {
        day_of_year = day_of_year - days_this_year;
        year++;
      }
    }

    Set(day_of_year, year);

  }   //Date:: Increment

/*****************************************************************************
*
*  TITLE:       Is Leap Year
*
*****************************************************************************/

  Boolean Date:: Is_Leap_Year(

    ushort for_year)              //year to check

  {

     Boolean is_leap_year = True;

     if (for_year == 0)
     {
        for_year = this->year;
     }

     if      ((for_year %   4) != 0) is_leap_year = False;
     else if ((for_year % 100) != 0) is_leap_year = True;
     else if ((for_year % 400) != 0) is_leap_year = False;

     return is_leap_year;

  }   //Date:: Is_Leap_Year(ushort for_year)

/*****************************************************************************
*
*  TITLE:       Is Valid
*
*****************************************************************************/

  Boolean Date:: Is_Valid(

    ushort for_year,
    uchar for_month,
    uchar for_day)

  {
    Boolean date_is_valid = True;
    Boolean on_leap_year = Is_Leap_Year(for_year);

//  if (date_is_valid && (for_year < 0)) date_is_valid = False;

    if (date_is_valid && (for_month < January)) date_is_valid = False;
    if (date_is_valid && (for_month > December)) date_is_valid = False;

    if (date_is_valid && (for_day < 1)) date_is_valid = False;
    if (for_day > days_in_month[on_leap_year][for_month])
    {
      date_is_valid = False;
    }

    return date_is_valid;

  }   //Date:: Is_Valid

/*****************************************************************************
*
*  TITLE:       Julian Day
*
*****************************************************************************/

  int Date:: Julian_Day(void)

  {
    Boolean is_leap_year = Is_Leap_Year(year);

    int days_so_far = days_to_month[is_leap_year][month];
    days_so_far += day;

    return days_so_far;

//    return days_to_month[Is_Leap_Year(year)][month] + day;

  }   //Date:: Julian_Day(void)

/*****************************************************************************
*
*  TITLE:       operator <
*
*****************************************************************************/

  Boolean Date:: operator < (Date& date)
  {

    if (year < date.year) return True;
    if (year > date.year) return False;

    if (month < date.month) return True;
    if (month > date.month) return False;

    if (day < date.day) return True;
    return False;

  }   //Date:: operater < (Date& date)

/*****************************************************************************
*
*  TITLE:       Set
*
*****************************************************************************/

  void Date:: Set(

    int _year,
    int _month,
    int _day)

  {

    if (Is_Valid((ushort) _year, (uchar) _month, (uchar) _day))
    {
      year = (ushort) _year;
      month = (uchar) _month;
      day = (uchar) _day;
    }

  }   //Date:: Set

/*****************************************************************************
*
*  TITLE:       Set
*
*****************************************************************************/

  void Date:: Set(

    Date& date)

  {

    Set(date.year, date.month, date.day);

  }   //Date:: Set(Date&)

/*****************************************************************************
*
*  TITLE:       Set
*
*****************************************************************************/

  void Date:: Set(

    int day_of_year,
    int _year)

  {

    Boolean on_leap_year = Is_Leap_Year(_year);

    if ((day_of_year < 1) ||
        (day_of_year > days_per_year[on_leap_year]))
    {
      return; //------------------------------> return!
    }

    year = _year;

    int for_month;

    for (for_month = February; for_month <= December; for_month++)
    {
      if (day_of_year < days_to_month[on_leap_year][for_month]) break;
    }

    month = for_month - 1;
    day = day_of_year - days_to_month[on_leap_year][month];

  }   //Date:: Set


/*****************************************************************************
*
*  TITLE:       Set System Date
*
*****************************************************************************/

  void Date:: Set_System_Date(void)
  {
    date dos_date;

    dos_date.da_year = year;
    dos_date.da_mon = month;
    dos_date.da_day = day;

    setdate(&dos_date);

  }   //Date:: Set_System_Date

/*****************************************************************************
*
*  TITLE:       Write String To Output
*
*  DESCRIPTION: The procedure "Write_String_To_Output" writes the given string
*               to the given output using the supplied width, and forcing the
*               case as specified.  The output string (pointer) is adjusted to
*               the next character.
*
*  REFERENCE:   None.
*
*****************************************************************************/

void Write_String_To_Output(
  char** output,                //output string pointer
  char* string_to_write,        //string to write to output
  int width,                    //desired width (negative for left-justify)
  Boolean to_upper_case,        //force string to upper case
  Boolean to_lower_case)        //force string to lower case
{
  sprintf(*output, "%*s", width, string_to_write);

  if (to_upper_case)
  {
    strupr(*output);
  }
  else if (to_lower_case)
  {
    strlwr(*output);
  }

  if (width == 0)
  {
    while (*(*output)) (*output)++;
  }
  else
  {
    *output += abs(width);
  }
}   //Write_String_To_Output(char**,char*,int,Boolean,Boolean)
