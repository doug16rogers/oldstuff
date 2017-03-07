#include <stdiostr.h>

#include "cmdline.hpp"



//===========================================================================

int  key = 0x1234;
char sea = '?';
char your_name[80] = "This is your name!";
char input_file[0x80];
char output_file[0x80];
char other_file[0x80];


CommandLine command_line
(
    *new Parameter("input file",input_file)
  + *new Parameter("output file",output_file)
  + *new Parameter("reference file",other_file),

    *new Switch("key", 'k', &key, "%x")
  + *new Switch("sea character", 'c', &sea, "%c")
  + *new Switch("your first name", 'n', &your_name[0], "%s")
);

int Help(int k)
{
  cout << "reached help function\n";
  return k;
}   //Help(int)


//===========================================================================

int  main(int argc,char* argv[])
{
  cout << "========================================================\n";
  cout << command_line;
  cout << "--------------------------------------------------------\n";
  if (command_line.Load(argc,argv) == 0) return Help(1);
  cout << command_line;
  return 0;
}   //main

