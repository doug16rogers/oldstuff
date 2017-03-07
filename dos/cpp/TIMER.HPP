#ifndef __TIMER_HPP
#define __TIMER_HPP
//===========================================================================
//  timer.cpp
//  This module contains functions to install a 1-ms long word counter.
//  Use InstallOneMillisecondTicker() to initialize the ticker, then use
//  GetTickerCount() and SetTickerCount() to keep track of the timer.
//  MAKE SURE YOU USE RemoveOneMillisecondTicker() AT THE END OF YOUR
//  PROGRAM!
//---------------------------------------------------------------------------


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


#define PIC            0x20    //programmable interrupt controller
#define READ_ISR       0x0B    //next read from PIC will be the ISR
#define NSEOI          0x20    //non-specific end-of-interrupt

#define IMR            0x21    //interrupt mask register

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
}; // PIT_COUNTER_MODES;

enum
{
  SELECT_COUNTER_0  = 0x0,
  SELECT_COUNTER_1  = 0x1,
  SELECT_COUNTER_2  = 0x2,
  READ_BACK_COMMAND = 0x3,
}; // PIT_SELECT_COUNTER_CODES;

enum
{
  COUNTER_LATCH   = 0x0,
  RW_LSB_ONLY     = 0x1,
  RW_MSB_ONLY     = 0x2,
  RW_LSB_THEN_MSB = 0x3,
}; // PIT_READ_WRITE_MODES;


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


//===========================================================================
//  ReadCMOS
//  Reads the BCD value of a CMOS register.  Use one of the registers (DAY,
//  MONTH, etc) #define'd above.
//---------------------------------------------------------------------------
byte inline ReadCMOS(byte reg)
{
  outportb(CMOS_PORT, reg);
  reg++;
  reg--;
  return inportb(CMOS_DATA);
}   //ReadCMOS


//===========================================================================
//  FromBCD
//  Converts a byte from BCD to binary.
//---------------------------------------------------------------------------
byte inline FromBCD(   //value in binary of 2-digit BCD byte
  byte bcd_value)      //BCD input byte
{
  return (10 * (bcd_value >> 4)) + (bcd_value & 0x0F);
}   //FromBCD


//===========================================================================
//  ChannelStatus
//  Returns the status byte for the given counter channel.
//---------------------------------------------------------------------------
byte ChannelStatus(    //status byte for specified counter
  byte counter);       //counter channel,, 0..2


//===========================================================================
//  ConvertStatusToText
//  Converts a status byte into a readable string.
//---------------------------------------------------------------------------
void ConvertStatusToText(   //no output
  byte status,         //status byte to display the fields of
  char* s);            //string to hold


//===========================================================================
//  SetChannelCounter
//  Sets the count-down counter for the given channel, without changing the
//  counter's modes.
//---------------------------------------------------------------------------
byte SetChannelCounter(  //1 if successful, 0 if not
  byte counter,        //counter to set
  word count);         //count to be set


//===========================================================================
//  GetTickerCount
//  This function returns the current ticker count.
//---------------------------------------------------------------------------
lword GetTickerCount(void);    //current value of 1-ms ticker


//===========================================================================
//  SetTickerCount
//  This function sets the one-millisecond ticker count.
//---------------------------------------------------------------------------
lword SetTickerCount(          //old value of 1-ms ticker
  lword count);                //new count to be loaded


//===========================================================================
//  RestoreToCMOSTime
//  Sets the DOS clock and system ticker to the time stored in the CMOS
//  clock.  During 1-ms ticker mode, the system ticker will lose about 5
//  seconds per hour.
//---------------------------------------------------------------------------
void RestoreToCMOSTime(void);  //no output


//===========================================================================
//  InstallOneMillisecondTicker
//  Installs the 1-ms ticker by replacing the appropriate interrupt vector.
//  MAKE SURE YOU CALL RemoveOneMilliseconTicker() BEFORE LEAVING YOUR
//  PROGRAM!
//---------------------------------------------------------------------------
void InstallOneMillisecondTicker(void);  //no output


//===========================================================================
//  RemoveOneMillisecondTicker
//  Removes the 1-ms ticker by restoring the appropriate interrupt vector.
//---------------------------------------------------------------------------
void RemoveOneMillisecondTicker(void);   //no output


#endif
