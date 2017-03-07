/*****************************************************************************
*
*  TITLE:        Is_Empty
*
*  DESCRIPTION:  The class method "Is_Empty"
*                of class BIT_STREAM
*                returns true if the bit stream is
*                empty.
*
*  *k "%n"
*  FILE NAME:    "IC_EMPT1.CPP"
*
*  *k "%v"
*  VERSION:      "1"
*
*  REFERENCE:    None.
*
*****************************************************************************/

#include "bit_stre.h"


BOOLEAN BIT_STREAM :: Is_Empty()
{
   return (BOOLEAN) (count == 0);

}   // BIT_STREAM :: Is_Empty

