#include <stdiostr.h>

#include <dir.h>
#include <dos.h>
#include <fstream.h>
#include <iomanip.h>
#include <string.h>
#include <ttypes.h>
#include <util.h>

#include "templist.hpp"
#include "timedate.hpp"


#define MAXNAME         ((MAXFILE) + (MAXEXT))          // from <dir.h>


#define ADVANCE_TO_END(s)       while (*s) s++
#define TERMINATE(s)            s[sizeof(s)-1] = 0


//===========================================================================
//
//  Global Types...
//
//===========================================================================

class File_Info
{
public:

   char* name;
   char* path;

   Time_Date when;
   long size;
   uchar attribute;

public:

   File_Info(void)
   {
      name = NULL;
      path = NULL;
      attribute = 0;
      size = 0L;
   }

public:

   File_Info(
      char* path,
      struct ffblk* file_block)
   {
      this->path = path;
      this->name = NULL;
      Load(file_block);
   }

public:

   File_Info(
      char* path,
      char* name)
   {
      this->path = path;
      this->name = newStr(name);
      Update_Stats();
   }

public:

   ~File_Info()
   {
      delete name;
   }

public:

   void Load(
      struct ffblk* file_block);

public:

   void Update_Stats(
      uchar only_attributes = (FA_ARCH | FA_HIDDEN | FA_RDONLY));

};   // class File_Info


void File_Info:: Load(
   struct ffblk* file_block)
{
   delete name;

   name = newStr(file_block->ff_name);

   if (name != NULL)
   {
      strlwr(name);
   }

   when.date.Set( (file_block->ff_fdate >> 9) + 1980,
                 (file_block->ff_fdate >> 5) & 0x0f,
                 (file_block->ff_fdate)      & 0x1f);

   when.time.Set( (file_block->ff_ftime >> 11),
                 (file_block->ff_ftime >> 5) & 0x3f,
                 (file_block->ff_ftime << 1) & 0x3f);

   size = file_block->ff_fsize;

   attribute = file_block->ff_attrib;

}   // File_Info:: Load


void File_Info:: Update_Stats(
   uchar only_attributes)
{
   char full_name[MAXPATH] = "";

   if (name == NULL) return; //--------------------------------> return!

   if (path != NULL)
   {
      strcpy(full_name, path);
   }

   strcat(full_name, name);

   struct ffblk file_block;

   if (findfirst(full_name, &file_block, only_attributes) == 0)
   {
      Load(&file_block);
   }
}   // File_Info:: Update_Stats



typedef Sorted_Pointer_List<File_Info> File_List;

int File_List:: Compare(
   File_Info* object1,
   File_Info* object2)
{
   int compared;

   compared = strcmp(object1->name, object2->name);
   if (compared != 0) return compared; //--------------> return!

   return strcmp(object1->path, object2->path);
}   // File_List:: Compare


//===========================================================================
//
//  Global Data...
//
//===========================================================================


#define SEARCH_PATH     ".\\"
#define VERBOSE         0

char run_name[MAXNAME] = "ADADEP.EXE";  // name of executable was run
char run_path[MAXPATH] = ".\\";         // path from which executable was run
char search_path[MAXPATH] = SEARCH_PATH; // where to find next next files
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
*  TITLE:        Combine Path And Name
*
*  DESCRIPTION:  The procedure "Combine_Path_And_Name"
*                puts a path and file name into the
*                given string.
*
*  REFERENCE:    None.
*
*****************************************************************************/

void Combine_Path_And_Name(

   char* path,                   // path part of file specification
   char* name,                   // name part of file specification
   char* to_target_string);      // to hold combination of path and name

/*****************************************************************************
*
*  TITLE:        Extract Path And Name
*
*  DESCRIPTION:  The procedure "Extract_Path_And_Name"
*                gets a path and file name from the
*                given string.
*
*  REFERENCE:    None.
*
*****************************************************************************/

void Extract_Path_And_Name(

   char* source_string,          // source string containing path and name
   char* to_path,                // to hold path part of file specification
   char* and_name);              // to hold name part of file specification

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
*  TITLE:        Combine Path And Name
*
*****************************************************************************/

void Combine_Path_And_Name(

   char* path,                   // path part of file specification
   char* name,                   // name part of file specification
   char* to_target_string)       // to hold combination of path and name

{
   int path_length;

   path_length = strlen(path);

   if ((path_length > 0) &&
        (path[path_length - 1] != '\\') &&
        (path[path_length - 1] != ':'))
   {
      sprintf(to_target_string, "%s\\%s", path, name);
   }
   else
   {
      sprintf(to_target_string, "%s%s", path, name);
   }

}   // Combine_Path_And_Name

/*****************************************************************************
*
*  TITLE:        Extract Path And Name
*
*****************************************************************************/

void Extract_Path_And_Name(

   char* source_string,          // source string containing path and name
   char* to_path,                // to hold path part of file specification
   char* and_name)               // to hold name part of file specification

{
   char dir[MAXDIR];
   char drive[MAXDRIVE];
   char file[MAXFILE];
   char ext[MAXEXT];

   fnsplit(source_string, dir, drive, file, ext);

   sprintf(to_path, "%s%s", dir, drive);
   sprintf(and_name, "%s%s", file, ext);

}   // Extract_Path_And_Name

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
   sprintf(run_name, "%s%s", file, extension);

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

   done = findfirst(full_name, FA_ARCH | FA_RDONLY, &file_block);

   if (done)
   {
      printf("warning: no files matching \"%s\"\n", full_name);
      return 1; //--------------------------------------> return!
   }

   while (!done)
   {
      file_list.Insert(new File_Info(path, &file_block));
      findnext(&file_block);
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

      for (first_non_blank = line,
         (*first_non_blank != 0) && isspace(*first_non_blank),
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
         case 'p':
            strncpy(search_path, option, sizeof(search_path));
            TERMINATE(search_path);
            ADVANCE_TO_END(option);
            break;

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

      printf("%12s %s %s %s\n",
         file->name,
         file->when.date.Image(),
         file->when.time.Image(),
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

      "Usage: %s [options] <file(s)>\n"
      "Where <file(s)> may contain wildcards.\n"
      "Options are (defaults in []):\n"
      "  @<file>    file to read options and files\n"
      "  -?         display usage information\n"
      "  -p<path>   path for following files [%s]\n"
      "  -v[+/-]    verbose output [%s]\n"
      "  -x<files>  exclude file(s)\n"

      ,run_name
      ,search_path
      ,minus_plus[VERBOSE]

   );

}   // Usage
