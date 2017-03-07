#include <stdiostr.h>

#include "textscr.hpp"
#include "scrutil.hpp"



TextScreen::TextScreen(WORD fill)      //constructor
{
  rows=cols=0;                         //initialize rows/columns
  save=NULL;                           //initialize save pointer
  video=(WORD far*)SCRSEG_COLOR;       //pretend its a color monitor
  if (!setscrseg()) return;            //make sure text mode is ok
  video=scrseg;                        //pointer to video RAM
  rows=getsrows();                     //rows available on screen
  cols=getscols();                     //columns available on screen
  roco=rows*cols;                      //positions on screen
  save=new WORD[roco];                 //get room for screen
  Save();                              //save current screen
  if (fill) Fill(fill);                //they want to fill it
}   //TextScreen::TextScreen


TextScreen::~TextScreen(void)          //destructor
{
  if (!save) return;                   //must have been graphics mode
  Restore();                           //restore screen
  delete save;                         //clear the new'd thing
}   //TextScreen::~TextScreen


void TextScreen::Fill(WORD fill)       //fill screen with attr:char
{
  for (int i=0;i<roco;i++) video[i]=fill;  //fill 'er up
}   //TextScreen::Fill

void TextScreen::Save(void)
{
  if (!save) return;
  row=getcrow();
  col=getccol();
  siz=getcsiz();
  for (int i=0;i<roco;i++) save[i]=video[i];   //save screen
}   //TextScreen::Save(void)

void TextScreen::Restore(void)         //restores the saved screen
{
  if (!save) return;                   //must have been graphics mode
  for (int i=0;i<roco;i++) video[i]=save[i];   //restore screen
  setcpos(row,col);
  setcsiz(siz);
}   //TextScreen::Restore

