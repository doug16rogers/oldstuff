#include <stdio.h>
#include <graphics.h>

#include "cutil.h"

#define X_BITS    64
#define Y_BITS    64
#define X_WID     6
#define Y_WID     4
#define DEF_FILENAME  "cursor.bit"

#define X_LEFT    1
#define Y_TOP     1
#define X_SIZE    X_WID*X_BITS
#define Y_SIZE    Y_WID*Y_BITS
#define X_RIGHT   X_LEFT+X_SIZE-1
#define Y_BOTTOM  Y_TOP+Y_SIZE-1
#define X_L       X_LEFT-1
#define X_R       X_RIGHT+1
#define Y_T       Y_TOP-1
#define Y_B       Y_BOTTOM+1

#define X_left    X_R+7
#define Y_top     Y_T

#define X_FN      0
#define Y_FN      Y_B+8
#define X_CURS    X_RIGHT-8*5
#define Y_CURS    Y_FN

#define X_c       X_left+X_BITS+16
#define Y_c       0

int far *gmode;
char far *gdriver;
char nun[20],one[20],cur[20],blank[20],fn[80],ss[80];
unsigned char tab[Y_BITS][X_BITS];
unsigned char cut[X_BITS];
int x,y;
int finished;
unsigned char ch;
void far *vfp;

main (int argn,char *argv[])
{
  if (argn>1) strcpy (fn,argv[1]); else strcpy (fn,DEF_FILENAME);
  initialize ();

  while (!finished) {
    showspot (x,y);
    showcurs (x,y,1);
    ch=upcase (getkey ());
    showcurs (x,y,0);
    do_char (ch);
  }   /*while not done*/

  closegraph ();
}   /*main*/


initialize ()
{
  int i,j;
  detectgraph (gdriver,gmode);
  initgraph (gdriver,gmode,(char far *)"");

  cleardevice ();
  getimage (0,0,X_WID-1,Y_WID-1,(void far *)nun);
  getimage (0,0,39,7,(void far *)blank);
  moveto (0,0);
  lineto (0,Y_WID-1);
  lineto (X_WID-1,Y_WID-1);
  lineto (X_WID-1,0);
  lineto (0,0);
  getimage (0,0,X_WID-1,Y_WID-1,(void far *)cur);
  for (j=0;j<Y_WID;j++) for (i=0;i<X_WID;i++) putpixel (i,j,1);
  getimage (0,0,X_WID-1,Y_WID-1,(void far *)one);
  putimage (0,0,(void far *)nun,AND_PUT);
  writescr ();
  x=0;
  y=0;
  finished=0;
}   /*initialize*/


do_char (unsigned char ch)
{
  int i,j;
  switch (ch) {
    case _RTAR:
      if (++x>=X_BITS) x=0;
      break;
    case _LTAR:
      if (--x<0) x=X_BITS-1;
      break;
    case _DNAR:
      if (++y>=Y_BITS) y=0;
      break;
    case _UPAR:
      if (--y<0) y=Y_BITS-1;
      break;
    case _HOME:
      if (--x<0) x=X_BITS-1;
      if (--y<0) y=Y_BITS-1;
      break;
    case _END:
      if (--x<0) x=X_BITS-1;
      if (++y>=Y_BITS) y=0;
      break;
    case _PGUP:
      if (++x>=X_BITS) x=0;
      if (--y<0) y=Y_BITS-1;
      break;
    case _PGDN:
      if (++x>=X_BITS) x=0;
      if (++y>=Y_BITS) y=0;
      break;
    case _INS:
      for (i=(X_BITS-1);i>x;i--) tab[y][i]=tab[y][i-1];
      tab[y][x]=0;
      showline (y);
      break;
    case _DEL:
      for (i=x;i<(X_BITS-1);i++) tab[y][i]=tab[y][i+1];
      tab[y][X_BITS-1]=tab[y][X_BITS-2];
      showline (y);
      break;
    case A_1:
    case A_S:
      for (i=0;i<X_BITS;i++) tab[y][i]=1;
      showline (y);
      break;
    case A_0:
    case A_X:
      for (i=0;i<X_BITS;i++) tab[y][i]=0;
      showline (y);
      break;
    case ' ':
    case '+':
      tab[y][x]=!tab[y][x];
      break;
    case A_C:
      for (i=0;i<X_BITS;i++) cut[i]=tab[y][i];
      break;
    case A_Z:
      for (i=0;i<X_BITS;i++) tab[y][i]=cut[i];
      showline (y);
      break;
    case 'R':
      writescr ();
      break;
    case _F2:
      savetab ();
      break;
    case _F3:
      loadtab ();
      break;
    case _F10:
    case ESC:
      finished=1;
      break;
    default:
      break;
  }   /*switch*/
}   /*do_char*/


showcurs (int x,int y,int txt)
{
  putimage (X_LEFT+(x*X_WID),Y_TOP+(y*Y_WID),(void far *)cur,XOR_PUT);
  if (txt) {
    sprintf (ss,"%02X %02X",y,x);
    outtextxy (X_CURS,Y_CURS,ss);
  }
  else
    putimage (X_CURS,Y_CURS,(void far *)blank,AND_PUT);
}   /*showcurs*/


showspot (int x,int y)
{
  if (tab[y][x]) {
    putimage (X_LEFT+(x*X_WID),Y_TOP+(y*Y_WID),(void far *)one,OR_PUT);
    putpixel (X_left+x,Y_top+y,1);
  }
  else {
    putimage (X_LEFT+(x*X_WID),Y_TOP+(y*Y_WID),(void far *)nun,AND_PUT);
    putpixel (X_left+x,Y_top+y,0);
  }
}   /*showspot*/


showline (int y)
{
  int x;
  for (x=0;x<X_BITS;x++) showspot (x,y);
}   /*showline*/


writescr ()
{
  int y;
  cleardevice ();
  moveto (X_L,Y_T);
  lineto (X_L,Y_B);
  lineto (X_R,Y_B);
  lineto (X_R,Y_T);
  lineto (X_L,Y_T);
  for (y=0;y<Y_BITS;y++) showline (y);
  outtextxy (X_FN,Y_FN,fn);
  y=Y_c;
  y+=8; outtextxy (X_c,y,"Keypad  : move cursor");
  y+=8; outtextxy (X_c,y,"Ins     : insert blank");
  y+=8; outtextxy (X_c,y,"Del     : delete");
  y+=8; outtextxy (X_c,y,"SPACE + : toggle bit");
  y+=8; outtextxy (X_c,y,"Alt-1,S : fill row");
  y+=8; outtextxy (X_c,y,"Alt-0,X : clear row");
  y+=8; outtextxy (X_c,y,"Alt-C   : cut row");
  y+=8; outtextxy (X_c,y,"Alt-Z   : paste row");
  y+=8; outtextxy (X_c,y,"F2      : save to file");
  y+=8; outtextxy (X_c,y,"F3      : load from file");
  y+=8; outtextxy (X_c,y,"F10 ESC : exit");
}   /*writescr*/


loadtab ()
{
  FILE *f;
  int x,y;
  unsigned int w;
  f=fopen (fn,"rb");
  if (f!=NULL) {
    for (y=0;y<Y_BITS;y++) {
      for (x=0;x<X_BITS;x++) {
        if ((x&0xF)==0) fread (&w,2,1,f);
        if ((w&0x8000)!=0)
          tab[y][x]=1;
        else
          tab[y][x]=0;
        w<<=1;
      }   /*for thru bits*/
    }   /*for thru rows*/
    fclose (f);
    writescr ();
  }   /*if file opened*/
}   /*loadtab*/


savetab ()
{
  FILE *f;
  int x,y;
  unsigned int w;
  f=fopen (fn,"wb");
  if (f!=NULL) {
    for (y=0;y<Y_BITS;y++) {
      w=0;
      for (x=0;x<X_BITS;x++) {
        w<<=1;
        w|=tab[y][x];
        if ((x&0xF)==0xF) {
          fwrite (&w,2,1,f);
          w=0;
        }   /*if need to write*/
      }   /*for each bit*/
    }   /*for thru rows*/
    fclose (f);
  }   /*if file opened ok*/
}   /*savetab*/