#include <stdio.h>
#include <dos.h>
#include <string.h>


//===========================================================================
//  Macro definitions...
//---------------------------------------------------------------------------
#ifndef byte
#define byte unsigned char
#endif

#ifndef word
#define word unsigned int
#endif

#ifndef lword
#define lword unsigned long int
#endif


#ifndef __CPPARGS              //used for interrupt routine C compatibility
#ifdef __cplusplus
  #define __CPPARGS ...
#else
  #define __CPPARGS
#endif
#endif

#define VIDEO_INT      0x10
#define CLOCK_INT      0x1A
#define DOS_INT        0x21
#define IDLE_INT       0x28
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


//===========================================================================
//  typedef definitions...
//---------------------------------------------------------------------------

typedef void interrupt (*intr_function)(__CPPARGS);

typedef struct
{
  byte  int_20_instructions[0x02];
  word  memory_size;
  byte  reserved_04;
  byte  call_to_DOS_displatcher[0x05];
  lword int_22_address;                //Terminate
  lword int_23_address;                //Ctrl-C
  lword int_24_address;                //Critical error
  byte  reserved_16[0x16];
  word  environment_segment;
  byte  reserved_2E[0x22];
  byte  int_21_retf_instructions[0x03];
  byte  reserved_53[0x09];
  byte  fcb_1[0x10];
  byte  fcb_2[0x14];
  union
  {
    byte commandline_data[0x80];
    byte default_dta[0x80];
  } u;
} PSP;


//===========================================================================
//  GetDOSEnvironment
//---------------------------------------------------------------------------
char far *GetDOSEnvironment(void)
{
  PSP far *psp;
  word es;

  _AX = 0x352e;                //secret password
  geninterrupt(DOS_INT);
  es = _ES;
  psp = (PSP far*) MK_FP(es,0);
  return (char far*) MK_FP(psp->environment_segment, 0);
}   //GetDOSEnvironment


//===========================================================================
//  main
//---------------------------------------------------------------------------
void main(void)
{
  char far *p;

  for (p = GetDOSEnvironment(); *p; p += strlen(p)+1)
  {
    printf("%s\n", p);
  }
}   //main
