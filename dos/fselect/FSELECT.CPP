/*****************************************************************************
*
*  TITLE:       File Select
*
*  DESCRIPTION: This program selects files according to selected criteria and
*               stores the name of each matching file in an output file.
*               
*  *k "%n"
*  FILE NAME: ""
*
*  *k "%v"
*  VERSION:   ""
*
*****************************************************************************/

#include <conio.h>
#include <ctype.h>
#include <dir.h>
#include <dos.h>
#include <stdiostr.h>
#include <string.h>

#include "object.hpp"           //generic object
#include "timedate.hpp"         //time and date

#define TERMINATE(s)   (s)[sizeof(s)-1] = 0

Time_Date today;

void main(void)
{

  while (!kbhit())
  {
    today.Get_System();
    cout << today.date.Image() << ' ' << today.time.Image() << '\r';
  }

  if (getch() == 0) getch();

}   //main

