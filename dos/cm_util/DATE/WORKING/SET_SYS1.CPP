/*****************************************************************************
*
*  TITLE:        Set System Date
*
*  DESCRIPTION:  The subprogram "DATE::Set_System_Date"
*                in the class DATE
*                sets the DOS date
*                to the object's date.
*
*  *k "%n"
*  FILE NAME:    "SET_SYS1.CPP"
*
*  *k "%v"
*  VERSION:      "1"
*
*  REFERENCE:    None.
*
*****************************************************************************/

#include <dos.h>

#ifndef __DATE_CLASS
#include "date.h"
#endif


void DATE::Set_System_Date()

{
   date dos_date;

   dos_date.da_year = year;
   dos_date.da_mon = month;
   dos_date.da_day = day;

   setdate(&dos_date);

}   // DATE::Set_System_Date


