/*****************************************************************************
*
*  TITLE:        operator <
*
*  DESCRIPTION:  The subprogram "DATE::operator <"
*                in the class DATE
*                returns wether the given date
*                is less than (earlier than)
*                the object's.
*
*  *k "%n"
*  FILE NAME:    "OP_LT1.CPP"
*
*  *k "%v"
*  VERSION:      "1"
*
*  REFERENCE:    None.
*
*****************************************************************************/

#ifndef __DATE_CLASS
#include "date.h"
#endif


BOOLEAN DATE::operator < (

     DATE& date)

{
   if (year < date.year) return true;
   if (year > date.year) return false;

   if (month < date.month) return true;
   if (month > date.month) return false;

   if (day < date.day) return true;

   return false;

}   // DATE::< 


