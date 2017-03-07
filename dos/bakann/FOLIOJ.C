#include <stdio.h>

#define MAIN
#include "consts.h"
#include "cutil.h"


#define MAX_LEN 32

char infn[80];
char scrap[255],*ss;
int finished;
FILE *inf;

main (int argn,char *argv[])
{
  printf ("\nSCHEMA page reader...\n");
  infn[0]=0;
  if (argn>1)
    strcpy (infn,argv[1]);
  else {
    do {
      printf ("Filename to update: ");
      gets (infn);
      strip_white (infn);
    } while (infn[0]==0);
  }   /* else read filename from command line */
  open_files ();
  scan_file ();
  fclose (inf);
}   /* main */


open_files ()
{
  inf=fopen (infn,"rb");
  if (inf==NULL) {
    printf ("Could not open \"%s\" for read.\n",infn);
    exit (1);
  }
}   /* open_files */


void printsch (unsigned int n)
{
  int i;
  i=n;
  if (i<0) {
    printf ("-");
    i=-i;
  }
  printf ("%02d.%02d",(i>>4),(i&0x0F));
}   /* printsch */


void object_rec ()
{
  unsigned int l,ulx,uly,brx,bry,unk;
  unsigned char b;
  l=fgetc (inf);
  b=fgetc (inf);  ulx=b|(fgetc (inf)<<8);
  b=fgetc (inf);  uly=b|(fgetc (inf)<<8);
  b=fgetc (inf);  brx=b|(fgetc (inf)<<8);
  b=fgetc (inf);  bry=b|(fgetc (inf)<<8);
  b=fgetc (inf);  unk=b|(fgetc (inf)<<8);
  ss=scrap;
  for (b=0x0C;b<l;b++) *ss++=fgetc (inf);  *ss=0;
  printf ("Object,");
  printf (" ul=");  printsch (ulx);  printf (",");  printsch (uly);
  printf (" lr=");  printsch (brx);  printf (",");  printsch (bry);
  printf (" unk=%04X \"%s\"\n",unk,scrap);
}   /* object_rec */


line_rec ()
{
  unsigned int ulx,uly,brx,bry,unk;
  unsigned char b;
  fgetc (inf);   /* length */
  b=fgetc (inf);  ulx=b|(fgetc (inf)<<8);
  b=fgetc (inf);  uly=b|(fgetc (inf)<<8);
  b=fgetc (inf);  brx=b|(fgetc (inf)<<8);
  b=fgetc (inf);  bry=b|(fgetc (inf)<<8);
  printf ("Line,");
  printf (" ul=");  printsch (ulx);  printf (",");  printsch (uly);
  printf (" lr=");  printsch (brx);  printf (",");  printsch (bry);
  printf ("\n");
}   /* line_rec */


page_rec ()
{
  unsigned int xsize,ysize;
  unsigned char b;
  fgetc (inf);  /* get length=6 */
  b=fgetc (inf);  xsize=b|(fgetc (inf)<<8);  xsize=(xsize<<3);
  b=fgetc (inf);  ysize=b|(fgetc (inf)<<8);
  printf ("Pagesize, ");
  printsch (xsize);  printf (" x ");  printsch (ysize);
  printf ("\n");
}   /* page_rec */


label_rec ()
{
  unsigned int l,ulx,uly,brx,bry,offx,offy;
  unsigned char b;
  l=fgetc (inf);
  b=fgetc (inf);  ulx=b|(fgetc (inf)<<8);
  b=fgetc (inf);  uly=b|(fgetc (inf)<<8);
  b=fgetc (inf);  brx=b|(fgetc (inf)<<8);
  b=fgetc (inf);  bry=b|(fgetc (inf)<<8);
  b=fgetc (inf);  offx=b|(fgetc (inf)<<8);
  b=fgetc (inf);  offy=b|(fgetc (inf)<<8);
  ss=scrap;
  for (b=0x0E;b<l;b++) *ss++=fgetc (inf);  *ss=0;
  printf ("Label,");
  printf (" ul=");  printsch (ulx);  printf (",");  printsch (uly);
  printf (" lr=");  printsch (brx);  printf (",");  printsch (bry);
  printf (" off=");  printsch (offx);  printf (",");  printsch (offy);
  printf (" \"%s\"\n",scrap);
}   /* label_rec */


note_rec ()
{
  unsigned int l,ulx,uly,brx,bry;
  unsigned char b,size,unk;
  l=fgetc (inf);
  b=fgetc (inf);  ulx=b|(fgetc (inf)<<8);
  b=fgetc (inf);  uly=b|(fgetc (inf)<<8);
  b=fgetc (inf);  brx=b|(fgetc (inf)<<8);
  b=fgetc (inf);  bry=b|(fgetc (inf)<<8);
  size=fgetc (inf);
  unk=fgetc (inf);
  ss=scrap;
  for (b=0x0C;b<l;b++) *ss++=fgetc (inf);  *ss=0;
  printf ("Note,");
  printf (" ul=");  printsch (ulx);  printf (",");  printsch (uly);
  printf (" lr=");  printsch (brx);  printf (",");  printsch (bry);
  printf (" tsize=%d unk=%02X \"%s\"\n",size,unk,scrap);
}   /* note_rec */


void unknown_rec ()
{
  unsigned int l,b;
  l=fgetc (inf)&0x0FF;
  printf ("Unknown[%02X],",l);
  for (b=2;(b<l)&&!feof (inf);b++) printf (" %02X",fgetc (inf)&0x0FF);
  printf ("\n");
}   /* unknown_rec */


scan_file ()
{
  unsigned char b;
  finished=FALSE;
  while ((!finished)&&(!feof (inf))) {
    b=fgetc (inf);
    printf ("%02X:",b);
    switch (b&0x0FF) {
      case 0x00: object_rec ();   break;
      case 0x01:
      case 0x02: line_rec ();     break;
      case 0x03: printf ("End\n",b); finished=TRUE; break;
      case 0x04: page_rec ();     break;
      case 0x07: label_rec ();    break;
      case 0x0E: note_rec ();     break;
      default:   unknown_rec ();  break;
    }   /* switch */
  }   /* while scanning */
}   /* scan_file */