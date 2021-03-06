#if !defined (KEYS_MAIN)
unsigned char getkey ();
#endif

#define NUL    0x00
#define ETX    0x03
#define EOT    0x04
#define ENQ    0x05
#define BEL    0x07
#define BS     0x08
#define HT     0x09
#define TAB    0x09
#define LF     0x0A
#define VT     0x0B
#define FF     0x0C
#define CR     0x0D
#define SO     0x0E
#define SI     0x0F
#define DC0    0x10
#define DC1    0x11
#define XON    0x11
#define DC2    0x12
#define DC3    0x13
#define XOFF   0x13
#define CAN    0x18
#define SUB    0x1A
#define ESC    0x1B
#define SP     0x20

/*  The following special keys are returned by the getkey routine.          */
/*  The keys are simply the second code with the msb set (128 added).       */
/*  Note that the exceptions are the Ctrl-PgUp, F11 and F12 keys, which     */
/*  normally have second function codes of 132, 133 and 134, respectively.  */
/*  S_ , C_ and A_ indicate shift, control and alt, respectively.           */
/*  Also, the Alt-1 .. Alt- have been moved from codes 120-131 (148-159)    */
/*  down to 129-141.                                                        */

#define _NUL   0xFF80
#define A_1    0xFF81
#define A_2    0xFF82
#define A_3    0xFF83
#define A_4    0xFF84
#define A_5    0xFF85
#define A_6    0xFF86
#define A_7    0xFF87
#define A_8    0xFF88
#define A_9    0xFF89
#define A_0    0xFF8A
#define A_MINUS  0xFF8B
#define A_EQUAL  0xFF8C
#define S_TAB  0xFF8F
#define A_Q    0xFF90
#define A_W    0xFF91
#define A_E    0xFF92
#define A_R    0xFF93
#define A_T    0xFF94
#define A_Y    0xFF95
#define A_U    0xFF96
#define A_I    0xFF97
#define A_O    0xFF98
#define A_P    0xFF99
#define _F11   0xFF9A
#define _F12   0xFF9B
#define S_F11  0xFF9C
#define S_F12  0xFF9D
#define A_A    0xFF9E
#define A_S    0xFF9F
#define A_D    0xFFA0
#define A_F    0xFFA1
#define A_G    0xFFA2
#define A_H    0xFFA3
#define A_J    0xFFA4
#define A_K    0xFFA5
#define A_L    0xFFA6
#define C_F11  0xFFA8
#define C_F12  0xFFA9
#define A_F11  0xFFAA
#define A_F12  0xFFAB
#define A_Z    0xFFAC
#define A_X    0xFFAD
#define A_C    0xFFAE
#define A_V    0xFFAF
#define A_B    0xFFB0
#define A_N    0xFFB1
#define A_M    0xFFB2
#define _F1    0xFFBB
#define _F2    0xFFBC
#define _F3    0xFFBD
#define _F4    0xFFBE
#define _F5    0xFFBF
#define _F6    0xFFC0
#define _F7    0xFFC1
#define _F8    0xFFC2
#define _F9    0xFFC3
#define _F10   0xFFC4
#define _HOME  0xFFC7
#define _UPAR  0xFFC8
#define _PGUP  0xFFC9
#define _LTAR  0xFFCB
#define _RTAR  0xFFCD
#define _END   0xFFCF
#define _DNAR  0xFFD0
#define _PGDN  0xFFD1
#define _INS   0xFFD2
#define _DEL   0xFFD3
#define S_F1   0xFFD4
#define S_F2   0xFFD5
#define S_F3   0xFFD6
#define S_F4   0xFFD7
#define S_F5   0xFFD8
#define S_F6   0xFFD9
#define S_F7   0xFFDA
#define S_F8   0xFFDB
#define S_F9   0xFFDC
#define S_F10  0xFFDD
#define C_F1   0xFFDE
#define C_F2   0xFFDF
#define C_F3   0xFFE0
#define C_F4   0xFFE1
#define C_F5   0xFFE2
#define C_F6   0xFFE3
#define C_F7   0xFFE4
#define C_F8   0xFFE5
#define C_F9   0xFFE6
#define C_F10  0xFFE7
#define A_F1   0xFFE8
#define A_F2   0xFFE9
#define A_F3   0xFFEA
#define A_F4   0xFFEB
#define A_F5   0xFFEC
#define A_F6   0xFFED
#define A_F7   0xFFEE
#define A_F8   0xFFEF
#define A_F9   0xFFF0
#define A_F10  0xFFF1
#define C_PRTSC  0xFFF2
#define C_LTAR 0xFFF3
#define C_RTAR 0xFFF4
#define C_END  0xFFF5
#define C_PGDN 0xFFF6
#define C_HOME 0xFFF7
#define C_PGUP 0xFFF8
