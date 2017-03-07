#ifndef __Z30INT_HPP
#define __Z30INT_HPP
//===========================================================================
//  z30int.cpp
//  Uses interrupt-driven i/o for the z8530 in SDLC mode.
//---------------------------------------------------------------------------
#include <iostream.h>
#include <dos.h>
#include <string.h>

//#include <vectimp.h>           //templates for vectors

#ifndef __Z30UTIL_HPP
#include "z30util.hpp"
#endif

#ifndef __ONEBUFF_HPP
#include "onebuff.hpp"
#endif


#define Z30_BUFFER_SIZE         0x400   //1k

#define Z30_BAUD_RATE           19200.0

#define Z30_IRQ_LINE   3        //IRQ3, replaces COM2
#define Z30_INT_MASK   (1 << Z30_IRQ_LINE)  //mask for disabling/enabling
#define Z30_INT        (0x08+Z30_IRQ_LINE)  //interrupt vector number


#ifndef __CPPARGS               //used for interrupt routine C compatibility
#ifdef __cplusplus
  #define __CPPARGS ...
#else
  #define __CPPARGS
#endif
#endif


//===========================================================================
//  types...
//---------------------------------------------------------------------------

//===========================================================================
//  class Z30_Int
//---------------------------------------------------------------------------
class Z30_Int
{
public:

     //  The message receiver will be called when a complete message is
     //  received.  It must return a pointer to a new buffer.
     //  Since this is run from an interrupt routine, lengthy or
     //  DOS calls (including malloc()/new) should NOT be performed
     //  inside the message receiver.

  typedef byte* (*Message_Receiver)(byte* data, word count);

protected:

  Message_Receiver receive_message;

  byte* receive_buffer;
  word receive_count;

  byte* transmit_buffer;
  word transmit_count;

  byte installed;

  void Install_Z30_Interrupt_Handler(void);   //no output
  void Remove_Z30_Interrupt_Handler(void);    //no output

  void interrupt (*old_interrupt)(__CPPARGS);
  byte old_imr;          //interrupt mask register
  double baud_rate;
  byte local_address;
  byte address_search;

public:
  int  Installed(void) { return (int)installed; }

  void Initialize_Z30_Setup(
       byte* buffer,
       Message_Receiver receiver);

  double Baud_Rate(void) { return baud_rate; }

  double Baud_Rate(double br, int re_initialize = 1)
  {
    double old = baud_rate;
    baud_rate = br;
    if (re_initialize) Initialize_Z30_Setup(receive_buffer, receive_message);
    return old;
  }

  byte Local_Address(void) { return local_address; }

  byte Local_Address(byte address, int re_initialize = 1)
  {
    byte old_address = local_address;
    local_address = address;
    if (re_initialize) Initialize_Z30_Setup(receive_buffer, receive_message);
    return old_address;
  }

  byte Address_Searching(void) { return address_search; }

  void Search_For_Address(byte local_address, int re_initialize = 1)
  {
    this->local_address = local_address;
    address_search = 1;
    if (re_initialize) Initialize_Z30_Setup(receive_buffer, receive_message);
  }

  void No_Address_Search(int re_initialize = 1)
  {
    address_search = 0;
    if (re_initialize) Initialize_Z30_Setup(receive_buffer, receive_message);
  }

  int  Receive(
       byte* data,
       word count);  //negative ==> error

  void Transmit(
       byte* data,
       word count);

  void Use_Receive_Buffer(
       byte* buffer,
       Message_Receiver receiver)
  {
    receive_buffer = buffer;
    receive_message = receiver;
    receive_count = 0;
  }

  friend void interrupt z30_Interrupt_Handler(__CPPARGS);

  Z30_Int(void);

  ~Z30_Int(void);

};   //class Z30_Int


//===========================================================================
//  z30_Interrupt_Handler ISR
//---------------------------------------------------------------------------
void interrupt z30_Interrupt_Handler(__CPPARGS);


//===========================================================================
//  externals...
//---------------------------------------------------------------------------
extern Z30_Int z30;        //use this guy for access


#endif
