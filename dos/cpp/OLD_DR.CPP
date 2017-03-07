#include <stdio.h>
#include <dir.h>
#include <dos.h>
#include <io.h>
#include <string.h>

#include "drstr.hpp"
#include "symutil.hpp"


typedef struct {
  char name[10];
  char ext[4];
  char* path;
  long size;
  int  year;
  BYTE mon;
  BYTE day;
  BYTE hour;
  BYTE min;
  BYTE sec;
  BYTE attr;
} File;

int  FileCompare(File* f1,File* f2);

class FileTable : public SymbolTable {
protected:
  long size;
public:
  File* Find(File* p) { return (File*)SymbolTable::Find(p); }
  File* Insert(File* p) {
    if (!Find(p)) size += p->size;
    return (File*)SymbolTable::Insert(p);
  }
  File* First(void) { return (File*)SymbolTable::First(); }
  File* Next(void) { return (File*)SymbolTable::Next(); }
  File* Last(void) { return (File*)SymbolTable::Last(); }
  File* Prev(void) { return (File*)SymbolTable::Prev(); }
  File* Curr(void) { return (File*)SymbolTable::Curr(); }

  FileTable(int _size=sizeof(File),CompareFunc _comp=(CompareFunc)FileCompare)
    : SymbolTable(_size,_comp) { size=0L; return; }

  long  Size(void) { return size; }
  File* operator [](int i);

};   //class FileTable

File* FileTable::operator [](int i)
{
  if ((i<0)||(i>=count)) return NULL;
  File* f=First();
  while (i && (f != NULL)) f=Next();
  return f;
}   //FileTable::operator [](int)


class PathTable : public SymbolTable {
public:
  char* Find(char* p) { return (char*)SymbolTable::Find(p); }
  char* Insert(char* p) { return (char*)SymbolTable::Insert(p); }
  char* First(void) { return (char*)SymbolTable::First(); }
  char* Next(void) { return (char*)SymbolTable::Next(); }
  char* Last(void) { return (char*)SymbolTable::Last(); }
  char* Prev(void) { return (char*)SymbolTable::Prev(); }
  char* Curr(void) { return (char*)SymbolTable::Curr(); }

  PathTable(int _size=sizeof(File),CompareFunc _comp=(CompareFunc)stricmp)
    : SymbolTable(_size,_comp) { return; }
};   //class FileTable


#define DEFAULT_FILES  "*.*"

#define LOWERCASE      1
#define FORMAT         "%-8n.%-3e_%7s_%d_%t_%a_%p"
#define ORDER          "nepdts"
#define ATTR_FMT       "%-d%-r%-a%-h"
#define DATE_FMT       "%4Y.%3M.%02d"
#define TIME_FMT       "%2H:%02m:%02s"
#define TOTAL          1
#define VERBOSE        0

char lowercase=LOWERCASE;
char format[0x80]=FORMAT;
char order[0x80]=ORDER;
char attr_fmt[0x80]=ATTR_FMT;
char date_fmt[0x80]=DATE_FMT;
char time_fmt[0x80]=TIME_FMT;
char total=TOTAL;
char verbose=VERBOSE;
int  args=0;
FileTable Files;
FileTable Excludes;
PathTable Paths;

char* lowercase_str[] = { "C", "c" };
char* minus_plus[] = { "-", "+" };
char* month[] = {
  "???", "Jan", "Feb", "Mar", "Apr", "May", "Jun",
         "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};   //month


//===========================================================================
int  FileCompare(File* f1,File* f2)
{
  int cmp=0;

  for (int i=0; (cmp == 0) && order[i]; i++) {
    switch (order[i]) {
    case 'n':
      cmp = strcmp(f1->name,f2->name);
      break;
    case 'e':
      cmp = strcmp(f1->ext,f2->ext);
      break;
    case 'p':
      cmp = strcmp(f1->path,f2->path);
      break;
    case 'd':
      cmp = (f1->year - f2->year);
      if (cmp) break;
      cmp = (f1->mon - f2->mon);
      if (cmp) break;
      cmp = (f1->day - f2->day);
      break;
    case 't':
      cmp = (f1->hour - f2->hour);
      if (cmp) break;
      cmp = (f1->min - f2->min);
      if (cmp) break;
      cmp = (f1->sec - f2->sec);
      break;
    case 'f':
      cmp = stricmp(f1->name,f2->name);
      if (cmp) break;
      cmp = stricmp(f1->ext,f2->ext);
      break;
    case 's':
      if      (f1->size < f2->size) cmp = -1;
      else if (f1->size > f2->size) cmp = +1;
      break;

    case 'N':
      cmp = -strcmp(f1->name,f2->name);
      break;
    case 'E':
      cmp = -strcmp(f1->ext,f2->ext);
      break;
    case 'D':
      cmp = -(f1->year - f2->year);
      if (cmp) break;
      cmp = -(f1->mon - f2->mon);
      if (cmp) break;
      cmp = -(f1->day - f2->day);
      break;
    case 'T':
      cmp = -(f1->hour - f2->hour);
      if (cmp) break;
      cmp = -(f1->min - f2->min);
      if (cmp) break;
      cmp = -(f1->sec - f2->sec);
      break;
    case 'F':
      cmp = -stricmp(f1->name,f2->name);
      if (cmp) break;
      cmp = -stricmp(f1->ext,f2->ext);
      break;
    case 'S':
      if      (f1->size < f2->size) cmp = +1;
      else if (f1->size > f2->size) cmp = -1;
      break;
    }
  }   //for
  return cmp;
}   //FileCompare


//===========================================================================
void FillOutOrder(char* ord)
{
  char p=0,n=0,e=0,s=0,d=0,t=0;
  while (*ord) {
    switch (*ord++) {
    case 'p': case 'P':  p=1; break;
    case 'n': case 'N':  n=1; break;
    case 'e': case 'E':  e=1; break;
    case 'f': case 'F':  n=1; e=1; break;
    case 'd': case 'D':  d=1; break;
    case 't': case 'T':  t=1; break;
    case 's': case 'S':  s=1; break;
    }   //switch
  }   //while
  if (!n) *ord++ = 'n';
  if (!e) *ord++ = 'e';
  if (!p) *ord++ = 'p';
  if (!d) *ord++ = 'd';
  if (!t) *ord++ = 't';
  if (!s) *ord++ = 's';
  *ord = 0;
}   //FillOutOrder


//===========================================================================
void DateToString(char* d,int year,int mon,int day,char* fmt)
{
  int field_size=0;
  char neg;
  char zero;
  char c;

  while (*fmt) {
    switch (c=*fmt++) {
    case '_': *d++ = ' '; break;
    case '%':
      if (*fmt == '%') { *d++ = '%'; break; }
      field_size = 0;
      neg = 0;
      zero = 0;
      if (*fmt == '-') { neg  = 1; fmt++; }
      if (*fmt == '0') { zero = 1; fmt++; }
      while ((*fmt >= '0') && (*fmt <= '9')) {
        field_size *= 10;
        field_size += (*fmt++ - '0');
      }
      if (neg) field_size = -field_size;
      switch (*fmt++) {
      case 'y':
        if (zero)
          sprintf(d,"%0*u",field_size,year%100);
        else
          sprintf(d,"%*u",field_size,year%100);
        while (*d) d++;
        break;
      case 'Y':
        if (zero)
          sprintf(d,"%0*u",field_size,year);
        else
          sprintf(d,"%*u",field_size,year);
        while (*d) d++;
        break;
      case 'm':
        if (zero)
          sprintf(d,"%0*u",field_size,mon);
        else
          sprintf(d,"%*u",field_size,mon);
        while (*d) d++;
        break;
      case 'M':
        sprintf(d,"%*s",field_size,month[mon]);
        while (*d) d++;
        break;
      case 'd':
        if (zero)
          sprintf(d,"%0*u",field_size,day);
        else
          sprintf(d,"%*u",field_size,day);
        while (*d) d++;
        break;
      default:
        *d++ = *fmt;
      }   //switch on format
      break;
    default:
      *d++ = c;
    }   //switch
  }   //while
  *d=0;
}   //DateToString


//===========================================================================
void TimeToString(char* d,int hour,int min,int sec,char* fmt)
{
  int field_size=0;
  char neg;
  char zero;
  char c;

  while (*fmt) {
    switch (c=*fmt++) {
    case '_': *d++ = ' '; break;
    case '%':
      if (*fmt == '%') { *d++ = '%'; break; }
      field_size = 0;
      neg = 0;
      zero = 0;
      if (*fmt == '-') { neg  = 1; fmt++; }
      if (*fmt == '0') { zero = 1; fmt++; }
      while ((*fmt >= '0') && (*fmt <= '9')) {
        field_size *= 10;
        field_size += (*fmt++ - '0');
      }
      if (neg) field_size = -field_size;
      switch (*fmt++) {
      case 'H':
        if (zero)
          sprintf(d,"%0*u",field_size,hour);
        else
          sprintf(d,"%*u",field_size,hour);
        while (*d) d++;
        break;
      case 'h':
        int h = hour;
        if (h > 12) h -= 12;
        if (h == 0) h  = 12;
        if (zero)
          sprintf(d,"%0*u",field_size,h);
        else
          sprintf(d,"%*u",field_size,h);
        while (*d) d++;
        break;
      case 'm':
        if (zero)
          sprintf(d,"%0*u",field_size,min);
        else
          sprintf(d,"%*u",field_size,min);
        while (*d) d++;
        break;
      case 's':
        if (zero)
          sprintf(d,"%0*u",field_size,sec);
        else
          sprintf(d,"%*u",field_size,sec);
        while (*d) d++;
        break;
      case 'a':
      case 'p':
        if (hour < 12) *d++ = 'a'; else *d++ = 'p';
        break;
      case 'A':
      case 'P':
        if (hour < 12) *d++ = 'A'; else *d++ = 'P';
        break;
      default:
        *d++ = *fmt;
      }   //switch on format
      break;
    default:
      *d++ = c;
    }   //switch
  }   //while
  *d=0;
}   //TimeToString


//===========================================================================
void AttrToString(char* d,BYTE attr,char* fmt)
{
  char ac;

  while (*fmt) {
    switch (*fmt) {
    case '_': *d++ = ' '; break;
    case '%':
      fmt++;
      if (*fmt == '%') { *d++ = '%'; break; }
      ac = 0;
      if (strchr("rRaAhHdDsSlL",*fmt) == NULL) {
        if (*fmt == '_') ac = ' '; else ac = *fmt;
        fmt++;
      }
      switch (*fmt) {
      case 'd': if (attr & FA_DIREC ) ac = 'd'; break;
      case 'r': if (attr & FA_RDONLY) ac = 'r'; break;
      case 'a': if (attr & FA_ARCH  ) ac = 'a'; break;
      case 'h': if (attr & FA_HIDDEN) ac = 'h'; break;
      case 's': if (attr & FA_SYSTEM) ac = 's'; break;
      case 'l': if (attr & FA_LABEL ) ac = 'l'; break;
      case 'D': if (attr & FA_DIREC ) ac = 'D'; break;
      case 'R': if (attr & FA_RDONLY) ac = 'R'; break;
      case 'A': if (attr & FA_ARCH  ) ac = 'A'; break;
      case 'H': if (attr & FA_HIDDEN) ac = 'H'; break;
      case 'S': if (attr & FA_SYSTEM) ac = 'S'; break;
      case 'L': if (attr & FA_LABEL ) ac = 'L'; break;
      case 0: fmt--; break;
      }   //switch
      if (ac) *d++ = ac;
      break;
    default:
      *d++ = *fmt;
    }   //switch
    fmt++;
  }   //while
  *d=0;
}   //AttrToString


//===========================================================================
void ShowToString(char* d,File* f,char* fmt)
{
  char c;
  char neg;
  int field_size=0;
  char str[0x80];

  while (*fmt) {
    switch(c=*fmt++) {
    case '%':
      if (*fmt == '%') { printf("%%"); break; }
      field_size = 0;
      neg = 0;
      if (*fmt == '-') { neg = 1; fmt++; }
      while ((*fmt >= '0') && (*fmt <= '9')) {
        field_size *= 10;
        field_size += (*fmt - '0');
        fmt++;
      }
      if (neg) field_size = -field_size;
      switch (*fmt++) {
      case 0:
        *d++ = '%';
        break;
      case 'p':
        sprintf(d,"%*s",field_size,f->path);
        while (*d) d++;
        break;
      case 'n':
        strtocstr(str, sizeof(str), f->name, strlen(f->name));
        sprintf(d,"%*s",field_size,str);
        while (*d) d++;
        break;
      case 'e':
        strtocstr(str, sizeof(str), f->ext, strlen(f->ext));
        sprintf(d,"%*s",field_size,str);
        while (*d) d++;
        break;
      case 'f':
        strtocstr(str, sizeof(str), f->name, strlen(f->name));
        int len = strlen(str);
        if (len < sizeof(str)) str[len++] = '.';
        if (len < sizeof(str))
          strtocstr(&str[len], (sizeof(str) - len), f->ext, strlen(f->ext));
        sprintf(d,"%*s",field_size,str);
        while (*d) d++;
        break;
      case 's':
        sprintf(d,"%*lu",field_size,f->size);
        while (*d) d++;
        break;
      case 'a':
        AttrToString(str,f->attr,attr_fmt);
        sprintf(d,"%*s",field_size,str);
        while (*d) d++;
        break;
      case 'd':
        DateToString(str,f->year,f->mon,f->day,date_fmt);
        sprintf(d,"%*s",field_size,str);
        while (*d) d++;
        break;
      case 't':
        TimeToString(str,f->hour,f->min,f->sec,time_fmt);
        sprintf(d,"%*s",field_size,str);
        while (*d) d++;
        break;
      }   //switch on format type
      break;
    case '_':
      *d++ = ' ';
      break;
    default:
      *d++ = c;
    }   //switch
  }   //while
  *d = 0;
}   //ShowToString


//===========================================================================
void ToFile(File* f,struct ffblk* ff,char* path)
{
  int i;
  char *s;

  f->path = path;
  f->attr = ff->ff_attrib;
  f->size = ff->ff_fsize;
  f->year = ((struct ftime*)&ff->ff_ftime)->ft_year + 1980;
  f->mon  = ((struct ftime*)&ff->ff_ftime)->ft_month;
  f->day  = ((struct ftime*)&ff->ff_ftime)->ft_day;
  f->hour = ((struct ftime*)&ff->ff_ftime)->ft_hour;
  f->min  = ((struct ftime*)&ff->ff_ftime)->ft_min;
  f->sec  = ((struct ftime*)&ff->ff_ftime)->ft_tsec << 1;

  f->name[0] = 0;
  f->ext[0] = 0;
  s = ff->ff_name;
  if (f->attr & FA_DIREC) {
    if      (strcmp(s,"." ) == 0) strcpy(f->name,"." );
    else if (strcmp(s,"..") == 0) strcpy(f->name,"..");
  }
  if (f->name[0] == 0) {
    for (i=0;(i<8) && *s && (*s != '.');i++) f->name[i] = *s++;
    f->name[i] = 0;
    s++;
    for (i=0;(i<3) && *s;i++) f->ext[i] = *s++;
    f->ext[i] = 0;
  }
//  if (lowercase) { strlwr(f->name);  strlwr(f->ext); }
  if ((f->attr & FA_DIREC) == 0) {
    strlwr(f->name);
    strlwr(f->ext);
  }
}   //ToFile


//===========================================================================
int  LoadFileSpecs(FileTable& Table,char* s)
{
  struct ffblk ff;
  File file;
  int done;
  char drv[MAXDRIVE];
  char dir[MAXDIR];
  char nam[MAXFILE];
  char ext[MAXEXT];
  char path[0x80]="";
  char* path_ptr;
  int  flags;

  flags = fnsplit(s,drv,dir,nam,ext);
  if (flags & DRIVE)     strcat(path,drv);
  if (flags & DIRECTORY) strcat(path,dir);
  path_ptr = Paths.Insert(path);
  if (verbose) {
    printf(
      "argument:  \"%s\"\n"
      "  flags:     drv%c dir%c nam%c ext%c wildcards%c\n"
      "  drive:     \"%s\"\n"
      "  directory: \"%s\"\n"
      "  name:      \"%s\"\n"
      "  extension: \"%s\"\n"
      ,s
      ,(flags & DRIVE)     ? '+' : '-'
      ,(flags & DIRECTORY) ? '+' : '-'
      ,(flags & FILENAME)  ? '+' : '-'
      ,(flags & EXTENSION) ? '+' : '-'
      ,(flags & WILDCARDS) ? '+' : '-'
      ,drv,dir,nam,ext
    );
  }   //if verbose

  done=findfirst(s,&ff,0xFF);
  while (!done) {
    ToFile(&file,&ff,path_ptr);
    if (Excludes.Find(&file) == NULL) Table.Insert(&file);
    done = findnext(&ff);
  }
  return 1;
}   //LoadFileSpecs


//===========================================================================
int  LoadFile(char* s)
{
  if ((*s=='-')||(*s=='/')) return 1;
  args++;
  return LoadFileSpecs(Files,s);
}   //LoadFile


//===========================================================================
int  LoadFlag(char* str,char* flag,char* name)
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
}   //LoadFlag


//===========================================================================
int  LoadSwitches(char* s)
{
  if ((*s!='-')&&(*s!='/')) return 1;
  s++;
  switch (*s++) {
  case '?':
  case 'h':
  case 'H':
    return 0;
  case 'c': lowercase=1; break;
  case 'C': lowercase=0; break;
  case 'd': strNcpy(date_fmt,s,sizeof(date_fmt)); break;
  case 't': strNcpy(time_fmt,s,sizeof(time_fmt)); break;
  case 'a': strNcpy(attr_fmt,s,sizeof(attr_fmt)); break;
  case 'f': strNcpy(format,s,sizeof(format)); break;
  case 's':
    strNcpy(order,s,sizeof(order));
    FillOutOrder(order);
    break;
  case 'x':
    if (verbose) printf("loading exclude files...\n");
    LoadFileSpecs(Excludes,s);
    break;
  case 'T': if (!LoadFlag(s,&total,  "total"  )) return 0; else break;
  case 'v': if (!LoadFlag(s,&verbose,"verbose")) return 0; else break;
  default:
    s--;
    printf("unknown switch \"%s\"\n",s);
    return 0;
  }   //switch
  return 1;
}   //LoadSwitches


//===========================================================================
int  Help(int k)
{
  printf(
    "\n"
    "Usage: dr [switches] [list-of-files]\n"
    "Switches [default]:\n"
    "  -f<format>  output format (\"%%[-][0][<#>]<item>\"): [%s]\n"
    "                p=file path    n=file name  e=file extension\n"
    "                f=name+ext     s=file size  a=file attributes\n"
    "                d=file date    dt=file time\n"
    "                Use '_' for a space.\n"
    "                See also: -d, -t and -a switches\n"
    "  -d<format>  date format:  Y=YYYY y=YY m=MM M=Mon d=DD [%s]\n"
    "  -t<format>  time format:  H=24 h=12 m=MM s=S a=a/p(am/pm) [%s]\n"
    "  -a<format>  attribute format (\"%%[<altchar>]<item>\"): [%s]\n"
    "                r=read only    a=archive     h=hidden\n"
    "                d=directory    s=system      l=volume label\n"
    "  -s<order>   sort order (n,e,f,s,a,t,d), uppercase=reverse [%s]\n"
    "  -T[+/-]     display totals [%s]\n"
    "  -x<file>    file(s) to exclude\n"
//    "  -c/C        use lower/upper case [%s]\n"
//    "  -v[+/-]     verbose output [%s]\n"
    ,FORMAT
    ,DATE_FMT
    ,TIME_FMT
    ,ATTR_FMT
    ,ORDER
    ,minus_plus[TOTAL!=0]
//    ,lowercase_str[LOWERCASE!=0]
//    ,minus_plus[VERBOSE!=0]
  );   //printf
  return k;
}   //Help



//===========================================================================
int  main(int argc,char* argv[])
{
  int i;

  for (i=1;i<argc;i++) if (!LoadSwitches(argv[i])) return Help(1);
  if (verbose) {
    printf(
      "order for sort: \"%s\"\n"
      "full format:    \"%s\"\n"
      "date format:    \"%s\"\n"
      "time format:    \"%s\"\n"
      "attr format:    \"%s\"\n"
      ,order
      ,format
      ,date_fmt
      ,time_fmt
      ,attr_fmt
    );
  }

  if (verbose) printf("loading filespecs...\n");
  for (i=1;i<argc;i++) if (!LoadFile(argv[i])) return Help(1);

  if (args==0) {
    if (verbose) printf("no filespecs, using \"%s\"...\n",DEFAULT_FILES);
    LoadFileSpecs(Files,DEFAULT_FILES);
  }

  char outstring[0x80];
  for (File* f=Files.First();f!=NULL;f=Files.Next()) {
    ShowToString(outstring,f,format);
    printf("%s\n",outstring);
  }
  if (total) printf("  Files=%u  Bytes=%lu\n",Files.Count(),Files.Size());
  return 0;
}   //main

