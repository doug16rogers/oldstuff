#include <stdiostr.h>
#include <conio.h>

#include "scrutil.hpp"
#include "r250.hpp"
#include "textscr.hpp"
#include "cursor.hpp"


#ifndef BYTE
  #define BYTE unsigned char
#endif
#ifndef WORD
  #define WORD unsigned int
#endif
#ifndef DWORD
  #define DWORD unsigned long int
#endif

#define ATTRBYTE       0xFF00
#define FILL           0x40B0
#define CATTR          0x1700

enum {                                 //names for border field values
  BORDER_0,                            //forces to no appendages match
  BORDER_1,                            //forces to 1 appendage
  BORDER_2,                            //forces to 2 appendages
  BORDER_X                             //don't care what touches it
};


#define TOP_MASK       0xC0
#define RIGHT_MASK     0x30
#define BOTTOM_MASK    0x0C
#define LEFT_MASK      0x03
#define POST_MASK      (TOP_MASK | RIGHT_MASK | BOTTOM_MASK | LEFT_MASK)
#define POST(t,r,b,l)  (((t)<<6)|((r)<<4)|((b)<<2)|((l)<<0))
struct PostChar
{
  BYTE Char;
  BYTE Post;
};
static PostChar post_list[] =
{
  { 0xb4, POST(1,0,1,1) },
  { 0xb5, POST(1,0,1,2) },
  { 0xb6, POST(2,0,2,1) },
  { 0xb7, POST(0,0,2,1) },
  { 0xb8, POST(0,0,1,2) },
  { 0xb9, POST(2,0,2,2) },
  { 0xba, POST(2,0,2,0) },
  { 0xbb, POST(0,0,2,2) },
  { 0xbc, POST(2,0,0,2) },
  { 0xbd, POST(2,0,0,1) },
  { 0xbe, POST(1,0,0,2) },
  { 0xbf, POST(0,0,1,1) },
  { 0xc0, POST(1,1,0,0) },
  { 0xc1, POST(1,1,0,1) },
  { 0xc2, POST(0,1,1,1) },
  { 0xc3, POST(1,1,1,0) },
  { 0xc4, POST(0,1,0,1) },
  { 0xc5, POST(1,1,1,1) },
  { 0xc6, POST(1,2,1,0) },
  { 0xc7, POST(2,1,2,0) },
  { 0xc8, POST(2,2,0,0) },
  { 0xc9, POST(0,2,2,0) },
  { 0xca, POST(2,2,0,2) },
  { 0xcb, POST(0,2,2,2) },
  { 0xcc, POST(2,2,2,0) },
  { 0xcd, POST(0,2,0,2) },
  { 0xce, POST(2,2,2,2) },
  { 0xcf, POST(1,2,0,2) },
  { 0xd0, POST(2,1,0,1) },
  { 0xd1, POST(0,2,1,2) },
  { 0xd2, POST(0,1,2,1) },
  { 0xd3, POST(2,1,0,0) },
  { 0xd4, POST(1,2,0,0) },
  { 0xd5, POST(0,2,1,0) },
  { 0xd6, POST(0,1,2,0) },
  { 0xd7, POST(2,1,2,1) },
  { 0xd8, POST(1,2,1,2) },
  { 0xd9, POST(1,0,0,1) },
  { 0xda, POST(0,1,1,0) },
//  { 0x20, POST(0,0,0,0) },
};   //post_list
#define POST_LIST_SIZE (sizeof(post_list)/sizeof(post_list[0]))

class Atom                             //chars/descriptions
{
private:
  static int lookup_init;
  static BYTE lookup_post[0x100];
  static BYTE lookup_char[0x100];
  void InitLookup(void);
  BYTE val;                            //character
  BYTE flag;
  char row;
  char col;

public:

  enum {
    bad_post      = 0x00,
    bad_character = 0x80,
  };

  typedef enum {
    top    = TOP_MASK,
    right  = RIGHT_MASK,
    bottom = BOTTOM_MASK,
    left   = LEFT_MASK
  } Posts;

  int Top   (void) { return ((val & TOP_MASK   ) >>6); }
  int Right (void) { return ((val & RIGHT_MASK ) >>4); }
  int Bottom(void) { return ((val & BOTTOM_MASK) >>2); }
  int Left  (void) { return ((val & LEFT_MASK  ) >>0); }
  BYTE Char(void) { return lookup_post[ val & POST_MASK ]; }
  char Row(void) { return row; }
  char Row(char _row) { char old = row;  row = _row;  return old; }
  char Col(void) { return col; }
  char Col(char _col) { char old = col;  col = _col;  return old; }
  Atom& Loc(int _row, int _col) { row = _row;  col = _col;  return *this; }

  void operator = (Atom& a)
  {
    val = a.val;
    flag = a.flag;
    row = a.row;
    col = a.col;
  }
  Atom& operator + (Posts p)
  {
    val |= (p & POST_MASK);
    return *this;
  }

  friend ostream& operator << (ostream& o,Atom& a);

  Atom(int _t,int _r,int _b,int _l, int _row = 0, int _col = 0)
  {
    if (lookup_init) InitLookup();
    val = POST(_t,_r,_b,_l) & POST_MASK;
    if (lookup_post[val] == 0) val = post_list[0].Post;
    row = _row;
    col = _col;
  }
  Atom(BYTE _p, int _row = 0, int _col = 0)
  {
    if (lookup_init) InitLookup();
    val = _p & POST_MASK;
    if (lookup_post[val] == 0) val = post_list[0].Post;
    row = _row;
    col = _col;
  }
  Atom(void)
  {
    if (lookup_init) InitLookup();
    val = post_list[0].Post;
    row = col = 0;
  }
};   //class Atom


int Atom::lookup_init = 1;
BYTE Atom::lookup_post[0x100];
BYTE Atom::lookup_char[0x100];

void Atom::InitLookup(void)
{
  lookup_init = 0;
  for (int i = 0; (i < 0x100); i++) lookup_post[i] = bad_post;
  for (i = 0; (i < POST_LIST_SIZE); i++)
    lookup_post[post_list[i].Post] = post_list[i].Char;

  for (i = 0; (i < 0x100); i++) lookup_char[i] = bad_character;
  for (i = 0; (i < POST_LIST_SIZE); i++)
    lookup_char[post_list[i].Char] = post_list[i].Post;
}   //Atom::InitLookup(void)


ostream& operator << (ostream& o,Atom& a)
{
  char one=0;
  if (a.val & TOP_MASK)    { if (one) o<<'+';  o << "top";    one=1; }
  if (a.val & RIGHT_MASK)  { if (one) o<<'+';  o << "right";  one=1; }
  if (a.val & BOTTOM_MASK) { if (one) o<<'+';  o << "bottom"; one=1; }
  if (a.val & LEFT_MASK)   { if (one) o<<'+';  o << "left";   one=1; }
  return o;
}   //ostream& operator << (ostream& o,Atom& a)



//===========================================================================
class Molecule
{
private:
  int size;
  Atom* atom;
  int length;
  WORD attr;
public:
  int   Size(void)   { return size; }
  int   Length(void) { return length; }
  void  Reset(void)  { length = 0; }
  void  NextColor(void) { attr += 0x1000; attr &= 0x7f00; }

  Atom& operator[] (int i)
  {
    if ((i >= 0) && (i < length)) return atom[i];
    return atom[0];
  }
  Molecule& operator +  (Atom& a);
  void operator = (Molecule& m);
  Molecule& operator += (Atom& a) { *this = *this + a;  return *this; }


  friend ostream& operator << (ostream& o,Molecule& m);
  friend TextScreen& operator << (TextScreen& t,Molecule& m);

  Molecule(int _size = 0x20)
  {
    length = 0;
    attr = 0x1F00;
    atom = new Atom[_size];
    if (atom)
      size = _size;
    else
      size = 0;
  }
 ~Molecule()
  {
    if (size) delete atom;
  }
};   //class Molecule


//---------------------------------------------------------------------------
void Molecule::operator = (Molecule& m)
{
  length = m.length;
  if (length > size) length = size;
  for (int i = 0; (i < length); i++)
    atom[i] = m.atom[i];
}   //Molecule::operator = (Molecule& m)


//---------------------------------------------------------------------------
Molecule& Molecule::operator + (Atom& a)
{
  if (length >= size) return *this;
  char r = a.Row();
  char rm1 = r - 1;
  char rp1 = r + 1;
  char c = a.Col();
  char cm1 = c - 1;
  char cp1 = c + 1;
  for (int i = 0; (i < length); i++)
  {
    if (r == atom[i].Row())
    {
      if (c == atom[i].Col()) return *this;
      if ((cm1 == atom[i].Col()) && (a.Left() != atom[i].Right())) return *this;
      if ((cp1 == atom[i].Col()) && (a.Right() != atom[i].Left())) return *this;
    }
    else if (c == atom[i].Col())
    {
      if ((rm1 == atom[i].Row()) && (a.Top() != atom[i].Bottom())) return *this;
      if ((rp1 == atom[i].Row()) && (a.Bottom() != atom[i].Top())) return *this;
    }
  }   //for
  atom[length++] = a;
  return *this;
}


int  Add(Molecule& m,Atom& a)
{
  int l = m.Length();
  m += a;
  return (l != m.Length());
}


//---------------------------------------------------------------------------
ostream& operator << (ostream& o,Molecule& m)
{
  o << "{\n";
  for (int i = 0; (i < m.length); i++)
  {
    o << '\t' << m.atom[i];
    if (i < (m.length - 1)) o << ',';
    o << '\n';
  }
  o << "}\n";
  return o;
}   //ostream& operator << (ostream& o,Molecule& m);


//---------------------------------------------------------------------------
TextScreen& operator << (TextScreen& t,Molecule& m)
{
  WORD ca = m.attr;
  int row = t.row;
  int col = t.col;
  for (int i = 0; (i < m.length); i++)
  {
    Atom& a = m.atom[i];
    ca &= ATTRBYTE;
    ca |= a.Char();
    row = t.row + a.Row();
    col = t.col + a.Col();
    while (row < 0) row += t.rows;
    while (col < 0) col += t.cols;
    while (row >= t.rows) row -= t.rows;
    while (col >= t.cols) col -= t.cols;
    t.video[row*t.cols + col] = ca;
  }
  t.row = row;
  t.col = col;
  return t;
}   //TextScreen& operator << (TextScreen& t,Molecule& m)



R250 R;                                //set up random number generator

BYTE guy[80];
int  lookup[0x100];
TextScreen scr(FILL);


#define COLOR_COUNT_MIN    0x10
#define COLOR_COUNT_RANGE  0x100

void main(void)
{
  Cursor cursor;
  cin >> cursor;
  cout << Cursor().Size(1,0);
  if (!scr.rows) return;
  scr.row = scr.rows/2;
  scr.col = scr.cols/2;

  Molecule m;
  int i;
  int color_count = 0;//COLOR_COUNT_MIN + R.rnd(COLOR_COUNT_RANGE);
  BYTE post;
  int r=0;
  int c=0;
  char continuous = 0;
  char one_shot = 1;
  char alive=1;
  while (alive)
  {
    if (continuous || one_shot)
    {
      one_shot = 0;
      m.Reset();
      r=0;
      c=0;
      i = 100;//+R.rnd(400);
      while (i--)
      {
        post = post_list[R.rnd(POST_LIST_SIZE)].Post;
        switch (R.rnd(4))
        {
        case 0: if (Add(m,Atom(post,r+1,c))) r++; break;
        case 1: if (Add(m,Atom(post,r-1,c))) r--; break;
        case 2: if (Add(m,Atom(post,r,c+1))) c++; break;
        case 3: if (Add(m,Atom(post,r,c-1))) c--; break;
        }   //switch
      }
      if (color_count-- == 0)
      {
        m.NextColor();
        color_count = 0;//COLOR_COUNT_MIN + R.rnd(COLOR_COUNT_RANGE);
      }
      scr << m;
    }   //if doing next molecule
    if (kbhit())
    {
      switch (getch())
      {
      case 'c': scr.Fill(FILL); break;
      case ' ': continuous = !continuous; break;
      case 'a': one_shot = 1; break;
      case 0x1b: alive = 0; break;
      }   //switch on kb
    }   //if kb
  }   //while
  cout << cursor;
}   //main
