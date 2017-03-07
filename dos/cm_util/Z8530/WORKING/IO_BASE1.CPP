/*****************************************************************************
*
*  TITLE:        IO_Base
*
*  DESCRIPTION:  The subprogram "Z8530::IO_Base"
*                for the class Z8530
*                returns the base i/o (port) address
*                for the 8530 chip.
*
*  *k "%n"
*  FILE NAME:    "IO_BASE1.CPP"
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


UINT16 Z8530::IO_Base()    // base IO address for the 8530 chip
{
   return io_base;

}   // Z8530::IO_Base

