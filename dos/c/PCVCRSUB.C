# include <stdio.h>
# include <stdlib.h>
# include <conio.h>
# include <dos.h>
# include <string.h>

/* INTNUM, the interrupt to be used to trap the timer interrupt, was	*/
/* tried out as INT 8 and INT 0x1c. Both should work, but for reasons	*/
/* not understood at this time, INT 8, which calls INT 0x1c, does NOT	*/
/* work. INT 0x1c does work, so was used for this program		*/
/*									*/
# define INTNUM 0x1c            /* define the timer interrupt number    */
/*									*/
# define CR 0xd
/*                                                                      */
/*      registers for serial port (not all used)                        */
/*                                                                      */
# define TX		0	/* Transmit register			*/
# define RX		0	/* receive register			*/
# define IER		1	/* Interrupt Enable			*/
# define IIR		2	/* Interrupt ID				*/
# define LCR		3	/* Line control				*/
# define MCR		4	/* Modem control			*/
# define LSR		5	/* Line Status				*/
# define MSR		6	/* Modem Status				*/
# define DLL		0	/* Divisor Latch Low			*/
# define DLH		1	/* Divisor latch high			*/
/*									*/
/*	Status values for modem register				*/
/*									*/
# define CTS     0x10
# define DSR     0x20

# define DTR     0x01
# define RTS     0x02
# define OUT2    0x08

# define IMR     0x21
# define ICR     0x20
# define EOI     0x20
# define RX_MASK 7
# define RX_ID   4

# define IRQ3    0xf7
# define IRQ4    0xef

# define MC_INT  8
# define RX_INT  1

# define BUFOVFL 1		/* buffer overflowed			*/

# define NO_PAR 0
# define EV_PAR 1
# define OD_PAR 2

# define DOS            0       /* to distinguish DOS vs LOCAL for the  */
# define LOCAL          1       /*   timer interrupt                    */
# define SBUFSIZ	0x2000


extern int baudrate;
extern int stopflag;
extern int flagflag;
extern int timer_interrupt;
extern int endbuf;
extern int startbuf;
extern unsigned char ccbuf[];
extern int portbase;
extern int comnum;
extern int SerialStarted;

extern void interrupt newtimer(void);
extern int SerialOut(char x);
extern void interrupt (*oldtimer)(void);
extern void interrupt com_int(void);
extern void interrupt (*oldvects[2])();

unsigned char getser(void);


/*                                                                      */
/************************************************************************/
/*									*/
/*	get_ser_string --- get a string from the serial input port (use	*/
/*			   the buffered I/O of getser() to do it.	*/
/*									*/
void get_ser_string(char *string)
{ int index;
  unsigned char tempc;

  index = 0;
  while(1)
  { tempc = getser();
    if (tempc != 0xff)
    { if (tempc == CR)
      { *(string + index) = '\0';
	break;
      }
      else
	*(string + index++) = tempc;
    }
  }
}
/*                                                                      */
/************************************************************************/
/*									*/
/*	getser --- get the current value in the serial input buffer	*/
/*									*/
unsigned char getser(void)
{ unsigned char res;

  if (endbuf == startbuf)
    return (0xff);
  res = (unsigned char) ccbuf[startbuf];
  startbuf = (startbuf + 1) % SBUFSIZ;
  return(res);
}
/*                                                                      */
/************************************************************************/
/*									*/
/*	closeserial ---							*/
/*									*/
void closeserial(void)
{ unsigned char cc;

  disable();					/* disable		*/
  cc = inportb(IMR) | ~IRQ3 | ~IRQ4;		/*			*/
  outportb(IMR, cc);				/*   the serial		*/
  outportb(portbase + IER, 0);			/*			*/
  cc = inportb(portbase + MCR) & ~MC_INT;	/*      port		*/
  outportb(portbase + MCR, cc);			/*			*/
  enable();					/*	  interrupts	*/

  outportb(portbase + MCR, 0);

  disable();			/* disable the interrupts while messing	*/
  setvect(0x0b,oldvects[0]);	/*   with the interrupt vectors. Reset	*/
  setvect(0x0c,oldvects[1]);	/*     the serial port interrupt vectors*/
  enable();			/*	 to their normal DOS values	*/

  SerialStarted = 0;

}
/*                                                                      */
/************************************************************************/
/*									*/
/*	string_out --- output a string to the serial port and follow	*/
/*		       it with a CR					*/
/*									*/
void string_out(char *string)
{ int ii;
  for (ii = 0; ii < strlen(string); ii++)
    SerialOut(*(string + ii));
  SerialOut(CR);
}
/*                                                                      */
/************************************************************************/
/*									*/
/*	SerialOut --- output a serial character				*/
/*									*/
int SerialOut (char x)
{ long timeout = 0x0000ffff;
  outportb(portbase + MCR, OUT2 | DTR | RTS);
  /* wait for clear to send     */
  while ((inportb(portbase + MSR) & CTS) == 0)
    if ((--timeout) == 0)
      return(-1);
  timeout = 0x0000ffff;
  /* wait for outport register to clear         */
  while ((inportb(portbase + LSR) & DSR) == 0)
    if ((--timeout)==0)
      return(-1);
  disable();
  outportb(portbase + TX, x);
  enable();
  return(0);
}
/*                                                                      */
/************************************************************************/
/*									*/
/*	SetSpeed --- this routine sets the speed; will accept abnormal	*/
/*		     baud rates						*/
/*									*/
int SetSpeed (int Speed)
{ unsigned char cc;
  int divisor;
  if (Speed == 0)
    return(-1);            /* avoid divide by zero */
  else
    divisor = (int)(115200L / Speed);
  if (portbase == 0)
    return(-11);
  disable();
  cc = inportb(portbase + LCR);
  outportb(portbase + LCR, (cc | 0x80));	/* set DLAB     */
  outportb(portbase + DLL, (divisor & 0xff));   /* set divisor  */
  outportb(portbase + DLH, ((divisor >> 8) & 0xff));
  outportb(portbase + LCR, cc);
  enable();
  return(0);
}
/*                                                                      */
/************************************************************************/
/*									*/
/*	SetOthers ---							*/
/*									*/
int SetOthers (int Parity,int Bits,int StopBit)
{ int temp;
  if (portbase == 0)
    return (-1);
  if ((Parity < NO_PAR) || (Parity > OD_PAR))
    return (-1);
  if ((Bits < 5) || (Bits > 8))
    return (-1);
  if ((StopBit < 1) || (StopBit > 2))
    return (-1);
  temp = Bits - 5;
  temp |= ((StopBit == 1) ? 0x00 : 0x04);
  switch (Parity)
  { case NO_PAR:
    { temp |= 0x00;
      break;
    }
    case OD_PAR:
    { temp |= 0x08;
      break;
    }
    case EV_PAR:
    { temp |= 0x18;
      break;
    }
  }
  disable();
  outportb(portbase + LCR,temp);
  enable();
  return(0);
}
/*                                                                      */
/************************************************************************/
/*									*/
/*	setserial --- set the serial port				*/
/*									*/
int setserial(int Speed, int Parity, int Bits, int StopBit)
{ unsigned char cc;

  if (SetSpeed(Speed) == -1)
    return(-1);
  if (SetOthers(Parity, Bits, StopBit) == -1)
    return (-1);
  endbuf = startbuf = 0;	/* initialize the queue to empty	*/

  disable();			/* disable the interrupts while messing	*/
/*oldvects[0]=getvect(0x0b);	/*   with the interrupt vectors. Save	*/
/*oldvects[1]=getvect(0x0c);	/*     the DOS values for the serial	*/
  setvect(0x0b,com_int);	/*	 port interrupts and point both	*/
  setvect(0x0c,com_int);	/*	   serial port interrupts to	*/
  enable();			/*	     the local handler. Enable	*/
				/*	       the interrupts again	*/
				/*		 when done.		*/
  /*									*/
  /* well, yes, it may seem strange to do an enable followed directly	*/
  /* by a disable, but this gives the interrupt system a chance to get	*/
  /* a word in edgewise here if necessary, so it's not as dumb as it	*/
  /* may seem.								*/
  /*									*/
  disable();					/* enable		*/
  cc = inportb(portbase + MCR) | MC_INT;	/*			*/
  outportb(portbase + MCR, cc);			/*   the serial		*/
  outportb(portbase + IER, RX_INT);		/*			*/
  cc = inportb(IMR) & (comnum == 2 ? IRQ3:IRQ4);/*      port		*/
  outportb(IMR, cc);				/*			*/
  enable();					/*	  interrupts	*/

  cc = inportb (portbase + MCR) | DTR | RTS;
  outportb(portbase + MCR, cc);

  SerialStarted = 1;
  return (0);

}
/*                                                                      */
/************************************************************************/
/*									*/
/*	shutdown --- shut down the local timer interrupt trap		*/
/*									*/
void shutdown(void)
{ if (timer_interrupt == LOCAL)	/* do shutdown only if startup was done	*/
  { stopflag = 1;		/* set the shutdown flag so the		*/
				/*   interrupt handler will shut itself	*/
				/*     off and restore DOS conditions	*/
    while (!flagflag)		/* wait until the interrupt handler	*/
    {};				/*   signals that it has shut down	*/
    timer_interrupt = DOS;	/* show that startup can start (again)	*/
  }
}
/*                                                                      */
/************************************************************************/
/*									*/
/*	normalize --- normalize a far pointer the same way as is done	*/
/*		      in the huge menory model				*/
/*									*/
void far *normalize(void far *pointer)
{ unsigned a,b,c;

  a = FP_OFF(pointer) >> 4;
  b = FP_SEG(pointer);
  c = FP_SEG(pointer) & 0x0f;
  return(MK_FP(a+b, c));
}
/*                                                                      */
/************************************************************************/
/*									*/
/*	startup --- set timer interrupt handler to local and set timer	*/
/*		    to 1/30th of a second				*/
void startup(void)
{ if (timer_interrupt == DOS)	/* only allow startup from DOS mode	*/
  { disable();			/* turn off interrupts while changing	*/
				/*   the interrupt vector and the	*/
				/*     timer tick rate			*/
    outportb(0x43, 0x36);	/* tell timer to accept # of ticks	*/
    outportb(0x40, 0x4e);	/* set timer to 30.0 ticks		*/
    outportb(0x40, 0x9b);	/*   per second				*/
    oldtimer = getvect(INTNUM);	/* save the address of the DOS		*/
				/*   timer interrupt handler		*/
    setvect(INTNUM, newtimer);	/* point the timer interrupt		*/
				/*   to our local handler		*/
    enable();			/* re-enable the interrupts		*/
    timer_interrupt = LOCAL;	/* set timer interrupt flag to local	*/
  }
}
/*                                                                      */
/************************************************************************/
/* 									*/
/*	StartSerial --- start the serial port				*/
/*									*/
void StartSerial(void)
{ if (setserial(baudrate,	/* set up the serial port. If		*/
	    NO_PAR, 8, 1) == -1)/*   there are any problems,		*/
  { clrscr();
    printf("\nserial port nonresponsive. \n\nGoodbye.\n\n");
    exit(-1);			/*     then exit & die			*/
  }
}
/*                                                                      */
/************************************************************************/
/* 									*/
/*	strlfpad --- string left pad function from the Hobbit House	*/
/*		     string function library				*/
/*									*/
char *strlfpad(char *instring, int nchar, char PadChar)
{
int ii;					/* general index		*/
int slen;				/* slen = the length of		*/
slen = strlen(instring);		/*   the input string		*/
for (ii = 0; ii <= slen; ii++)		/* move the existing string	*/
  *(instring + nchar + slen - ii) =	/*   characters to the right	*/
	*(instring + slen - ii);	/*     side of the new string	*/
for (ii = 0; ii < nchar; ii++)		/* fill in the new left side	*/
  *(instring + ii) = PadChar;		/*   with the pad character	*/
return(instring);			/* return a pointer to		*/
}					/*   the string			*/
/*                                                                      */
/************************************************************************/
/* 									*/
/*	strright --- string right function from the Hobbit House	*/
/*		     string function library				*/
/*									*/
char *strright(char *instring, int nchar)
{
int ii;
int slen;			/* strlen = the length of		*/
slen = strlen(instring);	/*   the original string		*/
if (nchar > slen)		/* if # chars requested is more than    */
  nchar = slen;			/*   string, reset nchar to length	*/
for (ii = 0; ii < nchar; ii++)	/* move the necessary			*/
  *(instring + ii) = *(instring	/*   characters from the end		*/
	+ slen - nchar + ii);	/*     of the string to the		*/
*(instring + nchar) = 0;	/*	 left side			*/
return(instring);		/* return the pointer to		*/
}				/*   the "new" string			*/





