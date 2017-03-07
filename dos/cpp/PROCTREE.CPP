#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


#include "symutil.hpp"


#define IS_EXTERN  0x0001
#define IS_PUBLIC  0x0002
#define IS_CALLED  0x0004


typedef char FName[40];


typedef struct _Proc {
  char name[80];
  char file[40];
  WORD flag;
  int  line;
  struct _Call {
    struct _Proc* proc;
    struct _Call* next;
  } *call;
} Proc;

char *functype[]={ "STATIC", "EXTERN", "PUBLIC", "PUBLIC" };


class ProcTable : public SymbolTable {
public:
  Proc* Insert(Proc* p) { return (Proc*)SymbolTable::Insert(p); }
  Proc* Find(Proc* p) { return (Proc*)SymbolTable::Find(p); }
  Proc* First(void) { return (Proc*)SymbolTable::First(); }
  Proc* Next(void) { return (Proc*)SymbolTable::Next(); }
  Proc* Last(void) { return (Proc*)SymbolTable::Last(); }
  Proc* Prev(void) { return (Proc*)SymbolTable::Prev(); }
  Proc* Curr(void) { return (Proc*)SymbolTable::Curr(); }

  ProcTable(int _size=SIZE,CompareFunc _comp=(CompareFunc)stricmp) :
    SymbolTable(_size,_comp) { return; }
};   //class ProcTable


int ProcComp(Proc* p1,Proc* p2)
{
  int cmp=stricmp(p1->name,p2->name);
  if (cmp) return cmp;
  return stricmp(p1->file,p2->file);
}   //ProcComp


int InsertOrder(void* str1,void* str2)
{
  if (stricmp((char*)str1,(char*)str2)==0) return 0;
  return 1;
}   //InsertOrder


ProcTable proc(sizeof(Proc)/*,(CompareFunc)ProcComp*/);
SymbolTable exclude(sizeof(FName));
SymbolTable files(sizeof(FName));


char* strNcpy(char* d,char* s,int n)
{
  if (n<=0) n=0x7FFF; else n--;
  while (n-- && *s) *d++ = *s++;
  *d=0;
  return d;
}   //strNcpy


enum KEYWORD_ENUMS {
  KEY_NONE=-1,
  KEY_AND=0,
  KEY_BEGIN,
  KEY_BYTE,
  KEY_CALL,
  KEY_DECLARE,
  KEY_DO,
  KEY_ELSE,
  KEY_END,
  KEY_EXTERNAL,
  KEY_IF,
  KEY_INPUT,
  KEY_INWORD,
  KEY_LITERALLY,
  KEY_OR,
  KEY_OUTPUT,
  KEY_OUTWORD,
  KEY_POINTER,
  KEY_PROCEDURE,
  KEY_PUBLIC,
  KEY_THEN,
  KEY_WHILE,
  KEY_WORD
};

char* keyword[]={
  "AND",
  "BEGIN",
  "BYTE",
  "CALL",
  "DECLARE",
  "DO",
  "ELSE",
  "END",
  "EXTERNAL",
  "IF",
  "INPUT",
  "INWORD",
  "LITERALLY",
  "OR",
  "OUTPUT",
  "OUTWORD",
  "POINTER",
  "PROCEDURE",
  "PUBLIC",
  "THEN",
  "WHILE",
  "WORD",
};
#define KEYWORDS   (sizeof(keyword)/sizeof(keyword[0]))


enum {
  UNKNOWN=-1,
  NOTHING=0,
  END_OF_FILE=1,
  CHARACTER,
  STRING,
  NUMBER,
  KEYWORD,
  NAME
};


FILE* File;
char  FileName[0x80];
int   FileLine=1;

char tok_string[0x100]="\0";
int curr_tok=0;
int tok_line=0;
int tok_keyword=0;
int tok_len=0;
char default_ext[8]=".PLM";

int  isAlpha(char c) { return isalpha(c)||(c=='_'); }
int  isAlnum(char c) { return isalnum(c)||(c=='_'); }



int  FindInOrderedList(char* list[],int n,char *targ)
{
  int tp,bp,p,newp;
  int cmp;

  if (!n) return n;
  bp=0;
  tp=n-1;
  p=-1;
  newp=(bp+tp)/2;
  while (1) {
    if (p==newp) break; else p=newp;
    cmp=stricmp(targ,list[p]);
    if (cmp==0) return p;
    if (cmp<0) {
      tp=p;
      newp=(bp+tp+1)/2;
    }
    else {
      bp=p;
      newp=(bp+tp)/2;
    }
  }
  return n;
}   //FindInOrderedList


#define RECALL_SIZE 0x80
class GetUnget {
private:
  int recall[RECALL_SIZE];
  int cur;
  int chr;
  int (*func)(void);
public:
  void Flush(void) { cur=0; chr=0; }
  void UnGet(int c) { chr=c; if (cur<RECALL_SIZE) recall[cur++]=chr; }
  int  Get(void) { if (cur) return chr=recall[--cur]; else return chr=func(); }
  int  Chr(void) { return chr; }

  int operator * (void) { return chr; }

  GetUnget(int (*_func)(void)=getchar) { func=_func; Flush(); return; }
};   //class GetUnget


int  get_char(void)
{
  int i=fgetc(File);
  if (i==EOF) i=0;
  if (i=='\n') FileLine++;
  return i;
}   //get_char


GetUnget GetChar(get_char);


int  get_token(void)
{
  int i=0;

  curr_tok=NOTHING;
  while (curr_tok==NOTHING) {
    i=0;
    while (isspace(GetChar.Get()));
    if (!*GetChar) return curr_tok=END_OF_FILE;

    tok_keyword=KEY_NONE;
    tok_string[i++]=*GetChar;

    switch (*GetChar) {
    case '/':
      GetChar.Get();
      if (*GetChar!='*') {
        GetChar.UnGet(*GetChar);
        curr_tok=CHARACTER;
      }
      else {
        int comment_line=FileLine;
        do {
          while (GetChar.Get()) if (*GetChar=='*') break;
          while (GetChar.Get()=='*') ;
        } while (*GetChar && (*GetChar!='/'));
        if (!*GetChar) {
          printf("Line %u: Unterminated comment.\n",comment_line);
          return curr_tok=END_OF_FILE;
        }   //if end of file
      }   //else we have a comment
      break;
    case '\'':
      curr_tok=STRING;
      i=0;
      while (1) {
        while (GetChar.Get()&&(*GetChar!='\'')&&(*GetChar != '\n'))
          tok_string[i++]=*GetChar;
        if (*GetChar!='\'') {
          printf("Line %d: unterminated string.\n",FileLine);
          break;
        }
        if (GetChar.Get()!='\'') break;
        tok_string[i++]=*GetChar;
      }   //while
      break;
    case '0': case '1': case '2': case '3': case '4':
    case '5': case '6': case '7': case '8': case '9':
      curr_tok=NUMBER;
      while (isxdigit(GetChar.Get())) tok_string[i++]=*GetChar;
      switch (*GetChar) {
      case 'h': case 'H':
        tok_string[i++]=*GetChar;
        break;
      default:
        GetChar.UnGet(*GetChar);
      }   //switch
      break;
    default:
      if (isAlpha(tok_string[0])) {
        while (isAlnum(GetChar.Get())) tok_string[i++]=*GetChar;
        curr_tok=NAME;
        GetChar.UnGet(*GetChar);
      }
      else if (isprint(tok_string[0]))
        curr_tok=CHARACTER;
      else
        curr_tok=UNKNOWN;
    }   //switch
  }   //while
  tok_string[i]=0;
  tok_line=FileLine;
  tok_len=i;
  if (curr_tok==NAME) {
    i=FindInOrderedList(keyword,KEYWORDS,tok_string);
    if (i<KEYWORDS) {
      tok_keyword=i;
      curr_tok=KEYWORD;
    }
  }
  return curr_tok;
}   //get_token


char* tok_name(int tok)
{
  switch (tok) {
  case UNKNOWN:     return "<<UNKNOWN>>";
  case END_OF_FILE: return "EOF";
  case CHARACTER:   return "CHR";
  case STRING:      return "STR";
  case NUMBER:      return "NUM";
  case KEYWORD:     return "KEY";
  case NAME:        return "NAM";
  }   //switch
  return "Ooops!";
}   //tok_name


int  strmatch(char *m,char *s)
{
#define WILD_ONE       '?'             /*single-char wildcard match*/
#define WILD_ALL       '*'             /*variable length wildcard match*/
  enum { MUST_MATCH, SEEK_MATCH, PART_MATCH };  /*states for match-checking*/
  char *curm;                          /*current pointer to match str*/
  char *curs;                          /*current pointer to target string*/
  char state=MUST_MATCH;               /*start checking the match*/

  curm=m;                              /*start at same place*/
  curs=s;
  while (1) {                          /*go 'til end of match*/
    switch(state) {                    /*depends on state*/
    case MUST_MATCH:                   /*if normal char, must match*/
      if (*curm==WILD_ALL) {           /*check for variable wild char*/
        while (*curm==WILD_ALL) curm++;  /*go to next char to match*/
        if (*curm==0) return 1;        /*nothing left to match, so quit*/
        state=SEEK_MATCH;              /*look for next section of match str*/
        break;
      }   /*if wild-all found*/
      if (!*curm) {                    /*no more to match; done, 1way|other*/
        if (*curs) return 0; else return 1;   /*if extra, bad; if not, match*/
      }   /*if end of match string*/
      if (!*curs) return 0;            /*found end of line*/
      if ((*curm!=WILD_ONE)&&(*curm!=*curs)) return 0;  /*no match!*/
      curm++;                          /*point to next match character*/
      curs++;                          /*point to next target character*/
      break;
    case SEEK_MATCH:                   /*looking for match*/
      if (!*curs) return 0;            /*no chars left for match*/
      if ((*curm==WILD_ONE)||(*curm==*curs)) {   /*see if matched*/
        curs++;                        /*point to next target position*/
        s=curs;                        /*point to target position*/
        m=curm;                        /*point to match position*/
        m++;                           /*point to next match position*/
        state=PART_MATCH;              /*we're on to one!*/
        break;
      }   /*if match started*/
      curs++;                          /*try next position*/
      break;
    case PART_MATCH:                   /*started matching after WILD_ALL*/
      if (!*m) {                       /*no more to match; done, 1way|other*/
        if (*s) return 0; else return 1;   /*if extra, bad; if not, match*/
      }   /*if end of match string*/
      if (*m==WILD_ALL) {              /*if up to next WILD_ALL*/
        curm=m;                        /*skip up to now*/
        curs=s;                        /*skip up to now on target, too*/
        state=MUST_MATCH;              /*great, now go back to start*/
        break;
      }   /*if end of this section of must-match*/
      if (!*s) return 0;               /*match mask not done but we are, bad*/
      if ((*m!=WILD_ONE)&&(*m!=*s)) {  /*not aligned -- bad*/
        state=SEEK_MATCH;              /*go back and scan for a match*/
        break;
      }   /*if*/
      m++;                             /*looks good, go to next char*/
      s++;                             /*in target, too*/
      break;
    default:                           /*should never reach here*/
      return 0;                        /*..so get out*/
    }   /*switch*/
  }   /*while forever*/
}   /*strmatch*/


int  load_proctable(void)
{
  Proc p;
  Proc* pp;
  int n=0;

  strNcpy(p.file,FileName,sizeof(p.file));
  FileLine=1;
  File=fopen(FileName,"rt");
  if (!File) return 0;

  get_token();
  while (curr_tok!=END_OF_FILE) {
    switch (curr_tok) {
    case UNKNOWN:
    case END_OF_FILE:
    case CHARACTER:
    case STRING:
    case KEYWORD:
      get_token();
      break;
    case NAME:
      strNcpy(p.name,tok_string,sizeof(p.name));
      p.line=tok_line;
      p.flag=0;
      p.call=NULL;
      get_token();
      if (tok_string[0]==':') {
        get_token();
        if (tok_keyword==KEY_PROCEDURE) {
          do {
            get_token();
            if (tok_keyword==KEY_EXTERNAL) p.flag|=IS_EXTERN;
            if (tok_keyword==KEY_PUBLIC)   p.flag|=IS_PUBLIC;
          } while ((curr_tok!=END_OF_FILE)&&(tok_string[0]!=';'));
          pp=proc.Find(&p);
          if (!pp) {
            proc.Insert(&p);
            n++;
            break;
          }
          if ((pp->flag & IS_PUBLIC)&&(p.flag & IS_PUBLIC)) {
            printf("File %s, %d: public proc \"%s\" defined twice.\n",
              p.file,p.line,p.name);
            break;
          }   //if duplicate public
          pp->flag |= p.flag;          //add our new attribute
          if (p.flag & IS_PUBLIC) {    //PUBLICs win over EXTERNALs
            strNcpy(pp->file,p.file,sizeof(p.file));
            pp->line=p.line;
          }
        }   //if proc
      }   //if name:
      break;
    default:
      get_token();
    }   //switch
  }   //while !eof

  fclose(File);
  return n;
}   //load_proctable


void load_calls(void)
{
  Proc* pp;
  Proc* call;
  struct _Call *callptr;
  Proc p;

  FileLine=1;
  File=fopen(FileName,"rt");
  if (!File) return;

  strNcpy(p.file,FileName,sizeof(p.file));

  get_token();
  while (curr_tok!=END_OF_FILE) {
    if (curr_tok!=NAME) { get_token(); continue; }
    strNcpy(p.name,tok_string,sizeof(p.name));
    p.line=FileLine;
    get_token();
    if (tok_string[0]!=':') continue;
    get_token();
    if (tok_keyword!=KEY_PROCEDURE) continue;

    pp=proc.Find(&p);
    if (!pp) continue;

    if ((pp->line != p.line) || stricmp(pp->file,p.file)) continue;

    do {
      get_token();
    } while ((curr_tok!=END_OF_FILE)&&(tok_string[0]!=';'));
    if (curr_tok==END_OF_FILE) break;

    do {
      get_token();
      if (tok_keyword==KEY_END) {
        get_token();
        if (stricmp(tok_string,pp->name)==0) break;
      }

      if (curr_tok!=NAME) continue;

      call=proc.Find((Proc*)tok_string);
      if (call) {
        if (pp->call==NULL) {
          pp->call=new struct _Call;
          if (pp->call==NULL) break;
          callptr=pp->call;
        }
        else {
          callptr->next=new struct _Call;
          if (callptr->next==NULL) break;
          callptr=callptr->next;
        }
        callptr->proc=call;
        callptr->next=NULL;
      }   //yes!! this is a call
    } while (curr_tok!=END_OF_FILE);
  }   //while !eof

  fclose(File);
}   //load_calls



#if 0
#define MAX_DEPTH  0x08
char *callproc[MAX_DEPTH];

void rec_show(Proc* p,int depth)
{
  if (!p) return;

  int i;
  struct _Call *call;

  if (depth) {
    for (i=0;i<(depth-1);i++) printf("    "); printf(" +->");
  }
  printf("%s   (%s,%u) %s",p->name,p->file,p->line,functype[p->flag&3]);
  if ((depth+1)>=MAX_DEPTH) { printf("  MAX DEPTH\n"); return; }

  callproc[depth]=p->name;
  for (i=0;i<depth;i++) if (stricmp(callproc[i],p->name)==0) break;
  if (i<depth) { printf("  [recurse]\n"); return; }
  printf("\n");

  for (call=p->call;call;call=call->next)
    rec_show(call->proc,depth+1);
}   //rec_show


#else  //????????????????????????????????????????????????????????????????????

void showtree(Proc* p)
{
#define MAX_DEPTH  0x08
  char *callproc[MAX_DEPTH];
  struct _Call *call[MAX_DEPTH];
  int depth=0;
  int i;

  if (!p) return;

  printf("\n%s   (%s,%u) %s ******\n",
    p->name,
    p->file,
    p->line,
    functype[p->flag&3]
  );


  callproc[0]=p->name;
  call[0]=p->call;
  depth=0;
  while (depth>=0) {
    if (call[depth]==NULL) { depth--; continue; }
    p=call[depth]->proc;
    call[depth]=call[depth]->next;

    for (i=0;i<depth;i++) printf(" |  "); printf(" |\n");
    for (i=0;i<depth;i++) printf(" |  "); printf(" +--");
    printf("%s   (%s,%u) %s",p->name,p->file,p->line,functype[p->flag&3]);

    if (p->call==NULL)        { printf("\n");            continue; }
    if ((depth+1)>=MAX_DEPTH) { printf("  MAX DEPTH\n"); continue; }

    for (i=0;i<=depth;i++) if (stricmp(callproc[i],p->name)==0) break;
    if (i<=depth)             { printf("  [recurse]\n"); continue; }

    printf("\n");
    depth++;
    call[depth]=p->call;
    callproc[depth]=p->name;
  }   //while
}   //showtree
#endif //????????????????????????????????????????????????????????????????????


enum {
  switch_ok=0,
  not_a_switch,
  switch_error
};

int  loadswitch(char* s)
{
  if ((*s!='-')&&(*s!='/')) return not_a_switch;
  s++;
  switch (*s++) {
  case 'x':
    strupr(s);
    exclude.Insert(s);
    break;
  default:
    s--;
    s--;
    printf("unknown switch \"%s\".\n",s);
    return switch_error;
  }   //switch
  return switch_ok;
}   //loadswitch


int  help(int k)
{
  printf(
    "\n"
    "Usage: proctree [switches] <file-list>\n"
    "More than one file may be listed, and wildcards may be used.\n"
    "Switches:\n"
    "  -x<filespec>    exclude file (possibly with wildcards)\n"
  );   //printf
  return k;
}   //help



int  main(int argc,char* argv[])
{
  int first;
  int i;
  char *f;

  for (first=1;first<argc;first++) {
    i=loadswitch(argv[first]);
    if (i==not_a_switch) break;
    if (i==switch_error) return help(1);
  }   //for

  if (first>=argc) return help(1);
  for (i=first;i<argc;i++) {
    strNcpy(FileName,argv[i],sizeof(FileName)-4);
    if (!strchr(argv[i],'.')) strcat(FileName,default_ext);
    for (f=(char*)exclude.First();f;f=(char*)exclude.Next())
      if (strmatch(f,FileName)) break;
    if (f) continue;                   //found an excluded file
    files.Insert(FileName);
  }   //for

  printf(
    "=================================================================\n");

  for (f=(char*)files.First();f;f=(char*)files.Next()) {
    strNcpy(FileName,(char*)f,sizeof(FileName));
    printf("parsing %s...",FileName);
    printf("%u procs.\n",load_proctable());
  }   //for

  printf("%d procedures found in %d files:\n",proc.Count(),argc-first);
  printf(
    "-----------------------------------------------------------------\n"
    "File                 Line Type   Procedure\n"
    "-------------------- ---- ------ --------------------------------\n"
  );
  for (Proc* p=proc.First();p;p=proc.Next()) {
    printf(
      "%-16s %4u %6s %-s\n"
      ,p->file,p->line
      ,functype[p->flag & 3]
      ,p->name
    );
  }   //for
  printf("-----------------------------------------------------------------\n");

  printf("Generating call lists:\n");
  for (i=first;i<argc;i++) {
    strcpy(FileName,argv[i]);
    if (!strchr(FileName,'.')) strcat(FileName,".PLM");
    printf("File %s\n",FileName);
    load_calls();
  }   //for

  printf("-----------------------------------------------------------------\n");

  struct _Call *call;
  for (p=proc.First();p;p=proc.Next()) {
    printf("%s\n",p->name);
    for (call=p->call;call;call=call->next) printf("  %s\n",call->proc->name);
  }   //for

  printf("-----------------------------------------------------------------\n");

  for (p=proc.First();p;p=proc.Next()) showtree(p);
}   //main

