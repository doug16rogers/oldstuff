/*****************************************************************************
*
*  TITLE:        operator !=
*
*  DESCRIPTION:  The subprogram "DATE::operator !="
*                in the class DATE
*                returns wether the given date
*                is different than
*                the object's.
*
*  *k "%n"
*  FILE NAME:    "OP_NEQ1.CPP"
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


BOOLEAN DATE::operator != (

     DATE& date)

{

   return (BOOLEAN) !(*this == date);

}   // DATE::operator !=

