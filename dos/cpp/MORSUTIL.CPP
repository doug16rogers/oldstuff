#include <stdio.h>
#include <iostream.h>
#include <dos.h>
#include <ctype.h>
#include <string.h>

#include "morsutil.h"


#define FREQ           1000.0
#define LETTERDEL      30
#define DITDAHDEL      10
#define DITLEN         10
#define DAHLEN         30
#define SCALE          8


typedef struct {
  char c;
  char *s;
} MORSE_LIST;


static MORSE_LIST Morse_List[]={
  { 'A',  ".-" },
  { 'B',  "-..." },
  { 'C',  "-.-." },
  { 'D',  "-.." },
  { 'E',  "." },
  { 'F',  "..-." },
  { 'G',  "--." },
  { 'H',  "...." },
  { 'I',  ".." },
  { 'J',  ".---" },
  { 'K',  "-.-" },
  { 'L',  ".-.." },
  { 'M',  "--" },
  { 'N',  "-." },
  { 'O',  "---" },
  { 'P',  ".--." },
  { 'Q',  "--.-" },
  { 'R',  ".-." },
  { 'S',  "..." },
  { 'T',  "-" },
  { 'U',  "..-" },
  { 'V',  "...-" },
  { 'W',  ".--" },
  { 'X',  "-..-" },
  { 'Y',  "-.--" },
  { 'Z',  "--.." },
  { '1',  ".----" },
  { '2',  "..---" },
  { '3',  "...--" },
  { '4',  "....-" },
  { '5',  "....." },
  { '6',  "----." },
  { '7',  "---.." },
  { '8',  "--..." },
  { '9',  "-...." },
  { '0',  "-----" },
  { 0  ,  NULL }
};


Morse::Morse (MorseON mon,MorseOFF moff)
{
  morseon=mon;
  morseoff=moff;
  delscale=SCALE;
  freq=FREQ;
  ditlen   =delscale*DITLEN;
  dahlen   =delscale*DAHLEN;
  ditdahdel=delscale*DITDAHDEL;
  letterdel=delscale*LETTERDEL;
  return;
}   /*Morse::Morse(void)*/


Morse::Morse (void)
{
  morseon=(MorseON)&sound;
  morseoff=(MorseOFF)&nosound;
  delscale=SCALE;
  freq=FREQ;
  ditlen   =delscale*DITLEN;
  dahlen   =delscale*DAHLEN;
  ditdahdel=delscale*DITDAHDEL;
  letterdel=delscale*LETTERDEL;
  return;
}   /*Morse::Morse(void)*/





/****************************************************************************/
/*  Morse::morsestring                                                      */
/****************************************************************************/
/*-----------------------------Description----------------------------------*/
/*  This function looks up the specified letter and returns its equivalent  */
/*  Morse code string.                                                      */
/*-----------------------------Arguments------------------------------------*/
/*-----------------------------Return value---------------------------------*/
/*-----------------------------Global constants-----------------------------*/
/*-------------------Mod-------Global variables-----------------------------*/
/*-----------------------------Functions called-----------------------------*/
/*-----------------------------Constraints/Gotchas--------------------------*/
/*--Date--------Programmer----------Comments--------------------------------*/
/*  1990.01.01  A. Turing           initial code                            */
/****************************************************************************/
char* Morse::morsestring(char c)
{
  MORSE_LIST *m;

  c=toupper(c);
  for (m=Morse_List;m->c;m++) if (m->c==c) break;
  if (!m->c) return " ";
  return m->s;
}   /*char* Morse::morsestring(char c)*/


/****************************************************************************/
/*  Morse::playmorsestring                                                   */
/****************************************************************************/
/*-----------------------------Description----------------------------------*/
/*  This function plays the Morse string (as returned from morsestring())   */
/*  on the speaker.                                                         */
/*-----------------------------Arguments------------------------------------*/
/*-----------------------------Return value---------------------------------*/
/*  Returns the value passed.                                               */
/*-----------------------------Global constants-----------------------------*/
/*-------------------Mod-------Global variables-----------------------------*/
/*-----------------------------Functions called-----------------------------*/
/*-----------------------------Constraints/Gotchas--------------------------*/
/*--Date--------Programmer----------Comments--------------------------------*/
/*  1990.01.01  A. Turing           initial code                            */
/****************************************************************************/
char* Morse::playmorsestring(char* s)
{
  char *mc;

  for (mc=s;*mc;mc++) {
    switch(*mc) {
    case '.':
      if (morseon) morseon(freq,*mc);
      delay(ditlen);
      break;
    case '-':
      if (morseon) morseon(freq,*mc);
      delay(dahlen);
      break;
    default:
      ;
    }   /*switch*/
    if (morseoff) morseoff();
    delay(ditdahdel);
  }   /*for*/
  delay(letterdel);
  return s;
}   /*char* Morse::playmorsestring(char* s)*/


/****************************************************************************/
/*  Morse::playchar                                                          */
/****************************************************************************/
/*-----------------------------Description----------------------------------*/
/*  This function plays the character (letter) passed to it.                */
/*-----------------------------Arguments------------------------------------*/
/*-----------------------------Return value---------------------------------*/
/*  Returns a pointer to the Morse code equivalent of the letter.           */
/*-----------------------------Global constants-----------------------------*/
/*-------------------Mod-------Global variables-----------------------------*/
/*-----------------------------Functions called-----------------------------*/
/*-----------------------------Constraints/Gotchas--------------------------*/
/*--Date--------Programmer----------Comments--------------------------------*/
/*  1990.01.01  A. Turing           initial code                            */
/****************************************************************************/
char* Morse::playchar(char c)
{
  char *s;
  s=morsestring(c);
  if (s) playmorsestring(s);
  return s;
}   /*char* Morse::playchar(char c)*/


Morse& Morse::operator<<(char c)
{
  playchar(c);
  return *this;
}   /*Morse& Morse::<<(char c)*/



/****************************************************************************/
/*  Morse::playstring                                                        */
/****************************************************************************/
/*-----------------------------Description----------------------------------*/
/*  This function plays the entire ASCII string passed to it.               */
/*-----------------------------Arguments------------------------------------*/
/*-----------------------------Return value---------------------------------*/
/*  Returns what was passed.                                                */
/*-----------------------------Global constants-----------------------------*/
/*-------------------Mod-------Global variables-----------------------------*/
/*-----------------------------Functions called-----------------------------*/
/*-----------------------------Constraints/Gotchas--------------------------*/
/*--Date--------Programmer----------Comments--------------------------------*/
/*  1990.01.01  A. Turing           initial code                            */
/****************************************************************************/
char* Morse::playstring(char* s)
{
  char *tc;

  tc=s;
  while (*s) playchar(*s++);
  return tc;
}   /*char* Morse::playstring(char* s)*/


Morse& Morse::operator<<(char* s)
{
  playstring(s);
  return *this;
}   //Morse& Morse::<<(char* s)


char  Morse::charof(char *mstring)
{
  MORSE_LIST *m;

  for (m=Morse_List;m->c;m++) if (strcmp(m->s,mstring)==0) break;
  if (!m->c) return 0;
  return m->c;
}   //char Morse::charof(char *)



int    Morse::scale(int s)
{
  if (s>0) {
    delscale=s;
    ditlen   =delscale*DITLEN;
    dahlen   =delscale*DAHLEN;
    ditdahdel=delscale*DITDAHDEL;
    letterdel=delscale*LETTERDEL;
  }
  return delscale;
}   //int Morse::scale(int)



