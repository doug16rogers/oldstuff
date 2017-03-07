#ifndef __KB_HPP
#define __KB_HPP

/****************************************************************************/
/*  ASCII control characters...                                             */
/****************************************************************************/
#ifndef __ASCII
#define __ASCII
#define NUL    0x00    /*null character*/
#define SOH    0x01    /*start of header*/
#define STX    0x02    /*start of text*/
#define ETX    0x03    /*end of text*/
#define EOT    0x04    /*end of transmission*/
#define ENQ    0x05    /*enquiry*/
#define ACK    0x06    /*acknowledge*/
#define BEL    0x07    /*bell, beep*/
#define BS     0x08    /*backspace*/
#define HT     0x09    /*horizontal tab*/
#define TAB    0x09    /*horizontal tab, common name*/
#define LF     0x0A    /*linefeed*/
#define VT     0x0B    /*vertical tab*/
#define FF     0x0C    /*formfeed*/
#define CR     0x0D    /*carriage return, enter*/
#define SO     0x0E    /*shift out*/
#define SI     0x0F    /*shift in*/
#define DC0    0x10    /*device control 0*/
#define DLE    0x10    /*data link escape*/
#define DC1    0x11    /*device control 1*/
#define XON    0x11    /*transmit on*/
#define DC2    0x12    /*device control 2*/
#define DC3    0x13    /*device control 3*/
#define XOFF   0x13    /*transmit off*/
#define DC4    0x14    /*device control 4*/
#define NAK    0x15    /*negative acknowledge*/
#define SYN    0x16    /*synchronous idle*/
#define ETB    0x17    /*end transmission block*/
#define CAN    0x18    /*cancel*/
#define EM     0x19    /*end of medium*/
#define SUB    0x1A    /*substitute*/
#define ESC    0x1B    /*escape*/
#define FS     0x1C    /*file separator*/
#define GS     0x1D    /*group separator*/
#define RS     0x1E    /*record separator*/
#define US     0x1F    /*unit separator*/
#define SPC    0x20    /*space, blank*/
#define DEL    0x7F    /*delete character*/
#endif                                 /*if ascii not already defined*/


/****************************************************************************/
/*  The following special keys are returned by the getkey() function.       */
/*  The keys are simply the second (scan) code with 256 added.              */
/****************************************************************************/
#ifndef __KBKEYS
#define __KBKEYS
#define A_ESC      (-0x01)
#define A_1        (-0x78)
#define A_2        (-0x79)
#define A_3        (-0x7A)
#define A_4        (-0x7B)
#define A_5        (-0x7C)
#define A_6        (-0x7D)
#define A_7        (-0x7E)
#define A_8        (-0x7F)
#define A_9        (-0x02)  //0x80
#define A_0        (-0x03)  //0x81
#define A_MINUS    (-0x04)  //0x82
#define A_EQUAL    (-0x05)  //0x83
#define A_BS       (-0x0E)
#define S_TAB      (-0x0F)
#define A_Q        (-0x10)
#define A_W        (-0x11)
#define A_E        (-0x12)
#define A_R        (-0x13)
#define A_T        (-0x14)
#define A_Y        (-0x15)
#define A_U        (-0x16)
#define A_I        (-0x17)
#define A_O        (-0x18)
#define A_P        (-0x19)
#define A_LTBRKT   (-0x1A)
#define A_RTBRKT   (-0x1B)
#define A_ENTER    (-0x1C)
#define A_A        (-0x1E)
#define A_S        (-0x1F)
#define A_D        (-0x20)
#define A_F        (-0x21)
#define A_G        (-0x22)
#define A_H        (-0x23)
#define A_J        (-0x24)
#define A_K        (-0x25)
#define A_L        (-0x26)
#define A_SEMICOL  (-0x27)
#define A_QUOTE    (-0x28)
#define A_TILDE    (-0x29)
#define A_BSLASH   (-0x2B)
#define _F11       (-0x45)  //0xD9
#define _F12       (-0x46)  //0xDA
#define S_F11      (-0x07)  //0x87
#define S_F12      (-0x08)  //0x88
#define C_F11      (-0x09)  //0x89
#define C_F12      (-0x0A)  //0x8A
#define A_F11      (-0x0B)  //0x8B
#define A_F12      (-0x0C)  //0x8C
#define A_Z        (-0x2C)
#define A_X        (-0x2D)
#define A_C        (-0x2E)
#define A_V        (-0x2F)
#define A_B        (-0x30)
#define A_N        (-0x31)
#define A_M        (-0x32)
#define A_COMMA    (-0x33)
#define A_PERIOD   (-0x34)
#define A_SLASH    (-0x35)
#define A_TAB      (-0x2A)  //0xA5

#define _F1        (-0x3B)
#define _F2        (-0x3C)
#define _F3        (-0x3D)
#define _F4        (-0x3E)
#define _F5        (-0x3F)
#define _F6        (-0x40)
#define _F7        (-0x41)
#define _F8        (-0x42)
#define _F9        (-0x43)
#define _F10       (-0x44)
#define _HOME      (-0x47)
#define _UPAR      (-0x48)
#define _PGUP      (-0x49)
#define _LTAR      (-0x4B)
#define _RTAR      (-0x4D)
#define _END       (-0x4F)
#define _DNAR      (-0x50)
#define _PGDN      (-0x51)
#define _INS       (-0x52)
#define _DEL       (-0x53)
#define S_F1       (-0x54)
#define S_F2       (-0x55)
#define S_F3       (-0x56)
#define S_F4       (-0x57)
#define S_F5       (-0x58)
#define S_F6       (-0x59)
#define S_F7       (-0x5A)
#define S_F8       (-0x5B)
#define S_F9       (-0x5C)
#define S_F10      (-0x5D)
#define C_F1       (-0x5E)
#define C_F2       (-0x5F)
#define C_F3       (-0x60)
#define C_F4       (-0x61)
#define C_F5       (-0x62)
#define C_F6       (-0x63)
#define C_F7       (-0x64)
#define C_F8       (-0x65)
#define C_F9       (-0x66)
#define C_F10      (-0x67)
#define A_F1       (-0x68)
#define A_F2       (-0x69)
#define A_F3       (-0x6A)
#define A_F4       (-0x6B)
#define A_F5       (-0x6C)
#define A_F6       (-0x6D)
#define A_F7       (-0x6E)
#define A_F8       (-0x6F)
#define A_F9       (-0x70)
#define A_F10      (-0x71)
#define C_PRTSC    (-0x72)
#define C_LTAR     (-0x73)
#define C_RTAR     (-0x74)
#define C_END      (-0x75)
#define C_PGDN     (-0x76)
#define C_HOME     (-0x77)
#define C_PGUP     (-0x06)  //0x84

#define C_A        0x01
#define C_B        0x02
#define C_C        0x03
#define C_D        0x04
#define C_E        0x05
#define C_F        0x06
#define C_G        0x07
#define C_H        0x08
#define C_I        0x09
#define C_J        0x0A
#define C_K        0x0B
#define C_L        0x0C
#define C_M        0x0D
#define C_N        0x0E
#define C_O        0x0F
#define C_P        0x10
#define C_Q        0x11
#define C_R        0x12
#define C_S        0x13
#define C_T        0x14
#define C_U        0x15
#define C_V        0x16
#define C_W        0x17
#define C_X        0x18
#define C_Y        0x19
#define C_Z        0x1A
#endif                                 /*if keys not already defined*/


#define KB_INT     0x16                //keyboard interrupt

class KB
{
public:
  int hit(void);
  int operator * () { return hit(); }  //non-zero if keyboard hit, else 0
  int operator ! () { return !hit(); } //opposite of above operator
  char Get(void);                      //get key (with codes #define'd above)
  KB& operator >> (char& c) { c = Get(); return *this; }
  KB& operator << (char& c);           //insert into kb buffer
};   //class KB


extern KB kbin;

#endif
