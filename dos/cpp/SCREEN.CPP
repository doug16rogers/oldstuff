#include <stdio.h>
#include <dos.h>

#include "screen.hpp"



Screen::Screen(void)
{
  union REGS r;

  r.h.ah=0x0F;                         /*get video mode*/
  int86(VIDINT,&r,&r);                 /*call video services interrupt*/
  mode=r.h.al;                         /*vidoe mode in al*/
  page=r.h.bh;                         /*video page in bh*/
  switch (mode) {                      /*check the mode*/
  case 0x00:                           /*Text, 25x40x16 gray/BW*/
    rows=25;                           /*25 rows*/
    cols=40;                           /*40 columns*/
    break;
  case 0x01:                           /*Text, 25x80x16 gray/BW*/
    rows=25;                           /*25 rows*/
    cols=80;                           /*80 columns*/
    break;
  case 0x02:                           /*Text, 25x40x16 color*/
    rows=25;                           /*25 rows*/
    cols=40;                           /*40 columns*/
    break;
  case 0x03:                           /*Text, 25x80x16 color*/
    rows=25;                           /*25 rows*/
    cols=80;                           /*80 columns*/
  default:                             /*other modes not supported*/
    sptr=NULL;                         /*point nowhere*/
    return;                            /*end of constructor*/
  }   /*switch*/
  sptr=(WORD far*)(MONPTR+*PAGOFFPTR); /*set up page pointer*/
  psiz=*PAGSIZPTR;                     /*get page size*/
  r.h.ah=0x03;                         /*get cursor position*/
  r.h.bh=page;                         /*current page*/
  int86(VIDINT,&r,&r);                 /*call video services interrupt*/
  crow=r.h.dh;                         /*row in dh*/
  ccol=r.h.dl;                         /*col in dl*/
  csiz=r.x.cx;                         /*size in ch:cl*/
}   /*Screen::Screen*/
