#include <stdio.h>

#include "z30util.h"
#include "z30.h"
#include "sdlc.h"

BYTE SDLC_tx_addr=TX_ADDR;             //transmit address
BYTE SDLC_rx_addr=RX_ADDR;             //receive address
double SDLC_baud=19200.0;              //baud rate for port
char SDLC_header=1;                    //if adding header

WORD rx_timeout=RX_TIMEOUT;            //55 ms timeout ticks (error=+/-55ms)

CFG_TYPE SDLC_cfg[]={
  { "tx_addr",   CFG_XBYTE,  &SDLC_tx_addr, NULL },
  { "rx_addr",   CFG_XBYTE,  &SDLC_rx_addr, NULL },
  { "baud_rate", CFG_DOUBLE, &SDLC_baud,    NULL },
  { "header",    CFG_BYTE,   &SDLC_header,  NULL },
  { NULL }
};   //SDLC_cfg


//===========================================================================
int  SDLC_init(void)
{
//modes...
  z30write(A,WR9 ,0xc0);               //hardware reset
  z30write(A,WR0 ,0x00);               //reset, cuz that's what book says
  z30write(A,WR4 ,0x20);               //x1 clock, SDLC mode
  z30write(A,WR1 ,0x00);               //no interrupts, reset DMA
  z30write(A,WR2 ,0x00);               //unused interrupt vector
  z30write(A,WR3 ,0xc0);  //04=search  //no rx 4 now,rx 8 data,addr search
  z30write(A,WR5 ,0x63);               //no DTR,tx 8 data,no tx 4 now,CRC on
  z30write(A,WR6 ,SDLC_rx_addr);       //our address
  z30write(A,WR7 ,0x7e);               //sync character, 6 1s
  z30write(A,WR9 ,0x00);               //disable all interrupts -- chipwide
  z30write(A,WR10,0x00);               //NRZ, no loop mode
  z30write(A,WR11,0x16);  //15?        //tx clock=BRG,rx clock=RTXC,TRxC=Tx
  z30setbaud(A,1,SDLC_baud);           //set baud rate
  z30write(A,WR14,0x03);               //no BR 4 now, local loopback
  z30write(A,WR14,0x03);               //again? just follow the book, ma'am
//enables...
  z30write(A,WR14,0x07);//03           //enable BR generator, local loopback
  z30write(A,WR3 ,0xc1);//c5           //rx enable
  z30write(A,WR5 ,0x6b);               //no DTR,tx 8 data bits,enable tx,CRC on
  z30write(A,WR0 ,0x80);               //reset tx CRC
  z30write(A,WR1 ,0x00);               //no interrupts/dma
//interrupt...
  z30write(A,WR15,0x00);               //reset miscellaneous flags
  z30write(A,WR0 ,0x10);               //reset ext/status
  z30write(A,WR0 ,0x10);               //reset ext/status, again
  z30write(A,WR9 ,0x00);               //nothin, but it's in the book

  return 1;
}   //SDLC_init


//===========================================================================
WORD SDLC_rx(void)
{
  rxn=z30recvbytes(A,rxbuf,sizeof(rxbuf),rx_timeout);  //add to current buf
  return rxn;
}   //SDLC_rx


//===========================================================================
WORD SDLC_tx(void)
{
  int i=0;
  if (SDLC_header) {
    txn+=moveup(txbuf,txn,3/*4*/);          //room for header
    txn++;                             //checksum!
    /*txbuf[i++]=0x00;*/
    txbuf[i++]=SDLC_tx_addr;
    txbuf[i++]=LO(txn);
    txbuf[i++]=HI(txn);
    txbuf[txn-1]=-csum(txbuf,txn-1);
  }   //if adding header

  return z30sendbytes(A,txbuf,txn);
}   //SDLC_tx


//===========================================================================
void SDLC_key(int ch)
{
  WORD k;
  switch (ch) {
  case A_B: case 'b': case 'B':
    sprintf(scrap,"%G",SDLC_baud);
    if (inputs("Enter baud rate: ",scrap,sizeof(scrap))==CR)
      sscanf(scrap,"%lG",&SDLC_baud);
    SDLC_init();
    break;
  case A_T: case 't': case 'T':
    sprintf(scrap,"%02X",SDLC_tx_addr);
    if (inputs("Enter hex transmit address: ",scrap,sizeof(scrap))==CR)
      if (sscanf(scrap,"%x",&k)) SDLC_tx_addr=k;
    SDLC_init();
    break;
  case A_R: case 'r': case 'R':
    sprintf(scrap,"%02X",SDLC_rx_addr);
    if (inputs("Enter hex receive address: ",scrap,sizeof(scrap))==CR)
      if (sscanf(scrap,"%x",&k)) SDLC_rx_addr=k;
    SDLC_init();
    break;
  case A_H: case 'h': case 'H':
    SDLC_header=!SDLC_header;
    break;
  }   //switch
}   //SDLC_key


//===========================================================================
char* SDLC_stat(char* stat)
{
  sprintf(stat
    ,"&Baud=%G &TxAddr=%02X &RxAddr=%02X &Header=%s"
    ,SDLC_baud
    ,SDLC_tx_addr
    ,SDLC_rx_addr
    ,onoff[SDLC_header!=0]
  );   //sprintf
  return stat;
}   //SDLC_stat


