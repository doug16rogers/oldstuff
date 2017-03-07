
// Text buffer (quasi-window) functions

#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdarg.h>

#include "tbutil.hpp"


TBuf::TBuf(void)
{
  rows=MAX_ROWS;                       //max rows
  cols=MAX_COLS;                       //max cols
  txt=new (WORD[rows*cols]);           //allocate space for text
  if (txt==NULL) {                     //if no room..
    rows=cols=0;                       //..reset things
    return;                            //..exit
  }
  row=0;                               //start at upper
  col=0;                               //left corner
  att=DEF_ATT;                         //use default attributes
  fil=DEF_FIL;                         //use default fill character
  nam[0]=0;                            //don't set up a name
  fl.wrap=1;                           //wrapping
  fl.scroll=1;                         //scrolling
  Clr();                               //clear the whole buffer
}   //TBuf default constructor



TBuf::TBuf(int rs,int cs)
{
  rows=cols=0;                         //first indicate failure
  if ((rs<MIN_ROWS)||(rs>MAX_ROWS)) return;   //fail
  if ((cs<MIN_COLS)||(cs>MAX_COLS)) return;   //fail
  rows=rs;                             //set rows
  cols=cs;                             //set columns
  txt=new (WORD[rows*cols]);           //allocate space for text
  if (txt==NULL) {                     //if no room..
    rows=cols=0;                       //..reset things
    return;                            //..exit
  }
  row=0;                               //start at upper
  col=0;                               //left corner
  att=DEF_ATT;                         //use default attributes
  fil=DEF_FIL;                         //use default fill character
  nam[0]=0;                            //don't set up a name
  fl.wrap=1;                           //wrapping
  fl.scroll=1;                         //scrolling
  Clr();                               //clear the buffer
}   //TBuf constructor given size


TBuf::TBuf(int rs,int cs,BYTE a)
{
  rows=cols=0;                         //first indicate failure
  if ((rs<MIN_ROWS)||(rs>MAX_ROWS)) return;   //fail
  if ((cs<MIN_COLS)||(cs>MAX_COLS)) return;   //fail
  rows=rs;                             //set rows
  cols=cs;                             //set columns
  txt=new (WORD[rows*cols]);           //allocate space for text
  if (txt==NULL) {                     //if no room..
    rows=cols=0;                       //..reset things
    return;                            //..exit
  }
  row=0;                               //start at upper
  col=0;                               //left corner
  fil=DEF_FIL;                         //use default fill character
  nam[0]=0;                            //don't set up a name
  fl.wrap=1;                           //wrapping
  fl.scroll=1;                         //scrolling
  att=a;                               //set attribute to passed value
  Clr();                               //clear the buffer
}   //TBuf constructor given corners


TBuf::~TBuf(void)
{
  delete txt;                          //delete the text from memory
}   //TBuf::~Tbuf


void TBuf::Name(char* s)               //sets text-buffer name
{
  int l=strlen(s);                     //length of name
  if (l>=MAX_NAM) l=MAX_NAM-1;         //make sure length is ok
  strncpy(nam,s,l-1);                  //copy into window's name
  nam[l]=0;                            //terminate string
}   //TBuf::Name(char*)


void TBuf::Pos(int r,int c=0)
{
  if ((r>=rows)||(r<0)) r=0;           //check row
  if ((c>=cols)||(c<0)) c=0;           //check column
  row=r;
  col=c;
}   //TBuf::Pos(int,int)


void TBuf::Clr(int dir)                //clear buffer (beg=-1,all=0,end=+1)
{
  WORD ca=(att<<8)+(BYTE)fil;          //fill char and attribute
  WORD* wp;                            //pointer into text window
  WORD n;                              //number of cells

  if (!dir) {                          //0==entire screen
    wp=txt;                            //start at beginning
    n=rows*cols;                       //go for entire screen
    row=col=0;                         //reset cursor position
  }
  else if (dir<0) {                    //-1==from beginning
    wp=txt;                            //start at beginning
    n=row*cols+col;                    //go to current row and column
  }
  else {                               //+1==to end
    wp=&txt[row*cols+col];             //set start to current row and column
    n=(rows-row)*cols-col;             //go to end of text buffer
  }

  while (n--) *wp++=ca;                //clear the whole thing
}   //TBuf::Clr(int)


void TBuf::ClrRow(int dir)             //clear row (beg=-1,all=0,end=+1)
{
  WORD ca=(att<<8)+(BYTE)fil;          //fill char and attribute
  WORD* wp;                            //pointer into text window
  WORD n;                              //number of cells

  if (!dir) {                          //0==entire row
    wp=&txt[row*cols];                 //start at beginning of row
    n=cols;                            //go for entire row
    col=0;                             //reset cursor position
  }
  else if (dir<0) {                    //-1==from beginning of row
    wp=&txt[row*cols];                 //start at beginning of row
    n=col;                             //go to current column
  }
  else {                               //+1==to end of row
    wp=&txt[row*cols+col];             //set start to current row and column
    n=cols-col;                        //go to end of row
  }

  while (n--) *wp++=ca;                //clear the whole thing
}   //TBuf::ClrRow(int)


void TBuf::ScrollUp(int n)
{
  WORD* wp1;                           //leading pointer
  WORD* wp2;                           //trailing pointer
  WORD  nb=cols<<1;                    //number of bytes per row

  if (!n) return;                      //no scroll!
  int r=row;                           //save the row,column
  int c=col;
  if (n>0) {                           //check for scroll-up/scroll-down
    if (n>=rows) { Clr(); Pos(r,c); return; }    //huge scroll
    wp1=txt;                           //point to beginning of buffer
    wp2=&txt[n*cols];                  //point to next available row
    for (int i=n;i<rows;i++) {         //go through each row to be scrolled
      memcpy(wp1,wp2,nb);              //copy it over
      wp1+=cols;                       //increment row pointers
      wp2+=cols;
    }   //for
    Pos(rows-n,0);                     //go to first new row
    Clr(CLR_END);                      //clear to end of buffer
    Pos(r,c);                          //restore cursor
  }   //if up
  else {                               //scroll down
    n=-n;                              //invert if scrolling down
    if (n>=rows) { Clr(); Pos(r,c); return; }    //huge scroll
    wp1=&txt[(rows-n-1)*cols];         //point to last row of buffer
    wp2=&txt[(rows-1)*cols];           //point to next available row
    for (int i=n;i<rows;i++) {         //go through each row to be scrolled
      memcpy(wp2,wp1,nb);              //copy it over
      wp1-=cols;                       //decrement row pointers
      wp2-=cols;
    }   //for
    Pos(n,0);                          //go to row
    Clr(CLR_BEG);                      //clear from beginning of buffer
    Pos(r,c);                          //restore cursor
  }   //if scrolling down
}   //TBuf::ScrollUp(int)


void TBuf::IncCol(void)
{
  col++;                               //increment column pointer
  if (col<cols) return;                //done if column is small enough
  col--;                               //back down
  if (!fl.wrap) return;                //if not wrapping, leave hangin'
  col=0;
  IncRow();                            //wrap to next row
}   //TBuf::IncCol(void)


void TBuf::IncRow(void)
{
  int c=col;                           //save column position
  col=0;                               //reset column for clears
  row++;                               //wrap to next row
  if (row<rows) return;                //if row ok, great
  if (!fl.scroll) {                    //if not scrolling, just wrap..
    row=0;                             //set to first row
    ClrRow(CLR_END);                   //clear the first row
    col=c;                             //restore column
    return;                            //all done
  }
  ScrollUp(1);                         //scroll up one row
  row=rows-1;                          //set to bottom row
  ClrRow(CLR_END);                     //clear bottom row
  col=c;                               //restore column pointer
}   //TBuf::IncRow(void)


void TBuf::Put(char c)
{
  switch (c) {                         //see if special character
  case '\r':                           //if CR, reset column
    col=0;
    break;
  case '\n':                           //if LF, newline
    col=0;                             //reset column
    IncRow();                          //go to next row
    break;
  default:
    txt[row*cols+col]=(WORD)(att<<8)+(WORD)c;  //put char into buffer
    IncCol();                          //point to next location
  }   //switch
}   //TBuf::Put(char)


void TBuf::Put(char* s)
{
  while (*s) Put(*s++);                //just write each character
}   //TBuf::Put(char*)


void TBuf::Put(char* s,int n)
{
  int i;                               //local index
  for (i=0;i<n;i++) Put(*s++);         //just write each character
}   //TBuf:Put(char*,int)


void TBuf::Printf(char *fmt,...)       //formatted print
{
  va_list va;                          //variable argument list
  int l;                               //length of string
  char b[256];                         //buffer to hold string

  va_start(va,fmt);                    //open va list
  l=vsprintf(b,fmt,va);                //write into a string
  va_end(va);                          //close va list
  Put(b,l);                            //put out string of l chars
}   //TBuf::Printf


