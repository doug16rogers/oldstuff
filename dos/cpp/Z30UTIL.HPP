#ifndef __Z30UTIL_HPP
#define __Z30UTIL_HPP

#include <dos.h>


#ifndef __icitypes
#include "icitypes.h"
#endif

#define CHANA          0
#define A              CHANA
#define CHANB          2
#define B              CHANB

#define Z30_BASE_1     0x0238          /*SW1-1 selected*/
#define Z30_BASE_2     0x02b8          /*SW1-2 selected*/
#define Z30_BASE_3     0x0338          /*SW1-3 selected*/
#define Z30_BASE_4     0x03b8          /*SW1-4 selected*/
#define Z30_BASE_5     0x0278          /*SW1-5 selected*/
#define Z30_BASE_6     0x02f8          /*SW1-6 selected*/
#define Z30_BASE_7     0x0378          /*SW1-7 selected*/
#define Z30_BASE_8     0x03f8          /*SW1-8 selected*/

#define Z30_BASE       Z30_BASE_2      //using switch 2 (SW1-2), 0x02b8
#define Z30_DATA_A     (Z30_BASE+0)
#define Z30_CTRL_A     (Z30_BASE+1)
#define Z30_DATA_B     (Z30_BASE+2)
#define Z30_CTRL_B     (Z30_BASE+3)

#define RR0            0x00
#define RR1            0x01
#define RR2            0x02
#define RR3            0x03
#define RR8            0x08
#define RR10           0x0A
#define RR12           0x0C
#define RR13           0x0D
#define RR15           0x0F

#define WR0            0x00
#define WR1            0x01
#define WR2            0x02
#define WR3            0x03
#define WR4            0x04
#define WR5            0x05
#define WR6            0x06
#define WR7            0x07
#define WR8            0x08
#define WR9            0x09
#define WR10           0x0A
#define WR11           0x0B
#define WR12           0x0C
#define WR13           0x0D
#define WR14           0x0E
#define WR15           0x0F

#define END            0xFF

#define TX_EMPTY_B     0x00
#define EXT_STAT_B     0x01
#define RX_AVAIL_B     0x02
#define RX_SPEC_COND_B 0x03
#define TX_EMPTY_A     0x04
#define EXT_STAT_A     0x05
#define RX_AVAIL_A     0x06
#define RX_SPEC_COND_A 0x07


#define Z30OUT(p,v)    \
  asm mov dx,p \
  asm mov ax,v \
  asm out dx,al
#define Z30IN(p) \
  asm mov dx,p \
  asm in  al,dx \
  asm mov ah,ah

#define TIMEOUT_55MS           18      //about 1 second

void z30write(UINT16 chan,UINT16 reg,UINT16 val);
UINT16 z30read(UINT16 chan,UINT16 reg);
int  z30writes(UINT16 chan,UINT8 *b,UINT16 n);
BOOLEAN z30txregmt(int c);
UINT16 z30setbaud(UINT16 chan,UINT16 div,double baud);
BOOLEAN z30check(void);                   //checks for presence of 8530
UINT16 z30sendbytes(UINT16 c,UINT8* s,UINT16 n);
UINT16 z30recvbytes(UINT16 c,UINT8* s,UINT16 nmax,UINT8 timeout_55ms);

UINT16 TxReadyA(UINT8 timeout_55ms = TIMEOUT_55MS);
UINT16 PutFrameA(UINT8* s, UINT16 n, UINT8 timeout_55ms = TIMEOUT_55MS);
UINT16 GetFrameA(UINT8* s, UINT16 nmax, UINT8 timeout_55ms = TIMEOUT_55MS);


extern UINT16 z30base;                   //base register for the card
extern UINT8 z30rxreset[];              //reset rx, use with z30writes
extern UINT8 z30txreset[];              //reset tx, use with z30writes


//===========================================================================
//  Channel A access functions...
//  Note that these functions do NOT use the variable z30base, but are hard-
//  coded to the macro definition Z30_BASE.
//---------------------------------------------------------------------------
inline UINT8 ReadA(UINT16 reg)
{
  outportb(Z30_CTRL_A, reg);
  _SI++;  _SI++;  _SI--;  _SI--;
  return inportb(Z30_CTRL_A);
}

inline void WriteA(UINT16 reg, UINT8 value)
{
  outportb(Z30_CTRL_A, reg);
  _SI++;  _SI++;  _SI--;  _SI--;
  outportb(Z30_CTRL_A, value);
}

inline UINT8 ReadDataA(void)
{
  return inportb(Z30_DATA_A);
}

inline void WriteDataA(UINT8 data)
{
  outportb(Z30_DATA_A, data);
}

//===========================================================================
//  Channel B access functions...
//  Note that these functions do NOT use the variable z30base, but are hard-
//  coded to the macro definition Z30_BASE.
//---------------------------------------------------------------------------
inline UINT8 ReadB(UINT16 reg)
{
  outportb(Z30_CTRL_B, reg);
  _SI++;  _SI++;  _SI--;  _SI--;
  return inportb(Z30_CTRL_B);
}

inline void WriteB(UINT16 reg, UINT8 value)
{
  outportb(Z30_CTRL_B, reg);
  _SI++;  _SI++;  _SI--;  _SI--;
  outportb(Z30_CTRL_B, value);
}

inline UINT8 ReadDataB(void)
{
  return inportb(Z30_DATA_B);
}

inline void WriteDataB(UINT8 data)
{
  outportb(Z30_DATA_B, data);
}


#endif                                 //if not already included
