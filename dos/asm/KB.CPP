#include <dos.h>

#include "kb.hpp"


KB kbin;               //keyboard input


int KB::hit (void)
{
  asm mov ah,0x01      //check-for-keystroke function
  asm int KB_INT       //keyboard interrupt
  asm mov ax,0         //clear for nope!
  asm jz  end          //yep, nope was correct, so skip to end
  asm inc ax           //set for yep!
end:
  return _AX;
}   //int KB::hit(void)


char KB::Get(void)
{
  union REGS r;

  r.h.ah = 0x00;       //read-keyboard-character function
  int86(KB_INT,&r,&r); //call keyboard interrupt
  if (r.h.al) return (char) r.h.al;
  if (r.h.ah < 0x80) return -((char)r.h.ah);
  switch (r.h.ah)
  {
  case 0x80: return A_9;
  case 0x81: return A_0;
  case 0x82: return A_MINUS;
  case 0x83: return A_EQUAL;
  case 0x84: return C_PGUP;
  case 0x87: return S_F11;
  case 0x88: return S_F12;
  case 0x89: return C_F11;
  case 0x8A: return C_F12;
  case 0x8B: return A_F11;
  case 0x8C: return A_F12;
  case 0xA5: return A_TAB;
  case 0xD9: return _F11;
  case 0xDA: return _F12;
  }   //switch
  return 0;
}   //char KB::Get(void)

KB& KB::operator << (char& c)
{
  union REGS r;

  r.h.ah = 0x05;
  r.h.cl = c;
  r.h.ch = 0;
  if (c < 0)
  {
    r.h.cl = 0;
    switch (c)
    {
    case A_9 :    r.h.cl = 0x80;  break;
    case A_0:     r.h.ch = 0x81;  break;
    case A_MINUS: r.h.ch = 0x82;  break;
    case A_EQUAL: r.h.ch = 0x83;  break;
    case C_PGUP:  r.h.ch = 0x84;  break;
    case S_F11:   r.h.ch = 0x87;  break;
    case S_F12:   r.h.ch = 0x88;  break;
    case C_F11:   r.h.ch = 0x89;  break;
    case C_F12:   r.h.ch = 0x8A;  break;
    case A_F11:   r.h.ch = 0x8B;  break;
    case A_F12:   r.h.ch = 0x8C;  break;
    case A_TAB:   r.h.ch = 0xA5;  break;
    case _F11:    r.h.ch = 0xD9;  break;
    case _F12:    r.h.ch = 0xDA;  break;
    default:
      r.h.ch = (unsigned char) -c;
    }
  }   //if special character
  int86(KB_INT,&r,&r);
  return *this;
}   //KB& KB::operator << (char& c)

