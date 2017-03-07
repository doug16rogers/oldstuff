//===========================================================================
//    Module Name: de
//    Description: Double-entry commandline/full-screen accounting system.
//         Author: dr
//           Date: 1991.10.29
//      Copyright: none
//   Dependencies: none
//---------------------------------------------------------------------------
#include <stdiostr.h>
#include <dos.h>

#include "drstr.hpp"


#define FULL_SCREEN        0
#define X_FILE_NAME_SIZE   0x40
#define X_FILE_NAME        "\0"



char full_screen = FULL_SCREEN;        //full-screen mode flag
char x_file_name[X_FILE_NAME_SIZE]     //double-entry transaction file
     = X_FILE_NAME;


class Date {
public:
  int y,m,d;
  Date(int _y=0,int _m=0,int _d=0);
};   //class Date

Date::Date(int _y,int _m,int _d)
{
  if ((_y == 0) || (_m == 0) || (_d == 0))
  {
    struct date dat;
    getdate(&dat);
    if (_y == 0) _y = dat.da_year;
    if (_m == 0) _m = dat.da_mon;
    if (_d == 0) _d = dat.da_day;
  }
  y = _y;
  m = _m;
  d = _d;
}   //Date::Date(int _y,int _m,int _d)



class DollarsAndCents {
public:
  long dollars;                        //positive/negative dollars
  int cents;                           //positive/negative with dollars

  DollarsAndCents(int _dollars=0,int _cents=0)
  {
    dollars = _dollars;
    cents   = _cents;
    if ((dollars < 0) && (cents > 0))
    {
      dollars += 1;
      cents   -= 100;
    }
  }
  DollarsAndCents& operator - ()
  {
    dollars = -dollars;
    cents = -cents;
    return *this;
  }
  void operator += (DollarsAndCents& d);
  void operator -= (DollarsAndCents& d);
};   //class DollarsAndCents


void DollarsAndCents::operator += (DollarsAndCents& d)
{
  dollars += d.dollars;
  cents   += d.cents;
  while (cents > 100)
  {
    cents   -= 100;
    dollars += 1;
  }
  while (cents < 0)
  {
    cents   += 100;
    dollars -= 1;
  }
  if ((dollars < 0) && (cents > 0))
  {
    dollars += 1;
    cents   -= 100;
  }
}   //DollarsAndCents::operator += (DollarsAndCents& d);


void DollarsAndCents::operator -= (DollarsAndCents& d)
{
  dollars -= d.dollars;
  cents   -= d.cents;
  while (cents > 100)
  {
    cents   -= 100;
    dollars += 1;
  }
  while (cents < 0)
  {
    cents   += 100;
    dollars -= 1;
  }
  if ((dollars < 0) && (cents > 0))
  {
    dollars += 1;
    cents   -= 100;
  }
}   //DollarsAndCents::operator -= (DollarsAndCents& d);


class Transaction {
public:
  int year;
  int mon;
  int day;
  int hour;
  int min;
  int sec;

  DollarsAndCents amount;
#define LEDGER_NAME_SIZE   0x10
  char source[LEDGER_NAME_SIZE];
  char destination[LEDGER_NAME_SIZE];

  void operator = (Transaction& t)
  {
    year = t.year;
    mon  = t.mon;
    day  = t.day;
    hour = t.hour;
    min  = t.min;
    sec  = t.sec;
    amount = t.amount;
    strNcpy(source,t.source,sizeof(source));
    strNcpy(destination,t.destination,sizeof(destination));
  }
  Transaction& operator + (Transaction& t) { return *this; }
  Transaction& operator + (Date& d)
  {
    year = d.y;
    mon  = d.m;
    day  = d.d;
    return *this;
  }
  Transaction& operator + (DollarsAndCents& d)
  {
    amount = d;
    return *this;
  }
  Transaction& operator += (DollarsAndCents& d)
  {
    amount += d;
    return *this;
  }
  Transaction& operator -= (DollarsAndCents& d)
  {
    amount -= d;
    return *this;
  }
  void Show(void)
  {
    printf("%u.%u.%u  ",year,mon,day);
    if ((amount.dollars < 0) || (amount.cents < 0))
      printf(" $ %6ld.%02u\n",amount.dollars,-amount.cents);
    else
      printf(" $ %6ld.%02u\n",amount.dollars,amount.cents);
  }
};   //class Transaction


//===========================================================================
//  Function Name: loadfile
//    Description: This function loads a string as a filename.
//         Author: dr
//           Date: 1991.10.29
//---------------------------------------------------------------------------
int  loadfile(                         //success/failure
  char* s)                             //string to load as a file
{
  static char x_file_name_entered = 0;   //set when filename is entered

  if ((*s == '-') || (*s == '/')) return 1;  //not a file, must be a switch

  if (x_file_name_entered) {
    printf("transaction file already specified (\"%s\")\n",s);
    return 0;
  }
  strNcpy(x_file_name,s,sizeof(x_file_name));
  x_file_name_entered = 1;
  return 1;
}   //loadfile


//===========================================================================
//  Function Name: loadswitch
//    Description: This function interprets a string as a commandline switch.
//                 It checks for errors and returns 1 on success, else 0.
//         Author: dr
//           Date: 1991.10.29
//---------------------------------------------------------------------------
int  loadswitch(                       //success/failure
  char* s)                             //string to interpret as switch
{
  if ((*s != '-') && (*s != '/')) return 1;  //not a switch, must be a file
  s++;
  switch (*s++)
  {
  case '?':
    return 0;
  case 'f':
    full_screen = 1;
    break;
  default:
    s--;
    printf("unknown switch \"%s\"\n",s);
    return 0;
  }   //switch
  return 1;
}   //loadswitch


//===========================================================================
//  Function Name: help
//    Description: This function displays the commandline switches available
//                 to the user.  It indicates the default settings for the
//                 switches.
//         Author: dr
//           Date: 1991.10.29
//---------------------------------------------------------------------------
int  help(                             //returns its parameter
  int k)                               //parameter for return value
{
  static char* minus_plus[] = { "-", "+" };  //constants for flag values

  printf(
    "\n"
    "Usage: de [switches] [de-file]\n"
    "Switches: [default]\n"
    "  -?            this help message\n"
    "  -f[+/-]       full-screen mode [%s]\n"
    ,minus_plus[FULL_SCREEN!=0]
  );   //printf
  return k;
}   //help


//===========================================================================
//  Function Name: main
//    Description: This is the main function.  It loads the commandline
//                 arguments, then runs the rest of the program.
//         Author: dr
//           Date: 1991.10.29
//---------------------------------------------------------------------------
int  main(                             //return code for DOS
  int argc,                            //number of commandline parameters
  char* argv[])                        //array of pointers to parameters
{
  int i;

  for (i = 1; (i < argc); i++)
  {
    if (!loadswitch(argv[i])) return help(1);
    if (!loadfile(argv[i])) return help(1);
  }   //for

  Transaction t;
  t = t + Date() + DollarsAndCents(20,72);
  t.Show();
  t -= DollarsAndCents(15,39);
  t.Show();
  t -= DollarsAndCents(10,0);
  t.Show();
}   //main
