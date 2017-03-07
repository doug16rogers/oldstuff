#include <stdio.h>

#define MAX_BUFSIZE 0x4000
#define NUM_TYPES  4
#define INTEL      0
#define TEKTRONIX  1
#define MOTOROLA   2
#define TI         3

char *ftyp[NUM_TYPES]={"INTEL","TEKTRONIX","MOTOROLA","TI"};
int from,to;
char infn[80],oufn[80];
char rec[256];
unsigned char buffer[MAX_BUFSIZE];
unsigned int addr,min_addr,max_addr;
FILE *inf,*ouf;

main (int argn, char *argv[])
{
  unsigned int i;
  check_line (argn,argv);
  printf ("Converting \"%s\" (%s) to \"%s\" (%s)...\n",infn,ftyp[from],oufn,ftyp[to]);
  for (i=0;i<MAX_BUFSIZES;i++) buffer[i]=0;
  max_addr=0;
  min_addr=0xFFFF;
  load_buffer ();
  write_buffer ();
  fclose (inf);
  if (to!=from) {
    switch (to) {
      case INTEL:
        fprintf (ouf,":00000001FF\n");
        break;
      case TEKTRONIX:
        fprintf (ouf,"/00000000\n");
        break;
      case MOTOROLA:
        fprintf (ouf,"S9030000FC\n");
        break;
      case TI:
        break;
      default:
        break;
    }  /* switch */
  }  /* if different */
  fclose (ouf);
}   /* main */


check_line (int argn,char *argv[])
{
  int i,l;
  char c;
  from=-1;
  if (argn!=4) {
    printf ("Usage: CONHEX <input_file> <output_file> <output_type>\n");
    printf ("  where <output_type> should be ");
    for (i=0;i<(NUM_TYPES-1);i++) printf ("%s, ",ftyp[i]);
    printf ("or %s\n",ftyp[NUM_TYPES-1]);
    exit (1);
  }
  strcpy (infn,argv[1]);
  strcpy (oufn,argv[2]);
  strcpy (rec,argv[3]);
  to=NUM_TYPES;
  for (i=0;i<NUM_TYPES;i++) {
    l=strlen (ftyp[i]);
    c=rec[l];
    rec[l]=0;
    if (stricmp (rec,ftyp[i])==0) to=i;
    rec[l]=c;
  }   /* for */
  if (to==NUM_TYPES) {
    printf ("Illegal type \"%s\"... enter CONHEX for help\n",rec);
    exit (1);
  }

  inf=fopen (infn,"rt");
  if (inf==NULL) {
    printf ("Could not open \"%s\" for read\n",infn);
    exit (1);
  }
  fscanf (inf,"%s",rec);
  switch (rec[0]&0xFF) {
    case ':':
      from=INTEL;
      break;
    case '/':
      from=TEKTRONIX;
      break;
    case 'S':
      from=MOTOROLA;
      break;
    case '9':
    case 'K':
    case 'H':
      from=TI;
      break;
    default:
      printf ("Could not figure out input file type for \"%s\"\n",infn);
      fclose (inf);
      exit (1);
  }   /* switch */
  if (to=from) {
    printf ("Source and destination are both type %s, use COPY\n",ftyp[to]);
    fclose (inf);
    exit (1);
  }
  inf=fopen (infn,"rt");
  ouf=fopen (oufn,"wt");
  if (ouf==NULL) {
    printf ("Could not open \"%s\" for write\n",oufn);
    exit (1);
  }
}   /* check_line */


load_buffer ()
{
  char *cp;
  int cs;
  int i,k;
  while (!feof (inf)) {
    fgets (rec,256,inf);
    switch (from) {
      case INTEL:
        for (cp=rec;(*cp!=0)&&(*cp!=':');cp++);
        if (*cp==':') {
          cp++;
          sscanf (cp,"%2x",&numb);
          if  (numb>0) {
            cp+=2;
            sscanf (cp,"%4x",&addr);
            cp+=4;
            cp+=2;   /* skip terminate/continue indicator */
            get_buffer (cp);
          }   /* if nothing in record */
        }   /* if a valid record */
        break;
      case TEKTRONIX:
        cp=rec;
        cp++;
        sscanf (cp,"%4x",&addr);
        cp+=4;
        sscanf (cp,"%2x",&numb);
        if (numb==0) return;
        cp+=2;
        cp+=2;   /* skip checksum for first 3 bytes */
        get_buffer (cp);
        break;
      case MOTOROLA:
        cp=rec;
        cp++;
        if (*cp++=='9') return;
        sscanf (cp,"%2x",&numb);  numb-=3;   /* get only bytes of data */
        if (numb==0) return;
        cp+=2;
        sscanf (cp,"%4x",&addr);
        cp+=4;
        get_buffer (cp);
        break;
      case TI:
        cp=rec;
        while ((*cp!=0)&&(*cp!='F')) {
          switch (*cp&0xFF) {
            case '9':
              cp++;   /* skip the 9 */
              sscanf (cp,"%4x",&addr);  addr+=addr;   /* double since TI in words */
              cp+=4;
              while (*cp=='B') {
                cp++;
                sscanf (cp,"%2x",&k);
                buffer[numb+1]=k;
                cp+=2;
                sscanf (cp,"%2x",&k);
                buffer[numb]=k;
                cp+=2;
                numb+=2;
              }
              break;
            }   /* while record is still active */
      default:
        printf ("Internal error: FROM has value %d\n",from);
        fclose (inf);
        fclose (ouf);
        exit (1);
        break;
    }   /* switch transferring to internal buffer */
  }   /* while !feof */
}   /* load_rec */


write_rec ()
{
  addr=min_addr;
  while (addr<max_addr) {
    switch (to) {
      case INTEL:
        fprintf (ouf,":%02X%04X00",numb&0xFF,addr);
        cs=addr+(addr>>8)+numb;
        for (i=0;i<numb;i++) {
          fprintf (ouf,"%02X",(buffer[i]&0xFF));
          cs+=buffer[i];
        }
        cs=-cs;
        fprintf (ouf,"%02X\n",(cs&0xFF));
        break;
      case TEKTRONIX:
        cs=addr+(addr>>8)+numb;
        fprintf (ouf,"/%04X%02X%02X",addr,numb,(cs&0xFF));
        cs=0;
        for (i=0;i<numb;i++) {
          fprintf (ouf,"%02X",(buffer[i]&0xFF));
          cs+=buffer[i];
        }
        fprintf (ouf,"%02X\n",(cs&0xFF));
        break;
      case MOTOROLA:
        fprintf (ouf,"S1%02X%04X",(numb+3)&0xFF,addr);
        cs=addr+(addr>>8)+(numb+3);
        for (i=0;i<numb;i++) {
          fprintf (ouf,"%02X",(buffer[i]&0xFF));
          cs+=buffer[i];
        }    /* for */
        cs^=0xFF;
        fprintf (ouf,"%02X\n",(cs&0xFF));
        break;
      case TI:
        break;
      default:
          printf ("Internal error: TO has value %d\n",to);
          fclose (inf);
          fclose (ouf);
          exit (1);
        break;
    }   /* switch xferring to output format */
}   /* write_rec */


get_buffer (char *s)
{
  int i,k;
  if (addr<min_addr) min_addr=addr;
  for (i=0;(i<numb)&&(addr<MAX_BUFFERSIZE);i++) {
    sscanf (s,"%2x",&k);
    buffer[addr++]=k;
    s+=2;
  }
  if (addr>max_addr) max_addr=addr;
  if (addr>=MAX_BUFFERSIZE) {
    printf ("Buffer too small for reference to address %04X\n",addr);
    fclose (inf);
    fclose (ouf);
    exit (1);
  }
}   /* get_buffer */