#include <stdiostr.h>
#include <dos.h>

#include "cursor.hpp"


Cursor::Cursor(int _row, int _col, int _start, int _end)
{
  Get();
  flags = 0;
  if (_row >= 0) { row = _row;  flags |= new_location; }
  if (_col >= 0) { col = _col;  flags |= new_location; }
  if (_start >= 0) { start = _start;  flags |= new_location; }
  if (_end >= 0) { end = _end;  flags |= new_location; }
}


void Cursor::Get(void)
{
  union REGS r;
  r.h.bh = 0x00;
  r.h.ah = 0x03;
  int86(0x10,&r,&r);
  row = r.h.dh;
  col = r.h.dl;
  start = r.h.ch;
  end = r.h.cl;
  flags = 0;
}


void Cursor::Set(int _row, int _col)
{
  union REGS r;
  if (_row >= 0) row = _row;
  if (_col >= 0) col = _col;
  r.h.bh = 0x00;
  r.h.ah = 0x02;
  r.h.dh = row;
  r.h.dl = col;
  int86(0x10,&r,&r);
  flags &= ~new_location;
}


Cursor& Cursor::Size(int _start, int _end)
{
  union REGS r;
  if (_start >= 0) start = _start;
  if (_end >= 0) end = _end;
  r.h.bh = 0x00;
  r.h.ah = 0x01;
  r.h.ch = start;
  r.h.cl = end;
  int86(0x10,&r,&r);
  flags &= ~new_size;
  return *this;
}


ostream& operator << (ostream& o,Cursor& c)
{
  c.Set();
  if (c.new_size) c.Size();
  return o;
}


ostream& operator >> (ostream& o,Cursor& c)
{
  c.Get();
  return o;
}


istream& operator >> (istream& i,Cursor& c)
{
  c.Get();
  return i;
}
