//===========================================================================
//  timer.cpp
//  This module contains functions to install a 1-ms long word counter.
//  Use InstallOneMillisecondTicker() to initialize the ticker, then use
//  GetTickerCount() and SetTickerCount() to keep track of the timer.
//  MAKE SURE YOU USE RemoveOneMillisecondTicker() AT THE END OF YOUR
//  PROGRAM!
//---------------------------------------------------------------------------
#include <stdio.h>
#include <dos.h>
#include <stdlib.h>
#include <conio.h>

#include "timer.hpp"

#ifdef __cplusplus
    #define __CPPARGS ...
#else
    #define __CPPARGS
#endif

static lword ticker_count = 0L;
static byte _55_ms_count = 55;
static byte isr;
void interrupt far (*old_system_timer)(__CPPARGS);

static byte counter_bit[] = { 0x02, 0x04, 0x08 };
static char* bcd_mode[] = { "Binary", "BCD" };
static char* mode[] =
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
static char* read_write_mode[] =
{
  "Counter latch",
  "LSB only",
  "MSB only",
  "LSB then MSB",
};


//===========================================================================
//  SystemTimer
//  This is the interrupt service routine (ISR) for the system timer
//  interrupt (IRQ0).  It's interrupt vector 0x08.
//---------------------------------------------------------------------------
void interrupt SystemTimer(__CPPARGS)  //no output
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
//  ChannelStatus
//  Returns the status byte for the given counter channel.
//---------------------------------------------------------------------------
byte ChannelStatus(    //status byte for specified counter
  byte counter)        //counter channel,, 0..2
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
}   //ChannelStatus


//===========================================================================
//  ConvertStatusToText
//  Converts a status byte into a readable string.
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
//  SetChannelCounter
//  Sets the count-down counter for the given channel, without changing the
//  counter's modes.
//---------------------------------------------------------------------------
byte SetChannelCounter(  //1 if successful, 0 if not
  byte counter,        //counter to set
  word count)          //count to be set
{
  byte msb;            //most significant byte of count
  byte lsb;            //least significant byte of count
  byte status;         //status of port before setting
  byte command;        //command to send to control word
  static byte counter_select[] = { 0x00, 0x40, 0x80 };

  if (counter > COUNTER_2) return 0;
  status = ChannelStatus(COUNTER_0);
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
}   //SetChannelCounter


//===========================================================================
//  GetTickerCount
//  This function returns the current ticker count.
//---------------------------------------------------------------------------
lword GetTickerCount(void)     //current value of 1-ms ticker
{
  lword count;

  disable();
  count = ticker_count;
  enable();
  return count;
}   //GetTickerCount


//===========================================================================
//  SetTickerCount
//  This function sets the one-millisecond ticker count.
//---------------------------------------------------------------------------
lword SetTickerCount(          //old value of 1-ms ticker
  lword count)                 //new count to be loaded
{
  word old_count;

  disable();
  old_count = ticker_count;
  ticker_count = count;
  enable();
  return old_count;
}   //SetTickerCount


//===========================================================================
//  RestoreToCMOSTime
//  Sets the DOS clock and system ticker to the time stored in the CMOS
//  clock.  During 1-ms ticker mode, the system ticker will lose about 5
//  seconds per hour.
//---------------------------------------------------------------------------
void RestoreToCMOSTime(void)   //no output
{
  static struct time tim;
  static struct date dat;

  tim.ti_sec  = FromBCD(ReadCMOS(SECOND));
  tim.ti_min  = FromBCD(ReadCMOS(MINUTE));
  tim.ti_hour = FromBCD(ReadCMOS(HOUR));
  dat.da_day  = FromBCD(ReadCMOS(DAY));
  dat.da_mon  = FromBCD(ReadCMOS(MONTH));
  dat.da_year = FromBCD(ReadCMOS(YEAR));
  if (dat.da_year < 80) dat.da_year += 2000; else dat.da_year += 1900;
  tim.ti_hund = 0;
  settime(&tim);
  setdate(&dat);
}   //RestoreToCMOSTime


//===========================================================================
//  InstallOneMillisecondTicker
//  Installs the 1-ms ticker by replacing the appropriate interrupt vector.
//  MAKE SURE YOU CALL RemoveOneMilliseconTicker() BEFORE LEAVING YOUR
//  PROGRAM!
//---------------------------------------------------------------------------
void InstallOneMillisecondTicker(void)   //no output
{
  old_system_timer = getvect(SYSTEM_TIMER_INT);
  setvect(SYSTEM_TIMER_INT, SystemTimer);
  SetChannelCounter(COUNTER_0, MILLISECOND_COUNT);
}   //InstallOneMillisecondTicker


//===========================================================================
//  RemoveOneMillisecondTicker
//  Removes the 1-ms ticker by restoring the appropriate interrupt vector.
//---------------------------------------------------------------------------
void RemoveOneMillisecondTicker(void)    //no output
{
  SetChannelCounter(COUNTER_0, 0x0000);  //back to real clock
  setvect(SYSTEM_TIMER_INT, old_system_timer);
}   //RemoveOneMillisecondTicker


