#include <stdio.h>
#include <dos.h>
#include <stdlib.h>
#include <conio.h>


#ifndef bit
#define bit  unsigned
#endif

#ifndef byte
#define byte unsigned char
#endif

#ifndef word
#define word unsigned int
#endif

#ifndef lword
#define lword unsigned long int
#endif

#define SYSTEM_TIMER_INT 0x08
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


#define PIC            0x20    //programmable interrupt controller
#define READ_ISR       0x0B    //next read from PIC will be the ISR
#define NSEOI          0x20    //non-specific end-of-interrupt

#define PIT            0x40    //programmable interval timer
#define COUNTER_0      0
#define COUNTER_1      1
#define COUNTER_2      2
#define PIT_CONTROL    3

enum
{
  INT_ON_TERMINAL_COUNT     = 0x0,
  HW_RETRIGGERABLE_ONE_SHOT = 0x1,
  REAL_TIME_CLOCK           = 0x2,
  SQUARE_WAVE               = 0x3,
  SW_TRIGGERED_STROBE       = 0x4,
  HW_TRIGGERED_STROBE       = 0x5,
} PIT_COUNTER_MODES;

enum
{
  SELECT_COUNTER_0  = 0x0,
  SELECT_COUNTER_1  = 0x1,
  SELECT_COUNTER_2  = 0x2,
  READ_BACK_COMMAND = 0x3,
} PIT_SELECT_COUNTER_CODES;

enum
{
  COUNTER_LATCH   = 0x0,
  RW_LSB_ONLY     = 0x1,
  RW_MSB_ONLY     = 0x2,
  RW_LSB_THEN_MSB = 0x3,
} PIT_READ_WRITE_MODES;


#define MILLISECOND_COUNT  1193      //1193180 Hz / (1000 ms/s)

typedef struct
{
  bit zero_0:1;        //must be 0
  bit count0:1;        //1 = read-back count/status for counter 0
  bit count1:1;        //1 = read-back count/status for counter 1
  bit count2:1;        //1 = read-back count/status for counter 2
  bit not_status:1;    //0 = latch status for selected counter(s)
  bit not_count:1;     //0 = latch count for selected counter(s)
  bit one_6:1;         //must be 1
  bit one_7:1;         //must be 1
} PIT_Read_Back_Command;

typedef struct
{
  bit bcd_mode:1;      //1 = counter is 4-decade BCD, 0 = 16-bit binary
  bit mode:3;          //counter mode, see enum above
  bit read_write:2;    //read-write mode, see enum
  bit null_count:1;    //1 = null count, 0 = count available
  bit output:1;        //output pin value
} PIT_Status_Byte;

typedef struct
{
  bit bcd_mode:1;      //1 = counter is 4-decade BCD, 0 = 16-bit binary
  bit mode:3;          //counter mode, see enum
  bit read_write:2;    //read-write mode, see enum
  bit select:2;        //counter select/read-back command, see enum
} PIT_Control_Word;


byte _val = 0x00;
byte counter_bit[] = { 0x02, 0x04, 0x08 };
char* bcd_mode[] = { "Binary", "BCD" };
char* mode[] =
{
  "Int on terminal count",
  "HW retriggerable one-shot",
  "Real-time clock",
  "Square wave",
  "SW-triggered strobe",
  "HW-triggered strobe",
  "Unknown mode 6",
  "Unknown mode 7",
};
char* read_write_mode[] =
{
  "Counter latch",
  "LSB only",
  "MSB only",
  "LSB then MSB",
};



lword ticker_count = 0L;
byte _55_ms_count = 55;
byte isr;
void interrupt (*old_system_timer)(void);


//===========================================================================
//  SystemTimer ISR
//---------------------------------------------------------------------------
void interrupt SystemTimer(void)
{
  disable();
  ticker_count++;
  _55_ms_count--;
  if (_55_ms_count == 0)
  {
    _55_ms_count = 55;
    old_system_timer();
  }
  else
  {
    outportb(PIC, NSEOI);
//    enable();
  }
}   //SystemTimer ISR


//===========================================================================
//  GetTickerCount
//---------------------------------------------------------------------------
word GetTickerCount(void)      //current value of 1-ms ticker
{
  word count;

  disable();
  count = ticker_count;
  enable();
  return count;
}   //GetTickerCount


//===========================================================================
//  SetTickerCount
//---------------------------------------------------------------------------
word SetTickerCount(           //old value of 1-ms ticker
  word count)                  //new count to be loaded
{
  word old_count;

  disable();
  old_count = ticker_count;
  ticker_count = count;
  enable();
  return old_count;
}   //SetTickerCount


//===========================================================================
//  CounterStatus
//---------------------------------------------------------------------------
byte CounterStatus(    //status byte for specified counter
  byte counter)        //counter channel
{
  byte status;         //status, as read from counter
  byte command;        //command to send to control register

  if (counter > COUNTER_2) return 0;
  command = 0xE0 | counter_bit[counter];
  disable();
  outportb(PIT+PIT_CONTROL, command);
  goto kill_time;  kill_time:
  status = inportb(PIT+counter);
  enable();
  return status;
}   //CounterStatus


//===========================================================================
//  SetCounter
//---------------------------------------------------------------------------
byte SetCounter(       //1 if successful, 0 if not
  byte counter,        //counter to set
  word count)          //count to be set
{
  byte msb;            //most significant byte of count
  byte lsb;            //least significant byte of count
  byte status;         //status of port before setting
  byte command;        //command to send to control word
  static byte counter_select[] = { 0x00, 0x40, 0x80 };

  if (counter > COUNTER_2) return 0;
  status = CounterStatus(COUNTER_0);
  command = status & 0x0F;     //keep the mode and BCD setting
  command |= counter_select[counter];  //select proper channel
  command |= 0x30;             //read/write lsb then msb
  msb = (count >> 8);
  lsb = (count & 0xFF);
  disable();
  outportb(PIT+PIT_CONTROL, command);
  goto kill_time0;  kill_time0:
  outportb(PIT+counter, lsb);
  goto kill_time1;  kill_time1:
  outportb(PIT+counter, msb);
  enable();
  return 1;
}   //SetCounter


//===========================================================================
//  ConvertStatusToText
//---------------------------------------------------------------------------
void ConvertStatusToText(   //no output
  byte status,         //status byte to display the fields of
  char* s)             //string to hold
{
  PIT_Status_Byte* stat = (PIT_Status_Byte*) &status;

  sprintf(s, "mode=%s, bcd=%s, r/w=%s, null=%d, output=%d",
    mode[stat->mode],
    bcd_mode[stat->bcd_mode],
    read_write_mode[stat->read_write],
    stat->null_count,
    stat->output
  );
}   //ConvertStatusToText


//===========================================================================
//  FromBCD
//---------------------------------------------------------------------------
byte FromBCD(          //value in binary of 2-digit BCD byte
  byte bcd_value)      //BCD input byte
{
  return (10 * (bcd_value >> 4)) + (bcd_value & 0x0F);
}   //FromBCD


//===========================================================================
//  FromBCD
//---------------------------------------------------------------------------
void RestoreToCMOSTime(void)   //no output
{
  static struct time tim;
  static struct date dat;

  READ_CMOS(tim.ti_sec,  SECOND);
  READ_CMOS(tim.ti_min,  MINUTE);
  READ_CMOS(tim.ti_hour, HOUR);
  READ_CMOS(dat.da_day,  DAY);
  READ_CMOS(dat.da_mon,  MONTH);
  READ_CMOS(dat.da_year, YEAR);
  tim.ti_sec  = FromBCD(tim.ti_sec);
  tim.ti_min  = FromBCD(tim.ti_min);
  tim.ti_hour = FromBCD(tim.ti_hour);
  dat.da_day  = FromBCD(dat.da_day);
  dat.da_mon  = FromBCD(dat.da_mon);
  dat.da_year = FromBCD(dat.da_year);
  if (dat.da_year < 80) dat.da_year += 2000; else dat.da_year += 1900;
  tim.ti_hund = 0;
  settime(&tim);
  setdate(&dat);
}   //RestoreToCMOSTime


//===========================================================================
//  main
//---------------------------------------------------------------------------
void main(void)
{
  word count;
  byte done = 0;

  old_system_timer = getvect(SYSTEM_TIMER_INT);
  setvect(SYSTEM_TIMER_INT, SystemTimer);
  SetCounter(COUNTER_0, MILLISECOND_COUNT);

  while (!done)
  {
    count = GetTickerCount();
    printf("Count = %05u\r", count);
    if (kbhit())
    {
      switch (getch())
      {
      case 0x1b: done = 1; break;
      case '0': SetTickerCount(0); break;
      }
    }   //if
  }   //while
  printf("\n");

  SetCounter(COUNTER_0, 0x0000);  //back to real clock
  setvect(SYSTEM_TIMER_INT, old_system_timer);

  RestoreToCMOSTime();
}   //main


