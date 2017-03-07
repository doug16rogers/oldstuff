#ifndef __WNDUTIL_HPP
#define __WNDUTIL_HPP

#include "textscr.hpp"                 //text screen utilities
#include "tbutil.hpp"                  //text buffer utilities


extern TextScreen tscr;                //create a single text screen

typedef enum { BASE_WINDOW } BASE_WINDOW_INITIATOR;



class Wind : public TBuf {
private:
  int srow;                            //row offset into screen of buffer
  int scol;                            //column offset into screen
  char update;                         //update flag
  BYTE border_att;                     //border attributes
  int  border_lines;                   //number of lines in border
public:
  Wind(void);                          //default constructor
  Wind(int r0,int c0,int r1,int c1,char *cfg=NULL,...);  //coords+config
  Wind(BASE_WINDOW_INITIATOR bwi);     //initializes the base window, wbase

  void Config(char* cfg);              //sets values with config string
  void Configf(char *cfg,...);         //does printf, then configures
  void Update(void);                   //write buffer to screen
  void NoUpdate(void) { update=0; }    //don't force updates
  void ShowBorder(int lines=1);        //put border on screen

  BYTE BorderAttr(void) { return border_att; }
  void BorderAttr(BYTE a) { border_att=a; if (update) Update(); }
  int  BorderLines(void) { return border_lines; }
  void BorderLines(int lines) { border_lines=lines; }

//TBuf virtuals...
  void  Name(char *s)           { TBuf::Name(s);     if (update) Update(); }
  void  Pos(int r,int c)        { TBuf::Pos(r,c);    if (update) Update(); }
  void  Clr(int dir=CLR_ALL)    { TBuf::Clr(dir);    if (update) Update(); }
  void  ClrRow(int dir=CLR_ALL) { TBuf::ClrRow(dir); if (update) Update(); }
  void  IncCol(void)            { TBuf::IncRow();    if (update) Update(); }
  void  IncRow(void)            { TBuf::IncCol();    if (update) Update(); }
  void  ScrollUp(int n)         { TBuf::ScrollUp(n); if (update) Update(); }
  void  Put(char c)             { TBuf::Put(c);      if (update) Update(); }
  void  Put(char *s)            { TBuf::Put(s);      if (update) Update(); }
  void  Put(char *s,int n)      { TBuf::Put(s,n);    if (update) Update(); }
  void  Printf(char *fmt,...);
  Wind& operator<<(char c)  { Put(c); return *this; }
  Wind& operator<<(char *s) { Put(s); return *this; }
};   //Wind class def


#endif
