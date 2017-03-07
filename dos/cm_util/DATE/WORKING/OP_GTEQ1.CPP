/*****************************************************************************
*
*  TITLE:        operator >=
*
*  DESCRIPTION:  The subprogram "DATE::operator >="
*                in the class DATE
*                returns wether the given date
*                is greater than or equal to
*                the object's.
*
*  *k "%n"
*  FILE NAME:    "OP_GTEQ1.CPP"
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


BOOLEAN DATE::operator >= (

     DATE& date)

{

   return (BOOLEAN) !(*this < date);

}   // DATE::operator >=

