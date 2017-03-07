#ifndef __TBUTIL_H
#define __TBUTIL_H

//
//    Text buffer functions, suitable for windowing integration
//

#include <conio.h>                     //include colors from tc++ conio.h


#ifndef BYTE
  #define BYTE unsigned char
#endif

#ifndef WORD
  #define WORD unsigned int
#endif

#define ATTR(f,b)      (((b)<<4)+(f))  //attribute calculator

#define MIN_ROWS       1               //must have at least 1 row
#define MIN_COLS       1               //must have at least 1 column
#define MAX_ROWS       48              //up to 48 rows
#define MAX_COLS       78              //up to 78 columns

#define DEF_ATT        ATTR(LIGHTGRAY,BLACK)  //use normal colors
#define DEF_FIL        ' '             //default fill character, space
#define MAX_NAM        0x20            //maximum length of window name w/NUL

#define CLR_ALL        (0)             //clear entire buffer/row
#define CLR_BEG        (-1)            //clear from beginning of buffer/row
#define CLR_END        (+1)            //clear to end of buffer/row


typedef struct Wf_struct {
  WORD wrap:1;                         //wrapping at end of line
  WORD scroll:1;                       //scrolling at bottom of window
} Wflag;


class TBuf {
protected:
  int   rows;                          //current number of rows
  int   cols;                          //current number of columns
  int   row;                           //current cursor row
  int   col;                           //current cursor column
  BYTE  att;                           //attribute for character writes
  char  fil;                           //fill character
  char  nam[MAX_NAM];                  //name of window
  Wflag fl;                            //flags for window
  WORD* txt;                           //pointer to data
public:
  TBuf(void);                          //default constructor
  TBuf(int rs,int cs);                 //open with size
  TBuf(int rs,int cs,BYTE a);          //size, attributes
  ~TBuf(void);                         //destructor, releases memory
  int   Rows(void) { return rows; }    //rows in buffer
  int   Cols(void) { return cols; }    //columns in buffer
  BYTE  Attr(void) { return att; }     //gets attribute setting
  void  Attr(BYTE a) { att=a; }        //sets attribute
  char  Fill(void) { return fil; }     //gets fill character
  void  Fill(char f) { fil=f; }        //sets fill character
  char* Name(void) { return nam; }     //gets name of window
  void  Name(char *s);                 //sets name of window
  WORD* TextBuffer(void) { return txt; }   //gets pointer to text buffer
  void  Pos(int r,int c);              //sets cursor posiiton
  WORD  Pos(void) { return (WORD)(col<<8)+row; }   //returns position
  int   Row(void) { return row; }      //cursor's row
  int   Col(void) { return col; }      //cursor's column
  void  Clr(int dir=CLR_ALL);          //clears text buffer
  void  ClrRow(int dir=CLR_ALL);       //clear row
  void  IncCol(void);                  //increments column, checks wrap
  void  IncRow(void);                  //increments row, checks scroll
  void  ScrollUp(int n);               //scroll up n lines (use -n for down)
  void  Put(char c);                   //puts character to the window
  void  Put(char *s);                  //puts string to the window
  void  Put(char *s,int n);            //puts string of n chars
  void  Printf(char *fmt,...);         //formatted print to window
  TBuf& operator<<(char c) { Put(c); return *this; }
  TBuf& operator<<(char *s) { Put(s); return *this; }
};   //TBuf class


#endif                                 //if file not already included
