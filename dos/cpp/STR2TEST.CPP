#include <stdiostr.h>

#include "string2.hpp"


String s1("hey!");
String s2;

void main(
   int count,
   char* argument[])
{
   String s3;
   String *string[20];

   s2 = "you!";

   s3 = s1 + " -- " + s2;

   for (int i = 0; (i < count); i++)
   {
      string[i] = new String(argument[i]);
      cout << i << ": " << *string[i];
   }

}   // main