#include <stdiostr.h>

#include <ctype.h>
#include <dir.h>
#include <dos.h>
#include <fstream.h>
#include <iomanip.h>
#include <string.h>
#include <ttypes.h>
#include <util.h>

#include "fileinfo.hpp"
#include "templist.hpp"
#include "timedate.hpp"


#define ADVANCE_TO_END(s)       while (*s) s++
#define TERMINATE(s)            s[sizeof(s)-1] = 0


//===========================================================================
//
//  Global Types...
//
//===========================================================================

typedef Sorted_List<File_Info*> File_List;

int Sorted_List<File_Info*>:: Compare(
  File_Info* object1,
  File_Info* object2)
{
  int compared;

  compared = strcmp(object1->name, object2->name);
  if (compared != 0) return compared; //--------------> return!

  return strcmp(object1->path, object2->path);

}   // Sorted_List<File_Info*>:: Compare


//===========================================================================
//
//  Global Data...
//
//===========================================================================

const int max_line_width = 90;
const int max_page_length = 60;



#define VERBOSE 0

char* path = newStr(".\\");     // path from which to load next files
char run_path[MAXPATH] = ".\\"; // path from which executable was run
char verbose = VERBOSE;

const char* minus_plus[] = { "-", "+" };  // text for flags

File_List file_list(10, 20);


//===========================================================================
//
//  Function Prototypes...
//
//===========================================================================

/*****************************************************************************
*
*  TITLE:        Initialize
*
*  DESCRIPTION:  The function "Initialize" loads the commandline arguments.
*
*  REFERENCE:    None.
*
*****************************************************************************/

int Initialize(                 // true (1) on success

  int count,                    // count of commandline arguments
  char* argument[]);            // list of commandline arguments

/*****************************************************************************
*
*  TITLE:        Load Argument
*
*  DESCRIPTION:  The function "Load_Argument" loads a single argument from the
*                commandline.  If the argument is ok, it returns 1, otherwise
*                0.
*
*  REFERENCE:    None.
*
*****************************************************************************/

int Load_Argument(      // on success 1, on failure 0

  char* argument);      // argument to load

/*****************************************************************************
*
*  TITLE:        Load Argument File
*
*  DESCRIPTION:  The function "Load_Argument_File" opens the given file and
*                reads each line as if it were an argument/option.  Returns 1
*                on success, 0 on failure.
*
*  REFERENCE:    None.
*
*****************************************************************************/

int Load_Argument_File( // 1 on success, 0 on failure

  char* filename);      // name of file to load

/*****************************************************************************
*
*  TITLE:        Load Flag
*
*  DESCRIPTION:  The function "Load_Flag" loads a flag and increments the
*                options string from which it reads the flag's value.
*
*  REFERENCE:    None.
*
*****************************************************************************/

void Load_Flag(

  char* flag,           // pointer to flag variable
  char** string);       // string to check for +/-

/*****************************************************************************
*
*  TITLE:        Load Option
*
*  DESCRIPTION:  The function "Load_Option" loads a string as a set of
*                options.  More than one option may reside in the string.
*                Returns 1 on success, 0 on failure.
*
*  REFERENCE:    None.
*
*****************************************************************************/

int Load_Option(        // 1 on success, 0 on failure

  char* option);        // the option/switch string to load

/*****************************************************************************
*
*  TITLE:        Main
*
*  DESCRIPTION:  The function "main" gets the commandline arguments and sends
*                DOS error status.  It runs the whole show.
*
*  REFERENCE:    None.
*
*****************************************************************************/

int main(                       // 0 on success, error code on failure

  int count,                    // count of commandline arguments
  char* argument_list[]);       // the commandline arguments

/*****************************************************************************
*
*  TITLE:        Usage
*
*  DESCRIPTION:  The procedure "Usage" displays usage information for the
*                program.
*
*  REFERENCE:    None.
*
*****************************************************************************/

void Usage(void);


//===========================================================================
//
//  Function Bodies...
//
//===========================================================================

/*****************************************************************************
*
*  TITLE:        Initialize
*
*****************************************************************************/

int Initialize(                 // true (1) on success

  int count,                    // count of commandline arguments
  char* argument[])             // list of commandline arguments

{
  char drive[MAXDRIVE];
  char dir[MAXDIR];
  char file[MAXFILE];
  char extension[MAXEXT];

  fnsplit(argument[0], drive, dir, file, extension);
  sprintf(run_path, "%s%s", drive, dir);

  for (int i = 1; i < count; i++)
  {
    if (!Load_Argument(argument[i]))
    {
      return 0;
    }
  }

  return 1;

}   // Initialize


/*****************************************************************************
*
*  TITLE:        Load Argument
*
*****************************************************************************/

int Load_Argument(      // on success 1, on failure 0

  char* argument)       // argument to load

{
  switch (*argument)
  {
    case '-':
    case '/':
      return Load_Option(++argument);

    case '@':
      return Load_Argument_File(++argument);

  }   // switch

  struct ffblk file_block;
  char full_name[MAXPATH];
  uchar done;

  sprintf(full_name, "%s%s", path, argument);

  done = findfirst(full_name, &file_block, FA_ARCH | FA_HIDDEN | FA_RDONLY);

  if (done)
  {
    printf("warning: no files matching \"%s\"\n", full_name);
    return 1; //--------------------------------------> return!
  }

  while (!done)
  {
    file_list.Insert(new File_Info(path, &file_block));
    done = findnext(&file_block);
  }

  return 1;

}   // Load_Argument

/*****************************************************************************
*
*  TITLE:        Load Argument File
*
*****************************************************************************/

int Load_Argument_File( // 1 on success, 0 on failure

  char* filename)       // name of file to load

{
  FILE* file;

  file = fopen(filename, "rt");

  if (file == NULL)
  {
    printf("could not open \"%s\" to read arguments\n", filename);
    return 0;
  }

  char line[0x100];
  char* first_non_blank;

  while (!feof(file))
  {
    fgets(line, sizeof(line), file);
    TERMINATE(line);

    for (first_non_blank = line;
         (*first_non_blank != 0) && isspace(*first_non_blank);
         first_non_blank++) ;

    if ((*first_non_blank != ';') &&
        (*first_non_blank != 0))
    {
      if (!Load_Argument(first_non_blank))
      {
        return 0; //----------------------------------> return!
      }
    }
  }   // while

  fclose(file);
  return 1;

}   // Load_Argument_File

/*****************************************************************************
*
*  TITLE:        Load Flag
*
*****************************************************************************/

void Load_Flag(

  char* flag,           // pointer to flag variable
  char** string)        // string to check for +/-

{

  if (**string == '-')
  {
    *flag = 0;
    (*string)++;
  }
  else if (**string == '+')
  {
    *flag = 1;
    (*string)++;
  }
  else
  {
    *flag = 1;
  }

}   // Load_Flag

/*****************************************************************************
*
*  TITLE:        Load Option
*
*****************************************************************************/

int Load_Option(        // 1 on success, 0 on failure

  char* option)         // the option/switch string to load

{

  while (*option)
  {
    switch (*option++)
    {
      case 'v':
        Load_Flag(&verbose, &option);
        break;

      default:
        option--;
        printf("unknown option \"%s\"\n", option);
        return 0;

    }   // switch
  }   // while

  return 1;

}   // Load_Option

/*****************************************************************************
*
*  TITLE:        Main
*
*****************************************************************************/

int main(                       // 0 on success, error code on failure

  int count,                    // count of commandline arguments
  char* argument_list[])        // the commandline arguments

{
  if (count < 2)
  {
    Usage();
    return 1; //--------------------------------------> return!
  }

  if (!Initialize(count, argument_list))
  {
    Usage();
    return 1; //--------------------------------------> return!
  }

  if (file_list.Count() < 1)
  {
    printf("no files specified/found\n");
    return 2; //--------------------------------------> return!
  }

  for (int i = 0; i < file_list.Count(); i++)
  {
    File_Info* file = file_list.At(i);

    printf("%-12s %s %s %s\n",
      file->Name_And_Extension(),
      file->date.Image(NULL, "%04y.%03j"),
      file->time.Image(),
      file->path);
  }

  return 0;

}   //main


/*****************************************************************************
*
*  TITLE:        Usage
*
*****************************************************************************/

void Usage(void)

{
  printf(

    "Usage: fp [options] <file(s)>\n"
    "Where <file(s)> may contain wildcards.\n"
    "Options are (defaults in []):\n"
    "  @<file>    file to read options and files\n"
    "  -?         display usage information\n"
    "  -v[+/-]    verbose output [%s]\n"
    "  -d<name>=<string>   define a symbol\n"
    "  -D<file>   escape sequence and name definitions file\n"

    ,minus_plus[VERBOSE]

  );

}   // Usage
