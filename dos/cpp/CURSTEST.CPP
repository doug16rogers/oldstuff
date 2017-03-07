#include <stdiostr.h>

#include "cursor.hpp"



//===========================================================================
Cursor cursor;


void main(void)
{
  cin >> cursor;
  cout << Cursor(12,30);
  for (int i = 0; (i < 10); i++)
  {
    cout << Cursor(12+i,30).Size(4,i) << "TESTEROO!!!";
    cin.get();
  }
  cout << cursor;
}   //main

