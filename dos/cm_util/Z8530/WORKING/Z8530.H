#ifndef __z8530
#define __z8530
/*****************************************************************************
*
*  TITLE:        Zilog 8530
*
*  DESCRIPTION:  The class "Z8530"
*                provides types and methods suitable
*                for access to a Zilog 8530 chip.
*
*                Certain constants must be set to
*                appropriate values (see below and
*                "Class_D1.cpp").
*
*  *k "%n"
*  FILE NAME:    "Z8530.H"
*
*  *k "%v"
*  VERSION:      "3"
*
*  REFERENCE:    Zilog 8530 programmer's reference.
*
*****************************************************************************/

#include <dos.h>


#ifndef __icitypes
#include "icitypes.h"
#endif


class Z8530
{

public:

   typedef enum
   {
      A = 0,
      B = 1,

   } CHANNEL;


   typedef enum
   {
      RR0 = 0x00,
      RR1 = 0x01,
      RR2 = 0x02,
      RR3 = 0x03,
      RR8 = 0x08,
      RR10 = 0x0A,
      RR12 = 0x0C,
      RR13 = 0x0D,
      RR15 = 0x0F,

   } READ_REGISTER;


   typedef enum
   {
      WR0 = 0x00,
      WR1 = 0x01,
      WR2 = 0x02,
      WR3 = 0x03,
      WR4 = 0x04,
      WR5 = 0x05,
      WR6 = 0x06,
      WR7 = 0x07,
      WR8 = 0x08,
      WR9 = 0x09,
      WR10 = 0x0A,
      WR11 = 0x0B,
      WR12 = 0x0C,
      WR13 = 0x0D,
      WR14 = 0x0E,
      WR15 = 0x0F,

      WREND = 0xFF,

   } WRITE_REGISTER;


   typedef struct
   {
      Z8530::WRITE_REGISTER _register;  // register for write, WREND for last
      UINT8 value;                      // value to write

   } WRITE_VALUE;


protected:

   UINT16 io_base;      // base i/o address (port) of this chip


/*-------------------------------------------------------------------------
   Source: "Class_D1.cpp"
  -------------------------------------------------------------------------*/

public:

   static const UINT16 control_offset;
   static const UINT16 data_offset;
   static UINT32 clock_speed;           // in Herz


/*-------------------------------------------------------------------------
   Source: "Baud_Ra1.cpp"
  -------------------------------------------------------------------------*/

public:

   UINT32 Baud_Rate(            // baud rate for channel

        Z8530::CHANNEL channel);    // channel for request

/*-------------------------------------------------------------------------
   Source: "Change_1.cpp"
  -------------------------------------------------------------------------*/

public:

   UINT16 Change_IO_Base(       // previous base IO address for the 8530 chip

        UINT16 new_base);        // new i/o base address for chip

/*-------------------------------------------------------------------------
   Source: "IO_Base1.cpp"
  -------------------------------------------------------------------------*/

public:

   UINT16 IO_Base();    // base IO address for the 8530 chip

/*-------------------------------------------------------------------------
   Source: "Read1.cpp"
  -------------------------------------------------------------------------*/

public:

   UINT8 Read(                  // value read from the given register

        Z8530::CHANNEL channel,             // channel (A/B) for read
        Z8530::READ_REGISTER _register);    // register from which to read

/*-------------------------------------------------------------------------
   Source: "Read_Da1.cpp"
  -------------------------------------------------------------------------*/

public:

   UINT8 Read_Data(             // data register (RR8) value for channel

        Z8530::CHANNEL channel);        // channel (A/B) for read

/*-------------------------------------------------------------------------
   Source: "Set_Bau1.cpp"
  -------------------------------------------------------------------------*/

public:

   UINT32 Set_Baud_Rate(        // baud rate actually set

        Z8530::CHANNEL channel,     // channel to change
        UINT32 baud_rate);          // new baud rate for channel

/*-------------------------------------------------------------------------
   Source: "Write1.cpp"
  -------------------------------------------------------------------------*/

public:

   void Write(

        Z8530::CHANNEL channel,              // channel (A/B) for write
        Z8530::WRITE_REGISTER _register,     // register for write
        UINT8 value);                        // value to write to register

/*-------------------------------------------------------------------------
   Source: "Write_D1.cpp"
  -------------------------------------------------------------------------*/

public:

   void Write_Data(

        Z8530::CHANNEL channel, // channel (A/B) on which to write data byte
        UINT8 value);           // value to write to data register

/*-------------------------------------------------------------------------
   Source: "Write_S1.cpp"
  -------------------------------------------------------------------------*/

public:

   void Write_Set(

        Z8530::CHANNEL channel,         // channel on which to write data byte
        Z8530::WRITE_VALUE* write_set,  // Z8530::WREND-terminated list
        UINT count = 0);                // count of values to write, 0 =
                                        //    terminate when WREND is register
/*-------------------------------------------------------------------------
   Source: "Z85301.cpp"
  -------------------------------------------------------------------------*/

public:

   Z8530(

        UINT16 io_base);        // base IO address for the 8530 chip

};   // class Z8530


#endif

