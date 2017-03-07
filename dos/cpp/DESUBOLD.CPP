#include <stdio.h>
#include <ctype.h>
#include <string.h>


/*--------------------------------------------------------------------------*/
typedef unsigned char byte;
typedef unsigned int  word;

#define UPCASE(c)      ( ((c)>'z')?(c):((c)<'a')?(c):((c)-0x20) )
#define LOCASE(c)      ( ((c)>'Z')?(c):((c)<'A')?(c):((c)+0x20) )
#define MIN(x,y)       ( ((x)<(y))?(x):(y) )
#define HEXVAL(c)      ( ((c)<'A') ? ((c)-'0') : \
                        (((c)<'a') ? ((c)-'A'+10) : ((c)-'a'+10)) )

#define NO_CHAR        -1              /*undefined char index value*/
#define IB_SIZE        0x2000          /*input buffer size*/
#define MAX_MAXLEN     0x20            /*max maximum correlation key length*/
#define MAX_DEPTH      0x20            /*maximum search depth for key char*/

#define ENGLISH_UPPER  "ETAONRISH"     /*most probables in English*/
#define ENGLISH_LOWER  "etaonrish"     /*ditto, lower case*/
#define TEXT_UPPER     " ETAONRISH"    /*put a space in there*/
#define TEXT_LOWER     " etaonrish"    /*put a space in there*/

#define ALPHABET       "a"             /*default alphabet, lowercase only*/
#define PROBABLE       "e"             /*default probable list, English*/
#define MAXLEN         0x18            /*default maxlen*/
#define FORCELEN       0               /*default forced key length*/
#define VERBOSE        0               /*default verbose flag*/
#define FORCECASE      0               /*default forcecase flag, force case*/
#define UPPER          0               /*default upper/lower flag, to lower*/

char* onoff[]={ "off", "on" };
char* forcestr[]={ "off", "off", "c=lower", "C=upper" };
char* forcelenstr[]={ "as calculated", "forced" };


/*--------------------------------------------------------------------------*/
byte forcecase=FORCECASE;              /*force to upper/lower case on input*/
byte upper=UPPER;                      /*force to upper if 1, lower if 0*/
byte verbose=VERBOSE;                  /*verbose output*/

byte alpha[0x100];                     /*alphabet*/
word alphan=0;                         /*number of chars in alphabet*/
int  index[0x100];                     /*inverted alphabet index*/
byte prob[0x100];                      /*most probable chars in alphabet*/
word probn=0;                          /*number of chars in prob*/

word maxlen=MAXLEN;                    /*maximum length to search*/
word keylen=0;                         /*length of key*/
word forcelen=FORCELEN;                /*forced key length*/

word subfreq[MAX_MAXLEN][0x100];       /*subtext frequencies*/
byte most[MAX_DEPTH][MAX_DEPTH];       /*most probable key character*/
word depth;                            /*depth of search (usually probn)*/

byte likely[MAX_MAXLEN][MAX_DEPTH];    /*likely key characters*/
byte key[MAX_MAXLEN];                  /*most likely key*/

byte ib[IB_SIZE];                      /*input buffer*/
word ibn=0;                            /*number of bytes in input buffer*/

char in[0x80]="\0";                    /*input filename*/

/*--------------------------------------------------------------------------*/
int  switches(char *s);
char* loadalpha(char *alph);
char* loadprob(char *p);
int  loadinput(char *fn);


/*--------------------------------------------------------------------------*/
int  help(int k)
{
  printf(
    "\n"
    "Usage: desub [switches] inputfile\n"
    "Switches [default]:\n"
    "  -v            verbose output [%s]\n"
    "  -a<plaintext>   plaintext alphabet (see option for -A) [%s]\n"
    "  -A<cyphertext>  cyphertext alphabet [%s]\n"
    "    i/I           use text/binary characters in input file
    "    p             printable characters\n"
    "    7             seven bits (ASCII)\n"
    "    8             eight bits (binary)\n"
    "    =<string>     <string> supplies the alphabet\n"
    "    A/a           upper/lower case alphabetics only (A-Z/a-z)\n"
    "    +             lower and upper case alphabetics (a-z + A-Z)\n"
    "    #             numerics only (0-9)\n"
    "    N/n           numerics and upper/lower case alphabetics\n"
    "    *             all numerics and alphabetics (0-9 + a-z + A-Z)\n"
    "    H/h           upper/lower case hexadecimal (0-9 + A-F/a-f)\n"
    "    f/F<file>     text/binary file (only printables loaded from text)\n"
    "  -C/c          force case of input alphabetics to upper/lower [%s]\n"
    "  -p<probable>  most probable characters in alphabet [%s]\n"
    "    E/e           most probable English letters, upper/lower case\n"
    "    T/t           ASCII text with upper/lower English\n"
    "    =<string>     use <string> as list of most probable\n"
    "    f/F<file>     text/binary file (only printables loaded from text)\n"
    "Notes:\n"
    "  Characters in the cyphertext (inputfile) which are not in the\n"
    "  alphabet are ignored (and pass directly through to output).\n"
    "  The probable list must be given in order of decreasing probability.\n"
    "  You may use '+' or '-' after a flag to turn it on or off.\n"
    ,onoff[VERBOSE!=0]
    ,PLAIN_ALPHABET
    ,CYPHER_ALPHABET
    ,forcestr[(FORCECASE<<1)+UPPER]
    ,FORCELEN,forcelenstr[FORCELEN>0]
    ,MAXLEN
    ,PROBABLE
  );   /*printf*/
  return k;
}   /*help*/


/*==========================================================================*/
int  main(int argc,char *argv[])
{
  int i;

  printf("desub: crypto-analyzess a substitution coded file.\n");
  loadplain(PLAIN_ALPHABET);           /*set up default plaintext alphabet*/
  loadcypher(CYPHER_ALPHABET);         /*set up default cyphertext alphabet*/
  loadprob(PROBABLE);                  /*set up probable list*/

  if (argc<=1) return help(1);

  for (i=1;i<argc;i++) if (!switches(argv[i])) return help(1);

  if (!in[0]) {
    printf("No input file specified on commandline.\n");
    return help(1);;
  }   /*oops! no pw*/

  if (!loadinput(in)) {
    printf("could not open \"%s\" for read, or no input in file.\n",in);
    return help(2);;
  }   /*if fi not open*/
  printf("Read %u bytes from \"%s\".\n",ibn,in);

  if (ibn<10) {
    printf("not enough cyphertext in \"%s\".\n",in);   /*printf*/
    return 3;
  }   /*if too little ct*/

  if (ibn<(2*maxlen)) {
    maxlen = ibn/2;
    printf("warning: only enough cyphertext to check to length %u.\n",maxlen);
  }

  keylen=keylength();
  if (forcelen) keylen=forcelen;
  if (verbose&&forcelen)
    printf(";  Key length is being forced to %u.\n",forcelen);
  printf("Using key length of %3u.\n",keylen);

  gensubfreqs();                       /*generate sublist frequencies*/

  return 0;
}   /*main*/


//===========================================================================
char* strcchr(char* d,char* s)
{
  int n=0;
  int dig=0;
  int base=8;
  char c;

  c=*s++;
  if (c!='\\') {                       //if not a special char
    *d=c;                              //..just copy it
    return s;                          //..and quit
  }   //if not escape char

  c=*s++;
  switch (c) {
  case 0:    *d=0x5C; s--; return s;   //\-terminated string, leave in
  case 'a':  *d=0x07; return s;        //special chars...
  case 'b':  *d=0x08; return s;
  case 't':  *d=0x09; return s;
  case 'n':  *d=0x0A; return s;
  case 'v':  *d=0x0B; return s;
  case 'f':  *d=0x0C; return s;
  case 'r':  *d=0x0D; return s;
  case 'e':  *d=0x1B; return s;        //extension to the standard (<ESC>)
  case '\"': *d=0x22; return s;
  case '\'': *d=0x27; return s;
  case '\\': *d=0x5C; return s;
  default:
    if      ((c=='x')||(c=='X')) { n=0;     dig=0; base=16; }
    else if ((c=='d')||(c=='D')) { n=0;     dig=0; base=10; }
    else if ((c>='0')&&(c<='7')) { n=c-'0'; dig=1; base=8; }
    else { *d=c;  return s; }          //ignore ill-used \ char
  }   /*switch*/

  while (dig<3) {
    c=digval(*s++);
    if (c<base) {
      n = n*base + c;
      dig++;
    }
    else {
      s--;
      break;
    }   //if base 16
  }   //while
  if (dig)
    *d = n;
  else {
    s--;
    *d = *s++;
  }
  return s;
}   //strcchr


//===========================================================================
int strcstr(char *d,char *s,int maxn)
{
  int i=0;
  if (maxn--)                          //reduce by 1 for terminator
    while (*s && (i<maxn)) s=strcchr(&d[i++],s);
  else
    while (*s) s=strcchr(&d[i++],s);
  d[i]=0;
  return i;
}   /*strcstr*/


/*--------------------------------------------------------------------------*/
char*  loadalpha(char *s)
{
  char c;
  word n;
  int i;
  FILE *f;                             /*used if loading alphabet from file*/

  alphan=0;                            /*reset alphabet size*/
  forcecase=0;                         /*if alphabet specified, don't force*/
  switch (*s++) {
  case 'a':                            /*lower case alphabetic*/
    for (i=0;i<26;i++) alpha[alphan++]='a'+i;
    break;
  case 'A':                            /*upper case alphabetic*/
    for (i=0;i<26;i++) alpha[alphan++]='A'+i;
    break;
  case '+':                            /*upper and lower case alphabetic*/
    for (i=0;i<26;i++) alpha[alphan++]='A'+i;
    for (i=0;i<26;i++) alpha[alphan++]='a'+i;
    break;
  case '#':                            /*numeric only*/
    for (i=0;i<10;i++) alpha[alphan++]='0'+i;
    break;
  case 'h':                            /*lower case hex*/
    for (i=0;i<10;i++) alpha[alphan++]='0'+i;
    for (i=0;i<6;i++) alpha[alphan++]='a'+i;
    break;
  case 'H':                            /*upper case hex*/
    for (i=0;i<10;i++) alpha[alphan++]='0'+i;
    for (i=0;i<6;i++) alpha[alphan++]='A'+i;
    break;
  case 'n':                            /*lower case alphanumeric*/
    for (i=0;i<10;i++) alpha[alphan++]='0'+i;
    for (i=0;i<26;i++) alpha[alphan++]='a'+i;
    break;
  case 'N':                            /*upper case alphanumeric*/
    for (i=0;i<10;i++) alpha[alphan++]='0'+i;
    for (i=0;i<26;i++) alpha[alphan++]='A'+i;
    break;
  case '*':                            /*upper and lower case alphnumerics*/
    for (i=0;i<10;i++) alpha[alphan++]='0'+i;
    for (i=0;i<26;i++) alpha[alphan++]='A'+i;
    for (i=0;i<26;i++) alpha[alphan++]='a'+i;
    break;
  case '7':                            /*seven bits (ASCII)*/
    for (i=0;i<0x80;i++) alpha[alphan++]=i;
    break;
  case '8':                            /*eight bits (binary)*/
    for (i=0;i<0x100;i++) alpha[alphan++]=i;
    break;
  case 'p':                            /*printable*/
    for (i=0;i<0x80;i++) if (isprint(i)) alpha[alphan++]=i;
    break;
  case '=':                            /*commandline supplied*/
    n=strcstr(s,s);                    /*convert allowing \r, etc*/
    while ((n--) && (alphan<sizeof(alpha))) alpha[alphan++]=*s++;
    break;
  case 'f':
    f=fopen(s,"rt");                   /*open text alphabet file*/
    if (!f) {
      printf("couldn't open text alphabet file \"%s\".\n",s);
      return NULL;
    }
    while (alphan<sizeof(alpha)) {     /*read in alphabet*/
      if (!fread(&c,1,1,f)) break;     /*no more to read*/
      if (isprint(c)) alpha[alphan++]=c;  /*only printable from text file*/
    }   /*for*/
    fclose(f);
    while (*s) s++;
    break;
  case 'F':
    f=fopen(s,"rb");                   /*open binary alphabet file*/
    if (!f) {
      printf("couldn't open binary alphabet file \"%s\".\n",s);
      return NULL;
    }
    while (alphan<sizeof(alpha))       /*read in alphabet*/
      if (fread(&alpha[alphan],1,1,f)) alphan++; else break;
    fclose(f);
    while (*s) s++;
    break;
  default:
    s--;
    printf("unknown alphabet selection \"%s\".\n",s);
    return NULL;
  }   /*switch*/

  if (!alphan) {
    printf("no alphabet supplied.\n");
    return NULL;
  }   /*if no chars in alphabet*/

/*generate index for alphabet*/
  for (i=0;i<0x100;i++) index[i]=NO_CHAR;  /*clear index array*/
  for (i=0;i<alphan;i++) {             /*now generate indices..*/
    if (index[alpha[i]]!=NO_CHAR)      /*..check for previous occurrence*/
      printf("warning: duplicate char '\\x%02X' in alphabet.\n",alpha[i]);
    else
      index[alpha[i]]=i;               /*..load index*/
  }   /*for each char*/

  if (alphan==0) {
    printf("No characters in alphabet.\n");
    return NULL;
  }
  return s;
}   /*loadalpha*/


/*--------------------------------------------------------------------------*/
char* loadprob(char *p)
{
  char* t;
  FILE* f;
  byte c;
  word n;

  probn=0;
  switch (*p++) {
  case 'e':
    t=ENGLISH_LOWER;
    while (*t && (probn<sizeof(prob))) prob[probn++]=*t++;
    break;
  case 'E':
    t=ENGLISH_UPPER;
    while (*t && (probn<sizeof(prob))) prob[probn++]=*t++;
    break;
  case 'T':
    t=TEXT_UPPER;
    while (*t && (probn<sizeof(prob))) prob[probn++]=*t++;
    break;
  case 't':
    t=TEXT_LOWER;
    while (*t && (probn<sizeof(prob))) prob[probn++]=*t++;
    break;
  case '=':
    n=strcstr(p,p);                    /*convert allowing \r, etc*/
    while ((n--) && (probn<sizeof(prob))) prob[probn++]=*p++;
    break;
  case 'f':
    f=fopen(p,"rt");                   /*open text problist file*/
    if (!f) {
      printf("couldn't open text probable list file \"%s\".\n",p);
      return NULL;
    }
    while (probn<sizeof(prob)) {       /*read in problist*/
      if (!fread(&c,1,1,f)) break;     /*no more to read*/
      if (isprint(c)) prob[probn++]=c; /*only printable from text file*/
    }   /*for*/
    fclose(f);
    while (*p) p++;
    break;
  case 'F':
    f=fopen(p,"rb");                   /*open binary problist file*/
    if (!f) {
      printf("couldn't open binary probable list file \"%s\".\n",p);
      return NULL;
    }
    while (probn<sizeof(prob))         /*read in problist*/
      if (fread(&prob[probn],1,1,f)) probn++; else break;
    fclose(f);
    while (*p) p++;
    break;
  default:
    p--;
    printf("Illegal probable list switch \"%s\".\n",p);
    return NULL;
  }   /*switch*/

  if (!probn) {
    printf("No probable list entries were specified.\n");
    return NULL;
  }

  return p;                            /*return position in list*/
}   /*loadprob*/


/*--------------------------------------------------------------------------*/
int  switches(char *s)
{
  if (!strchr("-/",*s)) {
    if (in[0]) {
      printf("Too many parameters (\"%s\") on commandline.\n",s);
      return 0;
    }
    strncpy(in,s,sizeof(in)-1);
    in[sizeof(in)-1]=0;                /*make sure not too big*/
    return 1;
  }   /*if not a switch*/

/*must be a switch...*/
  s++;                                 /*point to switch*/
  while (*s) {                         /*allow multiple switches per '-'*/
    switch(*s++) {
    case 'v':
      if      (*s=='-') { verbose=0; s++; }
      else if (*s=='+') { verbose=1; s++; }
      else verbose=1;
      break;
    case 'a':                          /*load requested alphabet*/
      s=loadalpha(s);                  /*..get pointer after alphabet*/
      if (!s) return 0;                /*..if NULL, error*/
      break;
    case 'C':                          /*force alphabetics to upper case*/
      if      (*s=='-') { s++; forcecase=0; }
      else if (*s=='+') { s++; forcecase=1; upper=1; }
      else { forcecase=1; upper=1; }
      break;
    case 'c':                          /*force alphabetics to lower case*/
      if      (*s=='-') { s++; forcecase=0; }
      else if (*s=='+') { s++; forcecase=1; upper=0; }
      else { forcecase=1; upper=0; }
      break;
    case 'L':
      if ((sscanf(s,"%u",&forcelen)!=1)||(forcelen>=MAX_MAXLEN)) {
        printf("Illegal value \"%s\" for forced key length.\n");
        return 0;
      }
      while ((*s>='0')&&(*s<='9')) s++;  /*skip data*/
      break;
    case 'l':
      if (sscanf(s,"%u",&maxlen)!=1) {
        printf("illegal value \"%s\" for maximum length search.\n",s);
        return 0;
      }
      break;
    case 'p':                          /*load probable list*/
      s=loadprob(s);                   /*..get pointer after problist*/
      if (!s) return 0;                /*..if NULL, error*/
      break;
    default:
      s--;
      s--;
      printf("unknown switch \"%s\"\n",s);
      return 0;
    }   /*switch*/
  }   /*while*/
  return 1;
}   /*switches*/


/*--------------------------------------------------------------------------*/
byte cvtinput(byte c)
{
  if (forcecase) {                     /*see if forcing case*/
    if (upper)                         /*..if forcing to upper case*/
      return UPCASE(c);                /*....set to upper case*/
    else                               /*..otherwise..*/
      return LOCASE(c);                /*....must force to lower*/
  }   /*see if forcing alpha case*/
  return c;                            /*just return the char*/
}   /*cvtinput*/


/*--------------------------------------------------------------------------*/
int  loadinput(char *fn)
{
  FILE *f;
  byte c;

  f=fopen(fn,"rb");                    /*open input file*/
  if (!f) return 0;                    /*oops! not there*/
  ibn=0;
  while (fread(&c,1,1,f) && (ibn<sizeof(ib))) { /*not EOF and not EOB*/
    c=cvtinput(c);                              /*convert the input char*/
    if (index[c]!=NO_CHAR) ib[ibn++]=c;         /*only include alphabet*/
  }   /*while*/
  fclose(f);
  return ibn;
}   /*loadinput*/


/*--------------------------------------------------------------------------*/
word keylength(void)
{
  word l,i,j,cnt;
  word maxcnt=0;
  word len=0;

  len=0;                               /*reset the global key length*/
  for (l=1;l<=maxlen;l++) {            /*for each key length*/
    cnt=0;                             /*reset the count of matches*/
    for (i=0,j=l;i<ibn;i++,j++) {      /*go through each buffer position*/
      if (j>=ibn) j=0;                 /*reset j*/
      if (ib[i]==ib[j]) cnt++;         /*increment count*/
    }   /*for each buffer position*/
    if (verbose) printf(";  Correlation count for length %3u = %5u\n",l,cnt);
    if (cnt>maxcnt) {                  /*if we have a new max..*/
      len=l;                           /*set keylength to this one*/
      maxcnt=cnt;                      /*set new max*/
    }   /*if new max*/
  }   /*for each key length*/
  if (verbose) printf(";  Most likely key length is %u.\n",len);

  return len;
}   /*keylength*/


/*--------------------------------------------------------------------------*/
void sorter(word w[],word n,word ndx[])  /*could be a LOT better...*/
{
  word i;
  word t;

  for (i=0;i<n;i++) ndx[i]=i;          /*initialize sort index table*/
  if (!n) return;
  i=0;
  while (i<(n-1)) {
    if (w[ndx[i]]>=w[ndx[i+1]])
      i++;                             /*..mark last nonswap*/
    else {                             /*need to swap..*/
      t=ndx[i];                        /*..swap*/
      ndx[i]=ndx[i+1];
      ndx[i+1]=t;
      if (i) i--; else i++;            /*..check backwards since swapped*/
    }
  }   /*while*/
}   /*sorter*/


/*--------------------------------------------------------------------------*/
void gensubfreqs(void)
{
  word sub,i,j;
  byte c;
  word sort[0x100];                    /*sort index*/
  word keys[0x100];                    /*possible key characters*/

  depth=MIN(MAX_DEPTH,probn);          /*depth of search, not too long*/
  if (verbose) printf("Generating subtext frequencies...\n");
  memset(subfreq,0,sizeof(subfreq));   /*clear all subfrequencies to 0*/
  for (i=0,sub=0;i<ibn;i++) {          /*go through each input char*/
    subfreq[sub++][index[ib[i]]]++;    /*increment subtext's count for char*/
    if (sub>=keylen) sub=0;            /*reset subtext index*/
  }   /*for*/

  if (verbose) {
    printf(";=====");
    for (sub=0;sub<keylen;sub++) printf("====",sub);
    printf("\n");

    printf(";Sub: ");
    for (sub=0;sub<keylen;sub++) printf(" %3u",sub);
    printf("\n");
    printf(";Char:");
    for (sub=0;sub<keylen;sub++) printf(" ---");
    printf("\n");

    for (i=0;i<alphan;i++) {
      if (isprint(alpha[i]))
        printf("; %c%3u",alpha[i],i);
      else
        printf(";%02X%3u",alpha[i],i);
      for (sub=0;sub<keylen;sub++) printf(" %3u",subfreq[sub][i]);
      printf("\n");
    }   /*for each char in alphabet*/

    printf(";=====");
    for (sub=0;sub<keylen;sub++) printf("====",sub);
    printf("\n");
  }   /*if printing list*/

  printf("Most likely key characters are:\n");
  for (sub=0;sub<keylen;sub++) {
    sorter(subfreq[sub],alphan,sort);  /*generate sorted list*/
    if (verbose) {
      printf(";----------------------------------------------------------\n");
      printf(";Subtext %3u info:\n",sub);
      printf(";  Sorted chars: \"");
      for (i=0;i<MIN(16,alphan);i++) {
        c=alpha[sort[i]];
        if (isprint(c)) printf("%c",c); else printf("\\x%02X",c);
      }   /*for*/
      printf("\"\n");
    }   /*if verbose*/

    memset(keys,0,sizeof(keys));       /*clear possible key table*/
    memset(most,0,sizeof(most));       /*reset lists*/
    for (i=0;i<depth;i++) {            /*calculate differences of..*/
      for (j=0;j<depth;j++) {          /*..plaintext from cyphertext*/
        c=(alphan+sort[i]-index[prob[j]])%alphan;
        most[i][j]=c;                  /*..hold copy for verbose*/
        keys[c]++;                     /*..increment count*/
      }   /*for j*/
    }   /*for i*/
    if (verbose) {
      printf(";  Differences between plaintext and cyphertext...\n");
      for (i=0;i<depth;i++) {
        c=alpha[sort[i]];
        if (isprint(c)) printf(";  %c =\"",c); else printf(";  %02X=\"",c);
        for (j=0;j<depth;j++) {
          c=alpha[most[i][j]];
          if (isprint(c)) printf("%c",c); else printf("\\x%02X",c);
        }   /*for j*/
        printf("\"\n");
      }   /*for i*/
    }   /*if verbose*/

    sorter(keys,alphan,sort);          /*get most occurring key chars*/
    printf("Key[%3u]: \"",sub);
    for (i=0;(i<depth)&&(keys[sort[i]]);i++) {
      c=alpha[sort[i]];
      likely[sub][i]=c;
      if (isprint(c)) printf("%c",c); else printf("\\x%02X",c);
    }   /*for*/
    key[sub]=likely[sub][0];
    printf("\"\n");
  }   /*for each subtext*/

  printf("\nMost likely key: \"");
  for (i=0;i<keylen;i++) {
    c=key[i];
    if (isprint(c)) printf("%c",c); else printf("\\x%02X",c);
  }   /*for each key char*/
  printf("\"\n");
}   /*gensubfreqs*/



