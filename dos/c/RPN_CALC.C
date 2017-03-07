#include <stdio.h>
#include <math.h>
#include <conio.h>


#include "scr_form.h"
#include "rpn_eval.h"
#include "variable.h"

#define FORM_FORE LIGHTGRAY
#define FORM_BACK BLUE
#define READ_FORE WHITE
#define READ_BACK MAGENTA
#define EDIT_FORE BLACK
#define EDIT_BACK LIGHTGRAY

#define FILENAME_SIZE 40
#define VARIABLE_SIZE 8
#define FUNCTION_SIZE 40


char filename[40] = "fun.out";
char variable[VARIABLE_SIZE] = "x";
char start[FUNCTION_SIZE] = "0.0";
char end[FUNCTION_SIZE]  = "pi";
char step[FUNCTION_SIZE] = "pi 8 /";
char function[FUNCTION_SIZE] = "x sin";

char last_value[0x80] = "0.0";
char last_message[0x80]  = RPN_NO_ERROR;


TextEntry rpn_display_texts[] =
{
  { WRITE, 2, 2,   "Filename: ",            0,  0,0 },
  { READ,  0, 0,   filename,    FILENAME_SIZE,  READ_FORE,READ_BACK },
  { WRITE, 2, 4,   "Variable name: ",       0,  0,0 },
  { READ,  0, 0,   variable,    VARIABLE_SIZE,  READ_FORE,READ_BACK },
  { WRITE, 2, 5,   "Start value: ",         0,  0,0 },
  { READ,  0, 0,   start,       FUNCTION_SIZE,  READ_FORE,READ_BACK },
  { WRITE, 2, 6,   "End value  : ",         0,  0,0 },
  { READ,  0, 0,   end,         FUNCTION_SIZE,  READ_FORE,READ_BACK },
  { WRITE, 2, 7,   "Step: ",                0,  0,0 },
  { READ,  0, 0,   step,        FUNCTION_SIZE,  READ_FORE,READ_BACK },
  { WRITE, 2, 9,   "Function: ",            0,  0,0 },
  { READ,  0, 0,   function,    FUNCTION_SIZE,  READ_FORE,READ_BACK },
  { WRITE, 2, 11,  "Last calculated: ",     0,  0,0 },
  { WRITE, 0, 0,   last_value,              0,  0,0 },
  { WRITE, 21,13,  "Messages: ",            0,  0,0 },
  { WRITE, 0, 0,   last_message,            0,  0,0 },
  { END_TEXT }
};   /*rpn_display*/

ScreenForm rpn_form =
{
  15,  5,
  75, 20,
  FORM_FORE,
  FORM_BACK,
  EDIT_FORE,
  EDIT_BACK,
  rpn_display_texts
};


void main(void)
{
  clrscr();
  DisplayForm(&rpn_form);
  while (GetKey() != ESC);
  window(1,1,50,80);
  gotoxy(1,1);
}   /*main*/

