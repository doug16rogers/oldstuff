#include <stdio.h>
#include <conio.h>

#define DHELP_MAIN
#include "dhelp.h"

main (int argn,char *argv[])
{
  printf ("DHelp (c) 1989...\n");
  if (argn!=2) {
    printf ("Usage: DHELP <helpfile>\n");
    exit (0);
  }
  if (strchr (argv[1],'.')==NULL)
    sprintf (hfn,"%s.hlp",argv[1]);
  else
    strcpy (hfn,argv[1]);
  hf=fopen (hfn,"rb");
  if (hf==NULL) {
    printf ("Could not find help file \"%s\"\n",hfn);
    exit (1);
  }
  init_values ();
  draw_border ();
  while (!feof (hf)) {
    if (fgets (buff,MAX_BUFF_SIZE,hf)!=NULL) cprintf ("%s\r",buff);
  }
  fclose (hf);
}   /* main */


void init_values (void)
{
  int i;
  mode=TEXT_MODE;
  for (i=0;i<MAX_DEPTH;i++) {
    attr[i]=LIGHTGRAY + (BLACK<<4);
    rowcol[i]=0x0101;
  }
  curattr=0;
  currowcol=0;
  numkeys=0;
  textattr (attr[curattr]);
  clrscr ();
}   /* init_values */


void draw_border (void)
{
  window (1,1,80,25);
  gotoxy (1,1);
  textattr (BORDER_ATTR);
  clreol ();
  cprintf (" DHELP (c) 1989 | %s",hfn);
  gotoxy (1,25);
  clreol ();
  textattr (attr[curattr]);
  window (1,2,80,24);
  gotoxy (rowcol[currowcol] & 0xff,rowcol[currowcol] >> 8);
}   /* draw_border */