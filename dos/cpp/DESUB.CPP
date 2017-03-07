#include <stdiostr.h>
#include <string.h>
#include <ctype.h>

#include "drstr.hpp"
#include "alphabet.hpp"
#include "buff.hpp"


#define CODED_ALPHA    "i"             //input (text) file for code alpha
#define PLAIN_ALPHA    "a"             //lowercase text
#define PROBABLE       "e"             //use lowercase of English probables

#define ALPHA_UPPERS   "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define ALPHA_LOWERS   "abcdefghijklmnopqrstuvwxyz"
#define ALPHA_DIGITS   "0123456789"
#define ALPHA_PUNCTS   " !\"'(),-./:;?"
#define ALPHA_EXTRAS   " !\"#$%&'()*+,-./:;<=>?[\\]^_{|}~"

#define ENGLISH_UPPERS "ETASOIRNHCLDPMFUYGBWKVXQZJ"
#define ENGLISH_LOWERS "etasoirnhcldpmfuygbwkvxqzj"
#define TEXT_UPPERS    " ETASOIRNH.CLDPMFUY,GBWKVXQZJ"
#define TEXT_LOWERS    " etasoirnh.cldpmfuy,gbwkvxqzj"

#define VERBOSE        0               //keep verbage at a minimum
#define FORCE_CASE     0               //don't force case
#define FORCE_UPPER    0               //if forcing, though, force lower
#define TRY_DEPTH      6               //depth of permutation
#define TRY_WIDTH      1               //width of permutation

Alphabet coded;                        //alphabet to filter codetext
Alphabet plain;                        //alphabet to attempt to substitute
Alphabet probable;                     //alphabet of probable list

char verbose=VERBOSE;                  //verbose output listing
char force_case=FORCE_CASE;            //whether to force input case
char force_upper=FORCE_UPPER;          //if forcing, to upper or lower case?
int  try_depth=TRY_DEPTH;              //depth = n for permutations
int  try_width=TRY_WIDTH;              //width = k for permutations

char *force_str[]={ "-","-","c","C" }; //for help strings
char *minus_plus[]={"-","+"};          //for help

char infile[0x80]="\0";                //input file
FILE* inf=NULL;                        //input file buffer
char outfile[0x80]="\0";               //output file (optional)

ByteBuffer codebuf;


//===========================================================================
int  input_files(char* s)
{
  if ((*s=='-')||(*s=='/')) return 1;  //a switch, so don't load
  if (infile[0]) {
    printf("input file already specified (\"%s\").\n",s);
    return 0;
  }
  strNcpy(infile,s,sizeof(infile));
  return 1;
}   //input_files


//===========================================================================
long int bit[]={0x1,0x2,0x4,0x8,0x10,0x20,0x40,0x80,0x100,0x200,0x400,0x800};
long int hit = 0L;
long int hit_mask = 0L;
void do_perm(int depth,int width,char *s,int cur)
{
  int b,e,i;
  if (cur >= depth)
  {
    if ((hit_mask & hit) != hit_mask) return;
    for (i=0;i<depth;i++) printf("%c",'a'+s[i]);
    printf("\n");
    return;
  }

  if (cur < width) b = 0; else b = cur - width;
  if ((cur+width) >= depth) e = depth-1; else e = cur + width;
  for (i=b;i<=e;i++) {
    if (s[i] < cur) continue;
    hit |= bit[i];
    s[i] = cur;
    do_perm(depth,width,s,cur+1);
    hit &= ~bit[i];
  }
}

void TryPermutations(int depth,int width,char* s)
{
  char *list = new char[depth];
  for (int i=0;i<depth;i++) list[i]=depth;
  hit_mask = 0L;
  for (i=0;i<depth;i++) { hit_mask <<= 1;  hit_mask |= 1; }
  do_perm(depth,width,list,0);
}   //TryPermutations


//===========================================================================
int  AddBySwitch(Alphabet& alpha,char* s)
{
  BYTE b;
  FILE* f;

  while (*s) {
    switch (*s++) {
    case 'a': alpha.Add(ALPHA_LOWERS); break;
    case 'A': alpha.Add(ALPHA_UPPERS); break;
    case '#': alpha.Add(ALPHA_DIGITS); break;
    case 'h': for (b='a';b<='f';b++) alpha.Add(b); break;
    case 'H': for (b='A';b<='F';b++) alpha.Add(b); break;
    case 'p': for (b=' ';b<='~';b++) alpha.Add(b); break;
    case '.':                                //common punctuation
      alpha.Add(ALPHA_PUNCTS);
      break;
    case ':':                                //printables except 0-9,A-Z,a-z
      alpha.Add(ALPHA_EXTRAS);
      break;
    case '*':
      alpha.Add(ALPHA_UPPERS);
      alpha.Add(ALPHA_LOWERS);
      alpha.Add(ALPHA_DIGITS);
      alpha.Add(ALPHA_PUNCTS);
      break;
    case '7': for (b=0;b<=0x7F;b++) alpha.Add(b); break;
    case '8': for (b=0;b<=0xFE;b++) alpha.Add(b); alpha.Add(0xFF); break;
    case '=':
      while (*s && (*s!='=')) {
        s = strcchr((char*)&b,s);
        alpha.Add(b);
      }   //while
      break;
    case 'f':
      f = fopen(s,"rt");
      if (f==NULL) {
        printf("could not open text alphabet file \"%s\".\n",s);
        return 0;
      }
      while (fread(&b,1,1,f)) if (isprint(b)) alpha.Add(b);
      while (*s) s++;
      fclose(f);
      break;
    case 'F':
      f = fopen(s,"rb");
      if (f==NULL) {
        printf("could not open binary alphabet file \"%s\".\n",s);
        return 0;
      }
      while (fread(&b,1,1,f)) alpha.Add(b);
      fclose(f);
      while (*s) s++;
      break;
    case 'i':
      f = fopen(infile,"rt");
      if (f==NULL) {
        printf("could not open text alphabet file \"%s\".\n",infile);
        return 0;
      }
      while (fread(&b,1,1,f)) if (isprint(b)) alpha.Add(b);
      fclose(f);
      break;
    case 'I':
      f = fopen(infile,"rb");
      if (f==NULL) {
        printf("could not open binary alphabet file \"%s\".\n",infile);
        return 0;
      }
      while (fread(&b,1,1,f)) alpha.Add(b);
      fclose(f);
      break;
    case 's': alpha.Add(' '); break;
    default:
      s--;
      printf("unknown alphabet type \"%s\".\n",s);
      return 0;
    }   //switch
  }   //while
  return 1;
}   //AddBySwitch(Alphabet&,char*)


//===========================================================================
int  AddProbableBySwitch(Alphabet& alpha,char* s)
{
  BYTE b;
  FILE* f;

  while (*s) {
    switch (*s++) {
    case 'e': alpha.Add(ENGLISH_LOWERS); break;
    case 'E': alpha.Add(ENGLISH_UPPERS); break;
    case 't': alpha.Add(TEXT_LOWERS); break;
    case 'T': alpha.Add(TEXT_UPPERS); break;
    case '=':
      while (*s && (*s!='=')) {
        s = strcchr((char*)&b,s);
        alpha.Add(b);
      }   //while
      break;
    case 'f':
      f = fopen(s,"rt");
      if (f==NULL) {
        printf("could not open text alphabet file \"%s\".\n",s);
        return 0;
      }
      while (fread(&b,1,1,f)) if (isprint(b)) alpha.Add(b);
      while (*s) s++;
      fclose(f);
      break;
    case 'F':
      f = fopen(s,"rb");
      if (f==NULL) {
        printf("could not open binary alphabet file \"%s\".\n",s);
        return 0;
      }
      while (fread(&b,1,1,f)) alpha.Add(b);
      fclose(f);
      while (*s) s++;
      break;
    case 's': alpha.Add(' '); break;
    default:
      printf("unknown probable list type \"%s\".\n",s);
      return 0;
    }   //switch
  }   //while
  return alpha.Count();
}   //AddProbableBySwitch(Alphabet&,char*)


//===========================================================================
int  LoadTryNumbers(char* s,int& depth,int &width)
{
  if (!*s) return 1;
  if (*s != ':')
  {
    if (!sscanf(s,"%u",&depth))
    {
      printf("invalid value \"%s\" for try width.\n",s);
      return 0;
    }   //if
  }   //if

  while (isdigit(*s)) s++;
  if (!*s) return 1;
  if (*s != ':')
  {
    printf("bad delimiter '%c' for try depth:width, should be ':'.\n",*s);
    return 0;
  }
  if (!sscanf(s,"%u",&width))
  {
    printf("invalid value \"%s\" for try width.\n",s);
    return 0;
  }
  return 1;
}   //LoadTryNumbers


//===========================================================================
int  loadflag(char* name,char *flag,char c)
{
  switch (c) {
  case '+':
  case '1':
  case 0:
    *flag=1;
    break;
  case '-':
  case '0':
    *flag=0;
    break;
  default:
    printf("illegal flag setting character '%c' for %s\n",c,name);
    return 0;
  }   //switch
  return 1;
}   //loadflag


//===========================================================================
int  switches(char* s)
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
  case 'c':
    if (!loadflag("force lower",&force_case,*s)) return 0;
    force_upper=0;
    break;
  case 'C':
    if (!loadflag("force upper",&force_case,*s)) return 0;
    force_upper=1;
    break;
  case 'i':                            //set code-text alphabet
    if (!AddBySwitch(coded,s)) return 0;
    break;
  case 'p':                            //set plain-text alphabet
    if (!AddBySwitch(plain,s)) return 0;
    break;
  case 'P':                            //set probable alphabet, in order!
    if (!AddProbableBySwitch(probable,s)) return 0;
    break;
  case 'o':
    strNcpy(outfile,s,sizeof(outfile));
    break;
  case 'T':
    if (!LoadTryNumbers(s,try_depth,try_width)) return 0;
    break;
  default:
    s--;
    printf("unknown commandline switch \"%s\".",s);
    return 0;
  }   //switch on switch
  return 1;
}   //switches


//===========================================================================
int  help(int k)
{
  printf(
    "\n"
    "Usage: desub [switches] inputfile\n"
    "Switches [default]:\n"
    "  -?/h/H        this help\n"
    "  -v            verbose output [%s]\n"
    "  -p<plaintext> plaintext alphabet (see option for -c) [%s]\n"
    "  -i<codetext>  codetext (input) alphabet (may be combined): [%s]\n"
    "    A/a           upper/lower case alphabetics only (A-Z/a-z)\n"
    "    #             numerics only (0-9)\n"
    "    H/h           upper/lower case hexadecimal (0-9 + A-F/a-f)\n"
    "    p             all printable characters\n"
    "    .             common punctuation ( !\"'(),-./:;?)\n"
    "    :             all punctuation ( !\"#$%&'()*+,-./:;<=>?@[\\]^_{|}~)\n"
    "    *             upper + lower + digits + common punctuation (Aa#.)\n"
    "    7             seven bits (ASCII)\n"
    "    8             eight bits (binary)\n"
    "    =<string>     <string> supplies alphabet characters (may end in '=')\n"
    "    f/F<file>     text/binary file (only printables loaded from text)\n"
    "    i/I           use text/binary characters in input file\n"
    "    s             add the space character (\x20) to the alphabet\n"
    "  -C/c          force case of input alphabetics to upper/lower [%s]\n"
    "  -P<probable>  most probable alphabet characters [%s]\n"
    "    E/e           most probable English letters, upper/lower case\n"
    "    T/t           ASCII text with upper/lower English\n"
    "    =<string>     use <string> as list of most probable (may end '=')\n"
    "    f/F<file>     text/binary file (only printables loaded from text)\n"
    "  -t[<depth>]:[<width>]   depth:width of permutations to try [%u:%u]\n"
    "Notes:\n"
    "  Characters in the cyphertext (inputfile) which are not in the\n"
    "  alphabet are ignored (and pass directly through to output).\n"
    "  The probable list must be given in order of decreasing probability.\n"
    "  You may use '+' or '-' after a flag to turn it on or off (none='+').\n"
    ,minus_plus[VERBOSE!=0]
    ,PLAIN_ALPHA
    ,CODED_ALPHA
    ,force_str[(FORCE_CASE<<1)+FORCE_UPPER]
    ,PROBABLE
    ,TRY_DEPTH,TRY_WIDTH
  );   //printf
  return k;
}   //help


//===========================================================================
int  main(int argc,char *argv[])
{
  int i;

  printf("desub: crypto-analyzess a substitution coded file.\n");

  for (i=1;i<argc;i++) if (!input_files(argv[i])) return help(1);
  if (!infile[0]) {
    printf("no input file specified.\n");
    return help(1);
  }   //if no input file
  inf = fopen(infile,"rb");
  if (inf == NULL) {
    printf("could not open input file \"%s\".\n",infile);
    return 2;
  }
  fclose(inf);

  for (i=1;i<argc;i++) if (!switches(argv[i])) return help(1);
  if (coded.Count()==0) AddBySwitch(coded,CODED_ALPHA);
  if (plain.Count()==0) AddBySwitch(plain,PLAIN_ALPHA);
  if (probable.Count()==0) AddProbableBySwitch(probable,PROBABLE);

  if (!codebuf.Load(coded,infile)) {
    printf("couldn't load from input file \"%s\".\n",infile);
    return 3;
  }

  if (!codebuf.Count()) {
    printf("no codetext loaded from \"%s\"! check alphabet (-i)\n",infile);
    return 4;
  }

  if (verbose) {
    coded.Show   ("code:  \"");
    plain.Show   ("plain: \"");
    probable.Show("prob:  \"");
    if (outfile[0]) printf("output:\t%s\n",outfile);
    if (force_case) {
      if (force_upper)
        printf("forcing upper case\n");
      else
        printf("forcing lower case\n");
    }
    else {
      printf("no case forcing on input alphabetics\n");
    }
    printf("loaded %u bytes from \"%s\"\n",codebuf.Count(),infile);
    codebuf.Sort();
    printf(
      "  Char Count Percent\n"
      "  ---- ----- -------\n"
    );
    for (i=0;i<codebuf.Different();i++) {
      BYTE b=codebuf.SortedByte(i);
      printf("  %02X ",b);
      if (isprint(b)) printf("%c",b); else printf(" ");
      printf(" %5u",codebuf.Count(b));
      printf(" %7.4f",100.0*((double)codebuf.Count(b))/codebuf.Count());
      printf("\n");
    }
  }   //if verbose

  char *fun = new char[try_depth+1];
  for (i=0;i<try_depth;i++) fun[i] = 'a'+i;
  fun[i] = 0;
  TryPermutations(try_depth,try_width,fun);
}   //main

