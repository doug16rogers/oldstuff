#ifndef __ARGLOAD_HPP
#define __ARGLOAD_HPP

#include <iostream.h>


class ArgSwitch
{
public:
  typedef   int (*Switch_Convert)(ArgSwitch* arg_switch,char* string);

  char  switch_char;
  char* name;
  void* data;
  char* format;
  Switch_Convert convert;
  ArgSwitch* next;

  ArgSwitch(ArgSwitch& arg_switch)
  {
    switch_char    = arg_switch.switch_char;
    name           = arg_switch.name;
    data           = arg_switch.data;
    format         = arg_switch.format;
    convert        = arg_switch.convert;
    next           = arg_switch.next;
  }
  ArgSwitch(
    char  _switch_char,
    char* _name,
    void* _data,
    char* _format,
    Switch_Convert _convert = NULL)
  {
    switch_char    = _switch_char;
    name           = _name;
    data           = _data;
    format         = _format;
    convert        = _convert;
    next           = NULL;
  }

  int  Check(char* s);
  ArgSwitch& operator + (ArgSwitch& other);

  friend ostream& operator << (ostream& o,ArgSwitch& arg_loader);
};   //class ArgSwitch


class ArgLoader
{
public:
  ArgSwitch* switch_list;
  int    argc;
  char** argv;

  ArgLoader(ArgSwitch& arg_switch);
  ~ArgLoader();

  int  Load(int argc,char* argv[]);

  friend ostream& operator << (ostream& o,ArgLoader& arg_loader);
};   //class ArgLoader


#endif
