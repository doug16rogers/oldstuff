/*-------------------------------------------------------------------------*\
 | Module:      Screen Forms
 | Description: This module allows for easily updating a set of forms on
 |              the screen.
\*-------------------------------------------------------------------------*/

#include <stdio.h>
#include <conio.h>

#include "scr_form.h"


/*-------------------------------------------------------------------------*\
 | Procedure:   GetKey
 | Description: Gets a key from the keyboard.  Returns the ASCII value, or
 |              one of the keys shown in scr_form.h.
\*-------------------------------------------------------------------------*/

int  GetKey(void)              /*key code/ASCII value*/
{
  int key;

  key = getch();
  if (key == 0) key = 0x100 + getch();
  return key;
}   /*GetKey*/


/*-------------------------------------------------------------------------*\
 | Procedure:   DisplayForm
 | Description: Displays a ScreenForm form.  Clears the screen first.
\*-------------------------------------------------------------------------*/

void DisplayForm(              /*no output*/
  ScreenForm* form)            /*pointer to screen form*/
{
  TextEntry* text;

  if (form == NULL) return;

  window(form->x0, form->y0, form->x1, form->y1);
  textattr(form->fore + (form->back << 4));
  clrscr();

  text = form->text;
  if (text == NULL) return;

  while (text->command != END_TEXT)   /*first do all the writing*/
  {
    switch (text->command)
    {
    case WRITE:
    case READ:
      gotoxy(text->x, text->y);
      if ((text->fore != 0) || (text->back != 0))
        textattr(text->fore + (text->back << 4));
      if (text->string != NULL) cputs(text->string);
      if ((text->fore != 0) || (text->back != 0))
        textattr(form->fore + (form->back << 4));
      break;
    }   /*switch*/
    text++;
  }   /*while*/
}   /*DisplayForm*/

