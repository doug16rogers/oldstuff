#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <dos.h>


char calfile[80]="\0";

typedef struct {
  int year;
  int mon;
  int day;
  int wday;
  int hour;
  int min;
  int sec;
} DATE_TIME;

typedef struct {
  int tok;
  int val;
} TOKEN;

enum TOKENS {
  NUM,
//  MONTH,
//  WEEKDAY,
//  SPACE,
  SLASH,
  COL,
  DOT,
  END,
  UNKNOWN
};

#define WILD       -1
#define NOW        -2

char* tokens[]={
  "NUM","MONTH","WEEKDAY","SPACE", "SLASH",
  "COL","DOT","END","UNKNOWN"
};

char* months[12]={
  "JAN","FEB","MAR","APR","MAY","JUN",
  "JUL","AUG","SEP","OCT","NOV","DEC"
};
char* days[7]={ "SUN","MON","TUE","WED","THU","FRI","SAT" };


DATE_TIME CurrentTime;


//===========================================================================
int  GetTime(DATE_TIME* t) {
  static unsigned char BCD2byt[160]={
     0, 1, 2, 3, 4,  5, 6, 7, 8, 9, 0,0,0,0,0,0,
    10,11,12,13,14, 15,16,17,18,19, 0,0,0,0,0,0,
    20,21,22,23,24, 25,26,27,28,29, 0,0,0,0,0,0,
    30,31,32,33,34, 35,36,37,38,39, 0,0,0,0,0,0,
    40,41,42,43,44, 45,46,47,48,49, 0,0,0,0,0,0,
    50,51,52,53,54, 55,56,57,58,59, 0,0,0,0,0,0,
    60,61,62,63,64, 65,66,67,68,69, 0,0,0,0,0,0,
    70,71,72,73,74, 75,76,77,78,79, 0,0,0,0,0,0,
    80,81,82,83,84, 85,86,87,88,89, 0,0,0,0,0,0,
    90,91,92,93,94, 95,96,97,98,99, 0,0,0,0,0,0
  };
  unsigned char ch,cl,dh,dl;

  _AH=0x04;
  asm int 0x1A;
  asm jc  badclock;
  geninterrupt(0x1A);
  ch=_CH;
  cl=_CL;
  dh=_DH;
  dl=_DL;
  t->day=BCD2byt[dl]-1;
  t->mon=BCD2byt[dh]-1;
  t->year=BCD2byt[cl]+1900;
  if (ch!=0x19) t->year+=100;

  _AH=0x02;
  geninterrupt(0x1A);
  ch=_CH;
  cl=_CL;
  dh=_DH;
  dl=_DL;
  t->hour=BCD2byt[ch&0x3F];
  t->min=BCD2byt[cl];
  t->sec=BCD2byt[dh];

t->wday=1;

  return 1;
badclock:
  return 0;
}   //GetTime


//===========================================================================
int  findmon(char* s,int* m) {
  static char t[4]="JAN";
  int i;

  t[0]=toupper(s[0]);
  t[1]=toupper(s[1]);
  t[2]=toupper(s[2]);
  for (i=0;i<12;i++) {
    if (strcmp(months[i],t)==0) {
      *m=i;
      return 3;
    }
  }
  return 0;
}   //findmon


//===========================================================================
int  findday(char* s,int* m) {
  static char t[4]="SUN";
  int i;

  t[0]=toupper(s[0]);
  t[1]=toupper(s[1]);
  t[2]=toupper(s[2]);
  for (i=0;i<12;i++) {
    if (strcmp(days[i],t)==0) {
      *m=i;
      return 3;
    }
  }
  return 0;
}   //findday


//===========================================================================
int  tokenize(char* s,TOKEN tok[],int maxn) {     //tokenize a string
  int n=0;
  int i=0;
  int j;

  if (maxn) maxn--; else maxn=3000;
  while (s[i] && isspace(s[i])) i++;
  while ((n<maxn) && s[i]) {
    while (s[i] && isspace(s[i])) i++;
    tok[n].val=0;
    switch (s[i]) {
    case ':': tok[n++].tok=COL; break;
    case '.': tok[n++].tok=DOT;   break;
    case '/': tok[n++].tok=SLASH; break;
    case '*':
      tok[n].tok=NUM;
      tok[n].val=WILD;
      n++;
      while (s[i] && (s[i]=='*')) i++;
      continue;
    case '@':
      tok[n].tok=NUM;
      tok[n].val=NOW;
      n++;
      while (s[i] && (s[i]=='@')) i++;
      continue;
    default:
  //case NUM:
      if (isdigit(s[i])) {
        tok[n].val=s[i++]-'0';
        while (s[i] && isdigit(s[i])) {
          tok[n].val*=10;
          tok[n].val+=s[i++]-'0';
        }
        tok[n++].tok=NUM;
        continue;
      }
  //case MONTH:
      if ((j=findmon(&s[i],&tok[n].val))!=0) {
        s+=j;
        tok[n++].tok=NUM;
        continue;
      }
  //case WEEKDAY:
      if ((j=findday(&s[i],&tok[n].val))!=0) {
        s+=j;
        tok[n++].tok=NUM;
        continue;
      }
  //case UNKNOWN:
      tok[n].tok=UNKNOWN;
      tok[n].val=s[i];
      n++;
    }   //switch
    i++;
  }

  tok[n].tok=END;
  tok[n].val=0;
  return i;
}   //tokenize


//===========================================================================
void wildtime(DATE_TIME* d) {
  d->year = WILD;
  d->mon  = WILD;
  d->day  = WILD;
  d->wday = WILD;
  d->hour = WILD;
  d->min  = WILD;
  d->sec  = WILD;
}   //wildtime


//===========================================================================
void nowtime(DATE_TIME* d) {
  d->year = NOW;
  d->mon  = NOW;
  d->day  = NOW;
  d->wday = NOW;
  d->hour = NOW;
  d->min  = NOW;
  d->sec  = NOW;
}   //nowtime


//===========================================================================
int  str2int(char* s,int* n) {
  int i;
  int k=0;
  for (i=0;s[i]&&isspace(s[i]);i++);
  if (s[i]=='*') { *n=WILD; return i+1; }
  if (!isdigit(s[i])) return 0;
  while (isdigit(s[i])) {
    k*=10;
    k+=s[i++]-'0';
  }
  return i;
}   //str2int


//===========================================================================
int  loaddate(TOKEN tok[],DATE_TIME* d) {
  int tok0,tok1,tok2,tok3,tok4;
  tok0=tok[0].tok;
  tok1=tok[1].tok;
  tok2=tok[2].tok;
  tok3=tok[3].tok;
  tok4=tok[4].tok;
  if ((tok0==NUM)&&(tok1==DOT)&&(tok2==NUM)&&(tok3==DOT)) {
    d->year=tok[0].val;
    d->mon=tok[2].val;
    if (tok4==SLASH) return 5;                               /* #.#./  */
    if (tok4!=NUM) return 4;                                 /* #.#.   */
    d->day=tok[4].val;                                       /* #.#.#  */
    if (tok[5].tok==SLASH) return 6;                         /* #.#.#/ */
    return 5;
  }
  if ((tok0==NUM)&&(tok1==DOT)&&(tok2==DOT)) {
    d->year=tok[0].val;
    if (tok3==SLASH) return 4;                               /* #../  */
    if (tok3!=NUM) return 3;                                 /* #..   */
    d->day=tok[3].val;                                       /* #..#  */
    if (tok4==SLASH) return 4;                               /* #..#/ */
    return 3;
  }
  if ((tok0==DOT)&&(tok1==NUM)&&(tok2==DOT)) {
    d->mon=tok[1].val;
    if (tok3==SLASH) return 4;                               /* .#./  */
    if (tok3!=NUM) return 3;                                 /* .#.   */
    d->day=tok[3].val;                                       /* .#.#  */
    if (tok4==SLASH) return 5;                               /* .#.#/ */
    return 3;
  }
  if ((tok0==DOT)&&(tok1==DOT)) {
    if (tok2==SLASH) return 3;                               /* ../   */
    if (tok2!=NUM) return 2;                                 /* ..    */
    d->day=tok[2].val;                                       /* ..#   */
    if (tok3==SLASH) return 4;                               /* ..#/  */
    return 3;
  }
  if ((tok0==NUM)&&(tok1==DOT)) {
    d->mon=tok[0].val;
    if (tok2==SLASH) return 3;                               /* #./   */
    if (tok2!=NUM) return 2;                                 /* #.    */
    d->day=tok[2].val;                                       /* #.#   */
    if (tok3==SLASH) return 4;                               /* #.#/  */
    return 2;
  }
  if (tok0==DOT) {
    if (tok1==SLASH) return 2;                               /* ./    */
    if (tok1!=NUM) return 1;                                 /* .     */
    d->day=tok[1].val;                                       /* .#    */
    if (tok2==SLASH) return 3;                               /* .#/   */
    return 2;
  }
  return 0;
}   //loaddate


//===========================================================================
int  loadwday(TOKEN tok[],DATE_TIME* d) {
  int tok0,tok1;
  tok0=tok[0].tok;
  tok1=tok[1].tok;
  if ((tok0==NUM)&&(tok1==SLASH)) {
    d->wday=tok[0].val;
    return 2;
  }
  if (tok0==SLASH) return 1;
  return 0;
}   //loadwday


//===========================================================================
int  loadtime(TOKEN tok[],DATE_TIME* d) {
  int tok0,tok1,tok2,tok3,tok4;
  tok0=tok[0].tok;
  tok1=tok[1].tok;
  tok2=tok[2].tok;
  tok3=tok[3].tok;
  tok4=tok[4].tok;
  if ((tok0==NUM)&&(tok1==COL)&&(tok2==NUM)&&(tok3==COL)) {
    d->hour=tok[0].val;
    d->min=tok[2].val;
    if (tok4!=NUM) return 4;                                 /* #:#:   */
    d->sec=tok[4].val;                                       /* #:#:#  */
    return 5;
  }
  if ((tok0==NUM)&&(tok1==COL)&&(tok2==COL)) {
    d->hour=tok[0].val;
    if (tok3!=NUM) return 3;                                 /* #::   */
    d->sec=tok[3].val;                                       /* #::#  */
    return 3;
  }
  if ((tok0==COL)&&(tok1==NUM)&&(tok2==COL)) {
    d->min=tok[1].val;
    if (tok3!=NUM) return 3;                                 /* :#:   */
    d->sec=tok[3].val;                                       /* :#:#  */
    return 3;
  }
  if ((tok0==COL)&&(tok1==COL)) {
    if (tok2!=NUM) return 2;                                 /* ::    */
    d->sec=tok[2].val;                                       /* ::#   */
    return 3;
  }
  if ((tok0==NUM)&&(tok1==COL)) {
    d->hour=tok[0].val;
    if (tok2!=NUM) return 2;                                 /* #:    */
    d->min=tok[2].val;                                       /* #:#   */
    return 2;
  }
  if (tok0==COL) {
    if (tok1!=NUM) return 1;                                 /* :     */
    d->min=tok[1].val;                                       /* :#    */
    return 2;
  }
  return 0;
}   //loadtime


//===========================================================================
int  str2time(char* s,DATE_TIME* t) {
  int n=0;
  TOKEN tok[40];
  DATE_TIME d;

  d=*t;
  while (*s && isspace(*s)) s++;       //skip initial white space
  tokenize(s,tok,sizeof(tok)/sizeof(tok[0]));
  n=0;
  n+=loaddate(&tok[n],&d);
  n+=loadwday(&tok[n],&d);
  n+=loadtime(&tok[n],&d);
  if (d.mon>=12)  return 0;
  if (d.day>=31)  return 0;
  if (d.wday>=7)  return 0;
  if (d.hour>=24) return 0;
  if (d.min>=60)  return 0;
  if (d.sec>=60)  return 0;
  if (d.year==NOW) d.year=CurrentTime.year;
  if (d.mon ==NOW) d.mon =CurrentTime.mon;
  if (d.day ==NOW) d.day =CurrentTime.day;
  if (d.wday==NOW) d.wday=CurrentTime.wday;
  if (d.hour==NOW) d.hour=CurrentTime.hour;
  if (d.min ==NOW) d.min =CurrentTime.min;
  if (d.sec ==NOW) d.sec =CurrentTime.sec;
  *t=d;
  return 1;
}   //str2date


//===========================================================================
char* yearstr(int year) {
  static char Year[5]="****";

  if (year==NOW) year=CurrentTime.year;
  if (year==WILD) strcpy(Year,"****"); else sprintf(Year,"%04u",year);
  return Year;
}   //yearstr


//===========================================================================
char* monstr(int mon) {
  static char Mon[5]="***";

  if (mon==NOW) mon=CurrentTime.mon;
  if (mon==WILD) strcpy(Mon,"***"); else sprintf(Mon,"%s",months[mon]);
  return Mon;
}   //monstr


//===========================================================================
char* daystr(int day) {
  static char Day[5]="**";

  if (day==NOW) day=CurrentTime.day;
  if (day==WILD) strcpy(Day,"**"); else sprintf(Day,"%02u",day+1);
  return Day;
}   //daystr


//===========================================================================
char* wdaystr(int wday) {
  static char Wday[5]="***";

  if (wday==NOW) wday=CurrentTime.wday;
  if (wday==WILD) strcpy(Wday,"***"); else sprintf(Wday,"%3s",days[wday]);
  return Wday;
}   //wdaystr


//===========================================================================
char* hourstr(int hour) {
  static char Hour[5]="**";

  if (hour==NOW) hour=CurrentTime.hour;
  if (hour==WILD) strcpy(Hour,"**"); else sprintf(Hour,"%02u",hour);
  return Hour;
}   //hourstr


//===========================================================================
char* minstr(int min) {
  static char Min[5]="**";

  if (min==NOW) min=CurrentTime.min;
  if (min==WILD) strcpy(Min,"**"); else sprintf(Min,"%02u",min);
  return Min;
}   //minstr


//===========================================================================
char* secstr(int sec) {
  static char Sec[5]="**";

  if (sec==NOW) sec=CurrentTime.sec;
  if (sec==WILD) strcpy(Sec,"**"); else sprintf(Sec,"%02u",sec);
  return Sec;
}   //secstr


//===========================================================================
int  time2str(DATE_TIME* t,char* s) {
  sprintf(&s[ 0],"%4s.",yearstr(t->year));
  sprintf(&s[ 5],"%3s.",monstr(t->mon));
  sprintf(&s[ 9],"%2s/",daystr(t->day));
  sprintf(&s[12],"%3s/",wdaystr(t->wday));
  sprintf(&s[16],"%2s:",hourstr(t->hour));
  sprintf(&s[20],"%2s:",minstr(t->min));
  sprintf(&s[24],"%2s" ,secstr(t->sec));
  return 1;
}   //time2str


//===========================================================================
int  help(int k) {
  printf(
    "\n"
    "cal  --  calendar check program\n"
    "Usage: cal <cal-file>\n"
    "\n"
  );
  return k;
}   //help


//===========================================================================
int  loadarg(char* s) {
  if (*s=='?') return 0;
  if ((*s!='-')&&(*s!='/')) {
    if (calfile[0]) {
      printf("Two filenames supplied (\"%s\").\n",s);
      return 0;
    }
    strcpy(calfile,s);
    return 1;
  }
  s++;
  switch (*s++) {
  case '?':
    return 0;
  default:
    s--;
    printf("unknown switch \"%s\".\n",s);
    return 0;
  }   //switch
  return 1;
}   //loadarg


//===========================================================================
int  main(int argc,char* argv[]) {
  int i;
  char s[0x80];
  DATE_TIME t;
  TOKEN tok[80];

  GetTime(&CurrentTime);
  printf(
    "====================================\n"
    "%04u.%s.%02u /%s/ %02u:%02u:%02u\n"
    ,CurrentTime.year
    ,months[CurrentTime.mon]
    ,CurrentTime.day
    ,days[CurrentTime.wday]
    ,CurrentTime.hour
    ,CurrentTime.min
    ,CurrentTime.sec
  );   //printf
  for (i=1;i<argc;i++) if (!loadarg(argv[i])) return help(1);
  if (!calfile[0]) {
    printf("No calendar file supplied.\n");
    return help(1);
  }

  printf("calfile was \"%s\".\n",calfile);
  while (1) {
    printf("String to convert: ");
    gets(s);
    if (s[0]=='q') break;
    tokenize(s,tok,sizeof(tok)/sizeof(tok[0]));
    i=0;
    do {
      printf("%3u: %s(%d)\n",i,tokens[tok[i].tok],tok[i].val);
    } while (tok[i++].tok!=END);
    str2time(s,&t);
    printf(
      "%04u.%s.%02u /%s/ %02u:%02u:%02u\n"
      ,t.year
      ,months[t.mon]
      ,t.day
      ,days[t.wday]
      ,t.hour
      ,t.min
      ,t.sec
    );   //printf
  }   //while
  return 0;
}   //main


