#ifndef __DRSTR_HPP
#define __DRSTR_HPP


#ifndef HEX0
#define HEX0(x)  (hexadecimal_table[(x) & 0x0F])
#endif

#ifndef HEX1
#define HEX1(x)  (hexadecimal_table[((x) >> 4) & 0x0F])
#endif


extern char hexadecimal_table[];       //0-9, A-F

//===========================================================================
//  strNcpy (char*,char*,int)
//  Does a strncpy(), but always inserts the terminator.
//  If destination_length is 0, length-checking is not performed.
//---------------------------------------------------------------------------
void strNcpy(                  //no output
  char* destination,           //destination string
  char* source,                //source string to copy
  int destination_length);     //maximum length of destination, with nul


//===========================================================================
//  strcchr (char*,char*)
//  Converts a c-style (\-notation) string into a single character.  Only
//  one destination character is produced, but multiple source characters
//  may be used to generate it.
//  Returns a pointer to the next character in the source string.
//---------------------------------------------------------------------------
char* strcchr(                 //next character to be converted
  char* d,                     //destination string
  char* s);                    //source string


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
  int maxn);                   //max length of destination, including nul


//===========================================================================
//  chrtocstr (char*,int)
//  Converts a binary character into a c-style (\-notation) string.
//  The destination string should be able to hold at least 5 characters.
//  The terminator (nul) is placed on the destination string.
//---------------------------------------------------------------------------
int  chrtocstr(                //number of characters in result string
  char* destination,           //destination string space
  char character);             //character to be converted


//===========================================================================
//  strtocstr (char*,int,char*,int)
//  Converts a string from binary character data into c-style \-notation.
//  If destination_length is 0, length-checking is not performed.
//---------------------------------------------------------------------------
int  strtocstr(                //number of source characters converted
  char* destination,           //destination string space
  int destination_length,      //maximum length allowed including terminator
  char* source,                //source string for the conversion
  int source_length = 0);      //number of characters to convert from source
                               //   if 0, go til NUL character


#endif
