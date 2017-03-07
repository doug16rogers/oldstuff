#include <stdio.h>
#include <process.h>
#include <string.h>
#include <dos.h>
#include <conio.h>


#ifndef byte
#define byte unsigned char
#endif

#ifndef word
#define word unsigned int
#endif


#define PROGRAM_NAME   "dr clock"

typedef struct
{
  char name[12];
  word intr_num;
} Program_ID;

Program_ID program_id = { PROGRAM_NAME, 0 };





#define VIDEO_INT      0x10
#define CLOCK_INT      0x1A
#define FIRST_USER_INT 0x60
#define LAST_USER_INT  0x66

#define CMOS_PORT      0x70
#define CMOS_DATA      0x71

#define SECOND         0x00
#define MINUTE         0x02
#define HOUR           0x04
#define WEEK_DAY       0x06
#define DAY            0x07
#define MONTH          0x08
#define YEAR           0x09

#define READ_CMOS(value,reg) \
  outportb(CMOS_PORT, reg);  \
  asm nop;                   \
  value = inportb(CMOS_DATA)

#define ATTR ((BLUE<<4)+YELLOW)


typedef struct
{
  char c;
  unsigned char a;
} attr_char;


typedef void interrupt (*intr_function)();

void interrupt (*clock_func)() = NULL;
void interrupt (*user_func)() = NULL;


word name_intr = 0;
word user_intr = 0;


word vidseg = 0xB800;
word old_bp;

word year;
byte month;
byte day;

byte hour;
byte minute;
byte second;
byte old_second = 0xff;


attr_char s[9] =
{
  { '0', ATTR },
  { '0', ATTR },
  { ':', ATTR },
  { '0', ATTR },
  { '0', ATTR },
  { ':', ATTR },
  { '0', ATTR },
  { '0', ATTR },
  {0}
};   //s



void interrupt DisplayClock(void)
{
  READ_CMOS(second, SECOND);
  if (second != old_second)
  {
    old_second = second;
    READ_CMOS(hour, HOUR);
    READ_CMOS(minute, MINUTE);
    if (hour >= 0x20)
    {
      hour &= 0x0f;
      hour += 0x12 + 0x06;
    }
    s[0].c = '0' + ((hour >> 4) & 3);
    s[1].c = '0' + (hour & 15);
    s[3].c = '0' + (minute >> 4);
    s[4].c = '0' + (minute & 15);
    s[6].c = '0' + (second >> 4);
    s[7].c = '0' + (second & 15);
    old_bp = _BP;
    _ES = FP_SEG(s);
    _BP = FP_OFF(s);
    _AX = 0x01302;
    _BX = 0;
    _CX = 8;
    _DX = 72;
    geninterrupt(VIDEO_INT);
    _BP = old_bp;
  }   //if changed
}   //DisplayClock


void main(void)
{
  word intr_num;

  _AH = 0xf;
  geninterrupt(VIDEO_INT);
  if (_AL == 0x07) vidseg = 0xB000;

  for (intr_num = FIRST_USER_INT; intr_num <= LAST_USER_INT; intr_num++)
  {
    if (getvect(intr_num) == NULL)
    {
      if (name_intr == 0)
        name_intr = intr_num;
      else
      {
        user_intr = intr_num;
        break;
      }
    }
  }   //for each user interrupt

  printf("Name is int #%02X; User is int #%02X.\n", name_intr, user_intr);


//  oldfunc = getvect(0x28);
//  setvect(0x28,DisplayClock);
//  keep(0, _SS + 10 - _psp);
}   //main


