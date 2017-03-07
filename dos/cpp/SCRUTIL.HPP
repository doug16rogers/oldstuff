#ifndef __SCRUTIL_H
#define __SCRUTIL_H

#ifndef BYTE
  #define BYTE unsigned char
#endif

#ifndef WORD
  #define WORD unsigned int
#endif

#ifndef LWORD
  #define LWORD unsigned long int
#endif

#define SCRSEG_MONO    0xB0000000UL    /*monochrome video RAM*/
#define SCRSEG_COLOR   0xB8000000UL    /*color video RAM*/
#define SCRMOD         0x00400049UL    /*pointer to video mode byte in RAM*/
#define SCRCOLUMN      0x0040004AUL    /*pointer to number of columns word*/
#define SCRPAGSIZ      0x0040004CUL    /*pointer to page size word in RAM*/
#define SCRPAGOFF      0x0040004EUL    /*pointer to current page offset word*/
#define SCRPAGCUR      0x00400050UL    /*pointer to 8 page cursor words*/
#define SCRCURSIZ      0x00400060UL    /*pointer to cursor size word in RAM*/
#define SCRPAG         0x00400062UL    /*pointer to current page number*/
#define SCREGAROWS     0x00400084UL    /*pointer to EGA (rows-1) byte*/

enum SCREEN_MODES {                    /*video modes for setscrmod*/
  TEXT_40_25_BW        =0x0000,        /*25 rows, 40 columns, b/w text*/
  TEXT_40_25           =0x0001,        /*same, but color*/
  TEXT_80_25_BW        =0x0002,        /*25 rows, 80 columns, b/w text*/
  TEXT_80_25           =0x0003,        /*same, but color*/
  GRAP_320_200_4       =0x0004,        /*graphics, 320x200, 4 color*/
  GRAP_320_200_4_BW    =0x0005,        /*same, grey-scale*/
  GRAP_640_200_2       =0x0006,        /*2-color (BW)*/
  MONO_80_25           =0x0007,        /*monochrome text*/
  GRAP_160_200_16      =0x0008,        /*back to graphics modes*/
  GRAP_320_200_16      =0x0009,
  GRAP_640_200_4       =0x000A,

  TEXT_80_HI           =0x1112         /*either 50 (VGA) or 43 (EGA) rows*/
};   /*screen video modes*/


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
#endif                                 //if ascii not yet defined (__ASCII)


/****************************************************************************/
/*  The following special keys are returned by the getkey() function.       */
/*  The keys are simply the second (scan) code with 256 added.              */
/****************************************************************************/
#ifndef __FKEYS
#define __FKEYS
#define A_ESC      0x0101
#define A_1        0x0178
#define A_2        0x0179
#define A_3        0x017A
#define A_4        0x017B
#define A_5        0x017C
#define A_6        0x017D
#define A_7        0x017E
#define A_8        0x017F
#define A_9        0x0180
#define A_0        0x0181
#define A_MINUS    0x010C
#define A_EQUAL    0x010D
#define A_BS       0x010E
#define S_TAB      0x010F
#define A_Q        0x0110
#define A_W        0x0111
#define A_E        0x0112
#define A_R        0x0113
#define A_T        0x0114
#define A_Y        0x0115
#define A_U        0x0116
#define A_I        0x0117
#define A_O        0x0118
#define A_P        0x0119
#define A_LTBRKT   0x011A
#define A_RTBRKT   0x011B
#define A_ENTER    0x011C
#define A_A        0x011E
#define A_S        0x011F
#define A_D        0x0120
#define A_F        0x0121
#define A_G        0x0122
#define A_H        0x0123
#define A_J        0x0124
#define A_K        0x0125
#define A_L        0x0126
#define A_SEMICOL  0x0127
#define A_QUOTE    0x0128
#define A_TILDE    0x0129
#define A_BSLASH   0x012B
#define _F11       0x01D9
#define _F12       0x01DA
#define S_F11      0x0187
#define S_F12      0x0188
#define C_F11      0x0189
#define C_F12      0x018A
#define A_F11      0x018B
#define A_F12      0x018C
#define A_Z        0x012C
#define A_X        0x012D
#define A_C        0x012E
#define A_V        0x012F
#define A_B        0x0130
#define A_N        0x0131
#define A_M        0x0132
#define A_COMMA    0x0133
#define A_PERIOD   0x0134
#define A_SLASH    0x0135
#define A_TAB      0x01A5

#define _F1        0x013B
#define _F2        0x013C
#define _F3        0x013D
#define _F4        0x013E
#define _F5        0x013F
#define _F6        0x0140
#define _F7        0x0141
#define _F8        0x0142
#define _F9        0x0143
#define _F10       0x0144
#define _HOME      0x0147
#define _UPAR      0x0148
#define _PGUP      0x0149
#define _LTAR      0x014B
#define _RTAR      0x014D
#define _END       0x014F
#define _DNAR      0x0150
#define _PGDN      0x0151
#define _INS       0x0152
#define _DEL       0x0153
#define S_F1       0x0154
#define S_F2       0x0155
#define S_F3       0x0156
#define S_F4       0x0157
#define S_F5       0x0158
#define S_F6       0x0159
#define S_F7       0x015A
#define S_F8       0x015B
#define S_F9       0x015C
#define S_F10      0x015D
#define C_F1       0x015E
#define C_F2       0x015F
#define C_F3       0x0160
#define C_F4       0x0161
#define C_F5       0x0162
#define C_F6       0x0163
#define C_F7       0x0164
#define C_F8       0x0165
#define C_F9       0x0166
#define C_F10      0x0167
#define A_F1       0x0168
#define A_F2       0x0169
#define A_F3       0x016A
#define A_F4       0x016B
#define A_F5       0x016C
#define A_F6       0x016D
#define A_F7       0x016E
#define A_F8       0x016F
#define A_F9       0x0170
#define A_F10      0x0171
#define C_PRTSC    0x0172
#define C_LTAR     0x0173
#define C_RTAR     0x0174
#define C_END      0x0175
#define C_PGDN     0x0176
#define C_HOME     0x0177
#define C_PGUP     0x0178

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
#endif                                 //if keys not yet defined (__FKEYS)


//===========================================================================
int  ptrcmp(void far *p,void far *q);  /*compares two pointers*/
void memcpynf(void near *n,void far *f,WORD k);   /*copies to near from far*/
void memcpyfn(void far *f,void near *n,WORD k);   /*copies to far from near*/
WORD getcrow(void);                    /*get cursor row on screen*/
WORD getccol(void);                    /*get cursor col on screen*/
void setcpos(WORD row,WORD col);       /*set cursor row, column on screen*/
WORD getcsiz(void);                    /*get cursor size (start/stop lines)*/
void setcsiz(WORD siz);                /*set cursor size*/
WORD getsmod(void);                    /*get screen video mode*/
void setsmod(WORD mod);                /*set screen video mode*/
WORD getsrows(void);                   /*get number of text rows available*/
WORD getscols(void);                   /*get number of text columns*/
WORD getsrowscols(void);               /*get rows in LSB, columns in MSB*/
WORD far *setscrseg(void);             /*sets scrseg to point to video RAM*/

int  getkey(void);

extern WORD far *scrseg;               /*pointer to video RAM*/

#endif
