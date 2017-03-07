/*****************************************************************************
*
*  TITLE:        Is_Full
*
*  DESCRIPTION:  The class method "Is_Full"
*                of class BIT_STREAM
*                returns true if the bit
*                stream is full.
*
*  *k "%n"
*  FILE NAME:    "IS_FULL1.CPP"
*
*  *k "%v"
*  VERSION:      "1"
*
*  REFERENCE:    None.
*
*****************************************************************************/

#include "bit_stre.h"


BOOLEAN BIT_STREAM :: Is_Full()
{
   return (BOOLEAN) (count >= size);

}   // BIT_STREAM :: Is_Full

