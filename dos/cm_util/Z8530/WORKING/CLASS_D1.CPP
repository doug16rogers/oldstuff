/*****************************************************************************
*
*  TITLE:        Class Data
*
*  DESCRIPTION:  The module "Class Data"
*                for the class Z8530
*                provides static data values.
*
*  *k "%n"
*  FILE NAME:    "CLASS_D1.CPP"
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


UINT32 Z8530::clock_speed = 4915200UL;  // 4.9152 MHz

     // the following two values are added to the io_base instance member,
     // along with the channel ID (A/B) to produce the io port to be used
     // for register access

const UINT16 Z8530::control_offset = 1;
const UINT16 Z8530::data_offset = 0;


