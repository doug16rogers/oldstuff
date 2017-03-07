#ifndef __KEYBOARD
#define __KEYBOARD


#ifndef __keys_h
#include "keys.h"
#endif


#define KB_INT     0x16                //keyboard interrupt

class KEYBOARD
{
public:

  BOOLEAN hit(void);

public:

  BOOLEAN operator * ()
  {
     return hit();
  }

public:

  BOOLEAN operator ! ()
  {
     return !hit();
  }

public:

  char Get(void);

public:

  KEYBOARD& operator >> (char& c) { c = Get(); return *this; }

public:

  KEYBOARD& operator << (char& c);           //insert into kb buffer

};   //class KB


extern KB kbin;

#endif
