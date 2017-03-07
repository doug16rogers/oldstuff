#include "string.hpp"


char String::scrap = ' ';


String::String(int _size,char* s)
{
  size = 0;
  len = 0;
  flags = clip;
  endchar = '\n';
  str = NULL;
  if (_size<1) _size = STRING_MAX_SIZE;
  str = new char[_size];
  if (str == NULL) return;
  size = _size;
  if (s != NULL)
    for (len=0;(len<(size-1)) && (s[len]);len++) str[len] = s[len];
  str[len] = 0;
}   //String::String(int,char*)


String::String(char* s)
{
  size = 0;
  len = 0;
  flags = clip;
  endchar = '\n';
  if (s == NULL)
    str = new char[strlen(s)+1];
  else
    str = new char[STRING_MAX_SIZE];
  if (str == NULL) return;
  while (*s) str[len++] = *s++;
  str[len]=0;
}   //String::String(char*)


String::~String()
{
  if (str) delete str;
  str = NULL;
}   //String::~String


void String::operator = (String& s)
{
  int l = !s;
  if (l>size) l = size-1;
  for (len=0;len<l;len++) str[len] = s.str[len];
  str[len]=0;
}   //String::operator = (String&)


void String::operator = (char* s)
{
  int l = strlen(s);
  if (l>size) l = size-1;
  for (len=0;len<l;len++) str[len] = *s++;
  str[len]=0;
}   //String::operator = (char*)


void String::operator = (char c)
{
  if (size<2) return;
  str[0] = c;
  str[1] = 0;
  len=1;
}   //String::operator = (char*)


String& String::operator + (String &s)
{
  static String* sum=NULL;

  if (sum != NULL) { delete sum; sum=NULL; }
  sum = new String(len + !s + 1);
  if (sum == NULL) return *this;
  int i;
  for (i=0;i<len;i++) sum->str[(sum->len)++] = str[i];
  for (i=0;i<!s;i++)  sum->str[(sum->len)++] = s.str[i];
  sum->str[sum->len] = 0;
  return *sum;
}   //String::operator + (String&)


String& String::operator + (char* s)
{
  static String* sum=NULL;

  if (sum != NULL) { delete sum; sum=NULL; }
  sum = new String(len + strlen(s) + 1);
  if (sum == NULL) return *this;
  int i;
  for (i=0;i<len;i++) sum->str[(sum->len)++] = str[i];
  while (*s)  sum->str[(sum->len)++] = *s++;
  sum->str[sum->len] = 0;
  return *sum;
}   //String::operator + (char*)


String& String::operator + (char c)
{
  static String* sum=NULL;

  if (sum != NULL) { delete sum; sum=NULL; }
  sum = new String(len + 2);
  if (sum == NULL) return *this;
  int i;
  for (i=0;i<len;i++) sum->str[(sum->len)++] = str[i];
  sum->str[(sum->len)++] = c;
  sum->str[sum->len] = 0;
  return *sum;
}   //String::operator + (char)


String& String::Sub(int first,int length)
{
  static String* sub = NULL;

  if (first > len) first = len;
  if (length > (len - first)) length = len - first;
  if (sub != NULL) { delete sub; sub=NULL; }
  sub = new String(length + 2);
  if (sub == NULL) return *this;
  for (int i = 0; (i < length); i++) *sub += str[first+i];
  return *sub;
}   //String::Sub(int,int)


ostream& operator << (ostream& o,String& s)
{
  if ((s.flags & String::clip) && (o.width() > 0))
  {
    s.scrap = s.str[o.width()];
    s.str[o.width()] = 0;
    o << s.str;
    s.str[o.width()] = s.scrap;
  }
  else
    o << s.str;
  return o;
}   //operator << (ostream&,String&)


String& operator >> (istream& i,String& s)
{
  i.getline(s.str,s.size,s.endchar);
  s.len = strlen(s.str);
  return s;
}   //operator >> (istream&,String&)

