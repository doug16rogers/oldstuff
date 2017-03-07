#include "z30util.hpp"


/* code to use with the ACB-2 board for the IBM PC */
UINT16 z30base=Z30_BASE_2;               /*use switch SW1-2 setting*/

double clkspeed=3.9936E+6;             /*clock speed on ACB-2*/

UINT8 z30rxreset[]={                    //reset sequence for receiving
  WR0 ,0x40,                           //reset rx CRC checker
  WR0, 0x30,                           //reset error
  END,0
};

UINT8 z30txreset[]={                    //reset sequence for transmitting
  WR0, 0x80,                           //reset tx CRC
  WR0, 0x10,                           //reset /*tx underrun and*/ ext status
  WR0, 0x10,                           //reset /*tx underrun and*/ ext status
  END,0
};



/****************************************************************************/
/*  z30write                                                                */
/****************************************************************************/
/*-----------------------------Description----------------------------------*/
/*  Writes val to z8530 register reg on channel chan.  If the register is   */
/*  not the data register, it automatically writes to the control register  */
/*  first.                                                                  */
/*-----------------------------Arguments------------------------------------*/
/*  UINT16 chan          channel specifier, either A or B                     */
/*  UINT16 reg           register specifier, WR0-WR15                         */
/*  UINT16 val           value to write to WRx                                */
/*-----------------------------Return value---------------------------------*/
/*-----------------------------Global constants-----------------------------*/
/*-------------------Mod-------Global variables-----------------------------*/
/*  z30base            base i/o register for the ACB-2                      */
/*-----------------------------Functions called-----------------------------*/
/*-----------------------------Constraints/Gotchas--------------------------*/
/*--Date--------Programmer----------Comments--------------------------------*/
/*  1990.01.01  A. Turing           initial code                            */
/****************************************************************************/
void z30write(UINT16 chan,UINT16 reg,UINT16 val)
{
  asm mov   dx,chan                    /*get channel into dx*/
  asm and   dx,CHANB                   /*make sure it's a legal channel*/
  asm add   dx,z30base                 /*add in base i/o address*/
  asm mov   bx,reg                     /*put register for write in al*/
  asm and   bx,0000fh                  /*make sure legal register*/
  asm cmp   bx,WR8                     /*see if it's write register 8*/
  asm jne   notWR8                     /*if not, skip*/
doWR8:
  asm mov   ax,val                     /*get value to put out*/
  asm out   dx,al                      /*write out the value*/
  asm jmp   end                        /*th-th-th-that's all*/
notWR8:
  asm inc   dx                         /*point to control register*/
  asm in    al,dx                      /*make sure we're in sync*/
  asm jmp   test1                      /*kill time*/
test1:
  asm mov   ax,bx                      /*get register*/
  asm out   dx,al                      /*write register to control register*/
  asm jmp   test2                      /*kill time*/
test2:
  asm mov   ax,val                     /*get value to write*/
  asm out   dx,al                      /*write out the value*/
  asm jmp   test3                      /*kill time*/
test3:
end:
  ;
}   /*z30write*/


/****************************************************************************/
/*  z30read                                                                 */
/****************************************************************************/
/*-----------------------------Description----------------------------------*/
/*  Reads value from z8530 register reg on channel chan.  If the register   */
/*  is not the data register, it automatically writes to the control        */
/*  register first.                                                         */
/*-----------------------------Arguments------------------------------------*/
/*  UINT16 chan          channel specifier, either A or B                     */
/*  UINT16 reg           register specifier, one of the legal RRx's           */
/*-----------------------------Return value---------------------------------*/
/*  Returns the value read from RRx.                                        */
/*-----------------------------Global constants-----------------------------*/
/*-------------------Mod-------Global variables-----------------------------*/
/*  z30base            base i/o register for the ACB-2                      */
/*-----------------------------Functions called-----------------------------*/
/*-----------------------------Constraints/Gotchas--------------------------*/
/*--Date--------Programmer----------Comments--------------------------------*/
/*  1990.01.01  A. Turing           initial code                            */
/****************************************************************************/
UINT16 z30read(UINT16 chan,UINT16 reg)
{
  asm mov   dx,chan                    /*get channel into dx*/
  asm and   dx,CHANB                   /*make sure it's a legal channel*/
  asm add   dx,z30base                 /*add in base i/o address*/
  asm mov   bx,reg                     /*put register for write in al*/
  asm and   bx,0000fh                  /*make sure legal register*/
  asm cmp   bx,RR8                     /*see if it's read register 8*/
  asm jne   notRR8                     /*if not, skip*/
doRR8:
  asm in    al,dx                      /*read the value*/
  asm jmp   end                        /*th-th-th-that's all*/
notRR8:
  asm inc   dx                         /*point to control register*/
  asm in    al,dx                      /*make sure we're in sync*/
  asm jmp   test1:                     /*kill time*/
test1:
  asm mov   ax,bx                      /*get register*/
  asm out   dx,al                      /*read register => control register*/
  asm jmp   test2                      /*kill time*/
test2:
  asm in    al,dx                      /*read the value*/
  asm jmp   test3                      /*kill time*/
test3:
end:
  asm xor   ah,ah                      /*clear high UINT8*/

  return _AX;
}   /*z30read*/


/****************************************************************************/
/*  z30writes                                                               */
/****************************************************************************/
/*-----------------------------Description----------------------------------*/
/*  Writes a string of n values to registers on channel chan.  b points to  */
/*  an array of alternating registers (WRx) specifier and data values.      */
/*  If n is 0, z30writes will write until the register is END (defined in   */
/*  the include file z30util.h).                                            */
/*  Examples:                                                               */
/*    UINT8 regval[4*2+2]={ WR9,0xC0, WR0,0xC0, WR0,0x80, WR0,0x40, END,0 }; */
/*    z30writes(A,regval,4);                                                */
/*  or..                                                                    */
/*    z30writes(A,regval,0);                                                */
/*-----------------------------Arguments------------------------------------*/
/*  UINT16 chan          channel specifier, either A or B                     */
/*  UINT16 n             number of values to write                            */
/*  UINT8 *b            pointer to UINT8 array -- first register then value   */
/*-----------------------------Return value---------------------------------*/
/*  Returns the number of registers written.                                */
/*-----------------------------Global constants-----------------------------*/
/*-------------------Mod-------Global variables-----------------------------*/
/*-----------------------------Functions called-----------------------------*/
/*  z30write           [8530util] writes value to 8530 register             */
/*-----------------------------Constraints/Gotchas--------------------------*/
/*--Date--------Programmer----------Comments--------------------------------*/
/*  1990.01.01  A. Turing           initial code                            */
/****************************************************************************/
int  z30writes(UINT16 chan,UINT8 *b,UINT16 n)
{
  UINT16 reg;
  UINT16 val;
  UINT16 k=0;

  if (n==0) {
    for (k=0;k<0x01ff;k++) {
      reg=*b++;
      val=*b++;
      if (reg==END) break;
      z30write(chan,reg,val);
    }   /*for*/
  }
  else {
    for (k=0;k<n;k++) {
      reg=*b++;
      val=*b++;
      z30write(chan,reg,val);
    }   /*for*/
  }   /*else n is specified*/
  return k;
}   /*z30writes*/


/****************************************************************************/
/*  z30txregmt                                                              */
/****************************************************************************/
/*-----------------------------Description----------------------------------*/
/*  This function waits for the transmitter holding register to be empty.   */
/*-----------------------------Arguments------------------------------------*/
/*  int c              channel to check                                     */
/*-----------------------------Return value---------------------------------*/
/*  Returns 1 if it goes empty, 0 on timeout.                               */
/*-----------------------------Global constants-----------------------------*/
/*-------------------Mod-------Global variables-----------------------------*/
/*-----------------------------Functions called-----------------------------*/
/*-----------------------------Constraints/Gotchas--------------------------*/
/*--Date--------Programmer----------Comments--------------------------------*/
/*  1990.01.01  A. Turing           initial code                            */
/****************************************************************************/
BOOLEAN z30txregmt(int c)
{
#if 1
  unsigned long k;
  for (k=0;k<0x40000UL;k++) if (z30read(c,RR0)&0x04) return true;
  return false;
#else
  asm mov  dx,c                        /*first get channel*/
  asm and  dx,002h                     /*make sure it's legal*/
  asm add  dx,z30base                  /*put the base address in dx*/
  asm inc  dx                          /*point to control register*/
  asm in   al,dx                       /*make sure we're in sync*/
  asm jmp  $+2                         /*kill time*/
  asm mov  cx,00040h                   /*outer loop..*/
lp0:
  asm push cx                          /*save outer counter*/
  asm mov  cx,01000h                   /*timeout counter*/
lp:
  asm mov  al,RR0                      /*specify RR0*/
  asm out  dx,al                       /*check read register 0*/
  asm jmp  $+2                         /*kill time*/
  asm in   al,dx                       /*get value*/
  asm jmp  $+2                         /*kill time*/
  asm test al,004h                     /*check tx empty ready*/
  asm je   mt                          /*if empty, go say so*/
  asm loop lp                          /*loop for timeout*/
  asm pop  cx                          /*get outer loop counter*/
  asm loop lp0                         /*outer loop*/
  asm mov  ax,0                        /*clear ax -- not empty*/
  asm jmp  end                         /*done!*/
mt:
  asm mov  ax,1                        /*set to 1 -- tx register empty*/
end:
  return _AX;
#endif
}   /*z30txregmt*/


/****************************************************************************/
/*  z30setbaud                                                              */
/****************************************************************************/
/*-----------------------------Description----------------------------------*/
/*  Sets the BR time constant for channel chan, with the divide-down count  */
/*  set to div.  All this is for baud rate baud.  You must program the      */
/*  divide-down count yourself, since other things are also programmed in   */
/*  WR4.                                                                    */
/*-----------------------------Arguments------------------------------------*/
/*  UINT16 chan          channel indicator, A or B                            */
/*  UINT16 div           divide down count must be 1, 16, 32 or 64            */
/*  double baud        desired baud rate, up to 1M for div=1                */
/*-----------------------------Return value---------------------------------*/
/*  Returns the time constant used, or 0xffff on error.                     */
/*-----------------------------Global constants-----------------------------*/
/*  WR12, WR13         baud rate time constant registers -- LSB and MSB     */
/*-------------------Mod-------Global variables-----------------------------*/
/*  clkspeed           [8530util] clock speed of ACB board BR clock         */
/*-----------------------------Functions called-----------------------------*/
/*  z30write           [8530util] writes value to 8530 register             */
/*-----------------------------Constraints/Gotchas--------------------------*/
/*  This function writes to WR12 and WR13, but NOT to WR4.  You must write  */
/*  the divide-down count yourself.                                         */
/*--Date--------Programmer----------Comments--------------------------------*/
/*  1990.01.01  A. Turing           initial code                            */
/****************************************************************************/
UINT16 z30setbaud(UINT16 chan,UINT16 div,double baud)
{
  double timecon;
  UINT16 tc;

  chan&=CHANB;
  if ((div!=1)&&(div!=16)&&(div!=32)&&(div!=64)) return 0xffff;
  timecon=clkspeed/2.0/baud/(double)div;
  tc=(UINT16)(timecon+0.5);
  if (tc<2) return 0xffff;             /*too fast for board*/
  tc-=2;
  z30write(chan,WR12,tc&0xFF);         /*LSB of time constant*/
  z30write(chan,WR13,tc>>8);           /*MSB of time constant*/
  return tc;                           /*return the time constant*/
}   /*z30setbaud*/



/****************************************************************************/
/*  z30check                                                                */
/****************************************************************************/
/*-----------------------------Description----------------------------------*/
/*  Checks for the presence of the 8530 card.                               */
/*-----------------------------Arguments------------------------------------*/
/*-----------------------------Return value---------------------------------*/
/*  Returns 1 if the card is present, 0 otherwise.                          */
/*-----------------------------Global constants-----------------------------*/
/*-------------------Mod-------Global variables-----------------------------*/
/*-----------------------------Functions called-----------------------------*/
/*-----------------------------Examples-------------------------------------*/
/*-----------------------------Constraints/Gotchas--------------------------*/
/*--Date--------Programmer----------Comments--------------------------------*/
/*  1990.01.01  A. Turing           initial code                            */
/****************************************************************************/
BOOLEAN z30check(void)                    //checks for presence of 8530
{
  BOOLEAN ok;
  UINT8 oldval;                        //previous value at port

  oldval=z30read(A,RR12);              //get the old value
  z30write(A,WR12,0xAA);               //try writing to baudrate LSB

  ok = (BOOLEAN) (z30read(A,RR12)==0xAA);       //make sure correct

  if (ok)                              //no error on first, but could be luck
  {
    z30write(A,WR12,0x55);             //try writing to baudrate LSB
    ok = (BOOLEAN) (z30read(A,RR12)==0x55);       //make sure correct
  }
  z30write(A,WR12,oldval);             //restore the previous value

  return ok;

}   //z30check


/****************************************************************************/
/*  z30sendbyte                                                             */
/****************************************************************************/
/*-----------------------------Description----------------------------------*/
/*  Sends the n bytes starting at s to channel c.                           */
/*-----------------------------Arguments------------------------------------*/
/*  UINT16 c             channel over which to send the bytes                 */
/*  UINT8* s            pointer to beginning of array of bytes to send       */
/*  UINT16 n             number of bytes to send                              */
/*-----------------------------Return value---------------------------------*/
/*  Returns the number of bytes successfully transmitted.                   */
/*-----------------------------Global constants-----------------------------*/
/*-------------------Mod-------Global variables-----------------------------*/
/*-----------------------------Functions called-----------------------------*/
/*-----------------------------Examples-------------------------------------*/
/*-----------------------------Constraints/Gotchas--------------------------*/
/*--Date--------Programmer----------Comments--------------------------------*/
/*  1990.01.01  A. Turing           initial code                            */
/****************************************************************************/
UINT16 z30sendbytes(UINT16 c, UINT8* s, UINT16 n)
{
  UINT16 i=0;
  z30writes(c,z30txreset,0);           //reset transmitter
  if (!n) return 0;                    //nothin' to do
  if (!z30txregmt(c)) return 0;        //oops! never room to transmit
  z30write(c,WR8,s[i++]);              //write out first UINT8
  z30write(c,WR0,0xC0);                //TX Underrun/EOM to allow CRC tx
  while (i<n) {                        //go 'til no more bytes to send
    if (!z30txregmt(c)) return i;      //..oops! timed out waiting to be empty
    z30write(c,WR8,s[i++]);            //..write out to data port
  }   //while
  return i;                            //success!
}   //z30sendbytes


//===========================================================================
UINT16 TxReadyA(UINT8 timeout_55ms)
{
#define TIME_TICK_ADDR   0x0000046CUL  //address of timer-tick

  UINT8 end_time;                       //timeout counter
  UINT8 now_time;                       //current time
  UINT16 n;                              //number of bytes received

  if (ReadA(RR0) & 4) return 1;        //check first
  if (timeout_55ms < 1) timeout_55ms = 1;
  if (ReadA(RR0) & 4) return 1;        //check again
  disable();
  end_time = *(UINT8*)TIME_TICK_ADDR;
  enable();
  if (ReadA(RR0) & 4) return 1;        //check again
  end_time += timeout_55ms;            //add in the timeout count

  while (!(ReadA(RR0) & 4))
  {
    disable();
    now_time = *(UINT8*)TIME_TICK_ADDR;
    enable();
    if (now_time == end_time) return 0;    //timeout!
  }   //while
  return 1;
}   //TxReadyA


//===========================================================================
UINT16 PutFrameA(UINT8* s, UINT16 n, UINT8 timeout_55ms)
{
  UINT16 i = 0;
  z30writes(CHANA,z30txreset,0);       //reset transmitter
  if (!n) return 0;                    //nothin' to do
  if (!TxReadyA(timeout_55ms)) return 0;   //oops! never room to transmit
  WriteDataA(s[i++]);                  //write out first UINT8
  WriteA(WR0,0xC0);                    //TX Underrun/EOM to allow CRC tx
  while (i<n)                          //go 'til no more bytes to send
  {
    if (!TxReadyA(timeout_55ms)) return i;   //timed out waiting to be empty
    WriteDataA(s[i++]);                //..write out to data port
  }   //while
  return i;                            //success!
}   //PutFrameA


/****************************************************************************/
/*  z30recvbytes                                                            */
/****************************************************************************/
/*-----------------------------Description----------------------------------*/
/*  Receives up to nmax bytes into the array s from channel c.  Times out   */
/*  after timeout_55ms system timer ticks (which are about 55ms apart).     */
/*-----------------------------Arguments------------------------------------*/
/*  UINT16 c             channel                                              */
/*  UINT8* s            destination UINT8 array pointer                       */
/*  UINT16 nmax          size of the array (to prevent catastrophe)           */
/*  UINT8 timeout_55ms  time ticks, max, for timeout if nothing is received  */
/*-----------------------------Return value---------------------------------*/
/*  Returns the number of bytes successfully received.                      */
/*-----------------------------Global constants-----------------------------*/
/*-------------------Mod-------Global variables-----------------------------*/
/*-----------------------------Functions called-----------------------------*/
/*-----------------------------Examples-------------------------------------*/
/*-----------------------------Constraints/Gotchas--------------------------*/
/*  Timeout is specified only up to 255*55ms = 14 seconds in order to keep  */
/*  computation time down.  End of day handling is NOT performed, so a      */
/*  wait of up to 20 seconds is possible at midnight (0x100+0x50)*55ms if   */
/*  the timeout is set to near 0x4F just before midnight.                   */
/*--Date--------Programmer----------Comments--------------------------------*/
/*  1990.01.01  A. Turing           initial code                            */
/****************************************************************************/
UINT16 z30recvbytes(UINT16 c,UINT8* s,UINT16 nmax,UINT8 timeout_55ms)
{
#define TIME_TICK_ADDR   0x0000046CUL  //address of timer-tick
#define MAX_TIME_TICK    0x001800B0UL  //timer ticks per diem

  UINT8 end_time;                       //timeout counter
  UINT8 now_time;                       //current time
  UINT16 n;                              //number of bytes received

  if (timeout_55ms<1) timeout_55ms=1;
  disable();
  end_time=*(UINT8*)TIME_TICK_ADDR;
  enable();
  end_time+=timeout_55ms;              //add in the timeout count

  n=0;                                 //reset rx counter
//DON'T DO THIS!!!  z30writes(c,z30rxreset,0);           //reset receiver
  while (!(z30read(c,RR1)&0x80)) {     //check for sdlc EOF (end of frame)
    if (z30read(c,RR0)&1) {            //if new char
      do {                             //while character available..
        if (n<nmax)                      //..only load into buffer if room..
          s[n++]=z30read(c,RR8);         //....load data reg to rx buffer
        else                             //..otherwise..
          z30read(c,RR8);                //..just read the thing
      } while (z30read(c,RR0)&1);
      disable();
      now_time=*(UINT8*)TIME_TICK_ADDR;
      enable();
      end_time=now_time+timeout_55ms;
    }   //if char to receive
    disable();
    now_time=*(UINT8*)TIME_TICK_ADDR;
    enable();
    if (now_time==end_time) break;     //timeout!
  }   //while
  if (z30read(c,RR0)&1) z30read(c,RR8);  //extra UINT8 -- flag
  return n;
}   //z30recvbytes


//===========================================================================
UINT16 GetFrameA(UINT8* s, UINT16 nmax, UINT8 timeout_55ms)
{
#define TIME_TICK_ADDR   0x0000046CUL  //address of timer-tick

  UINT8 end_time;                       //timeout counter
  UINT8 now_time;                       //current time
  UINT16 n;                              //number of bytes received

  if (timeout_55ms<1) timeout_55ms=1;
  disable();
  end_time=*(UINT8*)TIME_TICK_ADDR;
  enable();
  end_time+=timeout_55ms;              //add in the timeout count

  n = 0;                               //reset rx counter
  while (!(ReadA(RR1) & 0x80)) {       //check for sdlc EOF (end of frame)
    if (ReadA(RR0) & 1) {              //if new char
      do                               //while byte available..
      {
        if (n < nmax)                  //..only load into buffer if room..
          s[n++] = ReadDataA();        //....load data reg to rx buffer
        else                           //..otherwise..
          ReadDataA();                 //..just read the thing
      } while (ReadA(RR0) & 1);
      disable();
      now_time = *(UINT8*)TIME_TICK_ADDR;
      enable();
      end_time = now_time+timeout_55ms;
    }   //if char to receive
    disable();
    now_time = *(UINT8*)TIME_TICK_ADDR;
    enable();
    if (now_time == end_time) break;   //timeout!
  }   //while
  if (ReadA(RR0) & 1) ReadDataA();     //extra byte -- flag
  return n;
}   //GetFrameA


