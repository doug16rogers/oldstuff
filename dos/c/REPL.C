#include <stdio.h>
#include <string.h>
#include <ctype.h>


#define CASE       1
#define BACKUP     1
#define BACKUP_EXT "BAK"
#define NO_REPLACEMENT 0
#define VERBOSE    0

char case_=CASE;
char backup=BACKUP;
char backup_ext[8]=BACKUP_EXT;
char verbose=VERBOSE;

#define STRING_SIZE    0x400

unsigned char oldstr[STRING_SIZE];
unsigned char newstr[STRING_SIZE];
unsigned char bufstr[STRING_SIZE];
int  oldcnt=0;
int  newcnt=0;
int  repls=0;

char old_loaded=0;
char new_loaded=0;
char bak_name[0x100];

char no_replacement=NO_REPLACEMENT;

int  skip[0x100];

char *minus_plus[] = { "-","+" };


//===========================================================================
int digval(char c)
{
#define TOP 0x7F
  if (c< '0') return TOP;
  if (c<='9') return c-'0';
  if (c< 'A') return TOP;
  if (c<='Z') return c-'A'+10;
  if (c< 'a') return TOP;
  if (c<='z') return c-'a'+10;
  return TOP;
}   //digval


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


//===========================================================================
int  help(int k)
{
  printf(
    "\n"
    "Usage: repl [options] <old-string> <new-string> <files...>\n"
    "Strings may use quotes ('\"') and escape characters ('\\').\n"
    "Standard C conversion is performed on replacement strings:\n"
    "  \\a = BEL 07    \\v = VT  0B    \\\\    = \\   5C\n"
    "  \\b = BS  08    \\f = FF  0C    \\OOO  = up to three octals (0-7)\n"
    "  \\t = TAB 09    \\r = CR  0D    \\xHHH = up to three hexes (0-9,A-F)\n"
    "  \\n = LF  0A    \\e = ESC 1B*   \\dDDD = up to three decimals (0-9)*\n"
    "  * - not standard C\n"
    "Switches must start with '-' or '/'.\n"
    "To start a remove string with '-' or '/', use \"\\-...\", etc.\n"
    "Switches:\n"
    "  -c<+/->     case sensitive [%s]\n"
    "  -b<backup>  extension for backup files [%s]\n"
    "  -n          no replacement -- strip file of search string [%s]\n"
    "  -v<+/->     verbose listing [%s]\n"
    "<backup> may contain '?' to keep characters of the source extension.\n"
    ,minus_plus[CASE!=0]
    ,BACKUP_EXT
    ,minus_plus[NO_REPLACEMENT!=0]
    ,minus_plus[VERBOSE!=0]
  );   //printf
  return k;
}   //help


//===========================================================================
int  loadflag(char* name,char* flag,char val)
{
  switch (val) {
  case 0:                              //default is on
  case '+':
  case '1':
    *flag=1;
    break;
  case '-':
  case '0':
    *flag=1;
    break;
  default:
    printf("illegal flag setting for %s.\n",name);
    return 0;
  }   //switch
  return 1;
}   //loadflag


//===========================================================================
typedef enum {
  invalid_switch,
  not_a_switch,
  a_switch
} SWITCH;
SWITCH loadswitch(char* s)
{
  if ((*s!='-')&&(*s!='/'))
  {
    if (new_loaded) return not_a_switch;
    if (old_loaded)
    {
      new_loaded=1;
      if (verbose) printf("loading replacement string \"%s\".\n", s);
      newcnt = strcstr((char*)newstr,s,sizeof(newstr));
      return a_switch;
    }

    old_loaded = 1;
    if (verbose) printf("loading search string \"%s\".\n", s);
    oldcnt = strcstr((char*)oldstr,s,sizeof(oldstr));
    if (oldcnt)
    {
      int i;
      if (!case_) for (i=0;i<oldcnt;i++) oldstr[i] = toupper(oldstr[i]);
      for (i=0;i<sizeof(skip);i++) skip[i] = oldcnt;
      for (i = 0; i < (oldcnt - 1); i++)
      {
        skip[oldstr[i]] = oldcnt - i - 1;
      }
      return a_switch;
    }
    printf("<old-string> parameter \"%s\" must have a length.\n",s);
    return invalid_switch;
  }   //if not a switch

  s++;
  switch (*s++)
  {
  case '?':
    return invalid_switch;

  case 'b':
    if (!*s)
    {
      backup=0;
      if (verbose) printf("backup disabled.\n");
    }
    else
    {
      strncpy(backup_ext,s,sizeof(backup_ext));
      backup_ext[3]=0;
      if (verbose) printf("backup extension is \"%s\".\n",backup_ext);
    }   //else not disabling backups
    break;

  case 'c':
    if (!loadflag("case",&case_,*s)) return invalid_switch;
    if (verbose) printf("case sensitive set to \"%s\".\n",minus_plus[case_]);
    break;

  case 'n':
    no_replacement = 1;
    if (new_loaded)
    {
      printf("warning: both \"-n\" option and replacement string given.\n");
    }
    new_loaded = 1;
    newcnt = 0;
    if (verbose) printf("no replacement enabled.\n");
    break;

  case 'v':
    if (!loadflag("verbose",&verbose,*s)) return invalid_switch;
    printf("verbose set to \"%s\".\n",minus_plus[verbose]);
    break;

  default:
    s--;
    printf("invalid switch \"%s\".\n",s);
    return invalid_switch;
  }   //switch
  return a_switch;
}   //loadswitch


//===========================================================================
int create_backup(char* fn)
{
  char* p;
  int i;

  strncpy(bak_name,fn,sizeof(bak_name));
  bak_name[sizeof(bak_name)-5]=0;
  p=strchr(bak_name,'.');
  if (!p) {
    p = &bak_name[strlen(bak_name)];
    strcpy(p,".$$$");
  }
  p++;
  for (i=0;i<3;i++)
    if (backup_ext[i]!='?') p[i] = backup_ext[i];
  if (verbose) printf("  backing up to \"%s\".\n",bak_name);
  if (stricmp(fn,bak_name)==0) {
    printf("  can't back up to same filename (\"%s\")!\n",bak_name);
    return 0;
  }
  unlink(bak_name);
  if (rename(fn,bak_name)!=0) {
    printf("  can't rename file \"%s\" to backup file \"%\".\n",fn,bak_name);
    return 0;
  }
  return 1;
}   //create_backup


//===========================================================================
int  replacefile(char* fn)
{
  FILE* inf;
  FILE* ouf;
  int i,k;
  int skip_count;
  int chars_read;
  long offset;

  if (verbose) printf("file %s\:\n",fn);
  if (!create_backup(fn)) return 0;

  inf=fopen(bak_name,"rb");
  if (inf==NULL)
  {
    printf("  could not reopen backup file \"%s\" to read.\n",bak_name);
    return 0;
  }

  ouf=fopen(fn,"wb");
  if (ouf==NULL)
  {
    printf("  could not open \"%s\" for write.\n",fn);
    fclose(inf);
    rename(bak_name,fn);
    return 0;
  }

  offset=0L;
  if (fread(bufstr,oldcnt,1,inf)==0)
  {
    if (verbose) printf("  file smaller than search string.\n");
    return 1;
  }

  if (!case_) for (i=0;i<oldcnt;i++) bufstr[i] = toupper(bufstr[i]);

  repls = 0;
  k = oldcnt - 1;

  while (!feof(inf))
  {
    while (k>=0)
    {
      if (bufstr[k] != oldstr[k]) break;
      k--;
    }

    if (k<0)
    {
      if (newcnt) fwrite(newstr,newcnt,1,ouf);
      repls++;
      if (verbose) printf("  Offset 0x%06lX (%u).\n",offset,repls);
      chars_read = fread(bufstr, 1, oldcnt, inf);
      if (chars_read < oldcnt)
      {
        fwrite(bufstr, 1, chars_read, ouf);
        break;
      }

      if (!case_)
      {
        for (i = 0; i < oldcnt; i++)
        {
          bufstr[i] = toupper(bufstr[i]);
        }
      }

      offset += (long)oldcnt;
      k = oldcnt - 1;
    }
    else
    {
      skip_count = skip[bufstr[k]] - (oldcnt - k - 1);
      if (skip_count < 1)
      {
        skip_count = 1;
      }

      fwrite(bufstr, 1, skip_count, ouf);

      for (i = 0;i < (oldcnt - skip_count); i++)
      {
        bufstr[i] = bufstr[skip_count + i];
      }

      chars_read = fread(&bufstr[oldcnt-skip_count], 1, skip_count, inf);

      if (chars_read < skip_count)
      {
        fwrite(bufstr, 1, chars_read + oldcnt - skip_count, ouf);
        break;
      }

      if (!case_)
      {
        for (i = oldcnt - skip_count; i < oldcnt; i++)
        {
          bufstr[i] = toupper(bufstr[i]);
        }
      }

      offset += (long) skip_count;
      k = oldcnt - 1;
    }
  }   //while
  fclose(inf);
  fclose(ouf);
  if (verbose) printf("  Replacements made: %u.\n",repls);

  return 1;
}   //replacefile


//===========================================================================
int  main(int argc,char* argv[])
{
  int i;
  SWITCH sw;
  int files=0;

  printf("repl -- replace a string of characters/bytes in a file\n");

  if (argc == 1) return help(1);

  if (argc < 4)
  {
    printf("Too few parameters provided.  Use \"repl -?\" for usage.\n");
    return 1;
  }

  for (i=1;i<argc;i++)
  {
    sw = loadswitch(argv[i]);
    if (sw == invalid_switch) return help(1);
    if (sw == a_switch) continue;

    if (!new_loaded)
    {
      printf("no replacement string supplied; use \"repl -?\" for help\n");
      return 1;
    }

    files++;
    replacefile(argv[i]);
  }   //for each argument
  printf("%u files processed.\n",files);
  return 0;
}   //main

