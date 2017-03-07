#include <stdio.h>
#include <string.h>
#include <ctype.h>


#define min(a,b)       (((a) < (b)) ? (a) : (b))


//===========================================================================
//  variables
//---------------------------------------------------------------------------
#define TEXT_TAB_STOPS "9"
#define DELETE         1
#define VERBOSE        0

char verbose = VERBOSE;        //output listing will be verbose
char delete = DELETE;          //delete trailing spaces/tabs on each line
int  file_count = 0;           //number of files converted

char tab_stop[0x100];          //actual tab-stop array (1=stop, 0=no stop)

char* minus_plus[] = { "-", "+" };


//===========================================================================
//  MakeBackupName
//  Make a backup filename with the given extension for a filename.
//###########################################################################
//  NOTE: THIS FUNCTION DOES NOT HANDLE EMBEDDED DIRECTORY NAMES WITH "."'s
//        IN THEM!
//###########################################################################
//---------------------------------------------------------------------------
int  MakeBackupName(           //length of backup filename
  char* file_name,             //current filename
  char* back_name,             //backup filename
  int size,                    //maximum size of backup filename
  char* extension)             //file extension (up to three characters)
{
  int i;

  size--;                      //leave room for terminator
  for (i = 0; (i < size) && (file_name[i] != 0); i++)
  {
    if (file_name[i] == '.') break;
    back_name[i] = file_name[i];
  }
  if (i < size) back_name[i++] = '.';
  if (i < size) back_name[i++] = *extension++;
  if (i < size) back_name[i++] = *extension++;
  if (i < size) back_name[i++] = *extension++;
  back_name[i] = 0;
  return i;
}   //MakeBackupName


//===========================================================================
//  ReplaceTabsInFile
//  Replaces the tabs in a file according to the global tab-stop table.
//---------------------------------------------------------------------------
int  ReplaceTabsInFile(        //1 on success, 0 on failure
  char* file_name)             //name of file whose tabs are to be replaced
{
  FILE* file;
  FILE* out_file;
  static char buffer[0x200];
  static char backup_name[0x100];
  int bytes;
  int column;
  int line;
  char ch;
  int error;
  int tabs;
  int spaces;

  if ((*file_name == '/') || (*file_name == '-')) return 1;  //it's a switch

  strupr(file_name);
  MakeBackupName(file_name, backup_name, sizeof(backup_name), "BAK");
  if (strcmp(file_name, backup_name) == 0)
  {
    MakeBackupName(file_name, backup_name, sizeof(backup_name), "~BA");
  }

  if (verbose)
  {
    printf("Reading \"%s\", backing up to \"%s\"...\n",
           file_name, backup_name);
  }

  file = fopen(file_name, "rb");
  if (file == NULL)
  {
    printf("Couldn't open \"%s\".", file_name);
    return 0;
  }

  out_file = fopen(backup_name, "wb");
  if (out_file == NULL)
  {
    printf("Couldn't open backup file \"%s\".", backup_name);
    fclose(file);
    return 0;
  }

  while (!feof(file))
  {
    bytes = fread(buffer, 1, sizeof(buffer), file);
    if (bytes == 0) break;
    if (fwrite(buffer, 1, bytes, out_file) != bytes)
    {
      printf("Writing to backup file \"%s\" failed.\n", backup_name);
      fclose(file);
      fclose(out_file);
      return 0;
    }
  }   //while

  fclose(file);
  fclose(out_file);

  file = fopen(backup_name, "rt");     //open to start scanning for tabs
  if (file == NULL)
  {
    printf("Couldn't re-open backup file \"%s\" to read.\n", backup_name);
    return 0;
  }
  out_file = fopen(file_name, "wt");
  if (out_file == NULL)
  {
    printf("Couldn't open file \"%s\" to write new text.\n", file_name);
    fclose(file);
    return 0;
  }

  column = 0;
  line = 1;
  error = 0;
  tabs = 0;
  spaces = 0;
  while (!feof(file) && !error)
  {
    ch = fgetc(file);

    switch (ch)
    {
    case '\n':
      line++;
      column = 0;
      if (!delete)
      {
        while ((spaces > 0) && !error)
        {
          if (fputc(' ', out_file) == EOF) error = 1;
          spaces--;
        }
      }
      if (fputc(ch, out_file) == EOF) error = 1;
      spaces = 0;
      break;
    case '\t':
      spaces++;
      if (column < sizeof(tab_stop)) column++;
      while (column < sizeof(tab_stop) && !tab_stop[column])
      {
        spaces++;
        column++;
      }
      if (!error) tabs++;
      break;
    case ' ':
      spaces++;
      if (column < sizeof(tab_stop)) column++;
      break;
    default:
      while ((spaces > 0) && !error)
      {
        if (fputc(' ', out_file) == EOF) error = 1;
        spaces--;
      }
      if (fputc(ch, out_file) == EOF) error = 1;
      if (column < sizeof(tab_stop)) column++;
    }   //switch
  }   //while
  fclose(file);
  fclose(out_file);

  if (error)
  {
    printf("Error outputting to \"%s\" (%d lines completed).\n",
           file_name, line);
    return 0;
  }

  if (verbose)
  {
    printf("\"%s\": %d lines read/written, %d tabs replaced.\n",
           file_name, line, tabs);
  }
  file_count++;
  return 1;
}   //ReplaceTabsInFile


//===========================================================================
//  DumpTabStops
//  Dumps tab-stops to the screen.
//---------------------------------------------------------------------------
void DumpTabStops(             //no output
  int max_stop)                //maximum tab-stop to put out
{
  int i;
  int end_stop = min(sizeof(tab_stop), max_stop);

  printf("Tab stops <= %d:", max_stop);
  for (i = 0; (i < end_stop); i++)
  {
    if (tab_stop[i]) printf(" %d", i+1);
  }   //for
  printf("\n");
}   //DumpTabStops


//===========================================================================
//  LoadTabStops
//  Loads the tab_stop array according to the text tab-stop description.
//  The text description should be of the form "#1,#2,#3", etc, where each
//  number represents a column in a line.  Columns start at 1 (and no tab-
//  stop can exist in column 1, but supplying 1 to this function does not
//  cause an error).  The tab-stops must be supplied in increasing order.
//---------------------------------------------------------------------------
int  LoadTabStops(             //1 if tab-stops were loaded ok, 0 on error
  char* desc)                  //tab-stop description
{
  int stop = 0;
  int last = 0;
  int delta;

  memset(tab_stop, 0, sizeof(tab_stop));   //clear tab stops
  do
  {
    last = stop;
    if (!sscanf(desc, "%d", &stop)) break;
    stop--;                    //tab-stops start at 1
    if (stop <= last) break;
    if (stop >= sizeof(tab_stop)) break;
    tab_stop[stop] = 1;
    while ((*desc != 0) && isdigit(*desc)) desc++;
    if (*desc == 0) break;
    if (*desc != ',') break;
    desc++;
  } while (*desc);

  if (*desc != 0)
  {
    printf("Error in tab-stop description at \"%s\".", desc);
    return 0;
  }

  delta = stop-last;
  while (stop < sizeof(tab_stop))
  {
    tab_stop[stop] = 1;
    stop += delta;
  }

  return 1;
}   //LoadTabStops


//===========================================================================
//  LoadFlag
//  Loads a flag commandline switch.  Flags take the form "-f<value>", where
//  <value> is either unspecified or is one of either "+1tT", to set the
//  flag, or "-0fF" to reset the flag.
//---------------------------------------------------------------------------
int LoadFlag(          //1 if legal flag char, 0 otherwise
  char* name,          //name of flag being loaded
  char* s,             //string to be parsed
  char* flag)          //flag to be changed (set/reset)
{
  switch (*s)
  {
  case 0:
  case '+':
  case '1':
  case 't':
  case 'T':
    *flag = 1;
    break;
  case '-':
  case '0':
  case 'f':
  case 'F':
    *flag = 0;
    break;
  default:
    printf("illegal flag value \"%s\" for %s.\n", s, name);
    return 0;
  }
  return 1;
}   //LoadFlag


//===========================================================================
//  LoadSwitch
//  Loads and checks a commandline switch.
//---------------------------------------------------------------------------
int LoadSwitch(        //1 if commandline switch loaded ok (or not a switch)
  char* s)             //switch to load
{
  if ((*s != '/') && (*s != '-')) return 1;    //not a switch
  s++;                                         //skip switch indicator

  switch (*s++)
  {
  case '?':
    return 0;
  case 't':
    if (!LoadTabStops(s)) return 0;
    break;
  case 'd':
    if (!LoadFlag("delete trailing spaces/tabs", s, &delete)) return 0;
    break;
  case 'v':
    if (!LoadFlag("verbose listing", s, &verbose)) return 0;
    break;
  default:
    s -= 2;
    printf("unknown commandline switch \"%s\".\n", s);
    return 0;
  }   //switch
  return 1;
}   //LoadSwitch


//===========================================================================
//  Help
//  Displays help information for the program.
//---------------------------------------------------------------------------
int  Help(             //whatever is passed in (typically DOS return code)
  int parameter)       //parameter to be returned after displaying help
{
  printf(
    "\n"
    "untab [switches] file(s)\n"
    "Switches [default]:\n"
    "  -t<tab-stops>  tab stops (see below) [%s]\n"
    "  -d[-/+]        delete trailing spaces/tabs [%s]\n"
    "  -v[-/+]        verbose listing [%s]\n"
    "Tab-stops are delimited with commas (',').  Column numbers on each\n"
    "line begin with 1.  Tab-stops are continued naturally after the last\n"
    "stop is specified (\"-t9,13\" == \"-t9,13,17,21,25\"...).\n"
    "Tab-stops are only valid up to column %d.\n"
    ,TEXT_TAB_STOPS
    ,minus_plus[DELETE != 0]
    ,minus_plus[VERBOSE != 0]
    ,sizeof(tab_stop)
  );   //printf

  return parameter;
}   //Help



//===========================================================================
//  main
//  Loads arguments and runs.
//---------------------------------------------------------------------------
int  main(                     //DOS error code
  int argc,                    //number of commandline arguments
  char* argv[])                //array of pointers to commandline arguments
{
  int i;

  printf("untab -- replaces tabs with spaces in text files\n");

  LoadTabStops(TEXT_TAB_STOPS);
  for (i = 1; (i < argc); i++) if (!LoadSwitch(argv[i])) return Help(1);
  if (verbose) DumpTabStops(80);

  for (i = 1; (i < argc); i++) if (!ReplaceTabsInFile(argv[i])) return 2;
  printf("Tabs replaced in %d files.\n", file_count);

  return 0;
}   //main
