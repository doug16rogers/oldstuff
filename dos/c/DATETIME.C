static const int weekday_offset[13] = {        /*offset of month into wday*/
  0,0,3,3,6, 1,4,6,2, 5,0,3,5
};   /*week_offset*/



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
int  IsLeapYear(int year)
{
  if ((year%400)==0) return 1;
  if ((year%100)==0) return 0;
  if ((year&3)==0) return 1;
  return 0;
}   /*IsLeapYear*/


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
WEEK_DAY GetWeekDay(int year,int month,int day)
{
  int wday;
  int mod400;

  mod400 = year%400;                   /*year into 400yr cycle*/

  wday  = 6;                           /*400yr calendar starts on sat*/
  wday += mod400;                      /*add in 1 day of week per year*/
  wday += mod400/4;                    /*add in an extra for leap years*/
  wday -= mod400/100;                  /*take away every century - no leap*/
  wday += weekday_offset[month];       /*add in offset for month*/
  wday += day;                         /*add in day off month*/
  if (IsLeapYear(year) && (month<3)) wday--;   /*sub extra leapyear added*/
  wday %= 7;                           /*get day of week*/
  return (WEEK_DAY)wday;               /*yep, all ok*/
}   /*GetWeekDay*/


