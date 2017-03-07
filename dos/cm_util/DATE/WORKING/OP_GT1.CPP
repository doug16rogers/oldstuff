/*****************************************************************************
*
*  TITLE:        operator >
*
*  DESCRIPTION:  The subprogram "DATE::operator >"
*                in the class DATE
*                returns whether the given
*                date is later than the
*                object's.
*
*  *k "%n"
*  FILE NAME:    "OP_GT1.CPP"
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


BOOLEAN DATE::operator > (

     DATE& date)

{

   return (BOOLEAN) (!(*this == date) &&
                     !(*this < date));

}   // DATE::operator >

