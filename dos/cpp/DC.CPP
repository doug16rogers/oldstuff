#include <stdiostr.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdarg.h>
#include <math.h>
#include <string.h>


#include "string.hpp"


enum token_value {
  NAME,
  NUMBER,
  END,
  POWER,
  SHL,
  SHR,
  ROL,
  ROR,
  HELP='?',
  COMMA=',',
  PLUS='+',
  MINUS='-',
  AND='&',
  OR='|',
  XOR='^',
  COMP='~',
  MUL='*',
  DIV='/',
  MOD='%',
  PRINT=';',
  ASSIGN='=',
  FACTORIAL='!',
  LP='(',
  RP=')'
};   //tokens


typedef double (*FUNC)(double);
typedef unsigned long ulong;


struct name {
  char* string;
  name* next;
  FUNC  func;
  double value;
};

const int TBLSZ=0x43;

token_value curr_tok;
double      number_value;
char        name_string[80];
name*       table[TBLSZ];
int         error_count=0;


double termpow(void);                  //exponentiation
double term(void);
double expr(void);
double prim(void);
token_value get_token(void);
name*  look(char* p,int ins=0);
inline name*  insert(char* s) { return look(s,1); }


char scrap[80];


//===========================================================================
double error(char* fmt,...)
{
  va_list va;

  va_start(va,fmt);
  vsprintf(scrap,fmt,va);
  va_end(va);

  cout << "ERROR: " << scrap << ".\n";
  error_count++;
  return 1.0;
}   //error(char*,...)


//===========================================================================
ulong RotLeft(ulong l,int cnt)
{
  cnt&=0x1f;
  int c=((l&0x80000000UL)!=0);
  for (int i=0;i<cnt;i++) {
    l<<=1;
    if (c) l|=1;
    c=((l&0x80000000UL)!=0);
  }   //for each rotation
  return l;
}   //RotLeft


//===========================================================================
ulong RotRight(ulong l,int cnt)
{
  cnt&=0x1f;
  int c=(int)l&1;
  for (int i=0;i<cnt;i++) {
    l>>=1;
    if (c) l|=0x80000000UL;
    c=(int)l&1;
  }   //for each rotation
  return l;
}   //RotRight


//===========================================================================
double bits(double d)
{
  ulong v=d;
  unsigned c=0;

  for (int i=0;i<32;i++) {
    c += (unsigned)(v & 1);
    v>>=1;
  }
  return (double)c;
}   //bits


//===========================================================================
double factor(double d)
{
  ulong v=d;

  if (v<=3) return v;
  if ((v&1)==0) return 2.0;
  ulong top=sqrt(v);
  ulong p=3;
  while (p<=top) {
    if ((v%p)==0) return (double)p;
    p+=2;
  }
  return (double)v;
}   //factor(double)


//===========================================================================
double setwidth(double d)
{
  name* n=look("width");
  unsigned w=d;
  cout.width(w);
  n->value=(double)cout.width();
  return (double)n->value;
}   //setwidth(double)


//===========================================================================
double setprec(double d)
{
  name* n=look("prec");
  unsigned p=d;
  cout.precision(p);
  n->value=(double)cout.precision();
  return (double)n->value;
}   //setprec(double)


//===========================================================================
double expr(void)                      //expression (add/sub)
{
  double left=term();
  for (;;) {
    switch (curr_tok) {
    case PLUS:
      get_token();                     //eat the '+'
      left += term();                  //add in new term
      break;
    case MINUS:
      get_token();                     //eat the '-'
      left -= term();                  //subtract off the new term
      break;
    case AND:
      get_token();
      left = (ulong)left & (ulong)term();
      break;
    case OR:
      get_token();
      left = (ulong)left | (ulong)term();
      break;
    case XOR:
      get_token();
      left = (ulong)left ^ (ulong)term();
      break;
    case SHL:
      get_token();
      left = (ulong)left << (int)term();
      break;
    case SHR:
      get_token();
      left = (ulong)left >> (int)term();
      break;
    case ROL:
      get_token();
      left = RotLeft((ulong)left,(int)term());
      break;
    case ROR:
      get_token();
      left = RotRight((ulong)left,(int)term());
      break;
    default:
      return left;                     //all done!
    }   //switch
  }   //for ever
}   //expr(void)


//===========================================================================
double term(void)                      //term (mul/div)
{
  double left=termpow();               //get first primitive or power
  for (;;) {
    switch (curr_tok) {
    case MUL:
      get_token();                     //eat the '*'
      left *= termpow();
      break;
    case DIV:
      get_token();                     //eat the '-'
      double d=termpow();
      if (d==0) return error("divide by 0");
      left /= d;
      break;
    case MOD:
      get_token();
      left = (ulong)left % (ulong)termpow();
      break;
    default:
      return left;
    }   //switch
  }   //for ever
}   //term(void)


//===========================================================================
double termpow(void)                   //term (exponentiation)
{
  double left=prim();                  //get first primitive
  for (;;) {
    switch (curr_tok) {
    case POWER:
      get_token();
      left = pow(left,prim());
      break;
    case FACTORIAL:
      get_token();
      if (left<0.0) return error("can't take factorial of negative");
      unsigned top=left;
      left=1.0;
      while (top) { left *= (double)top; top--; }
      break;
    default:
      return left;
    }   //switch
  }   //for ever
}   //termpow(void)


//===========================================================================
double prim(void)
{
  name* n;
  switch (curr_tok) {
  case NUMBER:
    get_token();
    return number_value;

  case NAME:
    get_token();
    switch (curr_tok) {
    case ASSIGN:                       //assigning value to a name
      n=insert(name_string);
      get_token();
      n->value=expr();
      return n->value;
    case LP:                           //calling the named function
      n=look(name_string,0);
      get_token();                     //eat (
      if (!n->func) return error("\"%s\" not a function",n->string);
      n->value=(n->func)(expr());
      if (curr_tok!=RP) return error(") expected for \"%s\"",n->string);
      get_token();                     //eat )
      break;
    default:
      n=look(name_string);
    }   //switch for names
    return n->value;

  case MINUS:                          //unary -
    get_token();
    return -prim();

  case PLUS:                           //unary +
    get_token();
    return prim();

  case COMP:                           //unary bitwise complement
    get_token();
    return ~(ulong)prim();

  case LP:
    get_token();
    double e=expr();
    if (curr_tok != RP) return error(") expected");
    get_token();
    return e;

  case END:
    return 1.0;
  default:
    return error("number/name/function expected");
  }   //switch
}   //prim(void)


//===========================================================================
token_value get_token(void)
{
  char ch;

  do {
    if (!cin.get(ch)) return curr_tok=END;
  } while ((ch!='\n') && isspace(ch));

  switch (ch) {
    case ';':
    case '\n':
      return curr_tok=PRINT;
    case '*':
      cin.get(ch);
      if (ch=='*') return curr_tok=POWER;
      cin.putback(ch);
      return curr_tok=(token_value)'*';
    case '<':
      cin.get(ch);
      if (ch=='<') {
        cin.get(ch);
        if (ch=='<') return curr_tok=ROL;
        cin.putback(ch);
        return curr_tok=SHL;
      }
      cin.putback(ch);
      error("bad token at '<'");
      return curr_tok=PRINT;
    case '>':
      cin.get(ch);
      if (ch=='>') {
        cin.get(ch);
        if (ch=='>') return curr_tok=ROR;
        cin.putback(ch);
        return curr_tok=SHR;
      }
      cin.putback(ch);
      error("bad token at '>'");
      return curr_tok=PRINT;
    case '%':
    case '/':
    case '+':
    case '-':
    case '(':
    case ')':
    case '=':
    case '&':
    case '|':
    case '^':
    case '~':
    case ',':
    case '!':
    case '?':
      return curr_tok=(token_value)ch;
    case '0':
      cin.get(ch);
      if (ch=='x') {
        ulong k=0;
        cin >> hex >> k >> dec;
        number_value=k;
        return curr_tok=NUMBER;
      }
      cin.putback(ch);
      ch='0';
    case '1': case '2': case '3': case '4':
    case '5': case '6': case '7': case '8': case '9':
    case '.':
      cin.putback(ch);
      cin >> number_value;
      return curr_tok=NUMBER;
    default:
      if ((ch=='_')||isalpha(ch)) {
        char* p=name_string;
        *p++ = ch;
        while (cin.get(ch) && (isalnum(ch)||(ch=='_'))) *p++ = ch;
        cin.putback(ch);
        *p=0;
        return curr_tok=NAME;
      }
      error("bad token at '%c'",ch);
      return curr_tok=PRINT;
  }   //switch
}   //get_token(void)


//===========================================================================
name*  look(char* p,int ins)
{
  int ii=0;
  char* pp=p;

  while (*pp) ii = (ii<<1) ^ *pp++;    //calculate hashing value for string
  if (ii<0) ii=-ii;
  ii %= TBLSZ;

  for (name* n=table[ii]; n; n=n->next)
    if (strcmp(p,n->string)==0) return n;

  if (ins==0) error("name \"%s\" not found",p);

  name* nn=new name;                   //make room for new entry
  nn->string=new char[strlen(p)+1];
  strcpy(nn->string,p);
  nn->value=1.0;
  nn->func=NULL;
  nn->next=table[ii];                  //insert at start of that hash table
  table[ii]=nn;
  return nn;
}   //look(char*)


//===========================================================================
int  help(void)
{
  cout
    << "dc -- algebraic desk calculator.\n"
    << "Algebraic operators:  + - * / % ** ! ( )\n"
    << "Bitwise operators:    | & ^ ~ << >> <<< >>>\n"
    << "Predefined symbols:   pi e\n"
    << "Functions available:  abs acos asin atan ceil cos cosh exp floor\n"
    << "                      log log10 sin sinh sqrt tan tanh\n"
    << "Integer functions:    bits factor\n"
    << "Symbols may be defined (via =) and used.\n"
    << "The symbol \"_\" is set to the most recently printed value.\n"
    << "Enter hex by preceding with 0x.  Format functions:  width prec\n"
    << "Separate commands with ','.  Print with ';' or '\\n'.\n"
    << "Use from commandline by supplying a single argument.\n"
    << "Use from standard input by omitting the arguments.\n"
    << "EXAMPLE: h=0x17;a=pi/6;x=h*sin(a);y=h*cos(a);H=sqrt(x**2+y**2);\n"
    << "Hit ^Z to quit.\n"
  ;
  return 1;
}   //help(void)


//===========================================================================
int  main(int argc,char* argv[])
{
  String arguments;

  insert("pi")->value=M_PI;
  insert("e")->value=M_E;

  insert("abs")->func=fabs;
  insert("acos")->func=acos;
  insert("asin")->func=asin;
  insert("atan")->func=atan;
  insert("ceil")->func=ceil;
  insert("cos")->func=cos;
  insert("cosh")->func=cosh;
  insert("exp")->func=exp;
  insert("floor")->func=floor;
  insert("log")->func=log;
  insert("log10")->func=log10;
  insert("sin")->func=sin;
  insert("sinh")->func=sinh;
  insert("sqrt")->func=sqrt;
  insert("tan")->func=tan;
  insert("tanh")->func=tanh;

  insert("bits")->func=bits;
  insert("factor")->func=factor;

  insert("width")->func=setwidth;
  look("width")->value=(double)cout.width();
  insert("prec")->func=setprec;
  look("prec")->value=(double)cout.precision();

  if (argc == 1)
    help();
  else
  {
    if (argv[1][0]=='?') return help();
    for (int i = 1; (i < argc); i++)
    {
      arguments += ' ';
      arguments += argv[i];
    }
    cin=*new istream(arguments.Length(),arguments.S());
  }   //switch

  double e=0.0;
  insert("_")->value=e;
  while (cin && (curr_tok!=END)) {
    get_token();
    switch (curr_tok) {
    case END:                          /*skip any duplicate tokens*/
    case PRINT:
    case COMMA:
      break;
    case HELP:
      help();
      break;
    default:
      e=expr();
      if (curr_tok==COMMA) break;
      if (e>(double)(0xFFFFFFFFUL))
        cout << e << " <0x0>\n";
      else
        cout << e << " (0x" << hex << (ulong)e << dec << ")\n";
      look("_")->value=e;
      cout.width((int)look("width")->value);
      cout.precision((int)look("prec")->value);
    }   //switch
  }   //while
  if ((argc>1)&&error_count) help();
  return error_count;
}   //main(int,char**)


