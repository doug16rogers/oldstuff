#include "fun.h"

typedef unsigned short int ushort;

void Show(
     Fun* f,
     ushort with_quotes)
{
   if (with_quotes)
   {
      cout << '"' << f->name << '"';
   }
   else
   {
      cout << f->name;
   }

}   // Show


