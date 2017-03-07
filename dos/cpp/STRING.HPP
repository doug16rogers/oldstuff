#ifndef __STRING_HPP
#define __STRING_HPP

#include <iostream.h>
#include <string.h>


class String {
protected:
#define STRING_MAX_SIZE    0x80
  static char scrap;
  char endchar;
  char* str;
  int len;
  int size;
  unsigned char flags;
public:
  enum
  {
    clip   = 0x01,                     //clip if doesn't fit in ostream width
  };

  String(int _size=STRING_MAX_SIZE,char* s=NULL);
  String(char* s);
  ~String();
  int operator ! (void) { return len; }
  char* S (void) { return (char*) str; }
  int Size(void) { return size; }
  int Length(void) { return len; }
  int Length(int l);
  char End(void) { return endchar; }
  char End(char end) { char c = endchar; endchar = end; return c; }
  unsigned char Flags(void) { return flags; }
  unsigned char Flags(unsigned char _flags, unsigned char which_flags = 0xFF)
  {
    unsigned char old = flags;
    flags &= ~which_flags;
    flags |= (_flags & which_flags);
    return old;
  }

  int Upper(void) { strupr(str); return len; }
  int Lower(void) { strlwr(str); return len; }
  String& Sub(int first,int length = 0x4000);      //substring

  String& operator -- (void)
  {
    if (len > 0) len--;
    str[len] = 0;
    return *this;
  }
  String& operator ++ (void)
  {
    if (len < (size-1)) str[len++] = ' ';
    str[len] = 0;
    return *this;
  }
  char& operator [] (int i);
  void operator  = (String& s);
  void operator  = (char* s);
  void operator  = (char c);
  String& operator + (String& s);
  String& operator + (char* s);
  String& operator + (char c);
  void operator += (String& s) { *this = (*this+s); }
  void operator += (char* s)   { *this = (*this+s); }
  void operator += (char c)    { *this = (*this+c); }

  int operator <  (String& s) { return strcmp(str,s.str) <  0; }
  int operator == (String& s) { return strcmp(str,s.str) == 0; }
  int operator >  (String& s) { return strcmp(str,s.str) >  0; }
  int operator >= (String& s) { return strcmp(str,s.str) >= 0; }
  int operator != (String& s) { return strcmp(str,s.str) != 0; }
  int operator <= (String& s) { return strcmp(str,s.str) <= 0; }

  int operator <= (char* s)   { return strcmp(str,s)     <= 0; }
  int operator <  (char* s)   { return strcmp(str,s)     <  0; }
  int operator == (char* s)   { return strcmp(str,s)     == 0; }
  int operator >  (char* s)   { return strcmp(str,s)     >  0; }
  int operator >= (char* s)   { return strcmp(str,s)     >= 0; }
  int operator != (char* s)   { return strcmp(str,s)     != 0; }

  friend int operator <  (char* s,String& st) { return strcmp(s,st.str) <0; }
  friend int operator == (char* s,String& st) { return strcmp(s,st.str)==0; }
  friend int operator >  (char* s,String& st) { return strcmp(s,st.str) >0; }
  friend int operator >= (char* s,String& st) { return strcmp(s,st.str)>=0; }
  friend int operator != (char* s,String& st) { return strcmp(s,st.str)!=0; }
  friend int operator <= (char* s,String& st) { return strcmp(s,st.str)<=0; }

  friend ostream& operator << (ostream& o,String& s);
  friend String&  operator >> (istream& i,String& s);
};   //class String


inline char& String::operator [] (int i)
{
  if ((i < 0) || (i > len)) return scrap;
  return str[i];
}   //String::operator [](int)


inline int String::Length(int l)
{
  int old = len;
  if (l < 0)
    len = 0;
  else if (l < size)
    len = l;
  str[len] = 0;
  return old;
}   //String::Length(int l)


#endif

