#include <ctype.h>
#include <dir.h>
#include <dos.h>
#include <stdiostr.h>
#include <stdlib.h>
#include <string.h>

#include "icitypes.h"
#include "pc_info.h"

#ifndef __LARGE__
#error "must use large memory model"
#endif




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
   could_not_set_environment,   // DOS environment was full

} DOS_Return_Codes;


//===========================================================================
//
//  Global Data...
//
//===========================================================================

char run_file[MAXNAME];         // name for executing program
char run_path[MAXPATH];         // directory of file that was executed

#define LIB_LIST_FILE           "g:\\etc\\cmlibs"
#define LOCAL_ENVIRONMENT       true
#define MASTER_ENVIRONMENT      false

BOOLEAN local_environment = LOCAL_ENVIRONMENT;
BOOLEAN master_environment = MASTER_ENVIRONMENT;
char* master = NULL;            // pointer to the master DOS environment area
char* local = NULL;             // pointer to the local (parent's) env. area

const char* minus_plus[] = { "-", "+" };  // text for flags

char lib_list_file[0x100] = LIB_LIST_FILE;
char lib_name[0x100] = "";


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

   char* flag,          // pointer to flag variable
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
*  TITLE:        Usage
*
*  DESCRIPTION:  The procedure "Usage" displays usage information for the
*                program.
*
*  REFERENCE:    None.
*
*****************************************************************************/

void Usage(void);


//---------------------------------------------------------------------
// Function:    local_env
// Description: Returns a pointer to the local (parent) environment.
//              Uses the GETVECT 2Eh technique.
//---------------------------------------------------------------------
char* local_env()
{
   PSP* psp;

   asm mov ax, 0x6200            // get current PSP segment
   asm int 0x21

   psp = (PSP*) MK_FP(_BX, 0);
   psp = (PSP*) MK_FP(psp->parent_psp_segment, 0);

   return (char far*) MK_FP(psp->environment_segment, 0);
}


//---------------------------------------------------------------------
// Function:    master_env
// Description: Returns a pointer to the master environment.
//              Uses the GETVECT 2Eh technique.
//---------------------------------------------------------------------
char* master_env()
{
   UINT16 env_seg = 0;

   asm mov ax, 0x352e            // get INT 2Eh vector
   asm int 0x21
   asm mov dx, es:[0x002C]       // environment segment
   asm mov env_seg, dx

   return (char far*) MK_FP(env_seg, 0);
}


//--------------------------------------------------------------------------
// Function:    add_env
// Description: Scans the master environment for the last entry, then
//              adds the passed entry to end of the environment.
//              Returns error status.
// Example:     add_env("TEMP=r:\temp");
//--------------------------------------------------------------------------
BOOLEAN add_env(
     char* env,         // pointer to start of environment (with 0 offset)
     char* var)         // variable to add
{
   UINT16 size = (((MEMORY_CONTROL_BLOCK*) env) - 1) -> size;
   UINT16 var_length = strlen(var) + 1;
   UINT16 cur_length;

   while (*env)                         // while not end of vars
   {
     cur_length = strlen(env) + 1;      // get length
     env += cur_length;                 // move to next var
   }

   if ((cur_length + var_length) > size)
   {
      return false;
   }
   strcpy(env, var);                    // add var to environment
   env += var_length;                   // set cur to next env entry
   *env = 0;                            // set last NULL, end of env
   return true;
}


//--------------------------------------------------------------------------
// Function:    get_env
// Description: Scans the master environment for the passed variable name.
//              A variable match is detected when the charcters in var
//              match the starting characters of an environment string.
//              Returns a pointer to the variable, or NULL if the
//              variable was not found in the master environment.
// Example:     get_env("TEMP=");  Equals on end makes match accurate.
//--------------------------------------------------------------------------
char* get_env(
     char* env,         // pointer to start of environment (with 0 offset)
     char* var)         // variable to get
{
  int   found = 0;
  int   var_len = strlen(var);         // length of variable name
  while (!found && *env)               // while not found and more vars
  {
    if (!strnicmp(env, var, var_len))  // if var names match
      found = 1;                       // set found flag
    else                               // else no match
      env += strlen(env)+1;            // move to next var
  }
  if (!found) env = NULL;              // return null if not found
  return env;
}

//--------------------------------------------------------------------------
// Function:    del_env
// Description: Scans the master environment for the passed variable name.
//              Deletes the variable and compacts the environment if the
//              variable is found.
//              Returns true if the variable was deleted, false if it
//              wasn't found.
// Example:     del_env("TEMP=");    Equals on end makes match accurate.
//--------------------------------------------------------------------------
BOOLEAN del_env(
     char* env,         // pointer to start of environment (with 0 offset)
     char* var)         // variable to delete
{
   char* cur = get_env(env, var);       // find position in environment
   char* next;                          // pointer to next environment var
   int next_length;

   if (cur == NULL)
   {
      return false; //----------------------------------------> return!
   }
   next = cur + strlen(cur)+1; // next points to next var
   while (*next)               // while cur is not last
   {
      next_length = strlen(next)+1;
      strcpy(cur, next);        // overwrite cur with next
      cur += next_length;       // skip over the one just written
      next += next_length;      // skip over the one just copied
   }
   *cur = 0;
   return true;
}


//---------------------------------------------------------------------
// Function:    set_env
// Description: Sets the value of an existing environmental variable.
//              Returns true if successfully set.
// Example:     set_env("TEMP=r:\temp");
//--------------------------------------------------------------------
BOOLEAN set_env(
     char* env,         // pointer to start of environment (with 0 offset)
     char* var)         // variable to set
{
   char* equal = strchr(var,'=');
   char  first;                 // temp stores first char of value

   if (equal == NULL)           // if equal not found
   {
      return false; //----------------------------------------> return!
   }
  
   first  = equal[1];           // save first cchar of value
   equal[1] = 0;                // end var name string after = sign
   del_env(env, var);           // delete existing var in env
   equal[1] = first;            // set value back to original
   return add_env(env, var);    // add variable to environment
}


//---------------------------------------------------------------------
// Function:    set_value
// Description: Sets the value of an existing environmental variable.
//              Returns true if successfully set.
// Example:     set_env("TEMP=r:\temp");
//--------------------------------------------------------------------

BOOLEAN set_value(
     char* env,
     char* var_name,
     char* value)
{
   static char var[0x100];

   sprintf(var, "%s=%s", var_name, value);

   return set_env(env, var);
}


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

         case 'b':
            Load_Flag(&local_environment, &option);     // set local first,
            master_environment = local_environment;     //    then master
            break;

         case 'F':
            strcpy(lib_list_file, option);
            ADVANCE_TO_END(option);
            break;

         case 'l':
            Load_Flag(&local_environment, &option);
            break;

         case 'm':
            Load_Flag(&master_environment, &option);
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

DOS_Return_Codes main(

   int count,                   // count of commandline arguments
   char* argument_list[])       // the commandline arguments

{
   DOS_Return_Codes return_code = success;
   FILE* file;
   static char string[0x100];
   char* s;
   static char name[0x100];
   char* n;
   static char place[0x100];
   char* p;

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

   if (lib_name[0] == 0)
   {
      printf("no library name given\n");
      Usage();
      return commandline_error;
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

   while (!feof(file))
   {
      fgets(string, sizeof(string)-1, file);
      string[sizeof(string)-1] = 0;
      s = string;
      while (*s && isspace(*s)) s++;
      if (strchr("#*./", *s) != NULL) continue;
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
//         printf("ill-formed cmlib line: \"%s\"\n", string);
         continue;
      }

      if (stricmp(name, lib_name) == 0)
      {
         fclose(file);

         master = master_env();
         local = local_env();

         if ( master_environment &&
              (!set_value(master, "TLIB", name) ||
               !set_value(master, "TLIBDIR", place)) )
         {
            printf("could not set master environment\n");
            return could_not_set_environment;
         }

         if ( local_environment &&
              (local != master) &&
              (!set_value(local, "TLIB", name) ||
               !set_value(local, "TLIBDIR", place)) )
         {
            printf("could not set local environment\n");
            return could_not_set_environment;
         }

         printf("SET TLIB=%s\n", name);
         printf("SET TLIBDIR=%s\n", place);
         return success; //-----------------------------------> return!
      }
//      printf("Library \"%s\" is in directory \"%s\"\n", name, place);
   }

   fclose(file);

   return library_name_not_found;

}   // main


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
      "\n"
      "Options must begin with '-' or '/' (defaults in []):\n"
      "   @<file>    read more arguments and options from <file>\n"
      "   -?         display usage information\n"
      "   -b[-/+]    set both local and master environments [%s]\n"
      "   -F<libdir> where library list file is held [%s]\n"
      "   -l[-/+]    set local DOS environment [%s]\n"
      "   -m[-/+]    set master DOS environment [%s]\n"
      "The library list file can be given in any of the following order:\n"
      "   (1) the environment variable CMLIBS\n"
      "   (2) the default -F name (%s)\n"
      "   (3) the commandline argument to -F\n"
      "   (4) the file \"w:\\%%LOGIN%%\\etc\\cmlibs\", using environment"
                      " variable LOGIN\n"

      ,run_file
      ,minus_plus[LOCAL_ENVIRONMENT && MASTER_ENVIRONMENT]
      ,LIB_LIST_FILE
      ,minus_plus[LOCAL_ENVIRONMENT]
      ,minus_plus[MASTER_ENVIRONMENT]
      ,LIB_LIST_FILE

   );

}   // Usage

