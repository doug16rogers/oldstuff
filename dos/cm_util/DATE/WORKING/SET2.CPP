/*****************************************************************************
*
*  TITLE:        Set
*
*  DESCRIPTION:  The subprogram "DATE::Set"
*                in the class DATE
*                sets the object's date to
*                the given DATE object's date.
*
*  *k "%n"
*  FILE NAME:    "SET2.CPP"
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


void DATE::Set(

     DATE& date)

{
   Set(date.year, date.month, date.day);

}   // DATE::Set


