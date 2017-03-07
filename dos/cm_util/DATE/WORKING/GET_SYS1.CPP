/*****************************************************************************
*
*  TITLE:        Get System Date
*
*  DESCRIPTION:  The subprogram "DATE::Get_System_Date"
*                in the class DATE
*                loads the object date with
*                the DOS system date.
*
*  *k "%n"
*  FILE NAME:    "GET_SYS1.CPP"
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


void DATE::Get_System_Date()

{
   date dos_date;

   getdate(&dos_date);

   year = dos_date.da_year;
   month = dos_date.da_mon;
   day = dos_date.da_day;

}   // DATE::Get_System_Date


