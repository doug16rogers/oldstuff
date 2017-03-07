/***************************************************************************
/*
/*         TOOLS.H:  Various #defines and typedefs for GREP
/*
/*                Copyright (c) 1984 Allen Hollub
/*      Copyright (c) 1984 Software Engineering Consultants
/*                          P.O. Box 5679
/*                      Berkeley, CA,  94705
/*
/*                      All rights reserved.
/*
/**************************************************************************/

/*      #defines for non-printing ASCII characters     */

#define NUL     0x00     /* ^@   */
#define SOH     0x01     /* ^A   */
#define STX     0x02     /* ^B   */
#define ETX     0x03     /* ^C   */
#define EOT     0x04     /* ^D   */
#define ENQ     0x05     /* ^E   */
#define ACK     0x06     /* ^F   */
#define BEL     0x07     /* ^G   */
#define BS      0x08     /* ^H   */
#define HT      0x09     /* ^I   */
#define LF      0x0a     /* ^J   */
#define NL      LF
#define VT      0x0b     /* ^K   */
#define FF      0x0c     /* ^L   */
#define CR      0x0d     /* ^M   */
#define SO      0x0e     /* ^N   */
#define SI      0x0f     /* ^O   */
#define DLE     0x10     /* ^P   */
#define DC1     0x11     /* ^Q   */
#define DC2     0x12     /* ^R   */
#define DC3     0x13     /* ^S   */
#define DC4     0x14     /* ^T   */
#define NAK     0x15     /* ^U   */
#define SYN     0x16     /* ^V   */
#define ETB     0x17     /* ^W   */
#define CAN     0x18     /* ^X   */
#define EM      0x19     /* ^Y   */
#define SUB     0x20     /* ^Z   */
#define CPMEOF  SUB
#define ESC     0x1b     /* ^[   */
#define FS      0x1c     /* ^\   */
#define GS      0x1d     /* ^]   */
#define RS      0x1e     /* ^^   */
#define US      0x1f     /* ^_   */
#define DEL     0x7f     /* DEL  */

#define TRUE  1
#define FALSE 0

/*      Definitions of meta-characters used in pattern matching routines  */

#define BOL     '^'
#define EOL     '$'
#define ANY     '.'
#define LITCHAR 'L'
#define ESCAPE  '\\'
#define CCL     '['             /* Character class:  [...]                */
#define CCLEND  ']'
#define NEGATE  '^'
#define NCCL    '!'             /* Negative character class [^...]        */
#define CLOSURE '*'
#define OR_SYM  '|'

#define CLS_SIZE 128            /* Largest permitted size for an expanded */
                                /*      character size- i.e. class [a-z]  */
                                /*      will expand to 26 chars, [a-zA-Z] */
                                /*      will expand into 36 symbols.      */


/*    Tokens are used to hold pattern templates.                          */

typedef struct token {
                      char         tok;
                      char         lchar;
                      char         *string;
                      char         *lstring;
                      struct token *next;
} TOKEN;

#define TOKSIZE sizeof(TOKEN)


/*    An absolute maximum for strings.                                    */

#define MAXSTR     132

extern char   *matchs();
extern int    amatch();
extern char   *in_string();
extern TOKEN  *getpat();
extern int    esc();
extern int    dodash();
extern TOKEN  *makepat();
extern int    unmakepat();
extern int    insert();
extern int    delete();
extern int    isalphanum();
extern int    stoupper();
extern int    pr_tok();
extern int    pr_line();
extern int    max();