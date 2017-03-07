/*****************************************************************************
*
*  TITLE:        DATE
*
*  DESCRIPTION:  The default constructor "DATE::DATE" initializes
*                the date to 1980.01.01.
*
*  *k "%n"
*  FILE NAME:    "DATE1.CPP"
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


DATE::DATE()
{

   last_format = default_format;
   Set(1980, January, 1);

}   // DATE::DATE
