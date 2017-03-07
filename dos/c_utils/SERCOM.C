#include <stdio.h>
#include <alloc.h>
#include <dos.h>

#define _SERCOM_GLOBAL_
#include "sercom.h"                    /*constants and prototypes for module*/



dword get_ticks ()
{
  dword t;
  disable ();                          /*turn off interrupts during access*/
  t=*(dword *)TIMER_LO;                /*read in timer value from low memory*/
  enable ();                           /*ints back on*/
  return (t);
}   /*get_ticks*/


wait_ticks (word ticks)
{
  dword k;
  for (;ticks>0;ticks--) {
    k=get_ticks();                     /*get current system ticker LSW*/
    while (k==get_ticks());            /*wait for ticker transition*/
  }   /*for*/
}   /*wait_ticks*/


#if (USE_INTERRUPTS)

void interrupt s_int_rxcom1 ()
{
  struct serial_rec *sr;               /*local pointer to serial port record*/
  byte lcr;                            /*location to save LCR 8250 register*/
  disable ();                          /*disable interrupts*/
  sr=(struct serial_rec far *)&s_ports[COM1];
  if ((sr->rxcount)<SER_BUFF_SIZE) {   /*if buffer full, see ya*/
    (sr->rxcount)++;                   /*increment counter*/
    lcr=inportb (sr->uartbase+SIO_LCR);/*read current value of LCR*/
    outportb (sr->uartbase+SIO_LCR,lcr&~SIO_DLAB);  /*make sure DLAB is low*/
    sr->rxbuff[sr->rxhead]=inportb (sr->uartbase+SIO_RBR);  /*read char*/
    outportb (sr->uartbase+SIO_LCR,lcr);  /*write old value of LCR back in*/
    (sr->rxhead)++;                    /*forward to insert at buffer head*/
    if (sr->rxhead>=SER_BUFF_SIZE) sr->rxhead=0;   /*check for wrap-around*/
  }   /*if buffer not full*/
  outportb (PIC_REG,0x20);             /*tell the PIC that intterupt done*/
  enable ();                           /*enable interrupts*/
}   /*s_int_rxcom1*/


void interrupt s_int_rxcom2 ()
{
  struct serial_rec *sr;               /*local pointer to serial port record*/
  byte lcr;                            /*location to save LCR 8250 register*/
  disable ();                          /*disable interrupts*/
  sr=&s_ports[COM2];
  if (sr->rxcount<SER_BUFF_SIZE) {     /*if buffer full, see ya*/
    (sr->rxcount)++;                   /*increment counter*/
    lcr=inportb (sr->uartbase+SIO_LCR);/*read current value of LCR*/
    outportb (sr->uartbase+SIO_LCR,lcr&~SIO_DLAB);  /*make sure DLAB is low*/
    sr->rxbuff[sr->rxhead]=inportb (sr->uartbase+SIO_RBR);  /*read char*/
    outportb (sr->uartbase+SIO_LCR,lcr);  /*write old value of LCR back in*/
    (sr->rxhead)++;                    /*forward to insert at buffer head*/
    if (sr->rxhead>=SER_BUFF_SIZE) sr->rxhead=0;   /*check for wrap-around*/
  }   /*if buffer not full*/
  outportb (PIC_REG,0x20);             /*tell the PIC that intterupt done*/
  enable ();                           /*enable interrupts*/
}   /*s_int_rxcom2*/

#endif   /*if using interrupts*/



/****************************************************************************/
word s_config (word port)
{
  word k;
  struct serial_rec *sr;
  if (port>MAX_PORTS) return (S_ILLEGAL_PORT);
  sr=&s_ports[port];                   /*set local pointer to serial record*/
  if (sr->flags.init) return (S_ALREADY_CONFIGURED);
  k=*(word far *)(UART_TABLE+(dword)(port<<1));  /*uart base*/
  if (k==0) return (S_NO_PORT_AVAILABLE);
  sr->flags.init=1;                    /*indicate that port is configured*/
  sr->uartbase=k;                     /*set port base for 8250*/
  outportb (k+SIO_LCR,inportb (k+SIO_LCR) & 0x9F);  /*turn off stick parity*/
  sr->rxhead=0;                        /*initialize rx buffer head pointer*/
  sr->rxtail=0;                        /*initialize rx buffer tail pointer*/
  sr->rxcount=0;                       /*initialize rx buffer counter*/
  sr->flags.rxtimout=0;                /*disable timeouts on receive*/
  sr->flags.txtimout=1;                /*enable timeouts on transmit*/
  sr->flags.cnvrtcr=0;                 /*no conversion of CR to CR+LF*/
  sr->flags.rxframe=0;                 /*don't look for framing character*/
  sr->flags.framefound=0;              /*indicate that frame is not yet found*/
  sr->flags.rxterm=0;                  /*don't look for terminator*/
  sr->flags.txterm=0;                  /*don't generate terminator for tx*/
  sr->rxframechar='$';                 /*default to '$' for framing character*/
  sr->rxtermstr[0]=0;                  /*zero out receive termination string*/
  sr->rxtimout=10;                     /*set receive char timeout to 1/2 sec*/
  sr->txtermstr[0]=0;                  /*zero out transmit termination string*/
  sr->txtimout=10;                     /*set transmit char timeout to 1/2 sec*/
  sr->startbaud=s_baudrate (port,INQUIRE);      /*read initial baudrate*/
  sr->startparity=s_parity (port,INQUIRE);      /*read initial stopbits*/
  sr->startdatabits=s_databits (port,INQUIRE);  /*read initial databits*/
  sr->startstopbits=s_stopbits (port,INQUIRE);  /*read initial stopbits*/
#if (USE_INTERRUPTS)
  if (port==COM1) {
    sr->intnum=0x0C;                   /*COM1 is interrupt 0x0C*/
    sr->startvect=getvect (sr->intnum);/*get old interrupt vector*/
    sr->rxvect=s_int_rxcom1;           /*com1 interrupt handler*/
    setvect (sr->intnum,sr->rxvect);   /*put handler in int vector table*/
    outportb (IMR_REG,inportb (IMR_REG)&~INT0CEN); /*enable interrupt 0x0C*/
  }
  else {
    sr->intnum=0x0B;                   /*COM1 is interrupt 0x0B*/
    sr->startvect=getvect (sr->intnum);/*get old interrupt vector*/
    sr->rxvect=s_int_rxcom2;           /*com2 interrupt handler*/
    setvect (sr->intnum,sr->rxvect);   /*put handler in int vector table*/
    outportb (IMR_REG,inportb (IMR_REG)&~INT0BEN); /*enable interrupt 0x0B*/
  }
  outportb (sr->uartbase+SIO_IER,SIO_ERBFI);  /*enable only rx intrs*/
  outportb (sr->uartbase+SIO_MCR,SIO_INTEN);  /*enable interrupts*/
  inportb (sr->uartbase+SIO_RBR);
  inportb (sr->uartbase+SIO_RBR);
#endif   /*using interrupts for receive buffering*/
  return (S_OK);
}   /*s_config*/
/****************************************************************************/


void s_unconfig (word port,int reset)
{
  struct serial_rec *sr;
  if (port>MAX_PORTS) return;
  sr=&s_ports[port];                   /*set up local pointer to record*/
  if (!sr->flags.init) return;         /*make sure port has been configured*/
#if (USE_INTERRUPTS)                   /*do only if using interrupts...*/
  outportb (sr->uartbase+SIO_MCR,~SIO_INTEN);
  outportb (sr->uartbase+SIO_IER,
    inportb (sr->uartbase+SIO_IER) & ~SIO_ERBFI);  /*disable rx interrupts*/
  if (port==COM1) {
    outportb (IMR_REG,inportb (IMR_REG)|INT0CEN);  /*mask out interrupt 0x0C*/
  }
  else {
    outportb (IMR_REG,inportb (IMR_REG)|INT0BEN);  /*mask out interrupt 0x0B*/
  }
  setvect (sr->intnum,sr->startvect);  /*reset original interrupt handler*/
#endif
  if (reset) {
    s_baudrate (port,sr->startbaud);     /*reset original baud rate*/
    s_parity (port,sr->startparity);     /*reset original parity sense*/
    s_databits (port,sr->startdatabits); /*reset original data bits*/
    s_stopbits (port,sr->startstopbits); /*reset original stop bits*/
  }   /*if resetting port*/
}   /*s_unconfig*/


int  s_rxready (word port)             /*returns 1 if char ready in port*/
{
#if (USE_INTERRUPTS)
  struct serial_rec *sr;
  int r;
  disable ();
  sr=&s_ports[port];                   /*set up local pointer to record*/
  if (sr->rxcount<=0) sr->rxcount=0;   /*if empty, make sure counter is 0*/
  r=((sr->rxcount>0));                 /*return 1 if buffer not empty*/
  enable ();
  return (r);
#else
  word ubase;
  ubase=s_ports[port].uartbase;
  return ((inportb (ubase+SIO_LSR) & SIO_DR)!=0);  /*see if data ready*/
#endif
}   /*s_rxready*/


int  s_txready (word port)             /*returns 1 if tx char ready*/
{
  word ubase;
  ubase=s_ports[port].uartbase;
  return ((inportb (ubase+SIO_LSR) & SIO_THRE)!=0);  /*if tx hold reg empty*/
}   /*s_txready*/


char s_getc (word port)                /*gets a character from port*/
{
#if (USE_INTERRUPTS)
  struct serial_rec *sr;
  char c;
  c=0;                                 /*initialize character to NUL*/
  disable ();
  sr=&s_ports[port];                   /*point to port array*/
  if (sr->rxcount<=0) sr->rxcount=0;   /*if nothing to get, return NUL*/
  if (sr->rxcount>0) {
    c=sr->rxbuff[(sr->rxtail)];          /*get character*/
    (sr->rxtail)++;                      /*go to next char*/
    if (sr->rxtail>=SER_BUFF_SIZE) sr->rxtail=0;  /*see if wrapped around*/
    (sr->rxcount)--;                     /*decrement rx buffer count*/
    if (sr->rxcount<=0) sr->rxcount=0;
  }   /*if buffer not empty*/
  enable ();
  return (c);
#else
  word ubase;
  ubase=s_ports[port].uartbase;
  return (inportb (ubase+SIO_RBR));    /*return byte from receive buffer reg*/
#endif    /*else NOT using interrupt buffering of received characters*/
}   /*s_getc*/


int  s_putc (word port,char c)         /*puts a character to a port*/
{
  word ubase;
  dword tik;
  word k;
  struct serial_rec *sr;

  if (port>COM2) return (0);
  sr=&s_ports[port];
  if (!sr->flags.init) return (0);
  ubase=sr->uartbase;
  if (sr->flags.txtimout) {
    k=sr->txtimout;                    /*set local timeout value*/
    tik=get_ticks ();                  /*first ticks value*/
    while ((k>0) && !s_txready (port)) {  /*til timeout or ready*/
      if (tik!=get_ticks ()) {
        tik=get_ticks ();              /*set tik to look for next transition*/
        k--;                           /*decrement timeout counter*/
      }   /*if ticker transition occurred*/
    }   /*while waiting*/
    if (!s_txready (port)) return (0);
  }
  outportb (ubase+SIO_THR,(byte) c);
  return (1);
}   /*s_putc*/


int  s_puts (word port,char *s)
{
  int ok;
  ok=1;
  if (!s_ports[port].flags.init) return (0);
  for (;(*s!=0)&&ok;s++) {
    ok&=s_putc (port,*s);
    if ((*s==0x0D)&&s_ports[port].flags.cnvrtcr) {
      ok&=s_putc (port,0x0A);
    }   /*if translating CR to CR+LF*/
  }   /*for each character in string*/
  if (ok&&s_ports[port].flags.txterm) {
    s=s_ports[port].txtermstr;
    for (;(*s!=0)&&ok;s++) {
      ok&=s_putc (port,*s);
      if ((*s==0x0D)&&s_ports[port].flags.cnvrtcr) {
        ok&=s_putc (port,0x0A);
      }   /*if translating CR to CR+LF*/
    }   /*for each character in string*/
  }   /*if sending termination string*/
  return (ok);
}   /*s_puts*/


word s_baudrate (word port,word baud)  /*returns/sets baud rate for port*/
{
  word ubase;
  word divisor;
  ubase=s_ports[port].uartbase;
  switch (baud) {
    case 56000: divisor=   2; break;
    case 38400: divisor=   3; break;
    case 19200: divisor=   6; break;
    case  9600: divisor=  12; break;
    case  7200: divisor=  16; break;
    case  4800: divisor=  24; break;
    case  3600: divisor=  32; break;
    case  2400: divisor=  48; break;
    case  2000: divisor=  58; break;
    case  1800: divisor=  64; break;
    case  1200: divisor=  96; break;
    case   600: divisor= 192; break;
    case   300: divisor= 384; break;
    case   150: divisor= 768; break;
    case   134: divisor= 857; break;
    case   110: divisor=1047; break;
    case    75: divisor=1536; break;
    case    50: divisor=2304; break;
    case INQUIRE:
    default:
      divisor=0;
      break;
  }   /*switch*/
  if (divisor!=0) {      /*if setting baud rate*/
    outportb (ubase+SIO_LCR,inportb (ubase+SIO_LCR) | SIO_DLAB);  /*set DLAB bit*/
    outportb (ubase+SIO_DLL,(divisor & 0xFF));       /*divisor latch LSB*/
    outportb (ubase+SIO_DLM,((divisor>>8) & 0xFF));  /*divisor latch MSB*/
    outportb (ubase+SIO_LCR,inportb (ubase+SIO_LCR) & ~SIO_DLAB); /*reset DLAB bit*/
  }
  outportb (ubase+SIO_LCR,inportb (ubase+SIO_LCR) | SIO_DLAB);  /*set DLAB bit*/
  divisor=inportb (ubase+SIO_DLM);                      /*divisor latch MSB*/
  divisor=(divisor<<8) | inportb (ubase+SIO_DLL);       /*divisor latch LSB*/
  outportb (ubase+SIO_LCR,inportb (ubase+SIO_LCR) & ~SIO_DLAB); /*reset DLAB bit*/
  switch (divisor) {
    case    2: baud=56000; break;
    case    3: baud=38400; break;
    case    6: baud=19200; break;
    case   12: baud= 9600; break;
    case   16: baud= 7200; break;
    case   24: baud= 4800; break;
    case   32: baud= 3600; break;
    case   48: baud= 2400; break;
    case   58: baud= 2000; break;
    case   64: baud= 1800; break;
    case   96: baud= 1200; break;
    case  192: baud=  600; break;
    case  384: baud=  300; break;
    case  768: baud=  150; break;
    case  857: baud=  134; break;
    case 1047: baud=  110; break;
    case 1536: baud=   75; break;
    case 2304: baud=   50; break;
    default:   baud=UNKNOWN; break;
  }   /*switch to get baud rate*/
  return (baud);
}   /*s_baudrate*/


word s_databits (word port,word data)  /*returns/sets data bits for port*/
{
  byte curLCR;
  word ubase;
  ubase=s_ports[port].uartbase;

  switch (data) {
    case 5: data=0x00; break;
    case 6: data=0x01; break;
    case 7: data=0x02; break;
    case 8: data=0x03; break;
    case INQUIRE:
    default:
      data=INQUIRE;
      break;
  }   /*switch*/
  if (data!=INQUIRE) {
    outportb (ubase+SIO_LCR,inportb (ubase+SIO_LCR) & ~SIO_DLAB); /*reset DLAB bit*/
    outportb (ubase+SIO_LCR,(inportb (ubase+SIO_LCR) & ~SIO_WLS) | data);
  }
  outportb (ubase+SIO_LCR,inportb (ubase+SIO_LCR) & ~SIO_DLAB); /*reset DLAB bit*/
  switch (inportb (ubase+SIO_LCR) & SIO_WLS) {
    case 0x00: data=5; break;
    case 0x01: data=6; break;
    case 0x02: data=7; break;
    case 0x03: data=8; break;
    default: data=UNKNOWN; break;
  }   /*switch*/
  return (data);
}   /*s_databits*/


word s_stopbits (word port,word bits)  /*returns/sets stop bits for port*/
{
  word ubase;
  ubase=s_ports[port].uartbase;
  switch (bits) {
    case 1: bits=SIO_1STB;  break;
    case 2: bits=SIO_2STB; break;
    case INQUIRE:
    default: bits=INQUIRE; break;
  }   /*switch*/
  if (bits!=INQUIRE) {
    outportb (ubase+SIO_LCR,inportb (ubase+SIO_LCR) & ~SIO_DLAB); /*reset DLAB bit*/
    outportb (ubase+SIO_LCR,(inportb (ubase+SIO_LCR) & ~SIO_STB) | bits);
  }
  outportb (ubase+SIO_LCR,inportb (ubase+SIO_LCR) & ~SIO_DLAB); /*reset DLAB bit*/
  switch (inportb (ubase+SIO_LCR) & SIO_STB) {
    case SIO_1STB: bits=1; break;
    case SIO_2STB: bits=2; break;
    default: bits=UNKNOWN; break;
  }   /*switch*/
  return (bits);
}   /*s_stopbits*/


word s_parity (word port,word parity)  /*return/set parity*/
{
  word ubase;
  ubase=s_ports[port].uartbase;
  switch (parity) {
    case NO_PARITY:   parity=SIO_NONE; break;
    case ODD_PARITY:  parity=SIO_ODD;  break;
    case EVEN_PARITY: parity=SIO_EVEN; break;
    case INQUIRE:
    default: parity=INQUIRE; break;
  }   /*switch*/
  if (parity!=INQUIRE) {
    outportb (ubase+SIO_LCR,inportb (ubase+SIO_LCR) & ~SIO_DLAB); /*reset DLAB*/
    outportb (ubase+SIO_LCR,(inportb (ubase+SIO_LCR) & ~SIO_PAR) | parity);
  }
  outportb (ubase+SIO_LCR,inportb (ubase+SIO_LCR) & ~SIO_DLAB); /*reset DLAB*/
  switch (inportb (ubase+SIO_LCR) & SIO_PAR) {
    case SIO_NONE: parity=NO_PARITY;   break;
    case SIO_ODD:  parity=ODD_PARITY;  break;
    case SIO_EVEN: parity=EVEN_PARITY; break;
    default: parity=UNKNOWN; break;
  }   /*switch*/
  return (parity);
}   /*s_parity*/


word s_status (word port)              /*returns (line) status for port*/
{
  return (inportb (s_ports[port].uartbase+SIO_LSR));  /*line status register*/
}   /*s_status*/


void s_break (word port,word ticks)    /*holds break for ticks clock ticks*/
{
  word k,curLCR;
  word ubase;

  ubase=s_ports[port].uartbase;
  curLCR=inportb (ubase+SIO_LCR) & ~SIO_DLAB;    /*reset DLAB*/
  outportb (ubase+SIO_LCR,curLCR);
  wait_ticks (1);                                /*align to clock ticker*/
  outportb (ubase+SIO_LCR,curLCR | SIO_SBRK);    /*set break*/
  wait_ticks (ticks);                            /*wait number of ticks*/
  outportb (ubase+SIO_LCR,curLCR & ~SIO_SBRK);   /*reset break*/
}   /*s_break*/


word s_modstatus (word port)           /*modem status for port*/
{
  return (inportb (s_ports[port].uartbase+SIO_MSR));  /*modem status register*/
}   /*s_modstatus*/

