#include <stdiostr.h>
#include <string.h>

#include "cmdline.hpp"


int AlwaysOne(char *s) { if (s == NULL) return 1; return 1; }


//===========================================================================

void Parameter::operator = (Parameter& parameter)
{
  used   = parameter.used;
  name   = parameter.name;
  data   = parameter.data;
  check  = parameter.check;
  next   = parameter.next;
}   //Parameter::operator = (Parameter& parameter)


Parameter& Parameter::operator + (Parameter& parameter)
{
  Parameter* link = this;

  while (link->next != NULL) link = link->next;
  link->next = &parameter;
  return *this;
}   //Parameter::operator + (Parameter&)


int Parameter::Load(char* arg)
{
  if (used) return 0;
  used = 1;
  if (!check(arg)) return 0;
  strcpy((char*)data,arg);
  return 1;
}   //Parameter::Load(char* arg)


ostream& Parameter::Out(ostream& o)
{
  o << name;
  return o;
}   //Parameter::Out(ostream& o)


Parameter::Parameter(char* _name, char* _data, Check_Function _check)
{
  used   = 0;
  name   = _name;
  data   = _data;
  check  = _check;
  next   = NULL;
}   //Parameter::Parameter(char* _name,Check_Function _check)


Parameter::Parameter(Parameter& parameter)
{
  *this = parameter;
}   //Parameter::Parameter(Parameter& parameter)


//===========================================================================

ostream& Switch::Out(ostream& o)
{
  o << name << " (-" << switch_char << ", \"" << format << "\")";
  return o;
}   //Switch::Out(ostream& o)


void Switch::operator = (Switch& s)
{
  name = s.name;
  switch_char = s.switch_char;
  data = s.data;
  format = s.format;
  scan = s.scan;
  next = s.next;
}   //Switch::operator = (Switch& s)


Switch& Switch::operator + (Switch& s)
{
  Switch* link = this;

  while (link->next != NULL) link = link->next;
  link->next = &s;
  return *this;
}   //Switch::operator + (Switch&)


int Switch::Load(char* arg)
{
  if ((arg[0] != '-') && (arg[0] != '/')) return 0;
  if (arg[1] != switch_char) return 1;
  if (scan == NULL)
    return sscanf(&arg[2],format,data);
  else
    return scan(data,&arg[2]);
}   //Switch::Load(char* arg)


Switch::Switch(char* _name, char _switch_char, void* _data, char* _format,
       Scan_Function _scan)
{
  name        = _name;
  switch_char = _switch_char;
  data        = _data;
  format      = _format;
  scan        = _scan;
  next = NULL;
}   //Switch::Switch


Switch::Switch(Switch& s)
{
  name = s.name;
  switch_char = s.switch_char;
  data   = s.data;
  format = s.format;
  scan   = s.scan;
  next   = s.next;
}   //Switch::Switch(Switch& s)


//===========================================================================

CommandLine::CommandLine(Parameter& parameter, Switch& s)
{
  parameter_list = &parameter;
  switch_list = &s;
}   //CommandLine::CommandLine(Parameter& parameter, Switch& s);


int  CommandLine::Load(int argc,char* argv[])
{
  for (int i = 1; (i < argc); i++)
  {
    if ((argv[i][0] != '-') && (argv[i][0] != '/'))
    {
      Parameter* param = parameter_list;
      while ((param != NULL) && (param->used)) param = param->next;
      if (param != NULL) param->Load(argv[i]);
    }
    else
    {
      Switch* sw = switch_list;
      while (sw != NULL)
      {
        if (sw->Load(argv[i]) == 0) return 0;
        sw = sw->next;
      }
    }
  }
  return 1;
}   //CommandLine::Load(int argc,char* argv[])



//===========================================================================

ostream& operator << (ostream& o,Parameter& p)
{
  return p.Out(o);
}  //operator << (ostream& o,Parameter& p)

ostream& operator << (ostream& o,Switch& s)
{
  return s.Out(o);
}  //operator << (ostream& o,Switch& s)


ostream& operator << (ostream& o,CommandLine& c)
{
  o << "Command line has " << c.count << " loaded arguments:\n";

  int i = 0;
  Parameter* p = c.parameter_list;
  while (p != NULL)
  {
    o << "  param[" << i++ << "] = " << *p << "\n";
    p = p->next;
  }   //while

  Switch* s = c.switch_list;
  i = 0;
  while (s != NULL)
  {
    o << "  switch[" << i++ << "] = " << *s << "\n";
    s = s->next;
  }
  return o;
}  //operator << (ostream& o,CommandLine& c)


