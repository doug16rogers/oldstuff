#include <stdio.h>
#include <string.h>
#include <iostream.h>
#include <stdarg.h>


class Fun {
private:
  int* dat;
  int  siz;
  int  cur;
public:
  Fun(int size) { dat=new int[siz=size]; cur=0; };
  ~Fun(void)   { delete dat; };
  void reset(void) { cur=0; };
  int  set(int v=0) { dat[cur++]=v; if (cur>=siz) cur=0; return cur; };
  int  pos(void) { return cur; };
  friend ostream& operator << (ostream& str,Fun& f);
  ostream& printf(ostream& str,const char* fmt,...);
};   /*Fun*/


ostream& operator << (ostream& str,Fun& f) {
  str << '[' << f.dat[0];
  for (int i=1;i<f.siz;i++) str << ',' << f.dat[i];
  str << ']';
  return str;
}   /*overriding << for Fun's*/


class Number {
private:
  int siz;
  int pos;
  unsigned char* num;
public:
  Number(int size=16);
  ~Number(void)    { delete num; };
  void zero(void);
  void reset(void) { pos=0; }

  friend Number operator + (Number& a1,Number& a2);
  void operator = (Number& n);
  Number& operator << (int v) { num[pos++]=v; if (pos>=siz) pos=0; return *this; };
  Number& operator << (const char* val);

  friend ostream& operator << (ostream& str,Number& n);
};   /*Number*/


void Number::zero(void)
{
  for (int i=0;i<siz;i++) num[i]=0;
  pos=0;
}   /*Number::zero*/


Number::Number(int size)
{
  num=new unsigned char[siz=size];
  pos=0;
  for (int i=0;i<siz;i++) num[i]=0;
}   /*Number::Number*/


void Number::operator = (Number &n)
{
  int s=(siz<n.siz) ? siz : n.siz;
  int i;
  for (i=0;i<s;i++) num[i]=n.num[i];
  while (i<siz) num[i++]=0;
}   /*Number::=*/


Number operator + (Number& a1,Number& a2)
{
  unsigned int c=0;
  if (a1.siz>a2.siz) {
    Number s(a1.siz);
    int i;
    for (i=0;i<a2.siz;i++) s.num[i]=c=(c>>8)+a1.num[i]+a2.num[i];
    for (;(c&0xFF00)&&(i<s.siz);i++) s.num[i]=c=(c>>8)+a1.num[i];
    return s;
  }
  else {
    Number s(a2.siz);
    int i;
    for (i=0;i<a1.siz;i++) s.num[i]=c=(c>>8)+a1.num[i]+a2.num[i];
    for (;(c&0xFF00)&&(i<s.siz);i++) s.num[i]=c=(c>>8)+a2.num[i];
    return s;
  }
}   /*Number::+*/


inline int hexval(char c)
{
  if ((c>='0')&&(c<='9')) return c-'0';
  if ((c>='A')&&(c<='F')) return c-'A'+10;
  if ((c>='a')&&(c<='f')) return c-'a'+10;
  return 0;
}   /*hexval*/



Number& Number::operator << (const char* val)
{
  int l=strlen(val)-1;
  int v;
  while (l>=0) {
    v=hexval(val[l--]);
    if (l>=0) v+=hexval(val[l--])<<4;
    *this << v;
  }
  return *this;
}   /*Number::<<*/


ostream& operator << (ostream& str,Number& n)
{
  static char *hex="0123456789ABCDEF?";
  str << "0x";
  for (int i=(n.siz-1);i>=0;i--)
    str << hex[(n.num[i]>>4)&0x0F] << hex[n.num[i]&0x0F];
  return str;
}   /*Number*/


void main()
{
  Fun f(10);
  Number a(3),b(3);

  int i=10;
  while (f.set(i++));
  cout << "=============================\n" << "The Fun is " << f << '\n';
  a.zero();
  b.zero();
  a << "0789AB";
  b << "BA9876";
  Number c(5);
  c=a+b;
  cout << a << " + " << b << " = " << c << '\n';
}   /*main*/

