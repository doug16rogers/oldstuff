#ifndef __SCREEN_HPP
#define __SCREEN_HPP

#include "scrutil.hpp"


#ifndef BYTE
  #define BYTE unsigned char
#endif

#ifndef WORD
  #define WORD unsigned int
#endif

#ifndef LWORD
  #define LWORD unsigned long int
#endif




class Screen {
private:
  WORD mode;                           //mode found on initialization
  WORD page;                           //page used on init
  WORD psiz;                           //size of page
  WORD rows;                           //rows in screen
  WORD cols;                           //columns in screen
  WORD crow;                           //cursor row
  WORD ccol;                           //cursor column
  WORD csiz;                           //cursor size
  WORD* txt;                           //pointer to ?????
  BYTE attr;                           //current attribute
  BYTE fill;                           //fill (background) character
  WORD far* sptr;                      //pointer to saved screen
public:
  Screen(void);                        //construct screen
  ~Screen();                           //destruct screen
  int  put(char c);                    //puts out a single character
  int  put(char *s);                   //puts out a string of chars
  int  setcpos(int r,int c=0);         //sets the current cursor position
  int  getcrow(void) { return crow; }  //gets the cursor's row
  int  getccol(void) { return ccol; }  //gets the cursor's column
  void setattr(int a) { attr=a; }      //sets the current attributes
  int  getattr(void)  { return attr; } //gets the current attributes
  void setfill(char f) { fill=f; }     //sets the fill character
  int  getfill(void)   { return fill; }  //gets the fill character
};   //class Screen


#endif
