#include <stdio.h>
#include <dir.h>
#include <dos.h>

#include "strutil.h"


#define MAX_LEN          32
#define MAX_LABEL_WIDTH  20
#define MAX_IGNORE       400
#define DEF_FONT         2
#define DEF_SPACING      8
#define DEF_PREFIXSTR    "[SH "
#define DEF_INTERSTR     ","
#define DEF_SUFFIXSTR    "]"
#define LABEL_ID         0x07
#define NOTE_ID          0x0E

int font,spacing;
struct ffblk ff;
char prefixstr[80],suffixstr[80],interstr[80];
typedef char labstr[MAX_LABEL_WIDTH];
labstr ign[MAX_IGNORE];
int ignn;
int fnum;

int showing;
char infn[80];
char pagn[80],bakn[80];
char scrap[STR_LEN],scrap2[STR_LEN],*ss;
unsigned char buffer[256];
int finished;
FILE *inf,*ouf,*rptf;
char *oufn="temp.p01";
char *cfgn="addsh.cfg";
char *ignf="addsh.ign";


void dohelp (void);
void get_infn (int argn,char *argv[]);
void get_ignores (char *fn);
void load_ignore (void);
int  using_label (char *lab);
int  note_for_label (char *s);
void get_report (void);
int  label_reported (char *label,char *note);
void build_note (char *label,char *pages);
void update_err (int e);
int  update_file (void);
void update_all (void);
void cfgerr (int l,char *s);
void cfg_addsh (void);
int  read_rec (void);
int  write_rec (void);


int  main (int argn,char *argv[])
{
  int i;

  printf ("\nSheet number adder for SCHEMA pages "
	  "========================================\n");
  cfg_addsh ();
  get_infn (argn,argv);
  load_ignore ();
  get_report ();
  if (fnum==0)
    update_all ();
  else
    update_err (update_file ());
  fclose (rptf);
  return 0;
}   /* main */


void dohelp (void)
{
  printf (
"Usage: ADDSH <filename_only> [<page_number>]\n"
"Make sure you only specify the name of the file and NOT the extension.\n"
"To update single page, specify the page number as a second parameter.\n"
"If no page number is given, ADDSH will update all pages, starting at page 1\n"
"and continuing until a page is not found.  Therefore, be sure that your page\n"
"numbers are contiguous.\n"
"\n"
"You may specify labels which are to be ignored by creating a <filename>.IGN\n"
"file.  Use !<label> to force the use of a label.  Use <label>* to ignore any\n"
"label that matches <label> up to the *.  Use <label># to ignore any label that\n"
"matches <label> up to the #, but whose next digit is a numeral.  For example,\n"
"a schematic with a data bus might contain the following file:\n"
"   !DATA00                     always use label DATA00\n"
"   DATA#                       ignore all other DATA's with numerals\n"
"Forced-usage labels should be listed earlier in the .IGN file than wildcard\n"
"ignore labels.\n"
"ADDSH first loads the ADDSH.IGN file, then checks for a <filename>.IGN file.\n"
  );
  exit (1);
}   /* dohelp */


void get_infn (int argn,char *argv[])
{
  char *t;
  fnum=0;
  if ((argn<2)||(argn>3)) dohelp ();
  if (strchr (argv[1],'.')!=NULL) dohelp ();
  if (argn>2) {
    if (sscanf (argv[2],"%d",&fnum)!=1) {
      printf ("Illegal page number specification \"%s\"\n",argv[2]);
      exit (1);
    }
    if ((fnum<1)||(fnum>99)) {
      printf ("Page number \"%s\" is out of range; specify 1..99\n",argv[2]);
      exit (1);
    }
  }
  strcpy (infn,argv[1]);
  ss=infn;
  while ((*ss!=0)&&(*ss!='.')) ss++;
  *ss=0;
}   /* get_infn */


void get_ignores (char *fn)
{
  int l;
  l=0;
  while ((!feof (inf)) && (ignn<MAX_IGNORE)) {
    l++;
    scrap[0]=0;
    fgets (scrap,STR_LEN,inf);
    strcompress(scrap,scrap,0);
    strparse(scrap,NO_PSEP);
    if (pnum>1) {
      printf ("%s[%d] only one label allowed per line: \"%s\"\n",fn,l,scrap);
    }
    else if (pnum==1) {
      if (plen[0]>=MAX_LABEL_WIDTH)
        printf ("%s[%d] label \"%s\" too large, skipping\n",fn,l,scrap);
      else {
        strsub (scrap,ppos[0],plen[0],ign[ignn++]);
      }   /* else label short enough */
    }   /* else a label is there */
  }   /* while */
  fclose (inf);
}   /* get_ignores */


void load_ignore (void)
{
  ignn=0;
  inf=fopen (ignf,"rt");
  if (inf!=NULL) get_ignores (ignf);
  strcpy (scrap2,infn);
  strcat (scrap2,".ign");
  inf=fopen (scrap2,"rt");
  if (inf!=NULL) get_ignores (scrap2);
}   /* load_ignore */


int using_label (char *lab)
{
  int i,use_label;
  char *s,*t;
  for (i=0;i<ignn;i++) {
    use_label=FALSE;
    s=ign[i];
    t=lab;
    if (*s=='!') { s++; use_label=TRUE; }
    while ((*s!=0)&&(*t!=0)&&(toupper (*s)==toupper (*t))) { s++; t++; }
    if (*s=='*') return (use_label);
    if ((*s=='#')&&(*t>='0')&&(*t<='9')) return (use_label);
    if (*s==*t) return (use_label);
  }    /* for */
  return (TRUE);
}   /* using_label */


int note_for_label (char *s)
{
  while (*s!=0) if ((*s++&0x80)!=0) return (TRUE);
  return (FALSE);
}   /* note_for_label */


void get_report (void)
{
  strcpy (scrap2,infn);
  strcat (scrap2,".rpt");
  rptf=fopen (scrap2,"rt");
  if (rptf==NULL) {
    printf ("Report file \"%s\" not found\n",scrap2);
    exit (1);
  }
}   /* get_report */


int label_reported (char *label,char *note)
{
  int going;
  char rlab[256];
  char rlabel[MAX_LABEL_WIDTH];
  fseek (rptf,0,SEEK_SET);        /* start at top of report file */
  rlab[0]=0;
  going=TRUE;
  while ((!feof (rptf))&&going) {
    rlab[0]=0;
    fgets (rlab,255,rptf);
    strparse (rlab,NO_PSEP);
    if (pnum==2) {
      if (plen[0]<MAX_LABEL_WIDTH) {
        strsub (rlab,ppos[0],plen[0],rlabel);
        if (stricmp (label,rlabel)==0) {
          strsub (rlab,ppos[1],plen[1],note);
          return (TRUE);
        }   /* if label found in file */
      }   /* if label short enough */
    }   /* if two parameters on line */
  }   /* while */
  return (FALSE);
}   /* label_reported */


void build_note (char *label,char *pages)
{
  unsigned int ll,ulx,uly,brx,bry,offx,offy;
  unsigned int l,nulx,nuly,nbrx,nbry;
  unsigned char b;
  int mode,even;
  char *s,*t;
  buffer[0]=NOTE_ID;
  ulx=buffer[0x02]|(buffer[0x03]<<8);
  uly=buffer[0x04]|(buffer[0x05]<<8);
  brx=buffer[0x06]|(buffer[0x07]<<8);
  bry=buffer[0x08]|(buffer[0x09]<<8);
  offx=buffer[0x0A]|(buffer[0x0B]<<8);
  offy=buffer[0x0C]|(buffer[0x0D]<<8);
  l=0x0C;
  for (s=prefixstr;*s!=0;s++) buffer[l++]=*s;
  mode=0;
  b=0;
  for (s=pages;*s!=0;s++) {
    switch (mode) {
      case 0:
        if ((*s>='0')&&(*s<='9')) {
          b=(*s-'0');
          mode=1;
        }
        break;
      case 1:
        if ((*s>='0')&&(*s<='9')) {
          b=10*b+(*s-'0');
          if (b==fnum)
            mode=0;
          else {
            if (b>9) buffer[l++]='0'+(b / 10);
            buffer[l++]='0'+(b % 10);
            mode=2;
          }   /* if not this page */
        }   /* if a numeral */
        else
          mode=0;
        break;
      case 2:
        if (*s==',') {            /* insert separator */
          for (t=interstr;*t!=0;t++) buffer[l++]=*t;
          mode=0;
        }
        break;
      default:
        b=0;
        mode=0;
        break;
    }   /* switch */
  }   /* for writing out page numbers */
  for (s=suffixstr;*s!=0;s++) buffer[l++]=*s;
  ll=l-0x0C;
  for (s=label;*s!=0;s++) buffer[l++]=*s|0x80;  /* |0x80 */
  even=(offy<0x0008);
  if ( (even&&((offx&0x8000)==0)) || ((!even)&&(offx==0)) ) {
    nbrx=ulx;
    nbry=uly+0x0004;
    nulx=nbrx-ll*spacing;
    nuly=nbry-8;
  }
  else {
    nulx=brx;
    nbrx=nulx+ll*spacing;
    nbry=bry-4;
    nuly=nbry-8;
  }
  buffer[l++]=0;
  buffer[0x02]=nulx;  buffer[0x03]=nulx>>8;
  buffer[0x04]=nuly;  buffer[0x05]=nuly>>8;
  buffer[0x06]=nbrx;  buffer[0x07]=nbrx>>8;
  buffer[0x08]=nbry;  buffer[0x09]=nbry>>8;
  buffer[0x0A]=font;
  buffer[0x0B]=spacing;
  buffer[0x01]=l;
}   /* build_note */


void update_err (int e)
{
  switch (e) {
    case 0:
      break;   /* no error */
    case 1:
      printf ("File \"%s\" not found\n",pagn);
      exit (1);
      break;
    case 2:
      printf ("Reached last page (99).\n");
      break;
    case 3:
    case 4:
      printf ("Unable to backup file \"%s\"; cannot continue\n",pagn);
      exit (1);
      break;
    case 5:
      printf ("File \"%s\" backed up ok, but could not reopen \"%s\" to read\n",pagn,bakn);
      exit (1);
      break;
    case 6:
      printf ("Could not open \"%s\" to write after backing it up\n",pagn);
      exit (1);
      break;
    case 7:
      printf ("Abnormal end-of-file found updating \"%s\"; cannot continue\n",pagn);
      exit (1);
      break;
    default:
      printf ("Unknown internal error %d processing \"%s\"\n",e,pagn);
      exit (1);
      break;
  }   /* switch */
}   /* update_err */


int update_file (void)
{
  unsigned int numread,numwrote;
  int going;
  if ((fnum<1)||(fnum>99)) return (2);
  sprintf (pagn,"%s.p%02d",infn,fnum);
  sprintf (bakn,"%s.b%02d",infn,fnum);
  inf=fopen (pagn,"rb");
  if (inf==NULL) return (1);
  ouf=fopen (bakn,"wb");
  if (ouf==NULL) {
    fclose (inf);
    return (3);
  }
  do {
    numread=fread (buffer,1,256,inf);
    numwrote=fwrite (buffer,1,numread,ouf);
  } while ((numread!=0)&&(numread==numwrote));
  fclose (inf);
  fclose (ouf);
  if (numread!=numwrote) return (4);
  printf ("Updating \"%s\"\n",pagn);
  inf=fopen (bakn,"rb");
  if (inf==NULL) return (5);
  ouf=fopen (pagn,"wb");
  if (ouf==NULL) return (6);
  going=TRUE;
  while (read_rec ()&&going) {
    if (buffer[0]==NOTE_ID) {
      strcpy (scrap,&buffer[0x0C]);          /* get note string */
      if (!note_for_label (scrap)) going=write_rec ();
    }
    else {
      going=write_rec ();
    }
    if ((going)&&(buffer[0]==LABEL_ID)) {
      strcpy (scrap,&buffer[0x0E]);          /* get label out of record */
      if (using_label (scrap)) {
        if (label_reported (scrap,scrap2)) {
	  build_note (scrap,scrap2);
	  going=write_rec ();
        }   /* if label */
      }   /* if using label */
    }   /* checking a label */
  }   /* while */
  fputc (0x03,ouf);
  fclose (inf);
  fclose (ouf);
  if (!going) return (7);
  return (0);
}   /* update_file */


void update_all (void)
{
  int err;
  sprintf (pagn,"%s.p01",infn);
  if (fopen (pagn,"rt")==NULL) {
    printf ("Could not find first page of schematics for \"%s\"\n",infn);
    exit (1);
  }
  for (fnum=1;(fnum<100)&&((err=update_file ())==0);fnum++);
  if (err!=1) update_err (err);
}   /* update_all */


void cfgerr (int l,char *s)
{
  printf ("%s[%d] Config error: %s\n",cfgn,l,s);
}   /* cfgerr */


void cfg_addsh (void)
{
  FILE *cfgf;
  int line;

  ignn=0;
  font=DEF_FONT;
  spacing=DEF_SPACING;
  strcpy (prefixstr,DEF_PREFIXSTR);
  strcpy (interstr,DEF_INTERSTR);
  strcpy (suffixstr,DEF_SUFFIXSTR);
  cfgf=fopen (cfgn,"rt");
  if (cfgf==NULL)
    printf ("Warning: configuration file \"%s\" not found; using default values.\n",cfgn);
  else {
    line=0;
    while (!feof (cfgf)) {
      fgets (scrap2,STR_LEN,cfgf);
      line++;
      strcomment(scrap2,';');
      if (strparse(scrap2,NO_PSEP)>0) {
        strsub(scrap2,ppos[0],plen[0],scrap);
        if (stricmp(scrap,"FONT")==0) {
          if (pnum!=2) cfgerr (line,"Integer needed for FONT command");
          else if (!strparamint(scrap2,1,&font)) cfgerr (line,"Illegal FONT value");
        }
        else if (stricmp(scrap,"SPACING")==0) {
          if (pnum!=2) cfgerr (line,"Integer needed for SPACING command");
          else if (!strparamint(scrap2,1,&spacing)) cfgerr (line,"Illegal SPACING value");
        }
        else if (stricmp(scrap,"PREFIX")==0) {
          if (pnum!=2) cfgerr(line,"Missing PREFIX string");
          else strsub(scrap2,ppos[1],plen[1],prefixstr);
        } else if (stricmp (scrap,"SUFFIX")==0) {
          if (pnum!=2) cfgerr(line,"Missing SUFFIX string");
          else strsub(scrap2,ppos[1],plen[1],suffixstr);
        }
        else if (stricmp (scrap,"INTER")==0) {
          if (pnum!=2) cfgerr(line,"Missing INTER string");
          else strsub(scrap2,ppos[1],plen[1],interstr);
        }
        else {
          sprintf (scrap2,"Illegal command \"%s\"",scrap);
          cfgerr (line,scrap2);
        }   /* else not a valid command */
      }   /* if there are parameters on the line */
    }   /* while not eof */
  }   /* if config file found */
}   /* cfgf_addsh */


int read_rec (void)
{
  unsigned char b,l;
  b=0;
  if (feof (inf)) return (FALSE);
  buffer[b++]=fgetc (inf);
  if (feof (inf)) return (FALSE);
  l=buffer[b++]=fgetc (inf);
  return (fread (&buffer[2],1,l-2,inf)==(l-2));  /* rest o'rec */
}   /* read_rec */


int write_rec (void)
{
  return (fwrite (buffer,1,buffer[1],ouf)==buffer[1]);
}   /* write_rec */


