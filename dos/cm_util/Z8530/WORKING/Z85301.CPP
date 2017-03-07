/*****************************************************************************
*
*  TITLE:        Z8530
*
*  DESCRIPTION:  The subprogram "Z8530::Z8530"
*                for the class Z8530
*                creates a Zilog 8530
*                access object.
*
*  *k "%n"
*  FILE NAME:    "Z85301.CPP"
*
*  *k "%v"
*  VERSION:      "1"
*
*  REFERENCE:    Zilog 8530 programmer's reference.
*
*****************************************************************************/

#ifndef __z8530
#include "z8530.h"
#endif


Z8530::Z8530(

     UINT16 io_base)         // base IO address for the 8530 chip
{
   this->io_base = io_base;

}   // Z8530::Z8530


