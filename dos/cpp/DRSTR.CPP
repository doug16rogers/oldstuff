#include <string.h>

#include "drstr.hpp"


char hexadecimal_table[] = "0123456789ABCDEF?";


//===========================================================================
//  strNcpy (char*,char*,int)
//  Does a strncpy(), but always inserts the terminator.
//  If destination_length is 0, length-checking is not performed.
//---------------------------------------------------------------------------
void strNcpy(                  //no output
  char* destination,           //destination string
  char* source,                //source string to copy
  int destination_length)      //maximum length of destination, with nul
{
  if (destination_length--) {
    strncpy(destination, source, destination_length);
    destination[destination_length]=0;
  }
  else
    strcpy(destination, source);
}   //strNcpy


//===========================================================================
//  digval (char)
//  Converts a character as a digit into an integer.  Digit characters are
//  assumed to run from '0' to '9' and then from 'A' to 'Z'.  Lowercase
//  and uppercase are assumed to be equivalent.  Legal return values are
//  0 through 35, with 36 indicating an unrecognized digit.
//---------------------------------------------------------------------------
int  inline digval(            //value of digit-character, 0-35, 36 on error
  char c)                      //character to convert as a digit
{
  if ((c >= '0') && (c <= '9')) return c - '0';
  if ((c >= 'A') && (c <= 'Z')) return c - 'A' + 10;
  if ((c >= 'a') && (c <= 'z')) return c - 'a' + 10;
  return ('Z' - 'A' + 10) + 1;
}   //digval


//===========================================================================
//  strcchr (char*,char*)
//  Converts a c-style (\-notation) string into a single character.  Only
//  one destination character is produced, but multiple source characters
//  may be used to generate it.
//  Returns a pointer to the next character in the source string.
//---------------------------------------------------------------------------
char* strcchr(                 //next character to be converted
  char* d,                     //destination string
  char* s)                     //source string
{
  int n = 0;                   //current value during base conversion
  int dig = 0;                 //value of current digit
  int base = 8;                //base for conversion
  char c;                      //current character

  c=*s++;
  if (c!='\\') {                       //if not a special char
    *d=c;                              //..just copy it
    return s;                          //..and quit
  }   //if not escape char

  c=*s++;
  switch (c) {
  case 0:    *d=0x5C; s--; return s;   //\-terminated string, leave in
  case 'a':  *d=0x07; return s;        //special chars...
  case 'b':  *d=0x08; return s;
  case 't':  *d=0x09; return s;
  case 'n':  *d=0x0A; return s;
  case 'v':  *d=0x0B; return s;
  case 'f':  *d=0x0C; return s;
  case 'r':  *d=0x0D; return s;
  case 'e':  *d=0x1B; return s;        //extension to the standard (<ESC>)
  case '\"': *d=0x22; return s;
  case '\'': *d=0x27; return s;
  case '\\': *d=0x5C; return s;
  default:
    if      ((c=='x')||(c=='X')) { n=0;     dig=0; base=16; }
    else if ((c=='d')||(c=='D')) { n=0;     dig=0; base=10; }
    else if ((c>='0')&&(c<='7')) { n=c-'0'; dig=1; base=8; }
    else { *d=c;  return s; }          //ignore ill-used \ char
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
    *d = n;
  else {
    s--;
    *d = *s++;
  }
  return s;
}   //strcchr


//===========================================================================
//  strcstr (char*,char*,int)
//  Converts a c-style (\-notation) source string into a binary destination
//  string.
//  Returns the number of characters in the destination string.
//  If maxn is 0, length-checking is not performed on the destination.
//  A terminator is placed in the destination string.
//---------------------------------------------------------------------------
int strcstr(                   //number of characters in destination string
  char *d,                     //destination string
  char *s,                     //source c-style string
  int maxn)                    //max length of destination, including nul
{
  int i=0;
  if (maxn--)                          //reduce by 1 for terminator
    while (*s && (i<maxn)) s=strcchr(&d[i++],s);
  else
    while (*s) s=strcchr(&d[i++],s);
  d[i]=0;
  return i;
}   /*strcstr*/


//===========================================================================
//  chrtocstr (char*,int)
//  Converts a binary character into a c-style (\-notation) string.
//  The destination string should be able to hold at least 5 characters.
//  The terminator (nul) is placed on the destination string.
//---------------------------------------------------------------------------
int  chrtocstr(                //number of characters in result string
  char* destination,           //destination string space
  char character)              //character to be converted
{
  int n = 0;
  unsigned char unsigned_char = (unsigned char) character;

  if ((unsigned_char < 0x20) || (unsigned_char > 0x7E))
  {
    switch (unsigned_char) {
    case 0x07: destination[n++] = '\\';  destination[n++] = 'a';  break;
    case 0x08: destination[n++] = '\\';  destination[n++] = 'b';  break;
    case 0x09: destination[n++] = '\\';  destination[n++] = 't';  break;
    case 0x0A: destination[n++] = '\\';  destination[n++] = 'n';  break;
    case 0x0B: destination[n++] = '\\';  destination[n++] = 'v';  break;
    case 0x0C: destination[n++] = '\\';  destination[n++] = 'f';  break;
    case 0x0D: destination[n++] = '\\';  destination[n++] = 'r';  break;
    default:
      destination[n++] = '\\';
      destination[n++] = 'x';
      destination[n++] = HEX1(unsigned_char);
      destination[n++] = HEX0(unsigned_char);
    }   //switch on nonprintables
  }
  else
  {
    switch (unsigned_char)
    {
    case 0x22: destination[n++] = '\\';  destination[n++] = '\"';  break;
    case 0x27: destination[n++] = '\\';  destination[n++] = '\'';  break;
    case 0x5C: destination[n++] = '\\';  destination[n++] = '\\';  break;
    default:
      destination[n++] = character;
    }   //switch on printables
  }

  return n;
}   //chrtocstr


//===========================================================================
//  strtocstr (char*,int,char*,int)
//  Converts a string from binary character data into c-style \-notation.
//  If destination_length is 0, length-checking is not performed.
//---------------------------------------------------------------------------
int  strtocstr(                //number of source characters converted
  char* destination,           //destination string space
  int destination_length,      //maximum length allowed including terminator
  char* source,                //source string for the conversion
  int source_length)           //number of characters to convert from source
{
  static char one_char_converted[8];  //local copy of a single conversion
  int length_of_conversion = 0;       //length of single conversion
  int source_index;                   //index into source string
  int destination_index = 0;          //index into destination string

  if (destination_length == 0)
    destination_length = 0x7ffe - 8;  //leave room for comparisons
  else
    destination_length--;

  if (source_length == 0)
  {
     source_length = strlen(source);
  }

  for (source_index = 0; (source_index < source_length); source_index++)
  {
    length_of_conversion = chrtocstr(one_char_converted, source[source_index]);
    if ((destination_index + length_of_conversion) > destination_length)
      break;
    strcpy(&destination[destination_index], one_char_converted);
    destination_index += length_of_conversion;
  }   //for each source character

  destination[destination_index] = 0;
  return source_index;
}   //strtocstr

