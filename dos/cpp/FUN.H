#include <stdiostr.h>
#include <string.h>

#include "icitypes.h"

typedef unsigned short int ushort;


class Fun
{
public:
   int length;
   char* name;

   Fun(char* name)
   {
      length = 0;
      this->name = NULL;
      if (name != NULL)
      {
         length = strlen(name);
         this->name = new char[length + 1];
         if (this->name != NULL)
         {
            strcpy(this->name, name);
         }
      }
   }

   ~Fun()
   {
      delete[] name;
   }
};


void Show(
     Fun* f,
     ushort with_quotes);

