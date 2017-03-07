/*****************************************************************************
*
*  TITLE:        Change IO Base
*
*  DESCRIPTION:  The subprogram "Z8530::Change_IO_Base"
*                for the class Z8530
*                changes the i/o base address for the chip.
*
*                It returns the previous i/o base.
*
*  *k "%n"
*  FILE NAME:    "CHANGE_1.CPP"
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


UINT16 Z8530::Change_IO_Base(    // previous i/o base address of chip

     UINT16 new_base)            // new i/o base address for chip
{
   UINT16 old_base = io_base;

   io_base = new_base;

   return old_base;

}   // Z8530::Change_IO_Base


