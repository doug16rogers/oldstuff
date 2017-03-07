#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>

#include "wutil.h"
#include "scrutil.h"
#include "strutil.h"


#define STATTR     ATTR(BLACK,LIGHTGRAY)
#define BKATTR     ATTR(LIGHTGRAY,BLACK)
#define TXATTR     ATTR(YELLOW,BLUE)
#define HPATTR     ATTR(WHITE,BROWN)


typedef struct LineID_struct {
  struct LineID_struct* prv;
  struct LineID_struct* nxt;
  WORD len;
  WORD max;
  WORD num;
  char txt[2];
} LineID;
#define LIDSIZE    sizeof(LineID)

#define MAX_LEN    0x200
#define INDENT_INC 8

int rows=25,cols=80;
char buf[MAX_LEN];
char FileName[0x80]="\0";
long FileSize=0;
WORD FileLine=0;
WORD FileLines=0;
int  FileIndent=0;
int  FileColumn=0;
BYTE FileChar=0;
long FileOffset=0;
FILE* File=NULL;
LineID* BaseLine=NULL;
LineID* LastLine=NULL;
WIND* tw;
WIND* sw;
WIND* hw;
char search[0x100]="Hello";
LineID* Marks[5]={NULL,NULL,NULL,NULL,NULL};



//===========================================================================
void help(void)
{
  wclr(hw);
  wfront(hw);
  wupdate(NULL);

  wputs(hw,
    "<F1>        help\n"
    "<F10>       abort list (<Alt-X>)\n"
    "<ESC>       go to next file\n"
    "\n"
    "  hit a key\r"
  );
  if (wgetc(hw)==ESC) { whide(hw); return; }
  wputs(hw,
    "<Home>      go to first line\n"
    "<End>       go to last line\n"
    "<UpAr>      go up one line\n"
    "<DnAr>      go down one line\n"
    "<PgUp>      go up one page\n"
    "<PgDn>      go down one page\n"
    "<LtAr>      slide left\n"
    "<RtAr>      slide right\n"
    "\n"
    "  hit a key\r"
  );
  if (wgetc(hw)==ESC) { whide(hw); return; }
  wputs(hw,
    "<F3>        search forward\n"
    "<Shft-F3>   forward again\n"
    "<F4>        search backward\n"
    "<Shft-F4>   backward again\n"
    "\n"
    "  hit a key\r"
  );
  wgetc(hw);

  whide(hw);
}   //help


//===========================================================================
void statline(void)
{
  char twupd=wupd;
  wupd=0;
  wclr(sw);
  wprintf(sw,
    "File=%s  Size=%lu  Line=%u/%u  Column=%u  Char=%c(0x%02X)"
    ,FileName
    ,FileSize
    ,FileLine
    ,FileLines
    ,FileColumn+1
    ,FileChar
    ,FileChar
  );   //wprintf
  wupd=twupd;
  if (wupd) wupdate(sw);
}   //statline


//===========================================================================
int  InputS(char* prompt,char* s,int siz)
{
  int c;
  wclr(sw);
  wputs(sw,prompt);
  c=wgetsfield(sw,s,siz,0,ATTR(WHITE,BLACK));
  statline();
  return c==CR;
}   //InputS


//===========================================================================
LineID* SearchNxt(LineID* lin,char* s)
{
  while (lin) {
    if (strstr(lin->txt,s)) break;
    lin=lin->nxt;
  }
  return lin;
}   //SearchNxt


//===========================================================================
LineID* SearchPrv(LineID* lin,char* s)
{
  while (lin) {
    if (strstr(lin->txt,s)) break;
    lin=lin->prv;
  }
  return lin;
}   //SearchPrv


//===========================================================================
int  DisplayFile(void)
{
  LineID* line;
  LineID* lin;
  int i;
  char update=1;
  WORD csiz=wgetcsiz(tw);

  if (!BaseLine) {
    wclr(tw);
    statline();
    wprintf(sw,"Empty file.  Hit any key");
    if (wgetc(sw)==A_X) return 0; else return 1;
  }

  for (i=0;i<10;i++) Marks[i]=NULL;

  line=BaseLine;
  FileIndent=0;
  while (1) {
    if (update) {
      wupd=0;
      wclr(tw);
      lin=line;
      wsetattr(tw,TXATTR);
      for (i=0;(i<tw->rows)&&lin;i++) {
        if (lin->len>=FileIndent) {
          wsetcpos(tw,i,0);
          wputs(tw,&lin->txt[FileIndent]);
        }
        lin=lin->nxt;
      }
      wsetattr(tw,BKATTR);
      wupdate(tw);
    }   //if updating

    if (line) {
      FileLine=line->num;
      FileChar=line->txt[FileColumn];
      wsetcsiz(tw,csiz);
      wsetcpos(tw,0,FileColumn-FileIndent);
    }
    else {
      FileLine=0;
      FileChar=0;
      wsetcsiz(tw,csiz);
    }

    statline();

    update=1;
    switch (wgetc(tw)) {
    case _F1:
    case A_H:
      help();
      update=0;
      break;
    case _UPAR:
    case 'u':
      if (!line) break;
      if (line->prv) line=line->prv; else update=0;
      break;
    case _PGUP:
    case 'U':
      if (!line) break;
      for (i=1;(i<tw->rows)&&(line->prv);i++) line=line->prv;
      if (i==1) update=0;
      break;
    case _DNAR:
    case 'd':
      if (!line) break;
      if (line->nxt) line=line->nxt; else update=0;
      break;
    case _PGDN:
    case 'D':
      if (!line) break;
      for (i=1;(i<tw->rows)&&(line->nxt);i++) line=line->nxt;
      if (i==1) update=0;
      break;
    case _HOME:
    case 'h':
      FileIndent=0;
      FileColumn=0;
      break;
    case C_HOME:
    case C_PGUP:
    case 'H':
      if (!line) break;
      line=BaseLine;
      break;
    case _END:
    case C_PGDN:
    case 'e':
      if (!line) break;
      line=LastLine;
      break;
    case _LTAR:
    case 'l':
      if (!FileColumn) break;
      FileColumn--;
      if (FileIndent>FileColumn) FileIndent-=INDENT_INC; else update=0;
      break;
    case C_LTAR:
    case 'L':
      if (FileColumn>=INDENT_INC) FileColumn-=INDENT_INC; else FileColumn=0;
      if (FileIndent>FileColumn) FileIndent-=INDENT_INC; else update=0;
      while (FileIndent>FileColumn) FileIndent-=INDENT_INC;
      break;
    case _RTAR:
    case 'r':
      if (!line) break;
      if (FileColumn<(line->len-1)) FileColumn++;
      if ((FileColumn-FileIndent)>=tw->rows)
        FileColumn+=INDENT_INC;
      else
        update=0;
      break;
    case C_RTAR:
    case 'R':
      if (!line) break;
      if (FileColumn<line->len) FileColumn+=INDENT_INC;
      if (FileColumn>=line->len) FileColumn=line->len-1;
      if ((FileColumn-FileIndent)>=tw->rows)
        FileColumn+=INDENT_INC;
      else
        update=0;
      while ((FileColumn-FileIndent)>=tw->rows) FileColumn+=INDENT_INC;
      break;
    case 's':
    case _F3:
      if (!line) break;
      if (InputS("Enter search string: ",search,sizeof(search))) {
        if ((lin=SearchNxt(line->nxt,search))!=NULL) line=lin;
      }
      break;
    case 'S':
    case S_F3:
      if (!line) break;
      if ((lin=SearchNxt(line->nxt,search))!=NULL) line=lin;
      break;
    case 'b':
    case _F4:
      if (!line) break;
      if (InputS("Enter search string: ",search,sizeof(search))) {
        if ((lin=SearchPrv(line->prv,search))!=NULL) line=lin;
      }
      break;
    case 'B':
    case S_F4:
      if (!line) break;
      if ((lin=SearchPrv(line->prv,search))!=NULL) line=lin;
      break;
    case '1':  Marks[0]=line; break;
    case A_1:  if (Marks[0]) line=Marks[0]; break;
    case '2':  Marks[1]=line; break;
    case A_2:  if (Marks[1]) line=Marks[1]; break;
    case '3':  Marks[2]=line; break;
    case A_3:  if (Marks[2]) line=Marks[2]; break;
    case '4':  Marks[3]=line; break;
    case A_4:  if (Marks[3]) line=Marks[3]; break;
    case '5':  Marks[4]=line; break;
    case A_5:  if (Marks[4]) line=Marks[4]; break;
    case ESC:
    case A_N:
      return 1;
    case A_X:
    case C_Z:
    case _F10:
      return 0;
    default:
      update=0;
    }   //switch
  }   //forever
}   //DisplayFile


//===========================================================================
int  GetFile(void)
{
  WORD i=0;
  char done=0;
  LineID* lid;

  BaseLine=NULL;
  LastLine=NULL;
  FileLines=0;
  FileSize=0;
  while (!done) {
    i=0;
    while (i<MAX_LEN) {
      if (!fread(&buf[i],1,1,File)) { done=1; break; }
      if (buf[i++]=='\n') break;
    }   //while
    lid=malloc(LIDSIZE+i);
    if (!lid) return 0;
    FileSize+=i;
    FileLines++;
    memcpy(lid->txt,buf,i);            //copy record data
    lid->txt[i]=0;                     //put a string terminator
    lid->len=i;
    lid->max=i;
    lid->num=FileLines;
    lid->nxt=NULL;
    if (!BaseLine) {                   //insert node
      BaseLine=lid;
      lid->prv=NULL;
    }
    else {
      LastLine->nxt=lid;
      lid->prv=LastLine;
    }
    LastLine=lid;
  }   //while
  return 1;
}   //GetFile


//===========================================================================
void UnGetFile(void)
{
  while (LastLine) {
    BaseLine=LastLine->prv;
    free(LastLine);
    LastLine=BaseLine;
  }   //while
}   //UnGetFile


//===========================================================================
int  main(int argc,char* argv[])
{
  int i;

  if (argc<2) {
    printf("sho <files>\n");
    return 1;
  }

  cols=getscols();
  rows=getsrows();
  wupd=0;
  sw=wopen(0,NULL, 0,0,      1,cols, STATTR);
  tw=wopen(0,NULL, 1,0, rows-1,cols, BKATTR);
  hw=wopen(1," Help ",  rows/4,cols/4, rows/2,cols/2, HPATTR);
  if (!sw || !tw || !hw) {
    wpurge();
    printf("no room at the inn.\n");
    return 1;
  }
  sw->fl.wrap=0;
  sw->fl.scroll=0;
  sw->fl.showlf=1;
  sw->fl.showcr=1;
  sw->fl.showbs=1;
  tw->fchr=0xB1;
  tw->fl.wrap=0;
  tw->fl.scroll=0;
  tw->fl.showlf=1;
  tw->fl.showcr=1;
  tw->fl.showbs=1;
  wclr(tw);
  whide(hw);
  hw->fl.scroll=1;
  wupdate(NULL);

  for (i=1;i<argc;i++) {
    strcpy(FileName,argv[i]);
    File=fopen(FileName,"rb");
    if (!File) {
      wclr(sw);
      wprintf(sw,"Couldn't open \"%s\".  Hit any key",FileName);
      wgetc(sw);
      wclr(sw);
      continue;
    }
    if (!GetFile()) continue;
    fclose(File);
    statline();
    if (!DisplayFile()) { UnGetFile(); break; }
    UnGetFile();
  }   //for

  wpurge();
  return 0;
}   //main

