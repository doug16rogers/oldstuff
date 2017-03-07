#include <stdio.h>
#include <ctype.h>
#include <dos.h>
#include <conio.h>

#include "scrutil.hpp"

WORD far *scrseg=NULL;


/****************************************************************************/
/*  ptrcmp                                                                  */
/****************************************************************************/
/*-----------------------------Description----------------------------------*/
/*  This function determines if two pointers are equal.  It is necessary    */
/*  under the Intel 80x86 bullshit segmenting.                              */
/*-----------------------------Arguments------------------------------------*/
/*  void *p            first pointer to compare                             */
/*  void *q            second pointer to compare                            */
/*-----------------------------Return value---------------------------------*/
/*  ptrcmp() returns 1 if the args point to the same place, 0 otherwise.    */
/*-----------------------------Global constants-----------------------------*/
/*-------------------Mod-------Global variables-----------------------------*/
/*-----------------------------Functions called-----------------------------*/
/*-----------------------------Constraints/Gotchas--------------------------*/
/*--Date--------Programmer----------Comments--------------------------------*/
/*  1990.01.01  D. Rogers           initial code                            */
/****************************************************************************/
int  ptrcmp(void far *p,void far *q)
{
  LWORD lp,lq;
  lp=(LWORD)p;
  lq=(LWORD)q;
  lp=((lp>>12)&0x000FFFF0L)+(lp&0x0000FFFFL);   /*shift down segment and add*/
  lq=((lq>>12)&0x000FFFF0L)+(lq&0x0000FFFFL);   /*shift down segment and add*/
  return(lp==lq);
}   /*ptrcmp*/
/****************************************************************************/


/****************************************************************************/
/*  memcpynf, memcpyfn                                                      */
/****************************************************************************/
/*-----------------------------Description----------------------------------*/
/*  These function are analogous to memcpy, but for mixed near:far or       */
/*  far:near pointers.                                                      */
/*-----------------------------Arguments------------------------------------*/
/*-----------------------------Return value---------------------------------*/
/*-----------------------------Global constants-----------------------------*/
/*-------------------Mod-------Global variables-----------------------------*/
/*-----------------------------Functions called-----------------------------*/
/*-----------------------------Constraints/Gotchas--------------------------*/
/*--Date--------Programmer----------Comments--------------------------------*/
/*  1990.01.01  D. Rogers           initial code                            */
/****************************************************************************/
void memcpynf(void near *n,void far *f,WORD k)    /*copies to near from far*/
{
  BYTE far *bf;
  BYTE near *bn;
  bf=(BYTE far *)f;
  bn=(BYTE near *)n;
  while (k-->0) *bn++=*bf++;
}   /*memcpynf*/

void memcpyfn(void far *f,void near *n,WORD k)    /*copies to far from near*/
{
  BYTE far *bf;
  BYTE near *bn;
  bf=(BYTE far *)f;
  bn=(BYTE near *)n;
  while (k-->0) *bf++=*bn++;
}   /*memcpyfn*/
/****************************************************************************/


/****************************************************************************/
/*  getkey                                                                  */
/****************************************************************************/
/*-----------------------------Description----------------------------------*/
/*  This function returns the key pressed.  If a special function key is    */
/*  pressed, it returns the scan code of the key + 256.                     */
/*  Constants are defined in [cutil.h] for use with this function.          */
/*-----------------------------Arguments------------------------------------*/
/*-----------------------------Return value---------------------------------*/
/*  Returns 256+scan code for special keys, ASCII value for others.         */
/*-----------------------------Global constants-----------------------------*/
/*-------------------Mod-------Global variables-----------------------------*/
/*-----------------------------Functions called-----------------------------*/
/*-----------------------------Constraints----------------------------------*/
/*--Date--------Programmer---------Comments---------------------------------*/
/*  1990.01.01  D. Rogers          initial code                             */
/****************************************************************************/
int getkey(void)
{
  int c;
  c=getch();
  if (c)
    return(c);
  else
    return(0x0100+getch());
}   /*getkey*/
/****************************************************************************/


/****************************************************************************/
/*  getcrow                                                                 */
/****************************************************************************/
/*-----------------------------Description----------------------------------*/
/*  This function returns the current absolute cursor row.                  */
/*-----------------------------Arguments------------------------------------*/
/*-----------------------------Return value---------------------------------*/
/*  getcrow() returns the current row as an unsigned int.                   */
/*-----------------------------Global constants-----------------------------*/
/*-------------------Mod-------Global variables-----------------------------*/
/*-----------------------------Functions called-----------------------------*/
/*-----------------------------Constraints/Gotchas--------------------------*/
/*  Rows normally range from 0-24.                                          */
/*--Date--------Programmer----------Comments--------------------------------*/
/*  1990.01.01  D. Rogers           initial code                            */
/****************************************************************************/
WORD getcrow(void)
{
  union REGS r;

  r.x.bx=0;                            /*assume page 0*/
  r.h.ah=0x03;                         /*indicate get-cursor-info*/
  int86(0x10,&r,&r);                   /*call Video Services interrupt*/
  return (r.h.dh);                     /*return row (0=first row)*/
}   /*getcrow*/
/****************************************************************************/


/****************************************************************************/
/*  getccol                                                                 */
/****************************************************************************/
/*-----------------------------Description----------------------------------*/
/*  This function returns the current absolute cursor column.               */
/*-----------------------------Arguments------------------------------------*/
/*-----------------------------Return value---------------------------------*/
/*  getccol() return the absolute cursor column as an unsigned int.         */
/*-----------------------------Global constants-----------------------------*/
/*-------------------Mod-------Global variables-----------------------------*/
/*-----------------------------Functions called-----------------------------*/
/*-----------------------------Constraints/Gotchas--------------------------*/
/*  Columns usually range from 0-79.                                        */
/*--Date--------Programmer----------Comments--------------------------------*/
/*  1990.01.01  D. Rogers           initial code                            */
/****************************************************************************/
WORD getccol(void)
{
  union REGS r;

  r.x.bx=0;                            /*assume page 0*/
  r.h.ah=0x03;                         /*indicate get-cursor-info*/
  int86(0x10,&r,&r);                   /*call Video Services interrupt*/
  return (r.h.dl);                     /*return column (0=first row)*/
}   /*getccol*/
/****************************************************************************/


/****************************************************************************/
/*  setcpos                                                                 */
/****************************************************************************/
/*-----------------------------Description----------------------------------*/
/*  This function sets the current absolute cursor position to the row and  */
/*  column specified.                                                       */
/*-----------------------------Arguments------------------------------------*/
/*  WORD row           absolute cursor row on screen (usually 0-24)         */
/*  WORD col           absolute cursor column on screen (usually 0-79)      */
/*-----------------------------Return value---------------------------------*/
/*-----------------------------Global constants-----------------------------*/
/*-------------------Mod-------Global variables-----------------------------*/
/*-----------------------------Functions called-----------------------------*/
/*-----------------------------Constraints/Gotchas--------------------------*/
/*--Date--------Programmer----------Comments--------------------------------*/
/*  1990.01.01  D. Rogers           initial code                            */
/****************************************************************************/
void setcpos(WORD row,WORD col)
{
  union REGS r;

  r.x.bx=0;                            /*assume page 0*/
  r.h.dh=row;                          /*cursor row*/
  r.h.dl=col;                          /*cursor col*/
  r.h.ah=0x02;                         /*indicate set-cursor-position*/
  int86(0x10,&r,&r);                   /*call Video Services interrupt*/
}   /*setcpos*/
/****************************************************************************/


/****************************************************************************/
/*  getcsiz                                                                 */
/****************************************************************************/
/*-----------------------------Description----------------------------------*/
/*  This function finds out how big the current cursor is on the screen.    */
/*-----------------------------Arguments------------------------------------*/
/*-----------------------------Return value---------------------------------*/
/*  getcsiz() returns the cursor size in an unsigned int whose most         */
/*  significant byte contains the first scan line of the cursor and whose   */
/*  least significant byte contains the last scan line of the cursor.       */
/*-----------------------------Global constants-----------------------------*/
/*-------------------Mod-------Global variables-----------------------------*/
/*-----------------------------Functions called-----------------------------*/
/*-----------------------------Constraints/Gotchas--------------------------*/
/*--Date--------Programmer----------Comments--------------------------------*/
/*  1990.01.01  D. Rogers           initial code                            */
/****************************************************************************/
WORD getcsiz(void)
{
  union REGS r;

  r.x.bx=0;                            /*assume page 0*/
  r.h.ah=0x03;                         /*indicate get-cursor-info*/
  int86(0x10,&r,&r);                   /*call Video Services interrupt*/
  return (r.x.cx);                     /*return cursor scan lines*/
}   /*getcsiz*/
/****************************************************************************/


/****************************************************************************/
/*  setcsiz                                                                 */
/****************************************************************************/
/*-----------------------------Description----------------------------------*/
/*  This function sets the cursor size using the same format as is          */
/*  returned by getcsiz().                                                  */
/*-----------------------------Arguments------------------------------------*/
/*  WORD siz           cursor size (MSB: 1st scan line, LSB: last)          */
/*-----------------------------Return value---------------------------------*/
/*-----------------------------Global constants-----------------------------*/
/*-------------------Mod-------Global variables-----------------------------*/
/*-----------------------------Functions called-----------------------------*/
/*-----------------------------Constraints/Gotchas--------------------------*/
/*--Date--------Programmer----------Comments--------------------------------*/
/*  1990.01.01  D. Rogers           initial code                            */
/****************************************************************************/
void setcsiz(WORD siz)
{
  union REGS r;

  r.x.bx=0;                            /*assume page 0*/
  r.x.cx=siz;                          /*cursor size (start/stop scan line)*/
  r.h.ah=0x01;                         /*indicate set-cursor-size*/
  int86(0x10,&r,&r);                   /*call Video Services interrupt*/
}   /*setcsiz*/


/****************************************************************************/
/*  getsmod                                                                 */
/****************************************************************************/
/*-----------------------------Description----------------------------------*/
/*  This function returns the current video mode of the PC by making a      */
/*  call to the DOS Video Services interrupt.                               */
/*-----------------------------Arguments------------------------------------*/
/*-----------------------------Return value---------------------------------*/
/*  getsmod() returns the video mode as an unsigned int.                    */
/*-----------------------------Global constants-----------------------------*/
/*-------------------Mod-------Global variables-----------------------------*/
/*-----------------------------Functions called-----------------------------*/
/*-----------------------------Constraints/Gotchas--------------------------*/
/*--Date--------Programmer----------Comments--------------------------------*/
/*  1990.01.01  D. Rogers           initial code                            */
/****************************************************************************/
WORD getsmod(void)
{
  union REGS ir,or;                    /*register structs*/
  WORD mode;                           /*mode value*/

  ir.x.bx=0;                           /*assume page 0*/
  ir.h.ah=0x0F;                        /*indicate get-screen-mode*/
  int86(0x10,&ir,&or);                 /*call Video Services interrupt*/
  mode=or.h.al;                        /*see about mode*/

  if (or.h.al==TEXT_80_25) {           /*see if need to check for special*/
    ir.x.bx=0x0003;
    ir.x.ax=0x1130;
    int86(0x10,&ir,&or);
    switch(or.h.dl+1) {                /*look at rows..*/
    case 43:
    case 50:
      mode=TEXT_80_HI;
      break;
    default: ;
    }   /*switch*/
  }   /*if checking for special*/
  return mode;                         /*return video mode*/
}   /*getsmod*/


/****************************************************************************/
/*  setsmod                                                                 */
/****************************************************************************/
/*-----------------------------Description----------------------------------*/
/*  This function sets the current video mode of the PC by making a call    */
/*  to the DOS Video Services interrupt.                                    */
/*-----------------------------Arguments------------------------------------*/
/*  WORD mod           video mode to be set                                 */
/*-----------------------------Return value---------------------------------*/
/*-----------------------------Global constants-----------------------------*/
/*-------------------Mod-------Global variables-----------------------------*/
/*-----------------------------Functions called-----------------------------*/
/*-----------------------------Constraints/Gotchas--------------------------*/
/*--Date--------Programmer----------Comments--------------------------------*/
/*  1990.01.01  D. Rogers           initial code                            */
/****************************************************************************/
void setsmod(WORD mod)
{
  union REGS r;

  if (mod>0x1000) {                    /*if special mode..*/
    r.x.bx=0;                          /*..*/
    r.x.ax=TEXT_80_25;                 /*..first force to text mode*/
    int86(0x10,&r,&r);                 /*..call video services interrupt*/
  }   /*if higher mode*/
  r.x.bx=0;                            /*force page 0*/
  r.x.ax=mod;                          /*video mode to set*/
  int86(0x10,&r,&r);                   /*call Video Services interrupt*/
}   /*setsmod*/


WORD getsrows(void)
{
  union REGS r;                        /*register-holding union*/

  switch (getsmod()) {                 /*go rows via mode*/
  case TEXT_40_25_BW:
  case TEXT_40_25:
  case TEXT_80_25_BW:
  case TEXT_80_25:
  case MONO_80_25:
    return 25;
  case TEXT_80_HI:
    r.x.ax=0x1130;                     /*.*/
    r.x.bx=0x0003;                     /*.*/
    int86(0x10,&r,&r);                 /*call video services interrupt*/
    return r.h.dl+1;                   /*return number of rows on screen*/
  default: ;
  }   /*switch*/
  return 0;                            /*must be in a text mode*/
}   /*getsrows*/


WORD getscols(void)
{
  return *((WORD far *)SCRCOLUMN);     /*columns per row*/
}   /*getscols*/


WORD getsrowscols(void)
{
  union REGS r;                        /*register-holding union*/
  r.x.ax=0x1130;                       /*.*/
  r.x.bx=0x0003;                       /*.*/
  int86(0x10,&r,&r);                   /*call video services interrupt*/
  r.h.dh=*((BYTE far *)SCRCOLUMN);     /*columns per row*/
  return r.x.dx+1;                     /*return number of rows & columns*/
}   /*getsrows*/


/****************************************************************************/
/*  setscrseg                                                               */
/****************************************************************************/
/*-----------------------------Description----------------------------------*/
/*  This function sets the screen (video) segment variable, scrseg,         */
/*  according to the video monitor available.                               */
/*-----------------------------Arguments------------------------------------*/
/*-----------------------------Return value---------------------------------*/
/*-----------------------------Global constants-----------------------------*/
/*-------------------Mod-------Global variables-----------------------------*/
/*-----------------------------Functions called-----------------------------*/
/*-----------------------------Constraints/Gotchas--------------------------*/
/*--Date--------Programmer----------Comments--------------------------------*/
/*  1990.01.01  A. Turing           initial code                            */
/****************************************************************************/
WORD far *setscrseg(void)
{
  switch (getsmod()) {                 /*go rows via mode*/
  case MONO_80_25:
    scrseg=(WORD far*)SCRSEG_MONO;
    break;
  case TEXT_40_25_BW:
  case TEXT_40_25:
  case TEXT_80_25_BW:
  case TEXT_80_25:
  case TEXT_80_HI:
    scrseg=(WORD far*)SCRSEG_COLOR;
    break;
  default:
    scrseg=NULL;
  }   /*switch*/
  return scrseg;
}   /*setscrseg*/

