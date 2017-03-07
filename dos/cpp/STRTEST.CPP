#include <iostream.h>

#include "string.hpp"

String s1;
String s2;
String s3;
String s4;

void main(void)
{
  cout << "===============================================\n";
  s1 = "test";
  s2 = s1 + ' ' + s1 + "(doubled)";
  s3 = s1 + ' ' + s2 + "(tripled?)";
  cout << "Enter a string: ";
  cin  >> s4;
  cout << "s1=\"" << s1 << "\"\n";
  cout << "s2=\"" << s2 << "\"\n";
  cout << "s3=\"" << s3 << "\"\n";
  cout << "s4=\"" << s4 << "\"\n";
  if (s4 >  s1) cout << "s4 >  s1\n";
  if (s4 <  s1) cout << "s4 <  s1\n";
  if (s4 == s1) cout << "s4 == s1\n";
  if (s4 != s1) cout << "s4 != s1\n";
  if (s4 >= s1) cout << "s4 >= s1\n";
  if (s4 <= s1) cout << "s4 <= s1\n";
}   //main

