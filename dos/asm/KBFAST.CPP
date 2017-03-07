#include <stdio.h>
#include <dos.h>

//===========================================================================
//  kbfast.com
//  Checks for both shift keys pressed; if so it tells the keyboard to go
//  to max repeat rate.
//---------------------------------------------------------------------------


#ifdef __cplusplus
#define INT_ARGS ...
#else
#define INT_ARGS
#endif

#ifndef byte
#define byte unsigned char
#endif

#ifndef word
#define word unsigned int
#endif


#define KB_INTR         0x09            //HW Keyboard Interrupt
#define VIDEO_INTR      0x10            //ROM BIOS Video Functions
#define KB_SERVICE_INT  0x16            //ROM BIOS KB Services
#define DOS_IDLE_INTR   0x28            //Dos idle interrupt

#define KB_HEAD_PTR     0x0040001AUL
#define KB_TAIL_PTR     0x0040001CUL
#define KB_BUFF_PTR     0x00400000UL

#define KB_SHIFT_STATUS 0x00400017UL
#define   KB_INSERT     0x80
#define   KB_CAPS_LOCK  0x40
#define   KB_NUM_LOCK   0x20
#define   KB_SCROLL_LOCK 0x10
#define   KB_ALT        0x08
#define   KB_CTRL       0x04
#define   KB_LEFT_SHIFT 0x02
#define   KB_RIGHT_SHIFT 0x01

#define   KB_BOTH_SHIFT 0x03


#define KB_PRESSED_NOW  0x00400018UL
#define  KB_LEFT_ALT    0x02
#define  KB_LEFT_CTRL   0x01

#define VIDEO_PTR       0xB8000000UL
#define VIDEO_RAM       ((word far*)VIDEO_PTR)

void interrupt (*old_kb_isr)(INT_ARGS);     //old keyboard vector addr




//===========================================================================
//  kb_fast_int
//  Checks for both shift keys pressed; if so it tells the keyboard to go
//  to max repeat rate.
//---------------------------------------------------------------------------
void interrupt kb_fast_int(INT_ARGS)
{
  static byte already_issued_command = 0;  //used to only handle first press
  static byte keyhit;
  static word key_scan;
  static word old_kb_tail = 0;
//  static word attr = 0x1f00;
//  static byte hex[16] = "0123456789ABCDEF";

  old_kb_isr();
  if (old_kb_tail != *(word far*)KB_TAIL_PTR) /* if write character */
  {
    key_scan = *(word far*)(KB_BUFF_PTR + old_kb_tail);
    old_kb_tail = *(word far*)KB_TAIL_PTR;
    keyhit = ((byte*)&key_scan)[0];

//    VIDEO_RAM[0] = attr + keyhit;
//    VIDEO_RAM[1] = attr + hex[(*(byte far*)KB_SHIFT_STATUS) & 15]; //keyhit;
    if ((keyhit == 0x0A) &&   //<Enter> returns 0x0A if Ctrl is pressed
        (((*(byte far*)KB_SHIFT_STATUS) & KB_CTRL) == KB_CTRL) )
    {
      if (!already_issued_command)
      {
        _BH = 0;      //250 ms delay -- minimum
        _BL = 0;      //30.0 repeats per second -- maximum
        _AX = 0x0305;
        geninterrupt(KB_SERVICE_INT);
        already_issued_command = 1;
      }
    }
    old_kb_tail = *(word far*)KB_TAIL_PTR;
  }
  else
  {
    already_issued_command = 0;
  }
}


extern word _stklen = 0x400;
extern word _heaplen = 0x100;


void main(void)
{
  old_kb_isr = getvect(KB_INTR);
  setvect(KB_INTR, kb_fast_int);
  keep(0,(_SS+(_SP/16)-_psp+10));
}
