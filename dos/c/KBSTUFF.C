#include <dos.h>
#include <string.h>
#include <stdio.h>


#define XDIGIT(c)      ( (c<'A')?c-'0':(c<'a')?c-'A'+10:c-'a'+10 )
#define ISXDIGIT(c)    ( ((c>='0')&&(c<='9'))? 1 : \
			 ((c>='A')&&(c<='F'))? 1 : \
			 ((c>='a')&&(c<='f'))? 1 : 0 )


typedef unsigned int ushort;


//from tkeys.h for tvision...
//
//  Control keys
//
//  NOTE: these Control key definitions are intended only to provide
//  mnemonic names for the ASCII control codes.  They cannot be used
//  to define menu hotkeys, etc., which require scan codes.
/*
const ushort

    kbCtrlA     = 0x0001,   kbCtrlB     = 0x0002,   kbCtrlC     = 0x0003,
    kbCtrlD     = 0x0004,   kbCtrlE     = 0x0005,   kbCtrlF     = 0x0006,
    kbCtrlG     = 0x0007,   kbCtrlH     = 0x0008,   kbCtrlI     = 0x0009,
    kbCtrlJ     = 0x000a,   kbCtrlK     = 0x000b,   kbCtrlL     = 0x000c,
    kbCtrlM     = 0x000d,   kbCtrlN     = 0x000e,   kbCtrlO     = 0x000f,
    kbCtrlP     = 0x0010,   kbCtrlQ     = 0x0011,   kbCtrlR     = 0x0012,
    kbCtrlS     = 0x0013,   kbCtrlT     = 0x0014,   kbCtrlU     = 0x0015,
    kbCtrlV     = 0x0016,   kbCtrlW     = 0x0017,   kbCtrlX     = 0x0018,
    kbCtrlY     = 0x0019,   kbCtrlZ     = 0x001a,

// Extended key codes

    kbEsc       = 0x011b,   kbAltSpace  = 0x0200,   kbCtrlIns   = 0x0400,
    kbShiftIns  = 0x0500,   kbCtrlDel   = 0x0600,   kbShiftDel  = 0x0700,
    kbBack      = 0x0e08,   kbCtrlBack  = 0x0e7f,   kbShiftTab  = 0x0f00,
    kbTab       = 0x0f09,   kbAltQ      = 0x1000,   kbAltW      = 0x1100,
    kbAltE      = 0x1200,   kbAltR      = 0x1300,   kbAltT      = 0x1400,
    kbAltY      = 0x1500,   kbAltU      = 0x1600,   kbAltI      = 0x1700,
    kbAltO      = 0x1800,   kbAltP      = 0x1900,   kbCtrlEnter = 0x1c0a,
    kbEnter     = 0x1c0d,   kbAltA      = 0x1e00,   kbAltS      = 0x1f00,
    kbAltD      = 0x2000,   kbAltF      = 0x2100,   kbAltG      = 0x2200,
    kbAltH      = 0x2300,   kbAltJ      = 0x2400,   kbAltK      = 0x2500,
    kbAltL      = 0x2600,   kbAltZ      = 0x2c00,   kbAltX      = 0x2d00,
    kbAltC      = 0x2e00,   kbAltV      = 0x2f00,   kbAltB      = 0x3000,
    kbAltN      = 0x3100,   kbAltM      = 0x3200,   kbF1        = 0x3b00,
    kbF2        = 0x3c00,   kbF3        = 0x3d00,   kbF4        = 0x3e00,
    kbF5        = 0x3f00,   kbF6        = 0x4000,   kbF7        = 0x4100,
    kbF8        = 0x4200,   kbF9        = 0x4300,   kbF10       = 0x4400,
    kbHome      = 0x4700,   kbUp        = 0x4800,   kbPgUp      = 0x4900,
    kbGrayMinus = 0x4a2d,   kbLeft      = 0x4b00,   kbRight     = 0x4d00,
    kbGrayPlus  = 0x4e2b,   kbEnd       = 0x4f00,   kbDown      = 0x5000,
    kbPgDn      = 0x5100,   kbIns       = 0x5200,   kbDel       = 0x5300,
    kbShiftF1   = 0x5400,   kbShiftF2   = 0x5500,   kbShiftF3   = 0x5600,
    kbShiftF4   = 0x5700,   kbShiftF5   = 0x5800,   kbShiftF6   = 0x5900,
    kbShiftF7   = 0x5a00,   kbShiftF8   = 0x5b00,   kbShiftF9   = 0x5c00,
    kbShiftF10  = 0x5d00,   kbCtrlF1    = 0x5e00,   kbCtrlF2    = 0x5f00,
    kbCtrlF3    = 0x6000,   kbCtrlF4    = 0x6100,   kbCtrlF5    = 0x6200,
    kbCtrlF6    = 0x6300,   kbCtrlF7    = 0x6400,   kbCtrlF8    = 0x6500,
    kbCtrlF9    = 0x6600,   kbCtrlF10   = 0x6700,   kbAltF1     = 0x6800,
    kbAltF2     = 0x6900,   kbAltF3     = 0x6a00,   kbAltF4     = 0x6b00,
    kbAltF5     = 0x6c00,   kbAltF6     = 0x6d00,   kbAltF7     = 0x6e00,
    kbAltF8     = 0x6f00,   kbAltF9     = 0x7000,   kbAltF10    = 0x7100,
    kbCtrlPrtSc = 0x7200,   kbCtrlLeft  = 0x7300,   kbCtrlRight = 0x7400,
    kbCtrlEnd   = 0x7500,   kbCtrlPgDn  = 0x7600,   kbCtrlHome  = 0x7700,
    kbAlt1      = 0x7800,   kbAlt2      = 0x7900,   kbAlt3      = 0x7a00,
    kbAlt4      = 0x7b00,   kbAlt5      = 0x7c00,   kbAlt6      = 0x7d00,
    kbAlt7      = 0x7e00,   kbAlt8      = 0x7f00,   kbAlt9      = 0x8000,
    kbAlt0      = 0x8100,   kbAltMinus  = 0x8200,   kbAltEqual  = 0x8300,
    kbCtrlPgUp  = 0x8400,   kbNoKey     = 0x0000,

//  Keyboard state and shift masks

    kbRightShift  = 0x0001,
    kbLeftShift   = 0x0002,
    kbCtrlShift   = 0x0004,
    kbAltShift    = 0x0008,
    kbScrollState = 0x0010,
    kbNumState    = 0x0020,
    kbCapsState   = 0x0040,
    kbInsState    = 0x0080;
*/

typedef struct
{
  char*  name;
  ushort code;
} key_def;

key_def keys[] =
{
    { "CtrlA", 0x0001 },   { "CtrlB", 0x0002 },   { "CtrlC", 0x0003 },
    { "CtrlD", 0x0004 },   { "CtrlE", 0x0005 },   { "CtrlF", 0x0006 },
    { "CtrlG", 0x0007 },   { "CtrlH", 0x0008 },   { "CtrlI", 0x0009 },
    { "CtrlJ", 0x000a },   { "CtrlK", 0x000b },   { "CtrlL", 0x000c },
    { "CtrlM", 0x000d },   { "CtrlN", 0x000e },   { "CtrlO", 0x000f },
    { "CtrlP", 0x0010 },   { "CtrlQ", 0x0011 },   { "CtrlR", 0x0012 },
    { "CtrlS", 0x0013 },   { "CtrlT", 0x0014 },   { "CtrlU", 0x0015 },
    { "CtrlV", 0x0016 },   { "CtrlW", 0x0017 },   { "CtrlX", 0x0018 },
    { "CtrlY", 0x0019 },   { "CtrlZ", 0x001a },

// Extended key codes

    { "Esc", 0x011b },       { "AltSpace", 0x0200 }, { "CtrlIns", 0x0400 },
    { "ShiftIns", 0x0500 },  { "CtrlDel", 0x0600 },  { "ShiftDel", 0x0700 },
    { "Back", 0x0e08 },      { "CtrlBack", 0x0e7f }, { "ShiftTab", 0x0f00 },
    { "Tab", 0x0f09 },       { "AltQ", 0x1000 },     { "AltW", 0x1100 },
    { "AltE", 0x1200 },      { "AltR", 0x1300 },     { "AltT", 0x1400 },
    { "AltY", 0x1500 },      { "AltU", 0x1600 },     { "AltI", 0x1700 },
    { "AltO", 0x1800 },      { "AltP", 0x1900 },     { "CtrlEnter", 0x1c0a },
    { "Enter", 0x1c0d },     { "AltA", 0x1e00 },     { "AltS", 0x1f00 },
    { "AltD", 0x2000 },      { "AltF", 0x2100 },     { "AltG", 0x2200 },
    { "AltH", 0x2300 },      { "AltJ", 0x2400 },     { "AltK", 0x2500 },
    { "AltL", 0x2600 },      { "AltZ", 0x2c00 },     { "AltX", 0x2d00 },
    { "AltC", 0x2e00 },      { "AltV", 0x2f00 },     { "AltB", 0x3000 },
    { "AltN", 0x3100 },      { "AltM", 0x3200 },     { "F1", 0x3b00 },
    { "F2", 0x3c00 },        { "F3", 0x3d00 },       { "F4", 0x3e00 },
    { "F5", 0x3f00 },        { "F6", 0x4000 },       { "F7", 0x4100 },
    { "F8", 0x4200 },        { "F9", 0x4300 },       { "F10", 0x4400 },
    { "Home", 0x4700 },      { "Up", 0x4800 },       { "PgUp", 0x4900 },
    { "GrayMinus", 0x4a2d }, { "Left", 0x4b00 },     { "Right", 0x4d00 },
    { "GrayPlus", 0x4e2b },  { "End", 0x4f00 },      { "Down", 0x5000 },
    { "PgDn", 0x5100 },      { "Ins", 0x5200 },      { "Del", 0x5300 },
    { "ShiftF1", 0x5400 },   { "ShiftF2", 0x5500 },  { "ShiftF3", 0x5600 },
    { "ShiftF4", 0x5700 },   { "ShiftF5", 0x5800 },  { "ShiftF6", 0x5900 },
    { "ShiftF7", 0x5a00 },   { "ShiftF8", 0x5b00 },  { "ShiftF9", 0x5c00 },
    { "ShiftF10", 0x5d00 },  { "CtrlF1", 0x5e00 },   { "CtrlF2", 0x5f00 },
    { "CtrlF3", 0x6000 },    { "CtrlF4", 0x6100 },   { "CtrlF5", 0x6200 },
    { "CtrlF6", 0x6300 },    { "CtrlF7", 0x6400 },   { "CtrlF8", 0x6500 },
    { "CtrlF9", 0x6600 },    { "CtrlF10", 0x6700 },  { "AltF1", 0x6800 },
    { "AltF2", 0x6900 },     { "AltF3", 0x6a00 },    { "AltF4", 0x6b00 },
    { "AltF5", 0x6c00 },     { "AltF6", 0x6d00 },    { "AltF7", 0x6e00 },
    { "AltF8", 0x6f00 },     { "AltF9", 0x7000 },    { "AltF10", 0x7100 },
    { "CtrlPrtSc", 0x7200 }, { "CtrlLeft", 0x7300 }, { "CtrlRight", 0x7400 },
    { "CtrlEnd", 0x7500 },   { "CtrlPgDn", 0x7600 }, { "CtrlHome", 0x7700 },
    { "Alt1", 0x7800 },      { "Alt2", 0x7900 },     { "Alt3", 0x7a00 },
    { "Alt4", 0x7b00 },      { "Alt5", 0x7c00 },     { "Alt6", 0x7d00 },
    { "Alt7", 0x7e00 },      { "Alt8", 0x7f00 },     { "Alt9", 0x8000 },
    { "Alt0", 0x8100 },      { "AltMinus", 0x8200 }, { "AltEqual", 0x8300 },
    { "CtrlPgUp", 0x8400 },  { "NoKey", 0x0000 },
};
#define defined_key_count (sizeof(keys)/sizeof(keys[0]))


int strcchr(unsigned int* d,char *s)
{
  char* p;
  char key_name[0x20];
  int i;

  if (*s == '[')
  {
    s++;
    *d = '[';           //default to '[' (for when key is not found)
    strncpy(key_name, s, sizeof(key_name) - 1);
    key_name[sizeof(key_name) - 1] = 0;
    p = strchr(key_name, ']');
    if (p == NULL) return 1;
    *p = 0;
    for (i = 0; i < defined_key_count; i++)
    {
      if (strcmp(key_name, keys[i].name) == 0) break;
    }
    if (i >= defined_key_count) return 1;
    *d = keys[i].code;
    i = strlen(key_name);
    s += i + 1;     //skip name and ']' in original string
    return i + 2;   //used a '[', the name, and a ']'
  }

  if (*s != '\\')       //no conversion necessary
  {
    *d = *s;
    return 1;
  }

  s++;
  *d = *s++;            //default is to copy char following '\\'
  switch (*d)
  {
  case 0:    *d='\\'; return 1;
  case 'a':  *d=0x07; return 2;
  case 'b':  *d=0x08; return 2;
  case 't':  *d=0x09; return 2;
  case 'n':  *d=0x0A; return 2;
  case 'v':  *d=0x0B; return 2;
  case 'f':  *d=0x0C; return 2;
  case 'r':  *d=0x0D; return 2;
  case 'e':  *d=0x1B; return 2;
  case '\"': *d=0x22; return 2;
  case '\'': *d=0x27; return 2;
  case '\\': *d=0x5C; return 2;
  case 'x':
  case 'X':
    if     (!ISXDIGIT(*s)) { *d='\\'; return 1; }
    *d=        XDIGIT(*s); s++;
    if     (!ISXDIGIT(*s)) return 3;
    *d=(*d<<4)+XDIGIT(*s); s++;
    if     (!ISXDIGIT(*s)) return 4;
    *d=(*d<<4)+XDIGIT(*s); s++;
    if     (!ISXDIGIT(*s)) return 5;
    *d=(*d<<4)+XDIGIT(*s);
    return 6;
  default:
    if ((*s<'0')&&(*s>'7')) break;
    *d=*s-'0';  s++;
    if ((*s<'0')&&(*s>'7')) return 2;
    *d=(*d<<3)+*s-'0';  s++;
    if ((*s<'0')&&(*s>'7')) return 3;
    *d=(*d<<3)+*s-'0';  s++;
    if ((*s<'0')&&(*s>'7')) return 4;
    *d=(*d<<3)+*s-'0';  s++;
    if ((*s<'0')&&(*s>'7')) return 5;
    *d=(*d<<3)+*s-'0';  s++;
    if ((*s<'0')&&(*s>'7')) return 6;
    *d=(*d<<3)+*s-'0';
    return 7;
  }   //switch
  return 2;
}   //strcchr


int strcstr(unsigned int* d,char *s,int dsize)
{
  int i=0;
  int j=0;

  dsize--;                             //leave room for terminator
  for (i=0;i<dsize;i++) {
    if (!s[j]) break;
    j+=strcchr(d++,&s[j]);             //advance pointer
  }   //for
  *d=0;
  return i;
}   //strcstr


int  writekbbuf(unsigned int k)        //returns 1 on success, not full
{
  union REGS r;
  r.h.ah=0x05;
  r.x.cx=k;
  int86(0x16,&r,&r);
  return r.h.al==0;
}   //writekbbuf


int  main(int argc,char* argv[])
{
  unsigned int key_code[80];
  int len;
  int i;
//  unsigned int far* kb_head=(unsigned int far*)0x0040001AUL;
//  unsigned int far* kb_tail=(unsigned int far*)0x0040001CUL;
//  unsigned int far* kb_base=(unsigned int far*)0x00400000UL;
//  unsigned int head;
//  unsigned int tail;

  if (argc<2)
  {
    printf(
      "kbbuf -- puts characters into keyboard buffer.\n"
      "Usage: kbbuf <string>\n"
      "Return codes are:\n"
      "  0 -- success\n"
      "  n -- number of characters NOT loaded into buffer\n"
      "If <string> is to contain blanks, surround it with \"'s.\n"
      "Strings may contain standard C escape sequences:\n"
      "  \\OOOOOO=1-6 Octals            \\xHHHH=1-4 Hex digits\n"
      "  \\a=\\x07   \\b=\\x08   \\t=\\x09   \\n=\\x0A   \\v=\\x0B\n"
      "  \\f=\\x0C   \\r=\\x0D   \\e=\\x1B   \\\\=\\x5C   \\\"=\\x22\n"
      "  \\'=\\x27   NOTE: \\[=[\n"
      "Also, special keystrokes may be entered between '[' and ']':\n"
    );
    for (i = 0; i < defined_key_count; i++) printf("%s\t", keys[i].name);
    printf("\n");
    return 1;
  }

  len=strcstr(key_code,argv[1],sizeof(key_code)/sizeof(key_code[0]));
  for (i=0;i<len;i++) if (!writekbbuf(key_code[i])) break;
  return len-i;
}   //main


