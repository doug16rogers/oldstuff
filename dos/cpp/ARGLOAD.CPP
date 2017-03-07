#include <stdiostr.h>    //-- for sscanf

#include "argload.hpp"


//===========================================================================

ArgSwitch& ArgSwitch::operator + (ArgSwitch& other)
{
  ArgSwitch* s = this;
  while (s->next != NULL) s = s->next;
  s->next = &other;
  return *this;
}   //ArgSwitch::operator + (ArgSwitch& other)


ostream& operator << (ostream& o,ArgSwitch& arg_switch)
{
  o << '-'
    << arg_switch.switch_char << "  \""
    << arg_switch.format      << "\"  "
    << arg_switch.name;
  return o;
}   //operator << (ostream&,ArgSwitch&)


//===========================================================================

ArgLoader::ArgLoader(ArgSwitch& arg_switch)
{
  switch_list = new ArgSwitch(arg_switch);
  ArgSwitch* arg_next = arg_switch.next;
  ArgSwitch* s = switch_list;
  while (arg_next != NULL)
  {
    s->next = new ArgSwitch(*arg_next);
    arg_next = arg_next->next;
    s = s->next;
  }
}   //ArgLoader::ArgLoader(ArgSwitch& arg_switch);


ArgLoader::~ArgLoader()
{
  ArgSwitch* s;
  while (switch_list != NULL)
  {
    s = switch_list->next;
    delete switch_list;
    switch_list = s;
  }
}   //ArgLoader::~ArgLoader


int  ArgSwitch::Check(char* s)
{
  int ok = 1;
  if (convert == NULL)
    ok = sscanf(s,format,data);
  else
    ok = convert(this,s);
  if (!ok)
  {
    cout << "bad value \"" << s << "\" for " << name
         << " switch.\n";
    return 0;
  }
}   //ArgSwitch::Check(char*)


int  ArgLoader::Load(int _argc,char* _argv[])
{
  argc = _argc;
  argv = _argv;

  char* s;
  ArgSwitch* a = switch_list;
  for (int i = 1; (i < argc); i++)
  {
    s = argv[i];
    if ((*s == '-') || (*s == '/'))
    {
      s++;
      for (a = switch_list; (a != NULL); a = a->next)
        if (*s == a->switch_char) break;
      s++;
      if (a == NULL) continue;
      if (!a->Check(s)) return 0;
    }   //if it's a switch
    else
    {
    }   //else not a switch
  }   //for each argument
  return 1;
}   //ArgLoader::Load(int argc,char* argv[])


ostream& operator << (ostream& o,ArgLoader& arg_loader)
{
  o << "Switches:\n";
  for (ArgSwitch* s = arg_loader.switch_list; (s != NULL); s = s->next)
    o << "  " << *s << "\n";
  return o;
}   //operator << (ostream&,ArgLoader&)

