//===========================================================================
//  z30int.cpp
//  Uses interrupt-driven i/o for the z8530 in SDLC mode.
//---------------------------------------------------------------------------
#include <stdio.h>
#include <dos.h>
#include <conio.h>
#include <string.h>
#include <iomanip.h>

#include "timer.hpp"
#include "z30int.hpp"
#include "onebuff.hpp"


//===========================================================================
//  globals...
//---------------------------------------------------------------------------
Z30_Int z30;                   //create instance of Z30 interrupt service


//===========================================================================
//  Z30_Int::Z30_Int
//  Initializes the class data, sets up the z8530 and installs the interrupt
//  service routine for SDLC.
//---------------------------------------------------------------------------
Z30_Int:: Z30_Int(void)
{
  receive_message = NULL;
  installed = 0;
  baud_rate = Z30_BAUD_RATE;
  if (z30check())
  {
    installed = 1;
    Install_Z30_Interrupt_Handler();
//  Initialize_Z30_Setup();     //must be performed by user
  }
}   //Z30_Int::Z30_Int(void)


//===========================================================================
//  Z30_Int::~Z30_Int
//  Resets the z8530 chip and removes the interrupt handler.
//---------------------------------------------------------------------------
Z30_Int::~Z30_Int(void)
{
  if (installed)
  {
    WriteA(WR9, 0xC0);         //reset z8530 hardware
    Remove_Z30_Interrupt_Handler();
  }
}   //Z30_Int::~Z30_Int(void)


//===========================================================================
//  Z30_Int:: Transmit
//  Tells the ISR that sending is ok, then writes a byte out to start the
//  transmission process.
//---------------------------------------------------------------------------
void Z30_Int::Transmit(
  byte* data,
  word count)
{
  if (count == 0)
  {
    return; //------------------------------------------------> return!
  }

  if (transmit_count != 0)              //transmission already in progress
  {
    return; //------------------------------------------------> return!
  }

  transmit_buffer = data;
  transmit_count = count;

  if (installed)
  {
    WriteA(WR0 ,0xC0);         //reset tx underrun error (just in case)
    WriteA(WR0 ,0x30);         //reset error/special conditions
    WriteA(WR0 ,0x80);         //reset tx CRC
    WriteA(WR0 ,0x80);         //reset tx CRC
    WriteDataA(*transmit_buffer++);
    transmit_count--;
  }
  else
  {
    if (receive_buffer != NULL)
    {
      while (transmit_count)
      {
        receive_buffer[receive_count++] = *transmit_buffer++;
        receive_count++;
        transmit_count--;
      }

      if (receive_message != NULL)
      {
        receive_buffer = receive_message(receive_buffer, receive_count);
      }
    }
  }   //else not installed
}   //Z30_Int:: Transmit(data, count)


//===========================================================================
//  Z30_Int::InitializeZ30Setup
//  Initializes the SDLC port for interrupt-driven i/o by writing to the
//  z30's registers in the order prescribed in Zilog's data book.
//---------------------------------------------------------------------------
void Z30_Int::Initialize_Z30_Setup(
       byte* buffer,
       Message_Receiver receiver)
{
  Use_Receive_Buffer(buffer, receiver);

  if (!installed) return;          //check that z30 is actually there
//modes...
  WriteA(WR9 ,0xc0);               //hardware reset
  WriteA(WR0 ,0x00);               //reset, cuz that's what book says
  WriteA(WR4 ,0x20);               //x1 clock, SDLC mode
  WriteA(WR1 ,0x00);               //no DMA initialization
  WriteA(WR2 ,0);                  //unused (?) interrupt vector
  if (address_search)
    WriteA(WR3 ,0xcc);             //no rx 4 now,rx 8 data,addr search
  else
    WriteA(WR3 ,0xc8);             //no rx 4 now,rx 8 data,no addr search
  WriteA(WR5 ,0x61);               //no DTR,tx 8 data,no tx 4 now,CRC on
  WriteA(WR6 ,local_address);      //our address
  WriteA(WR7 ,0x7e);               //sync character, 6 1s
  WriteA(WR9 ,0x02);               //no vector on interrupt, use V321 stat
  WriteA(WR10,0x00);               //NRZ, no loop mode
  WriteA(WR11,0x16);  //15?        //tx clock=BRG,rx clock=RTXC,TRxC=Tx
  z30setbaud(A,1, baud_rate);          //set baud rate
  WriteA(WR14,0x02);               //no BR 4 now, local loopback
  WriteA(WR14,0x02);               //again? just follow the book, ma'am
//enables...
  WriteA(WR14,0x03);               //enable BR generator, local loopback
  WriteA(WR3 ,0xc9);               //rx enable
  WriteA(WR5 ,0x69);               //no DTR,tx 8 data bits,enable tx,CRC on
  WriteA(WR0 ,0x80);               //reset tx CRC
  WriteA(WR1 ,0x13);               //rx/tx/ext interrupts
//interrupt...
  WriteA(WR15,0x40);               //allow tx underrun to interrupt on ext
  WriteA(WR0 ,0x10);               //reset ext/status
  WriteA(WR0 ,0x10);               //reset ext/status, again
  WriteA(WR1 ,0x13);               //rx/tx interrupts, again?
  WriteA(WR9 ,0x0a);               //master interrupt enables, no vector
  ReadDataA();                     //clear receive data register
  ReadDataA();                     //clear receive data register
  ReadDataA();                     //clear receive data register
}   //Z30_Int:: Initialize_Z30_Setup


//===========================================================================
//  Z30_Int:: Install_Z30_Interrupt_Handler
//  Installs the z30 ISR.
//---------------------------------------------------------------------------
void Z30_Int:: Install_Z30_Interrupt_Handler(void)  //no output
{
  old_interrupt = getvect(Z30_INT);
  old_imr = inportb(IMR);
  goto kill_time0;  kill_time0:
  outportb(IMR, old_imr | Z30_INT_MASK);     //disable our interrupt
  setvect(Z30_INT, z30_Interrupt_Handler);   //replace interrupt vector
  outportb(IMR, old_imr & ~Z30_INT_MASK);    //enable our interrupt
}   //Z30_Int:: Install_Z30_Interrupt_Handler


//===========================================================================
//  Z30_Int:: Remove_Z30_Interrupt_Handler
//  Removes the z30 ISR.
//---------------------------------------------------------------------------
void Z30_Int:: Remove_Z30_Interrupt_Handler(void)   //no output
{
  byte imr;

  WriteA( WR9, 0xC0);      //reset the z30 chip (to stop interrupts)

  imr = inportb(IMR);
  goto kill_time0;  kill_time0:
  outportb(IMR, imr | Z30_INT_MASK);   //disable our interrupt
  setvect(Z30_INT, old_interrupt);     //replace interrupt vector
  if (old_imr & Z30_INT_MASK)          //restore old IMR setting
    outportb(IMR, imr | Z30_INT_MASK);
  else
    outportb(IMR, imr & ~Z30_INT_MASK);
}   //Z30_Int:: Remove_Z30_InterruptHandler


//===========================================================================
//  z30_Interrupt_Handler ISR
//---------------------------------------------------------------------------
void interrupt z30_Interrupt_Handler(__CPPARGS)
{
  static byte frame_status;
  static byte int_status;

  disable();

  while (((int_status = ReadB(RR2)) & 0x08) != 0x00)   //if chan A int pending
  {
    int_status >>= 1;                  //move code down lsb
    switch (int_status)
    {
    case TX_EMPTY_A:
      if ((z30.transmit_count > 0) && (ReadA(RR0) & 0x04))
      {
        WriteDataA(*z30.transmit_buffer++);
        z30.transmit_count--;
      }
      else
      {
        WriteA(WR0, 0x28);             //reset tx int pending
      }
      break;

    case RX_AVAIL_A:
      frame_status = ReadA(RR1);
      if (frame_status & 0x20)
      {
        WriteA(WR0, 0x30);
        WriteA(WR0, 0x10);
        ReadDataA();
        ReadDataA();
        ReadDataA();
        if (z30.receive_message != NULL)
        {
          z30.receive_buffer = z30.receive_message(
                                    z30.receive_buffer, z30.receive_count);
        }
        z30.receive_count = 0;
      }
      else
      {
        z30.receive_buffer[z30.receive_count++] = ReadDataA();
      }
      break;

    case EXT_STAT_A:
      z30.transmit_count = 0;
      WriteA(WR0, 0x10);
      WriteA(WR0, 0x28);               //reset tx int pending ?????
      break;

    default:
      ReadDataA();
      ReadDataA();
      ReadDataA();
      WriteA(WR0, 0x30);
      WriteA(WR0, 0x10);
      if (z30.receive_message != NULL)
      {
        z30.receive_buffer = z30.receive_message(
                                  z30.receive_buffer, z30.receive_count);
      }
      z30.receive_count = 0;

    }   //switch

  }   //while interrupts are pending

  WriteA(WR0 ,0x38);           //reset highest interrupt-under-service
  outportb(PIC, NSEOI);        //issue non-specific end-of-interrupt
  enable();
}   //z30InterruptHandler
