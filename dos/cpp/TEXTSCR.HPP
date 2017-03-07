#ifndef __TEXTSCR_HPP
#define __TEXTSCR_HPP


#ifndef BYTE
  #define BYTE unsigned char
#endif
#ifndef WORD
  #define WORD unsigned int
#endif
#ifndef DWORD
  #define DWORD unsigned long int
#endif



class TextScreen {                     //used to automatically save/restore
private:
  WORD* save;                          //pointer to saved screen data
public:
  int  rows;                           //rows available on text screen
  int  cols;                           //columns available on text screen
  int  roco;                           //rows times cols == screen positions
  int  row;
  int  col;
  int  siz;
  WORD far* video;                     //pointer to video page
  TextScreen(WORD fill=0);             //constructor
  ~TextScreen(void);                   //destructor
  void Fill(WORD fill);                //fill screen with attr:char
  void Save(void);                     //copies screen again to buffer
  void Restore(void);                  //restores the saved screen
};   //class TextScreen


#endif
