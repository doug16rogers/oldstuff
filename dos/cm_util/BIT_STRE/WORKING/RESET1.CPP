/*****************************************************************************
*
*  TITLE:        Reset
*
*  DESCRIPTION:  The class method "Reset"
*                of class BIT_STREAM
*                resets the bitstream.
*
*  *k "%n"
*  FILE NAME:    "RESET1.CPP"
*
*  *k "%v"
*  VERSION:      "1"
*
*  REFERENCE:    None.
*
*****************************************************************************/

#include "bit_stre.h"


void BIT_STREAM :: Reset()
{
   head = 0;
   tail = 0;
   count = 0;

}   // BIT_STREAM :: Reset

