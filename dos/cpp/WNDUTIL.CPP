#include <stdiostr.h>
#include <stdarg.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "scrutil.hpp"
#include "wndutil.hpp"


TextScreen tscr;                       //create a single text screen


//===========================================================================
Wind::Wind(void)
{
  srow=3*tscr.rows/8;
  scol=3*tscr.cols/8;
      int rs=tscr.rows/4,cs=tscr.cols/4;
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
      border_att=att;
      border_lines=1;
  update=1;                            //force update
      Clr();                               //clear the buffer
}   //Wind::Wind(void)


//===========================================================================
Wind::Wind(int r0,int c0,int r1,int c1,char *cfg,...)
{
  va_list va;
  char cfg_str[0x100];

  va_start(va,cfg);
  vsprintf(cfg_str,cfg,va);
  va_end(va);

  r1 -= r0-1;                          //get size in rows
  c1 -= c0-1;                          //get size in columns
  if ((r1<1)||(r1>tscr.rows)) r1=tscr.rows>>2;
  if ((c1<1)||(c1>tscr.cols)) c1=tscr.cols>>2;
  if ((r0<0)||((r0+r1)>tscr.rows)) r0=0;
  if ((c0<0)||((c0+c1)>tscr.cols)) c0=0;
  srow=r0;
  scol=c0;
//  TBuf::TBuf(r1,c1);
      int rs=r1,cs=c1;
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
      border_att=att;
      border_lines=1;
  update=1;                                //force and update
  Config(cfg_str);                         //set configuration string values
      Clr();                               //clear the buffer
}   //Wind(int,int,int,int,char*,...)


//===========================================================================
void Wind::Config(char* cfg)           //set values via configuration string
{
  if (!cfg) return;
  while (*cfg) {
    while (*cfg && isspace(*cfg)) cfg++;
    switch (*cfg++) {
    case 'f': att&=0xF0; att|=atoi(cfg)&0x0F; break;
    case 'b': att&=0x0F; att|=(atoi(cfg)&0x0F)<<4; break;
    case 'u': if (*cfg=='0') update=0; else update=1; break;
    case 's': if (*cfg=='0') fl.scroll=0; else fl.scroll=1; break;
    case 'w': if (*cfg=='0') fl.wrap=0; else fl.wrap=1; break;
    case 'r': row=atoi(cfg); if ((row<0)||(row>=rows)) row=0; break;
    case 'c': col=atoi(cfg); if ((col<0)||(col>=cols)) col=0; break;
    case 'L': border_lines=atoi(cfg); break;
    case 'F': border_att&=0xF0; border_att|=atoi(cfg)&0x0F; break;
    case 'B': border_att&=0x0F; border_att|=(atoi(cfg)&0x0F)<<4; break;
    case 'N': strncpy(nam,cfg,sizeof(nam)-1); nam[sizeof(nam)-1]=0; break;
    default:
      ;
    }   //switch
    while (*cfg && !isspace(*cfg)) cfg++;
  }   //while
}   //Wind::Config(char*)


//===========================================================================
void Wind::Configf(char *cfg,...)
{
  va_list va;
  char cfg_str[0x100];

  va_start(va,cfg);
  vsprintf(cfg_str,cfg,va);
  va_end(va);
  Config(cfg_str);
}   //Wind::Configf(char*,...)


//===========================================================================
void Wind::Printf(char *fmt,...)       //formatted print
{
  va_list va;                          //variable argument list
  int l;                               //length of string
  char b[256];                         //buffer to hold string

  va_start(va,fmt);                    //open va list
  l=vsprintf(b,fmt,va);                //write into a string
  va_end(va);                          //close va list
  Put(b,l);                            //put out string of l chars
}   //Wind::Printf



//===========================================================================
void Wind::Update(void)
{
  WORD far* vp;
  WORD* tp=txt;

  ShowBorder(border_lines);
  for (int r=0;r<rows;r++) {
    vp=&tscr.video[(r+srow)*tscr.cols+scol];
    for (int c=0;c<cols;c++) *vp++=*tp++;
  }   //for each row
  setcpos(srow+row,scol+col);
  update=1;
}   //Wind::Update(void)


//===========================================================================
void Wind::ShowBorder(int lines)
{
#define TOPLEFT    0
#define HORIZ      1
#define TOP_T      2
#define TOPRIGHT   3
#define VERT       4
#define LEFT_T     5
#define MIDDLE     6
#define RIGHT_T    7
#define BOTLEFT    8
#define BOT_T      9
#define BOTRIGHT   10
  static BYTE text_lines[]={           //text mode graphics characters
    '\x20','\x20','\x20','\x20','\x20','\x20',
    '\x20','\x20','\x20','\x20','\x20','\x00',
    '\xDA','\xC4','\xC2','\xBF','\xB3','\xC3',
    '\xC5','\xB4','\xC0','\xC1','\xD9','\x00',
    '\xC9','\xCD','\xCB','\xBB','\xBA','\xCC',
    '\xCE','\xB9','\xC8','\xCA','\xBC','\x00'
  };

  if (lines==0) lines=0;               //offset in table
  else if (lines==2) lines=24;
  else lines=12;                       //default to one line

  WORD ac;                             //attribute:character
  WORD far* vp;                        //pointer into video memory
  char *p;                             //pointer to name
  int  k;                              //row/col counter
  int  l;                              //length

  ac=border_att<<8;                    //get border attribute
  l=0;                                 //initialize length to 0
  p=nam;                               //point to name of window
  if (p!=NULL) l=strlen(p);            //get length of name
  vp=tscr.video;                       //start at top of screen
  vp+=(srow-1)*tscr.cols;              //offset to first border row
  vp+=(scol-1);                        //offset to correct column
  *vp++=ac|text_lines[lines+TOPLEFT];  //write top left character
  for (k=0;k<cols;k++) {               //go through each column
    if (k<l)                           //if still writing prompt...
      *vp++=ac|*p++;                   //...write out prompt character
    else                               //otherwise...
      *vp++=ac|text_lines[lines+HORIZ];  //...write out horizontal bar
  }     //for
  *vp++=ac|text_lines[lines+TOPRIGHT]; //write top right character
  ac|=text_lines[lines+VERT];
  for (k=0;k<rows;k++) {               //write out side walls on each row
    vp+=(tscr.cols-cols-2);            //point to left side wall
    *vp++=ac;                          //write out vertical character
    vp+=cols;                          //go to right wall
    *vp++=ac;                          //write out vertical character
  }     //for
  ac&=0xFF00;                          //clear out the vertical character
  vp+=(tscr.cols-cols-2);              //point to bottom left
  *vp++=ac|text_lines[lines+BOTLEFT];  //write bottom left character
  for (k=0;k<cols;k++)                 //go through each column
    *vp++=ac|text_lines[lines+HORIZ];  //write out horizontal
  *vp++=ac|text_lines[lines+BOTRIGHT]; //write bottom right character
}  //Wind::ShowBoarder(int)

