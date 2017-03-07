#include <ctype.h>
#include <dir.h>
#include <dos.h>
#include <stdiostr.h>
#include <stdlib.h>
#include <string.h>

#include "w:\rogers\cpp\util\icitypes.h"
#include "w:\rogers\cpp\util\templist.hpp"


#define ADVANCE_TO_END(s)       while (*s) s++
#define MAXNAME                 (MAXFILE + MAXEXT)
#define TERMINATE(s)            s[sizeof(s)-1] = 0


//===========================================================================
//
//  Global Types...
//
//===========================================================================

typedef enum
{
   success = 0,                 // program ran successfully

   commandline_error,           // error in a commandline parameter or switch
   could_not_open_file,         // couldn't open library file
   library_name_not_found,      // library being sought was not found
   more_than_one_library,       // only one library can be specified

} DOS_Return_Codes;


class LIBRARY_MAP
{
public:
   char* name;
   char* path;

   LIBRARY_MAP(
        char* name,
        char* path);

};   // class LIBRARY_MAP


typedef Sorted_List<LIBRARY_MAP*> Library_List;

// int Sorted_List<LIBRARY_MAP*>::Compare(
//      LIBRARY_MAP* obj1,
//      LIBRARY_MAP* obj2);

typedef Sorted_List<char*> String_List;

// int Sorted_List<char*>::Compare(
//      char* obj1,
//      char* obj2);


//===========================================================================
//
//  Global Data...
//
//===========================================================================

#define LIB_LIST_FILE   "g:\\etc\\cmlibs"
#define VERBOSE         false

char run_file[MAXNAME];         // name for executing program
char run_path[MAXPATH];         // directory of file that was executed

const char* minus_plus[] = { "-", "+" };  // text for flags

char lib_list_file[0x100] = LIB_LIST_FILE;
char lib_name[0x100] = "";

Library_List* map_list;
String_List* user_list;

BOOLEAN verbose = VERBOSE;

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
*                It returns success when all parameters are loaded
*                successfully.
*                Otherwise, it returns the first non-success return code
*                from the Load_Argument() function.
*
*  REFERENCE:    None.
*
*****************************************************************************/

DOS_Return_Codes Initialize(

   int count,                   // count of commandline arguments
   char* argument[]);           // list of commandline arguments

/*****************************************************************************
*
*  TITLE:        Load Argument
*
*  DESCRIPTION:  The function "Load_Argument" loads a single
*                argument from the commandline.
*                If the argument begins with either '-' or '/',
*                it is loaded as an option (via Load_Option()).
*                If the argument begins with '@', it is loaded
*                as an argument file (via Load_Argument_File()).
*
*                If the argument is accepted, Load_Argument()
*                returns success, otherwise commandline_error.
*
*  REFERENCE:    None.
*
*****************************************************************************/

DOS_Return_Codes Load_Argument(

   char* argument);     // argument to load

/*****************************************************************************
*
*  TITLE:        Load Argument File
*
*  DESCRIPTION:  The function "Load_Argument_File" opens the given file and
*                reads each line as if it were an argument/option.
*                The line comment character is ';' -- that is, any line
*                whose first non-blank character is ';' will be skipped.
*                If the given file can not be opened,
*                Load_Argument_File() will return commandline_error and
*                print an error message.
*                If Load_Argument() returns anything other than success,
*                Load_Argument_File() will stop processing and return the
*                error code.
*                Otherwise, it returns success.
*
*  REFERENCE:    None.
*
*****************************************************************************/

DOS_Return_Codes Load_Argument_File(

   char* filename);     // name of file to load

/*****************************************************************************
*
*  TITLE:        Load Flag
*
*  DESCRIPTION:  The function "Load_Flag" loads a flag
*                and increments the options string from
*                which it reads the flag's value.
*                The flag is assumed to be a character,
*                and will be set to 0 ('\x00') if
*                a minus character ('-') is found.
*                If any other character is found,
*                then the value of flag will be 1 ('\x01').
*                If a minus or plus ('+') is found,
*                the string pointer is incremented to
*                point to the next character in
*                the options list.
*
*  REFERENCE:    None.
*
*****************************************************************************/

void Load_Flag(

   BOOLEAN* flag,       // pointer to flag variable
   char** string);      // string to check for +/-

/*****************************************************************************
*
*  TITLE:        Load Option
*
*  DESCRIPTION:  The function "Load_Option" loads a string as a set of
*                options.  More than one option may reside in the string.
*                Returns success on success, commandline_error on failure.
*
*  REFERENCE:    None.
*
*****************************************************************************/

DOS_Return_Codes Load_Option(

   char* option);       // the option/switch string to load

/*****************************************************************************
*
*  TITLE:        Main
*
*  DESCRIPTION:  The function "main" gets the commandline arguments
*                and sends an error code back to DOS.
*                See the definition of DOS_Return_Codes
*                in the "Global Types" section.
*
*  REFERENCE:    None.
*
*****************************************************************************/

DOS_Return_Codes main(          // 0 on success, error code on failure

   int count,                   // count of commandline arguments
   char* argument_list[]);      // the commandline arguments

/*****************************************************************************
*
*  TITLE:        Show File
*
*  DESCRIPTION:  The subprogram "Show_File"
*                displays the file's information if it
*                is for one of the users in the list.
*                If the list is NULL or empty, it
*                always displays the information.
*
*  REFERENCE:    None.
*
*****************************************************************************/

BOOLEAN Show_File(
     LIBRARY_MAP* map,
     struct ffblk* block,
     String_List* users);

/*****************************************************************************
*
*  TITLE:        Show Locked In Library
*
*  DESCRIPTION:  The procedure "Show_Locked_In_Library"
*                lists all locked files in the library which
*                are being used by any of the given
*                users.
*
*                If for_users is NULL or is empty, all locked
*                files are listed.
*
*  REFERENCE:    None.
*
*****************************************************************************/

void Show_Locked_In_Library(
     LIBRARY_MAP* map,
     void* _for_users);

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

DOS_Return_Codes Initialize(

   int count,                   // count of commandline arguments
   char* argument[])            // list of commandline arguments

{
   char drive[MAXDRIVE];
   char dir[MAXDIR];
   char file[MAXFILE];
   char extension[MAXEXT];
   DOS_Return_Codes return_code = success;
   int index;


     //  Load name and path of running program.

   fnsplit(argument[0], drive, dir, file, extension);
   sprintf(run_file, "%s%s", file, extension);
   sprintf(run_path, "%s%s", drive, dir);

   index = 1;
   while ( (index < count) &&
           (return_code == success) )
   {
      return_code = Load_Argument(argument[index]);
      index++;
   }

   return return_code;

}   // Initialize


/*****************************************************************************
*
*  TITLE:        LIBRARY MAP constructor
*
*****************************************************************************/

LIBRARY_MAP::LIBRARY_MAP(
     char* name,
     char* path)
{
   this->name = Copy_Of_String(name);
   this->path = Copy_Of_String(path);

}   // LIBRARY_MAP::LIBRARY_MAP


/*****************************************************************************
*
*  TITLE:        Library List :: Compare
*
*****************************************************************************/

int Sorted_List<LIBRARY_MAP*>::Compare(
     LIBRARY_MAP* obj1,
     LIBRARY_MAP* obj2)
{
   return strcmp(obj1->name, obj2->name);

}   // Sorted_List<LIBRARY_MAP*>::Compare

/*****************************************************************************
*
*  TITLE:        Load Argument
*
*****************************************************************************/

DOS_Return_Codes Load_Argument(

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

   if (lib_name[0] != 0)
   {
      return more_than_one_library; //------------------------> return!
   }

   strcpy(lib_name, argument);

   return success;

}   // Load_Argument

/*****************************************************************************
*
*  TITLE:        Load Argument File
*
*****************************************************************************/

DOS_Return_Codes Load_Argument_File(

   char* filename)      // name of file to load

{
   FILE* file;
   DOS_Return_Codes return_code = success;
   char line[0x100];
   char* first_non_blank;

   file = fopen(filename, "rt");

   if (file == NULL)
   {
      printf("could not open \"%s\" to read arguments\n", filename);
      return commandline_error;
   }

   while ( !feof(file) &&
           (return_code == success) )
   {
      fgets(line, sizeof(line), file);
      TERMINATE(line);

      for (first_non_blank = line;
           (*first_non_blank != 0) && isspace(*first_non_blank);
           first_non_blank++)
      {
      }

      if ((*first_non_blank != ';') &&
          (*first_non_blank != 0))
      {
         return_code = Load_Argument(first_non_blank);
      }
   }   // while

   fclose(file);

   return return_code;

}   // Load_Argument_File

/*****************************************************************************
*
*  TITLE:        Load Flag
*
*****************************************************************************/

void Load_Flag(

   BOOLEAN* flag,       // pointer to flag variable
   char** string)       // string to check for +/-

{

   if (**string == '-')
   {
      *flag = false;
      (*string)++;
   }
   else if (**string == '+')
   {
      *flag = true;
      (*string)++;
   }
   else
   {
      *flag = true;
   }

}   // Load_Flag

/*****************************************************************************
*
*  TITLE:        Load Option
*
*****************************************************************************/

DOS_Return_Codes Load_Option(

   char* option)        // the option/switch string to load

{

   //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
   //!!!!
   //!!!!  This is where you place your own switch arguments...
   //!!!!
   //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   while (*option)
   {
      switch (*option++)
      {
         case '?':
            return commandline_error;

         case 'F':
            strcpy(lib_list_file, option);
            ADVANCE_TO_END(option);
            break;

         case 'u':
            user_list->Insert(Copy_Of_String(option));
            ADVANCE_TO_END(option);
            break;

         case 'v':
            Load_Flag(&verbose, &option);
            break;

         default:
            option--;
            printf("unknown switch option \"-%s\"\n", option);
            return commandline_error;

      }   // switch
   }   // while

   return success;

}   // Load_Option

/*****************************************************************************
*
*  TITLE:        Main
*
*****************************************************************************/

void Show_Map(LIBRARY_MAP* map, void* parameter)
{
   if (parameter != NULL) printf("%s", (char*) parameter);
   printf("library \"%s\" in path \"%s\"\n", map->name, map->path);
}

void Show_User(char* name, void* parameter)
{
   if (parameter != NULL) printf("%s", (char*) parameter);
   printf("user \"%s\"\n", name);
}



DOS_Return_Codes main(

   int count,                   // count of commandline arguments
   char* argument_list[])       // the commandline arguments

{
   DOS_Return_Codes return_code = success;
   FILE* file;
   char string[0x100];
   char* s;
   char name[0x100];
   char* n;
   char place[0x100];
   char* p;

   map_list = new Library_List(10, 10);
   user_list = new String_List(10, 10);

   char* env = getenv("CMLIB");
   if (env != NULL)
   {
      strcpy(lib_list_file, env);
   }

   return_code = Initialize(count, argument_list);

   if (return_code != success)
   {
      Usage();
      return return_code; //----------------------------------> return!
   }

   //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
   //!!!!
   //!!!!  Insert your application's code here...
   //!!!!
   //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   file = fopen(lib_list_file, "rt");
   if (file == NULL)
   {
      env = getenv("LOGIN");
      if (env != NULL)
      {
         sprintf(lib_list_file, "w:\\%s\\etc\\cmlibs", env);
         file = fopen(lib_list_file, "rt");
      }
   }

   if (file == NULL)
   {
      printf("could not open \"%s\"\n", lib_list_file);
      return could_not_open_file;
   }

   if (verbose)
   {
      printf("(v) scanning \"%s\" for libraries\n", lib_list_file);
   }

   while (!feof(file))
   {
      fgets(string, sizeof(string)-1, file);
      string[sizeof(string)-1] = 0;
      s = string;
      while (*s && isspace(*s)) s++;
      if (strchr("#*./", *s) != NULL) continue; // found comment
      n = name;
      while (*s && !isspace(*s)) *n++ = *s++;
      *n = 0;
      while (*s && isspace(*s)) s++;
      p = place;
      while (*s && !isspace(*s)) *p++ = *s++;
      *p = 0;

      if ( (strlen(name) == 0) ||
           (strlen(place) == 0) )
      {
         printf("ill-formed cmlib line: \"%s\"\n", string);
         continue;
      }

      map_list->Insert(new LIBRARY_MAP(name, place));

   }

   if (verbose)
   {
      printf("(v) %u libraries located:\n", map_list->Count());
      map_list->For_Each(Show_Map, "(v) ");
      printf("(v) %u users to check:\n", user_list->Count());
      user_list->For_Each(Show_User, "(v) ");
   }

   fclose(file);

   LIBRARY_MAP* lib_map = NULL;
   int index = 0;

   if (lib_name[0] != 0)
   {
      lib_map->name = lib_name;
      if (map_list->Search(lib_map, index))
      {
         Show_Locked_In_Library(map_list->At(index), user_list);
      }
      else
      {
         printf("Library \"%s\" not found in \"%s\"\n",
              lib_name, lib_list_file);
         Usage();
         return library_name_not_found;
      }
   }
   else
   {
      map_list->For_Each(Show_Locked_In_Library, user_list);
   }

   return success;

}   // main

/*****************************************************************************
*
*  TITLE:        Show File
*
*****************************************************************************/

BOOLEAN Show_File(
     LIBRARY_MAP* map,
     struct ffblk* block,
     String_List* users)
{
  FILE* file;
  char full_name[0x100];
  char first_line[0x100];

  char* p;

  typedef struct
  {
    unsigned second_over_2: 5;
    unsigned minute: 6;
    unsigned hour:   5;
  } FILE_TIME;

  typedef struct
  {
    unsigned day:    5;
    unsigned month:  4;
    unsigned years_since_1980: 7;
  } FILE_DATE;

  FILE_TIME* file_time;
  FILE_DATE* file_date;

  strcpy(full_name, map->path);
  strcat(full_name, "\\lock\\");
  strcat(full_name, block->ff_name);

  file = fopen(full_name, "rt");
  if (file == NULL)
  {
     printf("couldn't open file \"%s\"\n", full_name);
     return false; //-----------------------------------------> return!
  }

  fgets(first_line, sizeof(first_line), file);
  fclose(file);

  TERMINATE(first_line);
  first_line[8] = 0;

  p = strchr(first_line, '\n');
  if (p != NULL) *p = 0;

  if ( (users != NULL) &&
       (users->Count() > 0) )
  {
     int i;
     if (!users->Search(first_line, i))
     {
        return false; //--------------------------------------> return!
     }
  }

  file_time = (FILE_TIME*) &(block->ff_ftime);
  file_date = (FILE_DATE*) &(block->ff_fdate);

  strlwr(full_name);
  strlwr(block->ff_name);

  printf("%-8s %-12s %-16s %04u.%02u.%02u %02u:%02u:%02u\n",
       first_line,
       block->ff_name,
       map->name,
       1980 + file_date->years_since_1980,
       file_date->month,
       file_date->day,
       file_time->hour,
       file_time->minute,
       2 * file_time->second_over_2);

  return true;

}   // Show_File

/*****************************************************************************
*
*  TITLE:        Show Locked In Library
*
*****************************************************************************/

void Show_Locked_In_Library(
     LIBRARY_MAP* map,
     void* _for_users)
{
   String_List* for_users = (String_List*) _for_users;

   if (verbose)
   {
      printf("(v) scanning library directory \"%s\"\n", map->path);
   }

   struct ffblk file_block;
   char full_name[0x100];

   strcpy(full_name, map->path);
   strcat(full_name, "\\lock\\*.*");

   if (findfirst(full_name, &file_block, FA_ARCH | FA_RDONLY) != 0)
   {
      if (verbose)
      {
         printf("(v)    no files found\n");
      }

      return;  //---------------------------------------------> return!
   }

   int lock_count = 0;

   do
   {
      if (verbose)
      {
         printf("(v)    checking \"%s\"\n", file_block.ff_name);
      }

      if (Show_File(map, &file_block, for_users) == true)
      {
         lock_count++;
      }

   } while (findnext(&file_block) == 0);

}   // Show_Locked_In_Library

/*****************************************************************************
*
*  TITLE:        String List :: Compare
*
*****************************************************************************/

int Sorted_List<char*>::Compare(
     char* obj1,
     char* obj2)
{
   return stricmp(obj1, obj2);

}   // Sorted_List<char*>::Compare

/*****************************************************************************
*
*  TITLE:        Usage
*
*****************************************************************************/

void Usage(void)

{
   //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
   //!!!!
   //!!!!  This is where you place your own usage information...
   //!!!!
   //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   printf(

      "\n"
      "Usage: %s [options] <library_name>\n"
      "\n"
      "<library_name> should be name of the TLIB library, as listed in\n"
      "the library list file (see -L option below).\n"
      "If no <library_name> is given, all libraries in the library\n"
      "list file (see -F) will be checked.\n" 
      "\n"
      "Options must begin with '-' or '/' (defaults in []):\n"
      "   @<file>   read more arguments and options from <file>\n"
      "   -?        display usage information\n"
      "   -F        where library list file is held [%s]\n"
      "   -u<name>  user name to check, multiple -u's allowed [all]\n"
      "   -v[+/-]   verbose mode [%s]\n"
      "The library list file can be given in any of the following order:\n"
      "   (1) the environment variable CMLIBS\n"
      "   (2) the default -F name (%s)\n"
      "   (3) the commandline argument to -F\n"
      "   (4) the file \"w:\\%%LOGIN%%\\etc\\cmlibs\", using environment"
                      " variable LOGIN\n"


      ,run_file
      ,LIB_LIST_FILE
      ,minus_plus[VERBOSE != false]
      ,LIB_LIST_FILE

   );

}   // Usage
