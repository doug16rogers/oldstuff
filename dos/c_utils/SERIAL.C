/***			    Issue: B
************************************************************************
* File:  serial.c
*
************************************************************************
* Description:
*
* Contains the serial communications procedures.
*
************************************************************************
* Revision History:
*
* Date     Issue        Author          Reason
*
* 08/13/89   A		Eric Harmon	New.
* 09/02/89   B		Eric Harmon	Added the clear_port function.
*
************************************************************************
***/
#include <stdio.h>
#include <dos.h>
#include <conio.h>

#define  MSC
#include "sysdef.h"              /* Global definitions           */
#include "serial.h"              /* Structure declarations       */
#include "serdef.h"		 /* Serial function declarations */
/***
*********************************************************************
* Structure:  serial
*
*********************************************************************
* Description:
*
* Structure containing the serial port defaults and previous
* settings.
*
*********************************************************************
* Changes:  None
*
*********************************************************************
* Notes:  None
*
*********************************************************************
***/
struct serial s_ports[1] =
{
{
	0,				/* uart base		*/
	0,				/* user format		*/
	0,				/* user baud		*/
	9600,				/* start baud rate	*/
	EVEN_MASK+ONE_MASK+SEVEN_MASK,  /* start data format    */
	DTR_MASK+RTS_MASK,		/* default 232 pins on  */
	VIRGIN,				/* init flag		*/
	1,				/* user232		*/
	0,                              /* Interrupt number	*/
	NULL,				/* Old interrupt vect   */
	s_intsrv1,			/* New interrupt vect	*/
	COMM1,				/* starting comm	*/
	0,				/* Head of buffer       */
	0,				/* Tail of buffer       */
	0,
	""				/* Empty buffer		*/
}
};
/***
**********************************************************************
* Function:  setdtr
*
**********************************************************************
* Description:
*
* Places the DTR on the specified port high or low
*
**********************************************************************
* Calling Sequence:
*
* (void)setdtr(port, state)
*
**********************************************************************
* Inputs:
*
* port		Serial port to be changed
* state 	State to place DTR in
*
**********************************************************************
* Outputs:  None
*
**********************************************************************
* Notes:
*
* Two constants have been provided in sysdef.h:
*   ON	- Places the DTR high
*   OFF - Places the DTR low
*
**********************************************************************
***/
void setdtr(port, state)

        int port;
	int state;

{
	unsigned dtrsign;       /* Current DTR state               */

	/***********************************************************/
	/* Get current DTR state                                   */
	/***********************************************************/
	dtrsign = INPORTW(s_ports[port].uart_base + SIO_MCR);
	dtrsign &= 0xFF;

	/***********************************************************/
	/* Toggle DTR bit and send back to the serial port         */
	/***********************************************************/

	if (state == ON)
	  OUTPORTW(s_ports[port].uart_base + SIO_MCR, dtrsign | DTRON);
	else
	  OUTPORTW(s_ports[port].uart_base + SIO_MCR, dtrsign & DTROFF);
}
/***
**********************************************************************
* Function:  s_rcv
*
**********************************************************************
* Description:
*
* Retreives a character directly from the serial port.
*
**********************************************************************
* Calling Sequence:
*
* ret_val = (unsigned char)s_rcv(port)
*
**********************************************************************
* Inputs:
*
* port		int - Serial port to be changed.
*
**********************************************************************
* Outputs:
*
* ret_val	unsinged char - Character retreived from the serial
*				port.
*
**********************************************************************
* Notes:
*
* The port should first be checked for an incoming character using
* the funtion s_rcvstat().
*
**********************************************************************
***/
unsigned char s_rcv(port)

	int port;

{
	if (s_ports[port].uart_base != 0)
	  return(INPORTB(s_ports[port].uart_base + RCV));
	else
	  return(NULL);
}
/***
**********************************************************************
* Function:  s_intsrv1
*
**********************************************************************
* Description:
*
* Handles the serial port hardware interrupt.
*
**********************************************************************
* Calling Sequence:
*
* Initiated by the serial hardware.
*
**********************************************************************
* Inputs:  None
*
**********************************************************************
* Outputs: None
*
**********************************************************************
* Notes:
*
* The interrupt should first be set using the s_config() function and
* should be removed out using s_unconfig().  If the function is not
* removed than total system failure will most likely occur after
* program termination.
*
**********************************************************************
***/
void INTERRUPT s_intsrv1()
{
/*        DISABLE();            */
	_asm { CLI  }
	if (s_ports[0].buffcnt > SER_BUFF)
	  return;
	s_ports[0].buffer[s_ports[0].tail] = s_rcv(0);
	s_ports[0].tail++;
	if (s_ports[0].tail >= SER_BUFF)
	  s_ports[0].tail = 0;
	s_ports[0].buffcnt++;
	_asm {
	      MOV        AL,20H
	      OUT        20H,AL
	      STI
	    }
/*
	OUTPORTB(0x20, 0x20);
	ENABLE();
*/
}
/***
**********************************************************************
* Function:  s_rdbuff
*
**********************************************************************
* Description:
*
* Reads a character from the serial interrupt buffer. This is the
* function that should be utilized in user programs to receive
* characters.
*
**********************************************************************
* Calling Sequence:
*
* ret_val = (int)s_rdbuff(port)
*
**********************************************************************
* Inputs:
*
* port		int - Serial port to be changed.
*
**********************************************************************
* Outputs:
*
* ret_val	int - character retreived from serial interrupt
*		      buffer.
*
**********************************************************************
* Notes:
*
* This routine returns the head of the serial interrupt buffer if one
* is available.  Otherwise a 0 is returned.  The function s_buffst()
* should be used to check the status of the buffer (i.e. is a
* character available).
*
**********************************************************************
***/
int s_rdbuff(port)

	int port;

{
	int new_char;

	if (s_ports[port].buffcnt > 0)
	{
	  new_char =  s_ports[port].buffer[s_ports[port].head];
	  s_ports[port].head++;
	  if (s_ports[port].head >= SER_BUFF)
	    s_ports[port].head = 0;
	  s_ports[port].buffcnt--;
	  return(new_char);
	}
	return(0);
}
/***
**********************************************************************
* Function:  s_buffst
*
**********************************************************************
* Description:
*
* Check for the existance of any characters in the serial interrupt
* buffer.
*
**********************************************************************
* Calling Sequence:
*
* ret_val = (int)s_buffst(port)
*
**********************************************************************
* Inputs:
*
* port		int - Serial port to be changed.
*
**********************************************************************
* Outputs:
*
* ret_val	int - TRUE if a character is present.
*		      FALSE if no characters are in the buffer.
*
**********************************************************************
* Notes:  None
*
**********************************************************************
***/
int s_buffst(port)

	int port;

{
	if (s_ports[port].buffcnt > 0)
	  return(TRUE);
	else
	  return(FALSE);
}
/***
**********************************************************************
* Function:  delay
*
**********************************************************************
* Description:
*
* Will enter a wait state for approximately the specified number of
* milliseconds.
*
**********************************************************************
* Calling Sequence:
*
* (void)delay(tsecs)
*
**********************************************************************
* Inputs:
*
* tsecs 	int - The number of milli-seconds to wait.
*
**********************************************************************
* Outputs:  None
*
**********************************************************************
* Notes:
*
* This routine is not completely accurate... but close enough.
*
**********************************************************************
***/
void delay(tsecs)

	unsigned tsecs;

{
	unsigned tickref;
#if MSC
	int *curr_time;
#endif

	for (; tsecs > 0; --tsecs)
	{
#if TURBOC
	  tickref = peek(0, TIMER_LO);
	  while (tickref == peek(0, TIMER_LO));
#endif
#if MSC
	  curr_time = TIMER_LO;
	  tickref = *curr_time;
	  while (tickref == *curr_time);
#endif
	}
}
/***
**********************************************************************
* Function:  get_rs232
*
**********************************************************************
* Description:
*
* Returns the current rs232 configuration
*
**********************************************************************
* Calling Sequence:
*
* ret_val = (unsigned char)get_rs232(port)
*
**********************************************************************
* Inputs:
*
* port		int - Serial port (0 - 3)
*
**********************************************************************
* Outputs:
*
* ret_val	unsigned char - Current format of rs232
*
**********************************************************************
* Notes:  None
*
**********************************************************************
***/
unsigned char get_rs232(port)
{
	return(INPORTB(s_ports[port].uart_base + CONTROL_232));
}
/***
**********************************************************************
* Function:  rs232_off
*
**********************************************************************
* Description:
*
* Turns that rs232 port off
*
**********************************************************************
* Calling Sequence:
*
* (void)rs232_off(port)
*
**********************************************************************
* Inputs:
*
* port		int - Serial port (0 - 3)
*
**********************************************************************
* Outputs:  None
*
**********************************************************************
* Notes:  None
*
**********************************************************************
***/
void rs232_off(port, rs232_mask)

	int port;
	unsigned char rs232_mask;

{
	OUTPORTB(s_ports[port].uart_base + CONTROL_232,
		 get_rs232(port) & ~rs232_mask);
}
/***
**********************************************************************
* Function:  rs232_on
*
**********************************************************************
* Description:
*
* Turns that rs232 port on
*
**********************************************************************
* Calling Sequence:
*
* (void)rs232_on(port)
*
**********************************************************************
* Inputs:
*
* port		int - Serial port (0 - 3)
*
**********************************************************************
* Outputs:  None
*
**********************************************************************
* Notes:  None
*
**********************************************************************
***/
void rs232_on(port, rs232_mask)

	int port;
	unsigned char rs232_mask;

{
	OUTPORTB(s_ports[port].uart_base + CONTROL_232,
		 get_rs232(port) | rs232_mask);
}
/***
**********************************************************************
* Function:  s_rcvstat
*
**********************************************************************
* Description:
*
* Checks the serial port status bit to see if a character has just
* arrived.
*
**********************************************************************
* Calling Sequence:
*
* ret_val = (unsigned char)s_rcvstat(port)
*
**********************************************************************
* Inputs:
*
* port		int - Serial port (0 - 3)
*
**********************************************************************
* Outputs:
*
* ret_val	int - (-1) if no characters are available in the
*		      serial port receive port.
*
**********************************************************************
* Notes:
*
* This routine directly checks the serial port.  This does NOT take
* characters already received from the serial interrupt buffer.
*
**********************************************************************
***/
int s_rcvstat(port)

	int port;

{
	if (s_ports[port].uart_base != 0)
	  return(INPORTB(s_ports[port].uart_base + SIO_STAT) & RCV_MASK);
	else
	  return(-1);
}
/***
**********************************************************************
* Function:  s_xmit
*
**********************************************************************
* Description:
*
* Transmits a character directly through the serial port.
*
**********************************************************************
* Calling Sequence:
*
* (void)s_xmit(port)
*
**********************************************************************
* Inputs:
*
* port		int - Serial port (0 - 3)
*
**********************************************************************
* Outputs:  None
*
**********************************************************************
* Notes:
*
* This routine sends the character directly to the serial port.  The
* function s_xmtstat() should be used to check the serial buffer
* transmit bit to make sure that the serial port is available for
* transmission.
*
**********************************************************************
***/
void s_xmit(port, c)

	int port;
	unsigned char c;

{
	if (s_ports[port].uart_base != 0)
	  OUTPORTB(s_ports[port].uart_base + XMIT, c);
}
/***
**********************************************************************
* Function:  s_xmtstat
*
**********************************************************************
* Description:
*
* Checks the serial port status port for transmission availability.
*
**********************************************************************
* Calling Sequence:
*
* ret_val = (int)s_xmtstat(port)
*
**********************************************************************
* Inputs:
*
* port		int - Serial port (0 - 3)
*
**********************************************************************
* Outputs:
*
* ret_val	unsigned char - (-1) if transmission cannot take place
*				    at the moment.
*				(!= -1) if transmission can take place
*
**********************************************************************
* Notes:
*
* This function directly checks the serial port for transmission
* capability.
*
**********************************************************************
***/
int s_xmtstat(port)

	int port;

{
	if (s_ports[port].uart_base != 0)
	  return((unsigned char)(INPORTB(s_ports[port].uart_base + SIO_STAT) & XMIT_MASK));
	else
	  return(-1);
}
/***
**********************************************************************
* Function:  s_setfmt
*
**********************************************************************
* Description:
*
* Sets the data format on the specified serial port.
*
**********************************************************************
* Calling Sequence:
*
* (void)s_setfmt(port, format)
*
**********************************************************************
* Inputs:
*
* port		int - Serial port (0 - 3).
* format	unsigned char - New data format configuration.
*
**********************************************************************
* Outputs:  None
*
**********************************************************************
* Notes:  None
*
**********************************************************************
***/
void s_setfmt(port, format)

	int port;
	unsigned char format;

{
	if (s_ports[port].uart_base != 0)
	  OUTPORTB(s_ports[port].uart_base + DATA_FORMAT, format);
}
/***
**********************************************************************
* Function:  s_inchar
*
**********************************************************************
* Description:
*
* Checks for an unclaimed character in the serial port.
*
**********************************************************************
* Calling Sequence:
*
* ret_val = (unsigned char)s_inchar(port)
*
**********************************************************************
* Inputs:
*
* port		int - Serial port (0 - 3).
*
**********************************************************************
* Outputs:
*
* ret_val	int - (-1) if no character is available
*		      (!= -1) character in serial port
*
**********************************************************************
* Notes:
*
* This routine directly accesses the serial port.
*
**********************************************************************
***/
int s_inchar(port)

	int port;

{
	if (s_rcvstat(port) == -1)
	  return(-1);
	else
	  return(s_rcv(port));
}
/***
**********************************************************************
* Function:  s_intrid
*
**********************************************************************
* Description:
*
* I think it checks for the hardware interrupt involved with this
* serial port.... I just can't remember... Oh well...
*
**********************************************************************
* Calling Sequence:
*
* ret_val = (unsigned char)s_intrid(port)
*
**********************************************************************
* Inputs:
*
* port		int - Serial port (0 - 3).
*
**********************************************************************
* Outputs:
*
* ret_val	unsigned char - Interrupt id if available
*				(0) if not available
*
**********************************************************************
* Notes:
*
* This routine directly accesses the serial port.
*
**********************************************************************
***/
unsigned char s_intrid(port)

	int port;

{
	if (s_ports[port].uart_base != 0)
	  return(INPORTB(s_ports[port].uart_base + INTR_ID));
	else
	  return(NULL);
}
/***
**********************************************************************
* Function:  i_mask
*
**********************************************************************
* Description:
*
* Enables/Disables the specified hardware interrupt (8 - 15)
*
**********************************************************************
* Calling Sequence:
*
* (void)i_mask(i_number, action)
*
**********************************************************************
* Inputs:
*
* i_number	int - Interrupt to Enable/Disable
* action	int - Action to take (Enable or Disable interrupt)
*
**********************************************************************
* Outputs:  None
*
**********************************************************************
* Notes:
*
* This routine directly accesses the interrupt mask register.  The
* action variable can be set to one of the following previously
* defined constants (in serial.h):
*    I_ENABLE	- Enables the specified interrupt
*    I_DISABLE	- Disables the specified interrupt
*
**********************************************************************
***/
void i_mask(i_number, action)

	int i_number;
	int action;

{
	unsigned char new_mask;
	unsigned char old_mask;

	/************************************************************/
	/* Set the mask for the specific hardware interrupt	    */
	/************************************************************/

	switch(i_number)
	{
	  case 8:
	      new_mask = 0xFE;
	      break;
	  case 9:
	      new_mask = 0xFD;
	      break;
	  case 10:
	      new_mask = 0xFB;
	      break;
	  case 11:
	      new_mask = 0xF7;
	      break;
	  case 12:
	      new_mask = 0xEF;
	      break;
	  case 13:
	      new_mask = 0xDF;
	      break;
	  case 14:
	      new_mask = 0xBF;
	      break;
	  case 15:
	      new_mask = 0x7F;
	      break;
	  default:
	      return;
	};

	/************************************************************/
	/* Retreive old mask from the interrupt mask register and   */
	/* set the appropriate bit.				    */
	/************************************************************/

	old_mask = INPORTB(IMR_PORT);
	if (action == I_DISABLE)
	{
	  new_mask = ~new_mask;
	  new_mask |= old_mask;
	}
	else
	  new_mask &= old_mask;

	/************************************************************/
	/* Send back new interrupt mask register setting	    */
	/************************************************************/

	OUTPORTB(IMR_PORT, new_mask);
}
/***
**********************************************************************
* Function:  do_ivec
*
**********************************************************************
* Description:
*
* Initialize and enable appropriate serial interrupt port.
*
**********************************************************************
* Calling Sequence:
*
* (void)do_ivec(port, i_number)
*
**********************************************************************
* Inputs:
*
* port		int - serial port (0 - 3).
* i_number	int - Hardware interrupt to enable.
*
**********************************************************************
* Outputs:  None
*
**********************************************************************
* Notes:
*
* The hardware interrupt must be specified.
*
**********************************************************************
***/
void do_ivec(port, i_number)

	int port;
	int i_number;

{
	s_ports[port].oldvect = GETVECT(i_number);
	s_ports[port].int_num = i_number;
	SETVECT(i_number, s_ports[port].newvect);
	i_mask(i_number , I_ENABLE);
}

void undo_ivec(port)

	int port;

{
	if ((s_ports[port].int_num != 0) &&
	    (s_ports[port].oldvect != NULL))
	{
	  SETVECT(s_ports[port].int_num, s_ports[port].oldvect);
	  i_mask(s_ports[port].int_num, I_DISABLE);
	}
}
/***
**********************************************************************
* Function:  s_defintr
*
**********************************************************************
* Description:
*
* Enables the serial port
*
**********************************************************************
* Calling Sequence:
*
* (void)s_defintr(port, intswanted)
*
**********************************************************************
* Inputs:
*
* port		int - serial port (0 - 3).
* i_number	int - Hardware interrupt to enable.
*
**********************************************************************
* Outputs:  None
*
**********************************************************************
* Notes:
*
* This routine allows the specified type of serial port interrupts
* to take place.  In this program it is used to enable the serial
* port to receive characters.
*
**********************************************************************
***/
void s_defintr(port, intswanted)

	int port;
	unsigned char intswanted;

{
	OUTPORTB(s_ports[port].uart_base + INTR_ENABLE, intswanted);
}
/***
**********************************************************************
* Function:  s_intson
*
**********************************************************************
* Description:
*
* Enables the appropriate serial interrupt.
*
**********************************************************************
* Calling Sequence:
*
* ret_val = s_intson(port, ints_wanted)
*
**********************************************************************
* Inputs:
*
* port		int - serial port (0 - 3).
* ints_wanted	int - The type of function to enable (i.e. receive)
*
**********************************************************************
* Outputs:
*
* ret_val	int - NULL if the function succeeds
*		      Error code if the function fails
*
**********************************************************************
* Notes:  None
*
**********************************************************************
***/
int s_intson(port, ints_wanted)

	int port;
	unsigned char ints_wanted;	/* Type of serial interrupt  	*/

{
	/************************************************************/
	/* Make sure that the serial port has been initialized	    */
	/************************************************************/

	if (s_ports[port].init_flag != SIO_INIT_OK)
	  return(NO_INIT);

	/************************************************************/
	/* Make sure this is a valid serial port		    */
	/************************************************************/
	if (s_ports[port].commport >= 4)
	  return(NO_INTR);

	/************************************************************/
	/* Enable the interrupts for this serial port.		    */
	/************************************************************/

	s_defintr(port, ints_wanted);
	rs232_on(port, INTON_MASK);

	/************************************************************/
	/* Replace the appropriate interrupt vector		    */
	/************************************************************/

	if (s_ports[port].commport == 0)
	  do_ivec(port, 0xC);
	else
	  do_ivec(port, 0xB);

	/************************************************************/
	/* Empty the serial port of any spurious characters	    */
	/************************************************************/

	s_rcv(port);
	s_rcv(port);
	return(NULL);
}
/***
**********************************************************************
* Function:  s_putch
*
**********************************************************************
* Description:
*
* Sends a character to the serial port.
*
**********************************************************************
* Calling Sequence:
*
* (void)s_putch(port, c)
*
**********************************************************************
* Inputs:
*
* port		int - serial port (0 - 3).
* c		unsigned char - character to transmit
*
**********************************************************************
* Outputs: None
*
**********************************************************************
* Notes:  None
*
**********************************************************************
***/
void s_putch(port, c)

	int port;
	unsigned char c;

{
	while (s_xmtstat(port) == -1);
	s_xmit(port, c);
}
/***
**********************************************************************
* Function:  s_getfmt
*
**********************************************************************
* Description:
*
* Retreives the current serial port data format.
*
**********************************************************************
* Calling Sequence:
*
* ret_val = s_getfmt(port)
*
**********************************************************************
* Inputs:
*
* port		int - serial port (0 - 3).
*
**********************************************************************
* Outputs:
*
* ret_val	unsigned char - current serial port data format
*				NULL if the serial port is unitialized
*
**********************************************************************
* Notes:  None
*
**********************************************************************
***/
unsigned char s_getfmt(port)

	int port;

{
	if (s_ports[port].uart_base != 0)
	  return(INPORTB(s_ports[port].uart_base + DATA_FORMAT));
	else
	  return(NULL);
}
/***
**********************************************************************
* Function:  setbaud
*
**********************************************************************
* Description:
*
* Sets the baud rate of the serial port.
*
**********************************************************************
* Calling Sequence:
*
* (void)setbaud(port, newbaud)
*
**********************************************************************
* Inputs:
*
* port		int - serial port (0 - 3).
* newbaud	unsigned - Mask value for new baud rate
*
**********************************************************************
* Outputs:  None
*
**********************************************************************
* Notes:
*
* This routine should be called from the function changebaud.  The
* baud rate mask must be given in newbaud.
*
**********************************************************************
***/
void setbaud(port, newbaud)

	int port;
	unsigned newbaud;
{
	if (s_ports[port].uart_base != 0)
	{
	  s_setfmt(port, s_getfmt(port) | 0x80);
	  OUTPORTB(s_ports[port].uart_base + BAUD_LSB, newbaud & 0x0FF);
	  OUTPORTB(s_ports[port].uart_base + BAUD_MSB, newbaud >> 8);
	  s_setfmt(port, s_getfmt(port) & 0x7f);
	}
}
/***
**********************************************************************
* Function:  changebaud
*
**********************************************************************
* Description:
*
* Sets the baud rate of the serial port.
*
**********************************************************************
* Calling Sequence:
*
* ret_val = (unsigned)changebaud(port, newbaud)
*
**********************************************************************
* Inputs:
*
* port		int - serial port (0 - 3).
* newbaud	unsigned - New baud rate
*
**********************************************************************
* Outputs:
*
* ret_val	unsigned - Current serial port data format.
*			   (0) if function fails.
*
**********************************************************************
* Notes:  None
*
**********************************************************************
***/
unsigned changebaud(port, newbaud)

	int port;
	unsigned newbaud;

{
	unsigned divisor;

	switch(newbaud)
	{
	  case 300:			/* Set 300 baud bit setttings	*/
	    divisor = 0x180;
	    break;
	  case 600:
	    divisor = 0xC0;		/* Set 600 baud bit settings 	*/
	    break;
	  case 1200:
	    divisor = 0x60;		/* Set 1200 baud bit settings	*/
	    break;
	  case 2400:
	    divisor = 0x30;		/* Set 2400 baud bit settings	*/
	    break;
	  case 4800: 			/* Set 4800 baud bit settings	*/
	    divisor = 0x18;
	    break;
	  case 9600:			/* Set 9600 baud bit settings	*/
	    divisor = 0x0C;
	    break;
          case 19200:                   /* Set 19200 divide down setting */
            divisor = 0x06;
            break;
	  default:                      /* Else set error flag		*/
	    divisor = 0;
	    break;
	}
	if (divisor != 0)		/* Do not set if invalid baud	*/
	  setbaud(port, divisor);
	return(divisor);		/* Return divisor .. 0 = false	*/
}
/***
**********************************************************************
* Function:  getbaud
*
**********************************************************************
* Description:
*
* Retreives the current baud rate mask.
*
**********************************************************************
* Calling Sequence:
*
* ret_val = (unsigned)getbaud(port)
*
**********************************************************************
* Inputs:
*
* port		int - serial port (0 - 3).
*
**********************************************************************
* Outputs:
*
* ret_val	unsigned - Current serial port data format.
*
**********************************************************************
* Notes:  None
*
**********************************************************************
***/
unsigned getbaud(port)

	int port;

{
	unsigned baudnow;

	s_setfmt(port, s_getfmt(port) | 0x80);
	baudnow = INPORTB(s_ports[port].uart_base + BAUD_MSB);
	baudnow = baudnow << 8;
	baudnow += INPORTB(s_ports[port].uart_base + BAUD_LSB);
	s_setfmt(port, s_getfmt(port) & 0x7f);
	return(baudnow);
}
/***
**********************************************************************
* Function:  s_config
*
**********************************************************************
* Description:
*
* Configures the specified serial port.
*
**********************************************************************
* Calling Sequence:
*
* (void)s_config(port)
*
**********************************************************************
* Inputs:
*
* port		int - serial port (0 - 3).
*
**********************************************************************
* Outputs:  None
*
**********************************************************************
* Notes:  None
*
**********************************************************************
***/
void s_config(port)
{
	unsigned char tmpfmt = 0xE0;
#if MSC
	unsigned *curr_uart;

	curr_uart = UART_PTR + (2 * port);
	if ((s_ports[port].uart_base = *curr_uart) == NULL)
#endif
#if TURBOC
	if ((s_ports[port].uart_base = peek(0, UART_PTR + (2 * port))) == NULL)
#endif
	{
	    s_ports[port].init_flag = SIO_INIT_BAD;
	    return;
	}
	s_ports[port].usrbaud = getbaud(port);
	s_getfmt(port);
	get_rs232(port);
	changebaud(port, s_ports[port].startbaud);
	tmpfmt &= s_ports[port].usrfmt;
	s_ports[port].startfmt += tmpfmt;
	s_setfmt(port, s_ports[port].startfmt);
	rs232_on(port, s_ports[port].start232);
	s_ports[port].init_flag = SIO_INIT_OK;
	s_intson(port, ENABLE_RCV);
	return;
}
/***
**********************************************************************
* Function:  s_ungetc
*
**********************************************************************
* Description:
*
* Places the specified character back into the head of the serial
* interrupt buffer.
*
**********************************************************************
* Calling Sequence:
*
* (void)s_ungetc(port)
*
**********************************************************************
* Inputs:
*
* port		int - serial port (0 - 3).
*
**********************************************************************
* Outputs:  None
*
**********************************************************************
* Notes:  None
*
**********************************************************************
***/
void s_ungetc(port, new_char)

	int port;
	unsigned char new_char;

{
	DISABLE();
	if (s_ports[port].buffcnt > SER_BUFF)
	  return;
	if (s_ports[port].head <= 0)
	  s_ports[port].head = SER_BUFF-1;
	else
	   s_ports[port].head--;
	s_ports[port].buffer[s_ports[port].head] = new_char;
	s_ports[port].buffcnt++;
	ENABLE();
}
/***
**********************************************************************
* Function:  s_fgetc
*
**********************************************************************
* Description:
*
* Retreives a character from the specified serial port in a specified
* amount of time.
*
**********************************************************************
* Calling Sequence:
*
* ret_val = (int)s_fgetc(port, tout)
*
**********************************************************************
* Inputs:
*
* port		int - serial port (0 - 3).
* tout		unsigned - time in milliseconds to wait for character
*
**********************************************************************
* Outputs:
*
* ret_val	int - character retreived from serial port.
*
**********************************************************************
* Notes:  None
*
**********************************************************************
***/
int s_fgetc(port, tout)

	int port;
	unsigned int tout;

{
	unsigned int ctime;
	unsigned char b;
#if MSC
	unsigned *curr_time;

	curr_time = TIMER_LO;
	ctime = *curr_time;
#endif
#if TURBOC
	ctime = peek(0, TIMER_LO);
#endif
	while (TRUE)
	{
	  if (s_buffst(port))
	  {
	    b = s_rdbuff(port);
	    return((int)b);
	  }
	  else
#if TURBOC
	    if ((peek(0, TIMER_LO) - ctime) >= tout)
#endif
#if MSC
	    if ((*curr_time - ctime) >= tout)
#endif
	      return(TIMEOUT);
	}
}
/***
**********************************************************************
* Function:  s_getch
*
**********************************************************************
* Description:
*
* Retreives a character from the serial port with no time limit.
*
**********************************************************************
* Calling Sequence:
*
* ret_val = (unsigned char)s_getch(port)
*
**********************************************************************
* Inputs:
*
* port		int - serial port (0 - 3).
*
**********************************************************************
* Outputs:
*
* ret_val	unsigned char - character retreived from serial port.
*
**********************************************************************
* Notes:  None
*
**********************************************************************
***/
unsigned char s_getch(port)

	int port;

{
	unsigned char c;

	while (s_rcvstat(port) == -1);		/* wait for a character */
	c = s_rcv(port);			/* get character	*/
	return(c);				/* Return character	*/
}
/***
**********************************************************************
* Function:  s_fgets
*
**********************************************************************
* Description:
*
* Retreives a character string from the serial port.
*
**********************************************************************
* Calling Sequence:
*
* ret_val = (int)s_fgets(port, buff, numc)
*
**********************************************************************
* Inputs:
*
* port		int - serial port (0 - 3).
* buff		unsigned char * - output buffer
* numc		int - max number of characters to retreive
*
**********************************************************************
* Outputs:
*
* ret_val	int - number of characters retreived
*
**********************************************************************
* Notes:
*
* This function currently is set to time out after 1.5 seconds
* between characters.
*
**********************************************************************
***/
int s_fgets(port, buff, numc)

	int port;
	unsigned char *buff;
	int numc;

{
	int c, count;
        unsigned char *dbg;

	count = 0;
	while (count < numc)
	{
	  if ((c = s_fgetc(port, 13)) == TIMEOUT)
	    break;
	  *buff++ = (unsigned char)c;
	  count++;
	}
	*buff = NIL;
	return(count);
}
/***
**********************************************************************
* Function:  s_fputs
*
**********************************************************************
* Description:
*
* Sends a character string to the serial port.
*
**********************************************************************
* Calling Sequence:
*
* (void)s_fputs(port, cmdbuff)
*
**********************************************************************
* Inputs:
*
* port		int - serial port (0 - 3).
* cmdbuff	unsigned char * - string to transmit
*
**********************************************************************
* Outputs:  None
*
**********************************************************************
* Notes:  None
*
**********************************************************************
***/
void s_fputs(port, cmdbuff)

	int port;
	unsigned char *cmdbuff;

{
	int tmp;

	while (*cmdbuff != '\0')
	{
	  delay(2);
	  s_putch(port, *cmdbuff);
	  cmdbuff++;
/*          s_fgetc(port, 20);   */
	}
}
/***
**********************************************************************
* Function:  s_unconfig
*
**********************************************************************
* Description:
*
* Replaces all the serial port values and the serial port interrupt
* to their initial values.
*
**********************************************************************
* Calling Sequence:
*
* (void)s_unconfig(port)
*
**********************************************************************
* Inputs:
*
* port		int - serial port (0 - 3).
*
**********************************************************************
* Outputs:  None
*
**********************************************************************
* Notes:  None
*
**********************************************************************
***/
void s_unconfig(port)

	int port;

{
	if (s_ports[port].uart_base != NULL)
	{
	  setbaud(port, s_ports[port].usrbaud);
	  s_setfmt(port, s_ports[port].usrfmt);
	  rs232_off(port, 0xFF);
	  rs232_on(port, s_ports[port].usr232);
	  undo_ivec(port);
	}
}
/***
**********************************************************************
* Function:  clear_port
*
**********************************************************************
* Description:
*
* Clears the serial interrupt buffer of all incoming and received
* characters.
*
**********************************************************************
* Calling Sequence:
*
* (void)clear_port(port)
*
**********************************************************************
* Inputs:
*
* port		int - serial port (0 - 3).
*
**********************************************************************
* Outputs:  None
*
**********************************************************************
* Notes:  None
*
**********************************************************************
***/
void clear_port(port)

    int port;

{
    while (s_fgetc(port, 3) != TIMEOUT);
}
