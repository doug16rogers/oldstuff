#include <stdiostr.h>

#include "scrutil.hpp"
#include "wndutil.hpp"


#define MAX_WINDOWS    0x40

#ifndef WORD
  #define WORD unsigned int
#endif
#ifndef LWORD
  #define LWORD unsigned long int
#endif

typedef unsigned long int WINHAND;
typedef int far (*WINFUNC)(Wind* w,WORD cmd,WORD wp,LWORD lp);

typedef struct {
  WINHAND handle;
  WINFUNC func;
  Wind*????????/   window;
} WINDATA;

class WndManager {
protected:
  WINDATA *windata[MAX_WINDOWS];
  WINHAND issue;
  int wins;
public:
  WndManager() { wins=0; issue=0x100UL; }
  ~WndManager();
  WINHAND Add(WINFUNC f,int r0,int c0,int r1,int c1,char* cfg);
  WINHAND Add(WINFUNC f,int r0,int c0,int r1,int c1,char* cfg,...);
  void    Del(WINHAND h);
  void    Focus(WINHAND h);
  void    Back(WINHAND h);
};   //class WndManager


WndManager::~WndManager()
{
  for (int i=0;i<wins;i++)
    if (windata[i].handle != NULL) delete &(windata[i].window);
}   //WndManager::~WndManager


WINHAND WndManager::Add(WINFUNC f,int r0,int c0,int r1,int c1,char* cfg)
{
  if (wins>=MAX_WINDOWS) return NULL;
  windata[wins].handle = issue++;
  windata[wins].func   = f;
  windata[wins].window = * new Wind(r0,c0,r1,c1,cfg);
  wins++;
  return windata[wins-1].handle;
}   //WndManager::Add(WINFUNC,int,int,int,int,char*)


WINHAND WndManager::Add(WINFUNC f,int r0,int c0,int r1,int c1,char* cfg,...)
{
  char s[0x100];
  va_list va;
  va_start(va,cfg);
  vsprintf(s,cfg,va);
  va_end(va);
  return Add(f,r0,c0,r1,c1,s);
}   //WndManager::Add(WINFUNC,int,int,int,int,char*,...)


WndManager wm;

void main(void)
{
  wm.Add(5,20,20,60,"NHey!!");
}   //main(void)

