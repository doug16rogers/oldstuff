#ifndef __CURSOR_HPP
#define __CURSOR_HPP


class Cursor
{
public:
  int row;
  int col;
  int start;
  int end;
  int flags;

  enum
  {
    new_location = 0x0001,
    new_size     = 0x0002,
  };

  Cursor(int _row = -1,int _col = -1, int _start = -1, int _end = -1);
  ~Cursor() { }
  void Get(void);
  void Set(int _row = -1, int _col = -1);
  Cursor& Size(int _start = -1, int _end = -1);
};   //class Cursor

ostream& operator << (ostream& o,Cursor& c);
ostream& operator >> (ostream& o,Cursor& c);
istream& operator >> (istream& i,Cursor& c);

#endif
