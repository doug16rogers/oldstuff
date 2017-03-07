#include <graphics.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>

/*
arc                 bar                 bar3d
circle              cleardevice         clearviewport
closegraph          detectgraph         drawpoly
ellipse             fillellipse         fillpoly
floodfill           getarccoords        getaspectratio
getbkcolor          getcolor            getdefaultpalette
getdrivername       getfillpattern      getfillsettings
getgraphmode        getimage            getlinesettings
getmaxcolor         getmaxmode          getmaxx
getmaxy             getmodename         getmoderange
getpalette          getpalettesize      getpixel
gettextsettings     getviewsettings     getx
gety                graphdefaults       grapherrormsg
_graphfreemem       _graphgetmem        graphresult
imagesize           initgraph           installuserdriver
installuserfont     line                linerel
lineto              moverel             moveto
outtext             outtextxy           pieslice
putimage            putpixel            rectangle
registerbgidriver   registerbgifont     restorecrtmode
sector              setactivepage       setallpalette
setaspectratio      setbkcolor          setcolor
setfillpattern      setfillstyle        setgraphbufsize
setgraphmode        setlinestyle        setpalette
setrgbpalette       settextjustify      settextstyle
setusercharsize     setviewport         setvisualpage
setwritemode        textheight          textwidth
*/


//===========================================================================
int main(void)
{
   int gdriver = DETECT, gmode, errorcode;
   int maxx,maxy,maxc;
   char scrap[80];
   typedef struct { int x,y; } Point;
#define SC         5
#define SIZE       (4*SC)
   Point funpoints[][4]={
//square...
     { {-4*SC,+0*SC}, {+0*SC,-4*SC}, {+4*SC,+0*SC}, {+0*SC,+4*SC} },
     { {-4*SC,-1*SC}, {+1*SC,-4*SC}, {+4*SC,+1*SC}, {-1*SC,+4*SC} },
     { {-4*SC,-2*SC}, {+2*SC,-4*SC}, {+4*SC,+2*SC}, {-2*SC,+4*SC} },
     { {-3*SC,-3*SC}, {+3*SC,-3*SC}, {+3*SC,+3*SC}, {-3*SC,+3*SC} },
     { {-2*SC,-4*SC}, {+4*SC,-2*SC}, {+2*SC,+4*SC}, {-4*SC,+2*SC} },
     { {-1*SC,-4*SC}, {+4*SC,-1*SC}, {+1*SC,+4*SC}, {-4*SC,+1*SC} },
   };   //funpoints
   #define NUM_FRAMES   (sizeof(funpoints)/sizeof(funpoints[0]))
   #define NUM_POINTS   (sizeof(funpoints[0])/sizeof(funpoints[0][0]))
   void *funimg[NUM_FRAMES];
   Point tmppoints[NUM_POINTS];
   int ch;
   int x,y;
   int i;

   initgraph(&gdriver, &gmode, "");
   errorcode = graphresult();

   if (errorcode != grOk)  /* an error occurred */
   {
      printf("Graphics error: %s\n", grapherrormsg(errorcode));
      exit(1);             /* return with error code */
   }

   /* draw a line */
   maxx=getmaxx();
   maxy=getmaxy();
   maxc=getmaxcolor();
   line(0,0,maxx,maxy);
   line(maxx,0,0,maxy);
   sprintf(scrap,"MaxX=%u,MaxY=%u,MaxC=%u,Fr=%u",maxx,maxy,maxc,NUM_FRAMES);
   settextjustify(CENTER_TEXT,CENTER_TEXT);
   outtextxy(maxx/2,maxy/2,scrap);

   setcolor(RED);
   moveto(0,0);
   lineto(maxx,0);    getch();
   lineto(maxx,maxy); getch();
   lineto(0,   maxy); getch();
   lineto(0,   0);

   setfillstyle(SOLID_FILL,LIGHTBLUE);

#define X  40
#define Y  100
   for (i=0;i<NUM_FRAMES;i++) {
     for (y=0;y<NUM_POINTS;y++) {
       tmppoints[y].x=X+funpoints[i][y].x;
       tmppoints[y].y=Y+funpoints[i][y].y;
     }
     fillpoly(NUM_POINTS,(int*)tmppoints);
     funimg[i]=malloc(imagesize(X-SIZE,Y-SIZE,X+SIZE,Y+SIZE));  //get memory
     getimage(X-SIZE,Y-SIZE,X+SIZE,Y+SIZE,funimg[i]);           //get image
     putimage(X-SIZE,Y-SIZE,funimg[i],XOR_PUT);                 //erase image
   }

   while (!kbhit()) {
     for (i=0;(i<NUM_FRAMES)&&!kbhit();i++)
       putimage(X-SIZE,Y-SIZE,funimg[i],COPY_PUT);
   }   //while
   getch();

   outtextxy(maxx/2,8,"i=up,k=dn,j=lt,l=rt,s=spin,ESC=quit");
   ch=' ';
   i=0;
   putimage(x=X-SIZE,y=Y-SIZE,funimg[i],COPY_PUT);
   while (ch!=0x1B) {   //ESC
     ch=getch();
     putimage(x,y,funimg[i],XOR_PUT);
     switch (ch) {
     case 'i': case 'I':  if (y>0) y--; break;
     case 'k': case 'K':  if (y<(maxy/2)) y++; break;
     case 'j': case 'J':  if (x>0) x--; break;
     case 'l': case 'L':  if (x<(maxx/2)) x++; break;
     case 's': case 'S':  i++;  if (i>=NUM_FRAMES) i=0; break;
     }   //switch
     putimage(x,y,funimg[i],XOR_PUT);
   }   //while

   /* clean up */
   outtextxy(maxx/2,24,"Press any key to end.");
   getch();
   closegraph();
   return 0;
}


