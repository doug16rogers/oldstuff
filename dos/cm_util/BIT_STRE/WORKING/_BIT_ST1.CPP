/*****************************************************************************
*
*  TITLE:        ~BIT_STREAM
*
*  DESCRIPTION:  The class method "~BIT_STREAM"
*                of class BIT_STREAM
*                destroys the instance as it is deleted.
*
*  *k "%n"
*  FILE NAME:    "_BIT_ST1.CPP"
*
*  *k "%v"
*  VERSION:      "1"
*
*  REFERENCE:    None.
*
*****************************************************************************/

#include <stdio.h>

#include "bit_stre.h"


BIT_STREAM :: ~BIT_STREAM()
{

   if ( (base != NULL) &&
        !is_mapped )
   {
      delete base;
   }

}   // BIT_STREAM :: ~BIT_STREAM

