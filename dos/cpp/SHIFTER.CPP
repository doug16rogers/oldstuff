#include <stdiostr.h>
#include <ctype.h>
#include <iomanip.h>

#include "string.hpp"
#include "bitstr.hpp"



unsigned char rev[] = {
  0x00, 0x80, 0x40, 0xC0,  0x20, 0xA0, 0x60, 0xE0,
  0x10, 0x90, 0x50, 0xD0,  0x30, 0xB0, 0x70, 0xF0,
  0x08, 0x88, 0x48, 0xC8,  0x28, 0xA8, 0x68, 0xE8,
  0x18, 0x98, 0x58, 0xD8,  0x38, 0xB8, 0x78, 0xF8,
  0x04, 0x84, 0x44, 0xC4,  0x24, 0xA4, 0x64, 0xE4,
  0x14, 0x94, 0x54, 0xD4,  0x34, 0xB4, 0x74, 0xF4,
  0x0C, 0x8C, 0x4C, 0xCC,  0x2C, 0xAC, 0x6C, 0xEC,
  0x1C, 0x9C, 0x5C, 0xDC,  0x3C, 0xBC, 0x7C, 0xFC,
  0x02, 0x82, 0x42, 0xC2,  0x22, 0xA2, 0x62, 0xE2,
  0x12, 0x92, 0x52, 0xD2,  0x32, 0xB2, 0x72, 0xF2,
  0x0A, 0x8A, 0x4A, 0xCA,  0x2A, 0xAA, 0x6A, 0xEA,
  0x1A, 0x9A, 0x5A, 0xDA,  0x3A, 0xBA, 0x7A, 0xFA,
  0x06, 0x86, 0x46, 0xC6,  0x26, 0xA6, 0x66, 0xE6,
  0x16, 0x96, 0x56, 0xD6,  0x36, 0xB6, 0x76, 0xF6,
  0x0E, 0x8E, 0x4E, 0xCE,  0x2E, 0xAE, 0x6E, 0xEE,
  0x1E, 0x9E, 0x5E, 0xDE,  0x3E, 0xBE, 0x7E, 0xFE,
  0x01, 0x81, 0x41, 0xC1,  0x21, 0xA1, 0x61, 0xE1,
  0x11, 0x91, 0x51, 0xD1,  0x31, 0xB1, 0x71, 0xF1,
  0x09, 0x89, 0x49, 0xC9,  0x29, 0xA9, 0x69, 0xE9,
  0x19, 0x99, 0x59, 0xD9,  0x39, 0xB9, 0x79, 0xF9,
  0x05, 0x85, 0x45, 0xC5,  0x25, 0xA5, 0x65, 0xE5,
  0x15, 0x95, 0x55, 0xD5,  0x35, 0xB5, 0x75, 0xF5,
  0x0D, 0x8D, 0x4D, 0xCD,  0x2D, 0xAD, 0x6D, 0xED,
  0x1D, 0x9D, 0x5D, 0xDD,  0x3D, 0xBD, 0x7D, 0xFD,
  0x03, 0x83, 0x43, 0xC3,  0x23, 0xA3, 0x63, 0xE3,
  0x13, 0x93, 0x53, 0xD3,  0x33, 0xB3, 0x73, 0xF3,
  0x0B, 0x8B, 0x4B, 0xCB,  0x2B, 0xAB, 0x6B, 0xEB,
  0x1B, 0x9B, 0x5B, 0xDB,  0x3B, 0xBB, 0x7B, 0xFB,
  0x07, 0x87, 0x47, 0xC7,  0x27, 0xA7, 0x67, 0xE7,
  0x17, 0x97, 0x57, 0xD7,  0x37, 0xB7, 0x77, 0xF7,
  0x0F, 0x8F, 0x4F, 0xCF,  0x2F, 0xAF, 0x6F, 0xEF,
  0x1F, 0x9F, 0x5F, 0xDF,  0x3F, 0xBF, 0x7F, 0xFF,
};   //rev[]

char minus_plus[] = { '-','+' };

#define VERBOSE        0
#define NONPRINT       ' '
#define SEVEN_EIGHT    '7'

char verbose = VERBOSE;
char nonprint = NONPRINT;
char seven_eight = SEVEN_EIGHT;

BIT_STREAM nor_nor;
BIT_STREAM inv_nor;
BIT_STREAM nor_rev;
BIT_STREAM inv_rev;



//===========================================================================
unsigned int  inline digval(           //value up to base 36, 36 on error
  char c)                              //input character in [0-9,A-Z]
{
  if ((c>='0')&&(c<='9')) return c-'0';
  if ((c>='A')&&(c<='Z')) return c-'A'+10;
  if ((c>='a')&&(c<='z')) return c-'a'+10;
  return 36;
}   //digval


//===========================================================================
char* strcchr(                         //pointer to next source character
  char& d,                             //destination character for conversion
  char* s)                             //source string pointer
{
  int n=0;
  int dig=0;
  int base=8;
  char c;

  c=*s++;
  if (c!='\\') {                       //if not a special char
    d=c;                               //..just copy it
    return s;                          //..and quit
  }   //if not escape char

  c=*s++;
  switch (c) {
  case 0:    d=0x5C; s--; return s;    //\-terminated string, leave in
  case 'a':  d=0x07; return s;         //special chars...
  case 'b':  d=0x08; return s;
  case 't':  d=0x09; return s;
  case 'n':  d=0x0A; return s;
  case 'v':  d=0x0B; return s;
  case 'f':  d=0x0C; return s;
  case 'r':  d=0x0D; return s;
  case 'e':  d=0x1B; return s;         //extension to the standard (<ESC>)
  case '\"': d=0x22; return s;
  case '\'': d=0x27; return s;
  case '\\': d=0x5C; return s;
  default:
    if      ((c=='x')||(c=='X')) { n=0;     dig=0; base=16; }
    else if ((c=='d')||(c=='D')) { n=0;     dig=0; base=10; }
    else if ((c>='0')&&(c<='7')) { n=c-'0'; dig=1; base=8; }
    else { d=c;  return s; }           //ignore ill-used \ char
  }   /*switch*/

  while (dig<3) {
    c=digval(*s++);
    if (c<base) {
      n = n*base + c;
      dig++;
    }
    else {
      s--;
      break;
    }   //if base 16
  }   //while
  if (dig)
    d = n;
  else {
    s--;
    d = *s++;
  }
  return s;
}   //strcchr


//===========================================================================
String& ConvertFromCStyle(             //converted String (possibly binary)
  String& string,                      //destination String variable
  char* s)                             //source C-style string
{
  char ch;
  string.Length(0);
  while (*s)
  {
    s = strcchr(ch,s);
    string += ch;
  }   //while
  return string;
}   //ConvertFromCStyle


//===========================================================================
ostream& HexOut(                       //copy of output stream to pass along
  ostream& o,                          //stream to use for binary output
  String& s,                           //string to send out
  char output_mode = '7',              //seven or eight bit display
  char nonprint = ' ')                 //character to use for non-printables
{
#define PAD "                "
  String pad(17);
  int i;
  char c;

  o << setbase(16) << setfill('0') << setiosflags(ios::uppercase);
  pad = PAD;
  for (i = 0; (i < s.Length()); i++)
  {
    if ((s[i] < 0x20) || (s[i] == 0x7F))   //these are always nonprintable
      c = nonprint;
    else if (output_mode == '8')
      c = s[i];
    else if (isprint(s[i]))
      c = s[i];
    else
      c = nonprint;
    pad[i & 0x0F] = c;
    cout << setw(2) << (int)(unsigned char)s[i] << ' ';
    if ((i & 0x03) == 0x03) o << ' ';
    if ((i & 0x0F) == 0x0F)
    {
      o << '|' << pad.S() << "|\n";
      pad = PAD;
    }
  }   //for
  if (i & 0x0F)
  {
    while (i & 0x0F)
    {
      o << "   ";
      if ((i & 0x03) == 0x03) o << ' ';
      i++;
    }
    o << '|' << pad.S() << "|\n";
  }
  return o;
}   //HexOut(ostream&,String&)


//===========================================================================
ostream& operator << (ostream& o,BIT_STREAM& b)
{
  for (int i = 0; (i < b.Length()); i++)
  {
    o << (char) ('0' + b[i]);
    if ((i&7) == 7) o << ' ';
  }
  return o;
}   //operator << (ostream& o,BIT_STREAM& b)


//===========================================================================
String& operator << (String& s,BIT_STREAM& b)
{
  s.Length(0);
  for (int i = 0; (i < b.Length()); i += 8)
    s += rev[b.Peek(i,8)];
  return s;
}   //operator << (String& s,BIT_STREAM& b)


//===========================================================================
void DumpShift(char* name,BIT_STREAM& b)
{
  String out;
  cout << '\n';
  for (int i = 0; (i < 8); i++)
  {
    cout << name << " >>" << i << "= ";
    HexOut(cout, (out<<b), seven_eight, nonprint);
    b.BitUnGet(0);
  }
  for (i = 0; (i < 8); i++) b.BitGet();
}   //DumpShift(char* name,BIT_STREAM& b)


//===========================================================================
int  Help(int k)
{
  cout
    << "\n"
    << "Usage: shifter [switches] <string>\n"
    << "Switches: [default]\n"
    << "  -v[+/-]     verbose output ["
    <<                   minus_plus[VERBOSE!=0] << "]\n"
//    << "  -f/F        use <string> as text/binary filename\n"
    << "  -7/8        show 7-bit ASCII/8-bit graphics ["
    <<                   SEVEN_EIGHT << "]\n"
    << "  -8          show all except control characters\n"
    << "  -c<char>    character to use for nonprintables ["
    <<                   NONPRINT << "]\n"
  ;
  return k;
}   //Help(int)


//===========================================================================
int  LoadFlag(char *name,char *flag,char switch_char)
{
  switch (switch_char)
  {
  case '0':
  case '-':
    *flag = 0;
    break;
  case 0:
  case '1':
  case '+':
    *flag = 1;
    break;
  default:
    cout << "bad flag setting '" << switch_char
         << "' for " << name << " switch\n";
    return 0;
  }   //switch
  return 1;
}   //LoadFlag(char*,char*,char)


//===========================================================================
int  LoadSwitch(char *s)
{
  if ((*s != '-') && (*s != '/')) return 1;
  s++;
  switch (*s++)
  {
  case '?':
    return 0;
  case 'v':
    if (!LoadFlag("verbose",&verbose,*s)) return 0;
    break;
  case '7':
    seven_eight = '7';
    break;
  case '8':
    seven_eight = '8';
    break;
  case 'c':
    strcchr(nonprint,s);
    if (nonprint == 0) nonprint = ' ';
    break;
  default:
    s--;
    cout << "unknown switch \"" << s << "\"\n";
    return 0;
  }   //switch
  return 1;
}   //LoadSwitch(char*)


//===========================================================================
int  LoadString(char* s,String& string)
{
  if ((*s == '-') || (*s == '/')) return 1;
  if (string.Length())
  {
    cout << "input string already loaded (\"" << s << "\"\n";
    return 0;
  }
  ConvertFromCStyle(string,s);
  return 1;
}   //LoadString(char*,String&)


//===========================================================================
int  main(int argc,char* argv[])
{
  String source;

  cout << "shifter -- shifts a string in bits\n";

  int i;
  for (i = 1; (i < argc); i++)
    if (!LoadSwitch(argv[i])) return Help(1);
  for (i = 1; (i < argc); i++)
    if (!LoadString(argv[i],source)) return Help(1);

  if (!source.Length()) return Help(1);

//BIT_STREAM uses lsb first, so change directions (revbits)...
  for (i = 0; (i < source.Length()); i++)
  {
    unsigned int ch = (unsigned char)source[i];
    nor_nor.Put((unsigned int)rev[ch],8);
    inv_nor.Put((unsigned int)~rev[ch],8);
    nor_rev.Put(ch,8);
    inv_rev.Put(~ch,8);
  }   //for initialization

  if (verbose)
  {
    cout << "symbols:  -> = forward (msb first on each byte)\n"
            "          <- = backward (lsb first on each byte)\n"
            "          ~  = inverted\n"
            "\n";

    cout << "string = \"" << argv[1] << "\"\n";
    cout << "->  = " << nor_nor  << '\n';
    cout << "~-> = " << inv_nor  << '\n';
    cout << "<-  = " << nor_rev << '\n';
    cout << "~<- = " << inv_rev << '\n';
  }   //if verbose


  DumpShift("-> ",nor_nor);
  DumpShift("~->",inv_nor);
  DumpShift("<- ",nor_rev);
  DumpShift("~<-",inv_rev);

  return 0;

}   //main

