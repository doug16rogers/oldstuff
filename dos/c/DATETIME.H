#ifndef __DATETIME_H
#define __DATETIME_H
/****************************************************************************
         CSC NAME:  Date/Time
  CSC DESCRIPTION:  Handles date and time calculations.
****************************************************************************/


typedef enum {                          /*days of the week*/
  SUNDAY=0,
  MONDAY,
  TUESDAY,
  WEDNESDAY,
  THURSDAY,
  FRIDAY,
  SATURDAY,
  NO_DAY
} WEEK_DAY_enum;
typedef int WEEK_DAY;

typedef struct {                       /*time-of-day structure*/
  int hour;
  int minute;
  int second;
  int msec;
} TIME_TYPE;

typedef struct {                       /*date structure*/
  int year;
  int month;
  int day;
  WEEK_DAY weekday;
} DATE_TYPE;

typedef struct {
  DATE_TYPE date;
  TIME_TYPE time;
} DATETIME_TYPE;



/****************************************************************************
         CSU NAME:  IsLeapYear
  CSU DESCRIPTION:  Returns 1 if the year provided is a leap year, else 0.
  CSU's CALLED:                        CSC:
  -----------------------------------  -------------------------------------
  None.
****************************************************************************/

/*
BEGIN
  IF year is a multiple of 400 THEN Return that it is a leapyear.
  IF year is a multiple of 100 THEN Return that it is not a leapyear.
  IF year is a multiple of 4 THEN Return that it is a leapyear.
  Return that it is not a leapyear.
END
*/
int  IsLeapYear(int year);


/****************************************************************************
         CSU NAME:  GetWeekDay
  CSU DESCRIPTION:  Returns the day of the week (0=Sunday) for a given date.
  CSU's CALLED:                        CSC:
  -----------------------------------  -------------------------------------
  IsLeapYear                           Date/Time
****************************************************************************/

/*
BEGIN
  Set to day of week (Saturday) for first day in 400 year cycle.
  Add one day for each year in cycle (365==1 mod 7).
  Add leapyear days due to multiples of 4.
  Subtract any extra leapyears counted due to 100 being a multiple of 4.
  Add day of month.
  IF it's a leapyear AND we haven't passed 29 Feb THEN
    Subtract a day for counting this year's leapday before it occurred.
  ENDIF
  Take modulo 7 to put in 0..6 range.
END
*/
WEEK_DAY GetWeekDay(int year,int month,int day);


