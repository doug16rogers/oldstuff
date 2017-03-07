/*----------------------------------------------------------19920225 RWB---*/
/* decay.c                                                                 */
/*-------------------------------------------------------------------------*/
#include <stdlib.h>
#include <dos.h>
#include <conio.h>
#include "typdef.h"


#define screen_seg 0xb800

#define DOS_IDLE_INTR 0X28                     /* The dos idle interrupt */


void interrupt (*old_dos_idle)(void);          /* old vector addr */


word old_stack;
word isr_stack;


struct text_info tinfo;


word read_char( byte x, byte y )
{
  word offset;
  offset=(tinfo.screenwidth*2*y)+(x*2);           /* offset from screen seg */
  return(*(word far *)MK_FP(screen_seg,offset));
}



void write_char( byte x, byte y, word ch )
{
  word offset;
  offset=(tinfo.screenwidth*2*y)+(x*2);           /* offset from screen seg */
  *(word far *)MK_FP(screen_seg,offset)=ch;
}



void decay_one( void )
{
  byte x,y;
  word ch;
  x=random(tinfo.screenwidth);
  y=random(tinfo.screenheight);
  if (y<tinfo.screenheight)
  {
    ch=read_char(x,y);                            /* get char value         */
    write_char(x,y,(ch&0xff00)|0x0020);           /* erase old char         */
    write_char(x,y+1,ch);                         /* write in new location  */
  }
}


void interrupt decay_isr( void )
{
  (*old_dos_idle)();
  old_stack=_SS;
  _SS=isr_stack;
  decay_one();
  _SS=old_stack;
}


void main( int argc, char *argv[] )
{
  isr_stack=_SS;
  randomize();
  gettextinfo(&tinfo);
  if (argc>=2)
  {
    tinfo.screenwidth=atoi(argv[1]);
    tinfo.screenheight=atoi(argv[2]);
  }
  old_dos_idle=getvect(DOS_IDLE_INTR);             /* save old ISR          */
  setvect(DOS_IDLE_INTR,decay_isr);                /* install new ISR       */
  keep(0,(_SS+(_SP/16)-_psp));
}
