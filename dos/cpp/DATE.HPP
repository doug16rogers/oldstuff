#ifndef __date_hpp
#define __date_hpp

#include <ttypes.h>     // from Turbo Vision include files


/*****************************************************************************
*
*  TITLE:       Date
*
*  DESCRIPTION: The "Date" class provides operations for dates.
*
*  *k "%n"
*  FILE NAME: "DATE.HPP"
*
*  *k "%v"
*  VERSION:   "2"
*
*****************************************************************************/


class Date
{

//===========================================================================
//
//  Class types
//
//---------------------------------------------------------------------------

public:

  typedef
  enum
  {
    Sunday = 0,
    Monday,
    Tuesday,
    Wednesday,
    Thursday,
    Friday,
    Saturday
  } Days_Of_Week;

  typedef
  enum
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
  } Months_Of_Year;

//===========================================================================
//
//  Class data
//
//---------------------------------------------------------------------------

private:

  static char* default_format;          //1980.01.01

  static char local_image[0x80];        //for holding local image

public:

  char* last_format;                    //holds on to last format used

  static char* name_of_month[13];       //month names, indexed by
                                        //  Months_Of_Year

  static char* name_of_day[7];          //day-of-week names, indexed by
                                        //  Days_Of_Week

public:

  ushort year;
  uchar month;
  uchar day;

//===========================================================================
//
//  Class methods
//
//---------------------------------------------------------------------------

/*****************************************************************************
*
*  TITLE:       Date
*
*  DESCRIPTION: The default constructor "Date:: Date" initializes
*               the date to 1980.01.01.
*
*  REFERENCE:   None.
*
*****************************************************************************/

public:

  Date(void);

/*****************************************************************************
*
*  TITLE:       Date
*
*  DESCRIPTION: The constructor "Date:: Date" initializes the date to the
*               year, month and day provided.
*
*  REFERENCE:   None.
*
*****************************************************************************/

public:

  Date(

    int _year,
    int _month,
    int _day);

/*****************************************************************************
*
*  TITLE:       Day Of Week
*
*  DESCRIPTION: The function "Day_Of_Week" returns the day of week for the
*               date.
*
*  REFERENCE:   None.
*
*****************************************************************************/

public:

  Days_Of_Week Day_Of_Week(void);

/*****************************************************************************
*
*  TITLE:       Decrement
*
*  DESCRIPTION: The function "Date:: Decrement" decrements the date by the
*               number of days given.
*
*  REFERENCE:   None.
*
*****************************************************************************/

public:

  void Decrement(

    int by_days = 1);           //number of days to decrement by

/*****************************************************************************
*
*  TITLE:       Get System Date
*
*  DESCRIPTION: The procedure "Get_System_Date" loads the date with the
*               system date.
*
*  REFERENCE:   None.
*
*****************************************************************************/

public:

  void Get_System_Date(void);

/*****************************************************************************
*
*  TITLE:       Image
*
*  DESCRIPTION: The function "Date:: Image" formats the date according
*               to the format string provided.  It returns the buffer provided
*               to it for storing the formatted time.
*
*               The format string contains '%'-sequences:
*
*                 %[-][0][<width>][u/l]<control>
*
*               The optional [-] causes left justification.
*               The optional [0] causes zero-padding of the field.
*               The optional [<width>] specifies the width of the field.
*               The optional [u/l] forces strings to upper/lower case.
*               The <control> character may be:
*
*                 '%'  insert the '%' character
*                 'd'  insert day of month
*                 'j'  insert Julian day (day of year)
*                 'm'  insert month as a number
*                 'M'  insert month as a string
*                 'w'  insert day of week as a number (0-6)
*                 'W'  insert day of week as a string
*                 'y'  insert full year
*                 'Y'  insert year modulo 100 (without century)
*
*               The default format string is "%04y.%02m.%02d".
*               For strings use "%W %M %d, %y".
*
*
*  REFERENCE:   None.
*
*****************************************************************************/

public:

  char* Image(                  //buffer as passed in

    char* into_buffer = NULL,   //buffer for image (NULL = local)
    char* with_format = NULL);  //format string, NULL means last_format

/*****************************************************************************
*
*  TITLE:       Increment
*
*  DESCRIPTION: The function "Date:: Increment" increments the date by the
*               number of days given.
*
*  REFERENCE:   None.
*
*****************************************************************************/

public:

  void Increment(

    int by_days = 1);           //number of days to increment by

/*****************************************************************************
*
*  TITLE:       Is Leap Year
*
*  DESCRIPTION: The function "Date:: Is_Leap_Year" returns True if the
*               year given (or the year specified in the Date object) is
*               a leap year.
*
*  REFERENCE:   None.
*
*****************************************************************************/

public:

  Boolean Is_Leap_Year(

    ushort for_year = 0);       // year to check; if 0, use object's date

/*****************************************************************************
*
*  TITLE:       Is Valid
*
*  DESCRIPTION: The function "Date:: Is_Valid" checks a year/month/
*               day combination to make sure it's valid.
*
*  REFERENCE:   None.
*
*****************************************************************************/

public:

  Boolean Is_Valid(

    ushort for_year,
    uchar for_month,
    uchar for_day);

/*****************************************************************************
*
*  TITLE:       Julian Day
*
*  DESCRIPTION: The function "Date:: Julian_Day" returns the Julian day for
*               the date; that is, the day of the year, where January 1 is 1.
*
*  REFERENCE:   None.
*
*****************************************************************************/

public:

  int Julian_Day(void);

/*****************************************************************************
*
*  TITLE:       operator <
*
*  DESCRIPTION: The "operator <" function returns True if the left date is
*               less than the right date; otherwise, it returns False.
*
*  REFERENCE:   None.
*
*****************************************************************************/

public:

  Boolean operator < (Date& date);

/*****************************************************************************
*
*  TITLE:       operator ==
*
*  DESCRIPTION: The "operator ==" function returns True if the left date is
*               equal to the right date; otherwise, it returns False.
*
*  REFERENCE:   None.
*
*****************************************************************************/

public:

  Boolean operator == (Date& date)
  {

    return (Boolean) ((year == date.year) &&
                      (month == date.month) &&
                      (day == date.day));

  }   //Date:: operator == (Date& date)

/*****************************************************************************
*
*  TITLE:       operator >
*
*  DESCRIPTION: The "operator >" function returns True if the left date is
*               greater than the right date; otherwise, it returns False.
*
*  REFERENCE:   None.
*
*****************************************************************************/

public:

  Boolean operator > (Date& date)
  {

    return (Boolean) (!(*this == date) &&
                      !(*this < date));

  }   //Date:: operator > (Date& date)

/*****************************************************************************
*
*  TITLE:       operator <=
*
*  DESCRIPTION: The "operator <=" function returns True if the left date is
*               less than or equal to the right date; otherwise, it returns
*               False.
*
*  REFERENCE:   None.
*
*****************************************************************************/

public:

  Boolean operator <= (Date& date)
  {

    return (Boolean) ((*this < date) ||
                      (*this == date));

  }   //Date:: operator <= (Date& date)

/*****************************************************************************
*
*  TITLE:       operator !=
*
*  DESCRIPTION: The "operator !=" function returns True if the left date is
*               not equal to the right date; otherwise, it returns False.
*
*  REFERENCE:   None.
*
*****************************************************************************/

public:

  Boolean operator != (Date& date)
  {

    return (Boolean) !(*this == date);

  }   //Date:: operator != (Date& date)

/*****************************************************************************
*
*  TITLE:       operator >=
*
*  DESCRIPTION: The "operator >=" function returns True if the left date is
*               greater than or equal to the right date; otherwise, it returns
*               False.
*
*  REFERENCE:   None.
*
*****************************************************************************/

public:

  Boolean operator >= (Date& date)
  {

    return (Boolean) !(*this < date);

  }   //Date:: operator >= (Date& date)

/*****************************************************************************
*
*  TITLE:       Set
*
*  DESCRIPTION: This "Set" procedure sets the full date.
*
*  REFERENCE:   None.
*
*****************************************************************************/

public:

  void Set(

    int _year,
    int _month,
    int _day);

/*****************************************************************************
*
*  TITLE:       Set
*
*  DESCRIPTION: This "Set" procedure sets the full date as given.
*
*  REFERENCE:   None.
*
*****************************************************************************/

public:

  void Set(

    Date& date);

/*****************************************************************************
*
*  TITLE:       Set
*
*  DESCRIPTION: This "Set" procedure sets the date by Julian day and year.
*
*  REFERENCE:   None.
*
*****************************************************************************/

public:

  void Set(

    int day_of_year,            //Julian day to set
    int _year);                 //year in which to set it

/*****************************************************************************
*
*  TITLE:       Set System Date
*
*  DESCRIPTION: The procedure "Set_System_Date" loads the system date with the
*               object's date.
*
*  REFERENCE:   None.
*
*****************************************************************************/

public:

  void Set_System_Date(void);

};   //class Date


#endif //__timedate_hpp

