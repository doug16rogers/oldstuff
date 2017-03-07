#ifndef CUTIL_LOADED
  #define CUTIL_LOADED

#define MAX_PARAMS 32
#define locate(r,c) gotoxy (c,r)

#if !defined (TRUE)
  #define TRUE 1
#endif

#if !defined (FALSE)
  #define FALSE 0
#endif

#ifdef CUTIL_MAIN
  #define _UDecl
  #define DEF_NUM_PARAMS   =0
#else
  #define _UDecl extern
  #define DEF_NUM_PARAMS
#endif

_UDecl int num_params DEF_NUM_PARAMS;
_UDecl param_pos[MAX_PARAMS];
_UDecl param_len[MAX_PARAMS];

char upcase ();                              /* puts into uppercase */
unsigned char getkey ();                     /* returns codes listed below */
char *sub_str (char *s,int pos,int len,char *t);   /* returns t */
int white_char (char c);                     /* returns TRUE or FALSE, T/F */
int alphanumeric (char c);                   /* T/F */
int alphanumeric_str (char *s);              /* T/F */
int short_eq (char *s,char *t);              /* T/F */
char *strip_white (char *s);                 /* returns s */
char *strip_comment (char *s,char comchar);  /* returns s */
int c_chr_str (char *s);                     /* returns chars in result */
int parse_str (char *s);                     /* returns num_params */
char *param_str (char *s,int param,char *dst);  /*return dst*/
int param_int (char s[],int param,int *i);   /* returns T if parsed to int */
int show_params (char *s);                   /* returns num_params */
void cinit (char *s,int len1,int attr1,int attr2);

/*
     The key code names below are very convenient when used in
     conjunction with the getkey() function.
*/

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

#define A_1    0x0081
#define A_2    0x0082
#define A_3    0x0083
#define A_4    0x0084
#define A_5    0x0085
#define A_6    0x0086
#define A_7    0x0087
#define A_8    0x0088
#define A_9    0x0089
#define A_0    0x008A
#define A_MINUS  0x008B
#define A_EQUAL  0x008C
#define S_TAB  0x008F
#define A_Q    0x0090
#define A_W    0x0091
#define A_E    0x0092
#define A_R    0x0093
#define A_T    0x0094
#define A_Y    0x0095
#define A_U    0x0096
#define A_I    0x0097
#define A_O    0x0098
#define A_P    0x0099
#define _F11   0x009A
#define _F12   0x009B
#define S_F11  0x009C
#define S_F12  0x009D
#define A_A    0x009E
#define A_S    0x009F
#define A_D    0x00A0
#define A_F    0x00A1
#define A_G    0x00A2
#define A_H    0x00A3
#define A_J    0x00A4
#define A_K    0x00A5
#define A_L    0x00A6
#define C_F11  0x00A8
#define C_F12  0x00A9
#define A_F11  0x00AA
#define A_F12  0x00AB
#define A_Z    0x00AC
#define A_X    0x00AD
#define A_C    0x00AE
#define A_V    0x00AF
#define A_B    0x00B0
#define A_N    0x00B1
#define A_M    0x00B2
#define _F1    0x00BB
#define _F2    0x00BC
#define _F3    0x00BD
#define _F4    0x00BE
#define _F5    0x00BF
#define _F6    0x00C0
#define _F7    0x00C1
#define _F8    0x00C2
#define _F9    0x00C3
#define _F10   0x00C4
#define _HOME  0x00C7
#define _UPAR  0x00C8
#define _PGUP  0x00C9
#define _LTAR  0x00CB
#define _RTAR  0x00CD
#define _END   0x00CF
#define _DNAR  0x00D0
#define _PGDN  0x00D1
#define _INS   0x00D2
#define _DEL   0x00D3
#define S_F1   0x00D4
#define S_F2   0x00D5
#define S_F3   0x00D6
#define S_F4   0x00D7
#define S_F5   0x00D8
#define S_F6   0x00D9
#define S_F7   0x00DA
#define S_F8   0x00DB
#define S_F9   0x00DC
#define S_F10  0x00DD
#define C_F1   0x00DE
#define C_F2   0x00DF
#define C_F3   0x00E0
#define C_F4   0x00E1
#define C_F5   0x00E2
#define C_F6   0x00E3
#define C_F7   0x00E4
#define C_F8   0x00E5
#define C_F9   0x00E6
#define C_F10  0x00E7
#define A_F1   0x00E8
#define A_F2   0x00E9
#define A_F3   0x00EA
#define A_F4   0x00EB
#define A_F5   0x00EC
#define A_F6   0x00ED
#define A_F7   0x00EE
#define A_F8   0x00EF
#define A_F9   0x00F0
#define A_F10  0x00F1
#define C_PRTSC  0x00F2
#define C_LTAR 0x00F3
#define C_RTAR 0x00F4
#define C_END  0x00F5
#define C_PGDN 0x00F6
#define C_HOME 0x00F7
#define C_PGUP 0x00F8

#endif          /* cutil already loaded */