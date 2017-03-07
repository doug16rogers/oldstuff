#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <alloc.h>
#include <ctype.h>
#include <string.h>
#include <dos.h>


/*--------------------------------------------------------------------------*/
typedef unsigned char byte;
typedef unsigned int  word;
typedef unsigned long ulong;

#define UPCASE(c)      ( ((c)>'z')?(c):((c)<'a')?(c):((c)-0x20) )
#define LOCASE(c)      ( ((c)>'Z')?(c):((c)<'A')?(c):((c)+0x20) )
#define MIN(x,y)       ( ((x)<(y))?(x):(y) )
#define HEXVAL(c)      ( ((c)<'A') ? ((c)-'0') : \
                        (((c)<'a') ? ((c)-'A'+10) : ((c)-'a'+10)) )

#define VERBOSE        0               //verbose listing
#define BUFSIZE        0x8000          //32k of buffer space
#define SWAP_WORDS     "10:1"          //swap words
#define SWAP_DWORDS    "3210:1"        //swap double words
#define SWAP_SWITCH    SWAP_WORDS      //swap bytes in 16-bit words

char infile[0x80]="\0";                //input filename
char outfile[0x80]="\0";               //output filename
int  base=1;                           //bytes per unit
int  units=2;                          //units to swap
int  order[32]={1,0};                  //order of swaps
ulong loadsize;                        //size of data to load
byte *loadbuf;                         //load buffer
byte *swapbuf;                         //swap buffer
word bufsize=BUFSIZE;                  //size, in bytes, of load buffer
char verbose=VERBOSE;                  //verbose listing

unsigned long int bit[32]={
  0x00000001UL, 0x00000002UL, 0x00000004UL, 0x00000008UL,
  0x00000010UL, 0x00000020UL, 0x00000040UL, 0x00000080UL,
  0x00000100UL, 0x00000200UL, 0x00000400UL, 0x00000800UL,
  0x00001000UL, 0x00002000UL, 0x00004000UL, 0x00008000UL,
  0x00010000UL, 0x00020000UL, 0x00040000UL, 0x00080000UL,
  0x00100000UL, 0x00200000UL, 0x00400000UL, 0x00800000UL,
  0x01000000UL, 0x02000000UL, 0x04000000UL, 0x08000000UL,
  0x10000000UL, 0x20000000UL, 0x40000000UL, 0x80000000UL
};   //bit

char *minus_plus[]={ "-","+" };        //strings for flags


//---------------------------------------------------------------------------
void strNcpy(char* dst,char* src,int n)
{
  if (n--) {
    strncpy(dst,src,n);
    dst[n]=0;
  }
  else
    strcpy(dst,src);
}   //strNcpy


//---------------------------------------------------------------------------
int  digval(char c)
{
  if ((c>='0')&&(c<='9')) return c-'0';
  if ((c>='A')&&(c<='Z')) return c-'A'+10;
  if ((c>='a')&&(c<='z')) return c-'a'+10;
  return 37;
}   //digval


//---------------------------------------------------------------------------
int loadswapswitch(char* s)
{
  int dig;
  unsigned long int used=0;

  units = 0;
  while (*s && (*s!=':')) {
    dig=digval(*s);
    if (dig>=32) {
      printf("illegal digit character (or ':' delimiter) '%c' for -s\n",*s);
      return 0;
    }
    if (used & bit[dig]) {
      printf("mapping for unit '%c' (%u) already defined for -s\n",*s,dig);
      return 0;
    }
    used |= bit[dig];
    order[units++] = dig;
    s++;
  }   //while

  if (*s==':') {
    s++;
    if (!sscanf(s,"%u",&base)) {
      printf("ill-formed base value \"%s\" following ':' in -s\n",s);
      return 0;
    }
  }   //if redifing base size

  for (dig=0;dig<units;dig++) if ((order[dig]<0)||(order[dig]>=units)) {
    printf(
      "unit mapping %u is beyond unit count %u (maps start at 0)\n"
      ,order[dig],units
    );   //printf
    return 0;
  }   //for:if

  return 1;
}   //loadswapswitch


//---------------------------------------------------------------------------
int  loadfiles(char* s)
{
  if ((*s=='-')||(*s=='/')) return 1;  //a switch, so don't load
  if (infile[0]) {
    printf("input file already specified (\"%s\").\n",s);
    return 0;
  }
  strNcpy(infile,s,sizeof(infile));
  return 1;
}   //loadfiles


//---------------------------------------------------------------------------
int  loadflag(char* name,char* flag,char val)
{
  switch (val) {
  case 0:
  case '1':
  case '+':
    *flag=1;
    break;
  case '0':
  case '-':
    *flag=0;
    break;
  default:
    printf("illegal flag character '%c' for %s flag\n",val,name);
    return 0;
  }   //switch
  return 1;
}   //loadflag


//---------------------------------------------------------------------------
int  loadswitches(char* s)
{
  if ((*s!='-')&&(*s!='/')) return 1;  //not a switch, don't load

  s++;                                 //skip switch character
  switch (*s++) {
  case '?':
  case 'h':
  case 'H':
    return 0;
  case 'v':
    if (!loadflag("verbose",&verbose,*s)) return 0;
    break;
//  case 'o':
//    strNcpy(outfile,s,sizeof(outfile));
//    break;
  case 'B':
    if (sscanf(s,"%u",&bufsize)!=1) {
      printf("ill-formed buffer size \"%s\"\n",s);
      return 0;
    }
    if (bufsize>0x8000) {
      printf("illegal buffer size \"%s\" -- must be < 0x8000\n",s);
      return 0;
    }
    break;
  case 'w':
    if (!loadswapswitch(SWAP_WORDS)) return 0;
    break;
  case 'd':
    if (!loadswapswitch(SWAP_DWORDS)) return 0;
    break;
  case 's':
    if (!loadswapswitch(s)) return 0;
    break;
  default:
    s--;
    printf("unknown commandline switch \"%s\".",s);
    return 0;
  }   //switch on switch
  return 1;
}   //loadswitches


//---------------------------------------------------------------------------
void do_swap(byte* dst,byte* src)
{
  int i;
  int dstoff;
  int srcoff=0;

  for (i=0;i<units;i++) {
    dstoff = order[i]*base;
    memcpy(&dst[dstoff],&src[srcoff],base);
    srcoff += base;
  }
}   //do_swap


//---------------------------------------------------------------------------
int  swapfile(void)
{
  int file;
  ulong offs=0;
  word bytes_read;
  word bytes_written;
  word bufoffs;
  word i,lists;
  ulong total_bytes = 0;
  ulong total_lists = 0;

  file = _open(infile,O_BINARY | O_RDWR);
  if (file==-1) {
    printf("could not open input file \"%s\"\n",infile);
    return 0;
  }

  while (lseek(file,offs,SEEK_SET) != -1) {
    if (verbose) printf("%lu\r",offs);
    bytes_read = read(file,loadbuf,bufsize);
    total_bytes += (ulong) bytes_read;
    lists = bytes_read/(word)loadsize;
    total_lists += (ulong) lists;
    if (lists == 0) break;
    bufoffs=0;
    for (i=0;i<lists;i++) {
      do_swap(&swapbuf[bufoffs],&loadbuf[bufoffs]);
      bufoffs += (word)loadsize;
    }
    lseek(file,offs,SEEK_SET);
    bytes_written = write(file,swapbuf,bytes_read);
    if (bytes_written < bytes_read) {
      printf("couldn't write %u bytes to file offset %lu\n",bytes_read,offs);
      _close(file);
      return 0;
    }
    offs += (ulong)bytes_read;
  }   //while
  _close(file);

  if (verbose) {
    printf(
      "\n"
      "bytes processed = %lu\n"
      "swaps = %lu\n"
      ,total_bytes
      ,total_lists
    );
  }

  return 1;
}   //swapfile


//---------------------------------------------------------------------------
int  swapfileout(void)
{
  printf("feature not available\n");
  return 0;
}   //swapfileout


//---------------------------------------------------------------------------
int  help(int k)
{
  printf(
    "\n"
    "Usage: swap [switches] inputfile\n"
    "Switches [default]:\n"
    "  -?          list this help\n"
    "  -v[+/-]     verbose messages [%s]\n"
    "  -B<size>    buffer size in bytes [%u]\n"
//    "  -o<output>  write to file <output> instead of same file\n"
    "  -w          swap bytes within 16-bit words (same as -s10:1)\n"
    "  -d          swap bytes within 32-bit words (same as -s3210:1)\n"
    "  -s<list>[:<size>]  list of new order, see Notes [%s]\n"
    "Notes:\n"
    "  <list> for the -s switch is a sequence of digits (up to 32: 0-9,A-V)\n"
    "  indicating the position into which the new bytes (or byte-multiples)\n"
    "  are to be moved.  The :<size> decimal field may be added to change\n"
    "  the size of the base unit from 1 to any number of bytes.\n"
    "Examples:\n"
    "  swap -s1023 file    ;swaps the first two bytes of every four read\n"
    "  swap -s10:3 file    ;swaps the two groups of three bytes in each six\n"
    ,minus_plus[VERBOSE!=0]
    ,BUFSIZE
    ,SWAP_SWITCH
  );   //printf
  return k;
}   //help


//===========================================================================
int  main(int argc,char *argv[])
{
  int i;

  printf("swap: swaps the byte-order in a file\n");

  loadswapswitch(SWAP_SWITCH);
  for (i=1;i<argc;i++) if (!loadfiles(argv[i])) return help(1);
  if (!infile[0]) return help(0);
  for (i=1;i<argc;i++) if (!loadswitches(argv[i])) return help(1);

  if (verbose) {
    printf("input  = \"%s\"\n",infile);
    printf("output = \"%s\"\n",outfile);
    printf("units  = %u\n",units);
    printf("base   = %u\n",base);
    printf("order  = %u",order[0]);
    for (i=1;i<units;i++) printf(",%u",order[i]);
    printf("\n");
  }   //verbose

  loadsize =  (ulong)base;
  loadsize *= (ulong)units;
  if (verbose) printf("size of swap list = %ux%u = %lu.\n",units,base,loadsize);
  if (loadsize>(ulong)bufsize) {
    printf("load data size (base*units)=(%u*%u) > buffer size\n",base,units);
    return 4;
  }
  bufsize =  (bufsize/(word)loadsize);
  bufsize *= (word)loadsize;

  if (verbose) {
    printf("memory = %lu\n",farcoreleft());
    printf("actual buffer size = %u\n",bufsize);
  }

  loadbuf = (byte*)malloc(bufsize);
  swapbuf = (byte*)malloc(bufsize);
  if ((loadbuf==NULL)||(swapbuf==NULL)) {
    if (swapbuf!=NULL) free(swapbuf);
    printf("not enough memory\n");
    return 5;
  }

//  if (outfile[0]) {
    if (!swapfile()) return 3;
//  }
//  else {
//    if (!swapfileout()) return 3;
//  }

  return 0;
}   //main


