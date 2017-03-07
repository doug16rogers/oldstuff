/*****************************************************************************
*
*  TITLE:        Set_System_Time
*
*  DESCRIPTION:  The subprogram "Set_System_Time"
*                of class TIME_OF_DAY
*                sets the operating system's
*                (DOS's) time to that of
*                the object.
*
*  *k "%n"
*  FILE NAME:    ""
*
*  *k "%v"
*  VERSION:      ""
*
*  REFERENCE:    None.
*
*****************************************************************************/

#include <dos.h>

#include "timeoday.h"


void TIME_OF_DAY::Set_System_Time()
{
   struct time system_time;

   system_time.ti_hour = hour;
   system_time.ti_min  = minute;
   system_time.ti_sec  = second;
   settime(&system_time);

}   // TIME_OF_DAY::Set_System_Time

