/*****************************************************************************
*
*  TITLE:        Get_System_Time
*
*  DESCRIPTION:  The subprogram "Get_System_Time"
*                of class TIME_OF_DAY
*                sets the time to that of
*                the operating system (DOS).
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


void TIME_OF_DAY::Get_System_Time()
{
   struct time system_time;

   gettime(&system_time);
   Set(system_time.ti_hour, system_time.ti_min, system_time.ti_sec);

}   // TIME_OF_DAY::Get_System_Time

