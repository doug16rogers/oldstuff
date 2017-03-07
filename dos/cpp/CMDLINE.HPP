#ifndef __CMDLINE_HPP
#define __CMDLINE_HPP

#include <stdiostr.h>


typedef int (*Check_Function)(char* arg);
typedef int (*Scan_Function)(void* data, char* arg);


int AlwaysOne(char *s);  //always returns 1


//===========================================================================
class Parameter
{
public:
  char used;
  char *name;
  void *data;
  Check_Function check;
  Parameter* next;

  Parameter(char* _name, char* _data, Check_Function _check = AlwaysOne);
  Parameter(Parameter& parameter);
  void operator = (Parameter& parameter);
  Parameter& operator + (Parameter& parameter);
  Parameter& operator + (void) { return *this; }
  int Load(char* arg);
  ostream& Out(ostream& o);
};   //class Parameter


//===========================================================================
class Switch
{
public:
  char *name;
  char  switch_char;
  void* data;
  char* format;
  Scan_Function scan;
  Switch* next;

  Switch(char* _name, char _switch_char, void* _data, char* _format,
         Scan_Function _scan = NULL);
  Switch(Switch& s);

  ostream& Out(ostream& o);
  void operator = (Switch& s);
  Switch& operator + (Switch& s);
  Switch& operator + (void) { return *this; }
  int Load(char* arg);
};   //class Switch


//===========================================================================
class CommandLine
{
public:
  Parameter* parameter_list;
  Switch* switch_list;
  int count;

  CommandLine(Parameter& parameter, Switch& s);
  int  Load(int argc,char* argv[]);
};   //class CommandLine


//===========================================================================
ostream& operator << (ostream& o,Parameter& p);
ostream& operator << (ostream& o,Switch& s);
ostream& operator << (ostream& o,CommandLine& c);


#endif
