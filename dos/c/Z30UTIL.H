#ifndef __8530util_h
#define __8530util_h

#include <dos.h>

#ifndef BYTE
  #define BYTE unsigned char
#endif

#ifndef WORD
  #define WORD unsigned int
#endif

#ifndef DWORD
  #define DWORD unsigned long
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

#define Z30OUT(p,v)    \
  asm mov dx,p \
  asm mov ax,v \
  asm out dx,al
#define Z30IN(p) \
  asm mov dx,p \
  asm in  al,dx \
  asm mov ah,ah

void z30write(WORD chan,WORD reg,WORD val);
WORD z30read(WORD chan,WORD reg);
int  z30writes(WORD chan,BYTE *b,WORD n);
int  z30txregmt(int c);
WORD z30setbaud(WORD chan,WORD div,double baud);
int  z30check(void);                   //checks for presence of 8530
WORD z30sendbytes(WORD c,BYTE* s,WORD n);
WORD z30recvbytes(WORD c,BYTE* s,WORD nmax,BYTE timeout_55ms);

extern WORD z30base;                   //base register for the card
extern BYTE z30rxreset[];              //reset rx, use with z30writes
extern BYTE z30txreset[];              //reset tx, use with z30writes



void ResetHistory(void);
char* HistoryString(BYTE cmd,BYTE val);

extern BYTE z30history[];              //history of reads and writes
extern WORD z30histcnt;                //history count
extern BYTE z30histon;                 //use history

#endif                                 //if not already included
