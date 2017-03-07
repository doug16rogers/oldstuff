#include <stdiostr.h>
#include <fstream.h>
#include <string.h>
#include <ctype.h>


#include "bitstr.hpp"

#define COMMENT_CHAR   ';'
#define MAX_INPUTS     32

#define BITS           32
#define INPUTS         1
#define FALLING_EDGE   1
#define INVERT         0
#define MSB_FIRST      1
#define BEG_CLOCK      3
#define END_CLOCK      13
#define RANDOM_FILL    0x100
#define FILL           0x000
#define VERBOSE        0

UBIT16 bits=BITS;
UBIT16 inputs=INPUTS;
char   falling_edge=FALLING_EDGE;
char   invert=INVERT;
char   msb_first=MSB_FIRST;
UBIT16 beg_clock=BEG_CLOCK;
UBIT16 end_clock=END_CLOCK;
UBIT16 fill=FILL;
char   verbose=VERBOSE;
char   infn[0x80]="\0";
char   oufn[0x80]="\0";
char   clk0='0';
char   clk1='1';

ifstream inf;
ofstream ouf;

BIT_STREAM *in_bits[MAX_INPUTS];

char *rise_fall[]  = { "rising", "falling" };
char *lsb_msb[]    = { "lsb",    "msb"     };
char *minus_plus[] = { "-",      "+"       };


//===========================================================================
void strNcpy(char* d,char* s,int n)
{
  if (n==0)
    strcpy(d,s);
  else {
    strncpy(d,s,n);
    d[n-1]=0;
  }
}   //strNcpy


//===========================================================================
void InsertFill(UBIT16 clocks)
{
  for (UBIT16 input_word = 0; (input_word < inputs); input_word++)
  {
    in_bits[input_word]->Flush();
    if (msb_first)
      in_bits[input_word]->UnGet(fill,8);
    else
      in_bits[input_word]->Put(fill,8);
  }   //for each initialization

  char bit[MAX_INPUTS];
  for (UBIT16 i = 0; (i < clocks); i++)
  {
    for (input_word=0;input_word<inputs;input_word++)
    {
      bit[input_word] = '0' + (*in_bits[input_word])[i&7];
      ouf << bit[input_word] << '.' << clk0;
      if (input_word < (inputs-1)) ouf << '.';
    }
    ouf << '\n';

    for (input_word=0;input_word<inputs;input_word++)
    {
      ouf << bit[input_word] << '.' << clk1;
      if (input_word < (inputs-1)) ouf << '.';
    }
    ouf << '\n';
  }   //for each clock
}   //InsertFill


//===========================================================================
void Convert(char* s,int line)
{
  if (verbose)
    ouf << COMMENT_CHAR << "        " << s << '\n';

  for (UBIT16 i = 0; s[i]; i++)
    if (s[i] == COMMENT_CHAR) break;
  s[i] = 0;

  UBIT32 inbits;
  UBIT16 input_words = 0;

  while (sscanf(s,"%X",&inbits)==1)
  {
    if (invert) inbits = ~inbits;
    in_bits[input_words]->Flush();
    if (msb_first)
      in_bits[input_words]->UnGet(inbits,32);
    else
      in_bits[input_words]->Put(inbits,32);
    if (input_words >= MAX_INPUTS) break;
    input_words++;
    while (isspace(*s)) s++;
    while (isxdigit(*s)) s++;
  }   //while

  if ((input_words != 0) && (input_words < inputs))
  {
    cerr << "not enough inputs on line " << line << '\n';
    return;
  }

  char bit[MAX_INPUTS];
  UBIT16 input_word;

  for (i=0;i<bits;i++)
  {
    for (input_word=0;input_word<inputs;input_word++)
    {
      bit[input_word] = '0' + in_bits[input_word]->BitGet();
      ouf << bit[input_word] << '.' << clk0;
      if (input_word < (inputs-1)) ouf << '.';
    }
    if (verbose) ouf << "  ;" << i;
    ouf << '\n';

    for (input_word=0;input_word<inputs;input_word++)
    {
      ouf << bit[input_word] << '.' << clk1;
      if (input_word < (inputs-1)) ouf << '.';
    }
    ouf << '\n';
  }   //for each bit
}   //Convert


//===========================================================================
int help(int k)
{
  printf(
    "\n"
    "Usage: datclk [switches] inputfile outputfile\n"
    "Switches:\n"
    "  -n<bits>        number of bits per data word, 1-32 [%u]\n"
    "  -p<inputs>      number of parallel inputs on each line [%u]\n"
    "  -c<rise/fall>   clock data in on r=rising or f=falling edge [%s]\n"
    "  -f<msb/lsb>     first bit out on each word (l=lsb, m=msb) [%s]\n"
    "  -i[+/-]         invert data from inputfile [%s]\n"
    "  -B#             decimal number of clocks at beginning [%u]\n"
    "  -E#             decimal number of clocks at end [%u]\n"
    "  -F<hex>         fill byte for ending clocks [%02X]\n"
    "  -v[+/-]         verbose output -- contains comments [%s]\n"
    ,BITS
    ,INPUTS
    ,rise_fall[FALLING_EDGE!=0]
    ,lsb_msb[MSB_FIRST!=0]
    ,minus_plus[INVERT!=0]
    ,BEG_CLOCK
    ,END_CLOCK
    ,FILL
    ,minus_plus[VERBOSE!=0]
  );   //printf
  return k;
}   //help


//===========================================================================
int  loadflag(char* str,char* flag,char* name)
{
  switch (*str) {
  case 0:
  case '+':
  case '1':
    *flag = 1;
    break;
  case '-':
  case '0':
    *flag = 0;
    break;
  default:
    printf("illegal flag value '%c' for %s flag\n",*str,name);
    return 0;
  }   //switch
  return 1;
}   //loadflag


//===========================================================================
int loadswitch(char* s)
{
  if ((*s!='-')&&(*s!='/')) {
    if (oufn[0]) {
      printf("extra filename \"%s\" on commandline.\n",s);
      return 0;
    }   //if three filenames
    if (infn[0])
      strNcpy(oufn,s,sizeof(oufn));
    else
      strNcpy(infn,s,sizeof(infn));
    return 1;
  }   //if not switch

  s++;
  switch (*s++) {
  case 'n':
    if (sscanf(s,"%u",&bits) && (bits>=1) && (bits<=32)) break;
    printf("invalid <bits> field \"%s\".\n",s);
    return 0;
  case 'c':
    if      (*s=='r') falling_edge=0;
    else if (*s=='f') falling_edge=1;
    else {
      printf("illegal clock edge switch '%c'.\n",*s);
      return 0;
    }
    break;
  case 'f':
    if      (*s=='l') msb_first=0;
    else if (*s=='m') msb_first=1;
    else {
      printf("illegal msb/lsb first switch '%c'.\n",*s);
      return 0;
    }
    break;
  case 'i':
    if (!loadflag(s,&invert,"invert")) return 0;
    break;
  case 'p':
    if (sscanf(s,"%u",&inputs) && inputs && (inputs <= MAX_INPUTS)) break;
    printf("invalid parallel inputs number \"%s\".\n",s);
    return 0;
  case 'B':
    if (sscanf(s,"%u",&beg_clock)) break;
    printf("invalid beginning clocks number \"%s\".\n",s);
    return 0;
  case 'E':
    if (sscanf(s,"%u",&end_clock)) break;
    printf("invalid ending clocks number \"%s\".\n",s);
    return 0;
  case 'F':
    //////////if (*s=='r') { fill=RANDOM_FILL; break; }
    if (sscanf(s,"%x",&fill) && (fill<0x100)) break;
    printf("invalid fill byte \"%s\".\n",s);
    return 0;
  case 'v':
    if (!loadflag(s,&verbose,"verbose")) return 0;
    break;
  default:
    s--;
    s--;
    printf("unknown switch \"%s\".\n",s);
    return 0;
  }   //switch
  return 1;
}   //loadswitch


//===========================================================================
int  main(int argc,char* argv[])
{
  printf("datclk  --  bit stream for data from file and clock\n");

  for (int i=0;i<MAX_INPUTS;i++)
    in_bits[i] = new BIT_STREAM(32);

  for (i=1;i<argc;i++) if (!loadswitch(argv[i])) return help(1);

  if (!oufn[0])
  {
    printf("too few arguments supplied.\n");
    return help(1);
  }   //if no input/output file

  inf.open(infn,ios::nocreate);
  if (!inf)
  {
    cout << "couldn't open \"" << infn << "\".\n";
    return 2;
  }

  ouf.open(oufn);
  if (!ouf)
  {
    inf.close();
    cout << "couldn't open \""<<oufn<<"\".\n";
    return 2;
  }

  if (!falling_edge)
  {
    clk0='1';
    clk1='0';
  }

  if (verbose)
    ouf << ";from " << infn << "\n;Beginning clocks...\n";
  InsertFill(beg_clock);

  char s[0x100];
  int line = 0;
  while (inf.get(s,sizeof(s)))
  {
    Convert(s,line++);
    inf.get();
  }

  if (verbose)
    ouf << ";Ending clocks...\n";
  InsertFill(end_clock);

  inf.close();
  ouf.close();

  cout << "wrote data bits from " << infn << " to " << oufn << ".\n";
}   //main

