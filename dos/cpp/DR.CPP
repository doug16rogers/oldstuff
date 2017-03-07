#include <stdio.h>
#include <ctype.h>
#include <dir.h>
#include <dos.h>
#include <io.h>
#include <string.h>

#include <ttypes.h>     // from Turbo Vision

//#include "attconst.hpp"
//#include "constrnt.hpp"
#include "drstr.hpp"
#include "fileinfo.hpp"
#include "templist.hpp"
#include "timedate.hpp"


#define ADVANCE_TO_END(s)       while (*s) s++
#define MAXNAME                 (MAXFILE + MAXEXT)
#define TERMINATE(s)            s[sizeof(s)-1] = 0



class File_Table : public Sorted_List<File_Info*>
{
public:

   long total_size;

public:

   File_Table(void) : Sorted_List<File_Info*>()
   {
      total_size = 0L;
   }

public:

   int Insert(File_Info* file)
   {
      int inserted = Sorted_List<File_Info*>:: Insert(file);

      if (inserted)
      {
         total_size += file->size;
      }

      return inserted;

   }

};   // class File_Table

typedef Sorted_List<char *> String_Table;


//===========================================================================
//
//  Global Types...
//
//===========================================================================

   //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
   //!!!!
   //!!!!  This is where you define your own return codes...
   //!!!!
   //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

typedef enum
{
   success = 0,         // program ran successfully

   commandline_error,   // error in a commandline parameter or switch

} DOS_Return_Codes;

//===========================================================================
//
//  Global Data...
//
//===========================================================================

#define DEFAULT_FILES  "*.*"

#define ATTRIBUTE_FORMAT        "%-d%-r%-a%-h"
#define DATE_FORMAT             "%4y.%02m.%02d"
#define FORMAT                  "%-8n.%-3e %7s %d %t %a %p"
#define INCLUDING_DIRECTORIES   1
#define LOWERCASE               1
#define ORDER                   "nepdts"
#define SPACE_CHARACTER         '_'
#define TIME_FORMAT             "%2h:%02m:%02s"
#define TOTAL                   1
#define USE_HEXADECIMAL         0
#define USE_LOWERCASE           1
#define USE_OCTAL               0
#define USE_UPPERCASE           0
#define VERBOSE                 0
#define WARNINGS_ARE_ENABLED    1

char attribute_format[0x80] = ATTRIBUTE_FORMAT;
char date_format[0x80] = DATE_FORMAT;
char format[0x80] = FORMAT;
char including_directories = INCLUDING_DIRECTORIES;
int number_of_arguments_loaded = 0;
char order[0x80] = ORDER;
char space_character = SPACE_CHARACTER;
char time_format[0x80] = TIME_FORMAT;
char total  = TOTAL;
char use_hexadecimal = USE_HEXADECIMAL;
char use_lowercase = USE_LOWERCASE;
char use_octal = USE_OCTAL;
char use_uppercase = USE_UPPERCASE;
char verbose = VERBOSE;
char warnings_are_enabled = WARNINGS_ARE_ENABLED;

File_Table files;
File_Table excludes;
String_Table paths;

char run_file[MAXNAME];         // name for executing program
char run_path[MAXPATH];         // directory of file that was executed

const char* minus_plus[] = { "-", "+" };  // text for flags


//===========================================================================
//
//  Function Prototypes...
//
//===========================================================================

/*****************************************************************************
*
*  TITLE:        Copy With Spaces
*
*  DESCRIPTION:  The subprogram "Copy_With_Spaces"
*                copies the source string to the destination
*                string (up to the length of the destination)
*                while converting characters to spaces
*                according to the current space character.
*
*  REFERENCE:    None.
*
*****************************************************************************/

void Copy_With_Spaces(

   char* source,                // source string for copy
   char* target,                // destination string for copy
   UINT  target_length);        // size of destination string

/*****************************************************************************
*
*  TITLE:        Fill Out Order String
*
*  DESCRIPTION:  The procedure "Fill_Out_Order_String" looks
*                for the presence of each of the sort keys
*                within the given string.  Any that are not found
*                are appended to the end of the string in
*                the following order: "nepdts".
*
*  REFERENCE:    None.
*
*****************************************************************************/

void Fill_Out_Order_String(

   char* order_string);         // string with partial list of sort keys

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
*  TITLE:        Load Files
*
*  DESCRIPTION:  The function "Load_Files"
*                loads the files specified by the
*                given filename (with wildcards) into
*                the file list.
*
*                It returns true if at least one file matching
*                the filename mask is found.
*                If no file is found, it returns false.
*
*  REFERENCE:    None.
*
*****************************************************************************/

int  Load_Files(

   char* file_name_mask,        // file name (possibly with wildcards)
   File_Table& file_list,       // list of files
   char check_excludes = 1);    // whether to check the excludes list

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
*  TITLE:        New String
*
*  DESCRIPTION:  The function "New_String"
*                allocates a copy of the given string.
*
*  REFERENCE:    None.
*
*****************************************************************************/

char* New_String(

   const char* string_to_copy);         // source string to copy

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

/*****************************************************************************
*
*  TITLE:        Write Attribute To String
*
*  DESCRIPTION:  The procedure "Write_Attribute_To_String"
*                writes the given file attribute bits to
*                a string according to the selected format
*                descriptor.
*
*  REFERENCE:    None.
*
*****************************************************************************/

void Write_Attribute_To_String(

   uchar attribute,     // file attribute (as collection of bits)
   char* format,        // format descriptor for attribute
   char* target);       // target string to hold formatted attribute

/*****************************************************************************
*
*  TITLE:        Write File Information To String
*
*  DESCRIPTION:  The procedure "Write_File_Information_To_String"
*                writes the given file's information to the
*                target string using the supplied format
*                descriptor.
*
*  REFERENCE:    None.
*
*****************************************************************************/

void Write_File_Information_To_String(

   File_Info* file,     // file whose info is to be written
   char* format,        // format descriptor for output to target
   char* target);       // string to hold result

//===========================================================================
//
//  Function Bodies...
//
//===========================================================================

/*****************************************************************************
*
*  TITLE:        Compare Files
*
*  DESCRIPTION:  The function Sorted_List<File*>:: "Compare"
*                compares two files and returns the result.
*                If the first file is less than the second one,
*                it returns a negative number.
*                If the first is greater than the second, it
*                returns a  positive number.
*                If they are equal, it returns 0.
*
*                The global variable "order" is used to hold
*                the keys.
*
*  REFERENCE:    None.
*
*****************************************************************************/

int  Sorted_List<File_Info*>:: Compare(

   File_Info* file_1,
   File_Info* file_2)

{
   int comparison = 0;

   for (int i = 0;
        (comparison == 0) && (order[i] != 0);
        i++)
   {
      switch (tolower(order[i]))
      {
         case 'n':
            comparison = strcmp(file_1->name, file_2->name);
            break;

         case 'e':
            comparison = strcmp(file_1->extension, file_2->extension);
            break;

         case 'p':
            comparison = strcmp(file_1->path, file_2->path);
            break;

         case 'd':
            if      (file_1->date < file_2->date) comparison = -1;
            else if (file_1->date > file_2->date) comparison = +1;
            break;

         case 't':
            if      (file_1->time < file_2->time) comparison = -1;
            else if (file_1->time > file_2->time) comparison = +1;
            break;

         case 'f':
            comparison = stricmp(file_1->name, file_2->name);
            if (comparison) break;

            comparison = stricmp(file_1->extension, file_2->extension);
            break;

         case 's':
            if      (file_1->size < file_2->size) comparison = -1;
            else if (file_1->size > file_2->size) comparison = +1;
            break;

      }   // switch

      if (order[i] < 'a') comparison = -comparison;

   }   //for each sort field

   return comparison;

}   // Sorted_List<File*>:: Compare

/*****************************************************************************
*
*  TITLE:        Copy With Spaces
*
*****************************************************************************/

void Copy_With_Spaces(

   char* source,                // source string for copy
   char* target,                // destination string for copy
   UINT  target_length)         // size of destination string

{
   target_length--;             // pre-decrement to leave room for terminator

   while ( (target_length-- > 0) &&
           (*source != 0) )
   {
      if (*source == '%')
      {
         if (source[1] == 0)
         {
            *target++ = *source++;
         }
         else if (source[1] == space_character)
         {
            *target++ = space_character;
            source += 2;
         }
         else
         {
            *target++ = *source++;
         }
      }
      else if (*source == space_character)
      {
         *target++ = ' ';
         source++;
      }
      else
      {
         *target++ = *source++;
      }

   }   // while

   *target = 0;

}   // Copy_With_Spaces

/*****************************************************************************
*
*  TITLE:        Compare Strings
*
*  DESCRIPTION:  The function Sorted_List<String*>:: "Compare"
*                compares two strings and returns the result.
*                If the first string is less than the second one,
*                it returns a negative number.
*                If the first is greater than the second, it
*                returns a  positive number.
*                If they are equal, it returns 0.
*
*  REFERENCE:    None.
*
*****************************************************************************/

int Sorted_List<char*>:: Compare(

   char* string_1,
   char* string_2)

{

   return strcmp(string_1, string_2);

}   // Sorted_List<String*>:: Compare


/*****************************************************************************
*
*  TITLE:        Fill Out Order String
*
*****************************************************************************/

void Fill_Out_Order_String(

   char* order_string)          // string with partial list of sort keys

{
   char p = 0,  // these flags are set when their corresponding character
        n = 0,  //    is found within the order string
        e = 0,
        s = 0,
        d = 0,
        t = 0;

   while (*order_string)
   {
      switch (tolower(*order_string))
      {
         case 'p':
            p = 1;
            break;

         case 'n':
            n = 1;
            break;

         case 'e':
            e = 1;
            break;

         case 'f':
            n = 1;
            e = 1;
            break;

         case 'd':
            d = 1;
            break;

         case 't':
            t = 1;
            break;

         case 's':
            s = 1;
            break;

      }   //switch

      order_string++;

   }   //while

   if (!n) *order_string++ = 'n';
   if (!e) *order_string++ = 'e';
   if (!p) *order_string++ = 'p';
   if (!d) *order_string++ = 'd';
   if (!t) *order_string++ = 't';
   if (!s) *order_string++ = 's';

   *order_string = 0;

}   // Fill_Out_Order_String


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


   //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
   //!!!!
   //!!!!  This is where you place your own normal arguments...
   //!!!!
   //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   Load_Files(argument, files);
   number_of_arguments_loaded++;

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

      int last_character = strlen(line) - 1;
      while ((last_character >= 0) && isspace(line[last_character]))
      {
         line[last_character--] = 0;
      }

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
*  TITLE:        Load Files
*
*****************************************************************************/

int  Load_Files(

   char* file_name_mask,        // file name (possibly with wildcards)
   File_Table& file_list,       // list of files
   char check_excludes)         // whether to check the excludes list

{
   struct ffblk file_block;
   File_Info* file;
   int done;
   int location = 0;
   char drive[MAXDRIVE] = "";
   char directory[MAXDIR] = "";
   char name[MAXFILE] = "";
   char extension[MAXEXT] = "";
   char path[0x80] = "";
   char* path_ptr;
   int  flags;
   char inserted;
   int  number_of_files = 0;

   flags = fnsplit(file_name_mask, drive, directory, name, extension);
   if (flags & DRIVE)     strcat(path, drive);
   if (flags & DIRECTORY) strcat(path, directory);

   if (path[0] == 0)
   {
      strcpy(path, ".\\");
   }
   path_ptr = New_String(path);
   if (!paths.Insert(path_ptr))
   {
      delete[] path_ptr;
   }
   paths.Search(path, location);
   path_ptr = paths.At(location);

   if (verbose)
   {
      printf("argument:  \"%s\"\n", file_name_mask);
      printf("   flags:     ");
      printf("drive%c ",      (flags & DRIVE)     ? '+' : '-');
      printf("directory%c ",  (flags & DIRECTORY) ? '+' : '-');
      printf("name%c ",       (flags & FILENAME)  ? '+' : '-');
      printf("extension%c ",  (flags & EXTENSION) ? '+' : '-');
      printf("wildcards%c\n", (flags & WILDCARDS) ? '+' : '-');
      printf("   drive:     \"%s\"\n", drive);
      printf("   directory: \"%s\"\n", directory);
      printf("   name:      \"%s\"\n", name);
      printf("   extension: \"%s\"\n", extension);
   }   //if verbose

   done = findfirst(file_name_mask, &file_block, 0xFF);

      //  check for explicit directory name.
      //  if there is one, load that directory.

   if ( (!done) &&
        ((flags & WILDCARDS) == 0) &&
        ((file_block.ff_attrib & FA_DIREC) != 0) )
   {
      sprintf(path, "%s%s", path_ptr, file_block.ff_name);
      path_ptr = New_String(path);
      if (!paths.Insert(path_ptr))
      {
         delete[] path_ptr;
      }
      paths.Search(path, location);
      path_ptr = paths.At(location);

      strcat(path, "\\*.*");

      done = findfirst(path, &file_block, 0xFF);
   }

   while (!done)
   {
      if ( ((file_block.ff_attrib & FA_DIREC) == 0) ||
           including_directories )
      {
         number_of_files++;

         file = new File_Info(path_ptr, &file_block);

         inserted = 0;

         if (check_excludes)
         {
            if (excludes.Search(file, location) == 0)
            {
               inserted = file_list.Insert(file);
            }
         }
         else
         {
            inserted = file_list.Insert(file);
         }

         if (!inserted)
         {
            delete file;
         }

      }   // if need to include file

      done = findnext(&file_block);

   }   // while

   if ( (number_of_files == 0) &&
        warnings_are_enabled )
   {
      printf("warning: no matches found for \"%s\"\n", file_name_mask);
   }

   return 1;

}   // Load_Files


/*****************************************************************************
*
*  TITLE:        Load Flag
*
*****************************************************************************/

void Load_Flag(

   char* flag,          // pointer to flag variable
   char** string)       // string to check for +/-

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
         case 'h':
         case 'H':
            return commandline_error;   // forces printing of usage info

         case 'a':
            Copy_With_Spaces(
                 option,
                 attribute_format,
                 sizeof(attribute_format));
            ADVANCE_TO_END(option);
            break;

         case 'd':
            Copy_With_Spaces(option, date_format, sizeof(date_format));
            ADVANCE_TO_END(option);
            break;

         case 'D':
            Load_Flag(&including_directories, &option);
            break;

         case 'f':
            Copy_With_Spaces(option, format, sizeof(format));
            ADVANCE_TO_END(option);
            break;

         case 'L':
            Load_Flag(&use_lowercase, &option);
            if (use_lowercase)
            {
               use_uppercase = 0;
            }
            break;

         case 'O':
            Load_Flag(&use_octal, &option);
            break;

         case 's':
            strNcpy(order, option, sizeof(order));
            Fill_Out_Order_String(order);
            ADVANCE_TO_END(option);
            break;

         case 'S':
            space_character = *option;
            if (*option != NULL)
            {
               option++;
            }
            break;

         case 't':
            Copy_With_Spaces(option, time_format, sizeof(time_format));
            ADVANCE_TO_END(option);
            break;

         case 'T':
            Load_Flag(&total, &option);
            break;

         case 'U':
            Load_Flag(&use_uppercase, &option);
            if (use_uppercase)
            {
               use_lowercase = 0;
            }
            break;

         case 'x':
            if (verbose)
            {
               printf("loading exclude files \"%s\"\n", option);
            }
            Load_Files(option, excludes, 0);
            ADVANCE_TO_END(option);
            break;

         case 'X':
            Load_Flag(&use_hexadecimal, &option);
            break;

         case 'v':
            Load_Flag(&verbose, &option);
            break;

         case 'W':
            Load_Flag(&warnings_are_enabled, &option);
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

   if (verbose)
   {
      printf("order for sort: \"%s\"\n", order);
      printf("full format:    \"%s\"\n", format);
      printf("date format:    \"%s\"\n", date_format);
      printf("time format:    \"%s\"\n", time_format);
      printf("attr format:    \"%s\"\n", attribute_format);
   }

   if (number_of_arguments_loaded == 0)
   {
      Load_Files("*.*", files, 1);
   }

   char output_string[0x100];

   for (int i = 0;
        (i < files.Count());
        i++)
   {
      Write_File_Information_To_String(
           files.At(i),
           format,
           output_string);
      printf("%s\n", output_string);
   }

   if (total)
   {
      int count = files.Count();
      long total_size = files.total_size;

      if (use_hexadecimal)
      {
         if (use_uppercase)
         {
            printf("  Files=%X  Bytes=%lX\n", count, total_size);
         }
         else
         {
            printf("  Files=%x  Bytes=%lx\n", count, total_size);
         }
      }
      else if (use_octal)
      {
         printf("  Files=%o  Bytes=%lo\n", count, total_size);
      }
      else
      {
         printf("  Files=%u  Bytes=%lu\n", count, total_size);
      }
   }

   return success;

}   // main

/*****************************************************************************
*
*  TITLE:        New String
*
*  DESCRIPTION:  The function "New_String"
*                allocates a copy of the given string.
*
*  REFERENCE:    None.
*
*****************************************************************************/

char* New_String(

   const char* string_to_copy)          // source string to copy

{
   char* copy_of_string;

   if (string_to_copy == NULL)
   {
      return NULL; //-----------------------------------------> return!
   }

   copy_of_string = new char[strlen(string_to_copy) + 1];

   if (copy_of_string != NULL)
   {
      strcpy(copy_of_string, string_to_copy);
   }

   return copy_of_string;

}   // New_String

/*****************************************************************************
*
*  TITLE:        Usage
*
*****************************************************************************/

void Usage(void)

{
   printf("\n");
   printf("Usage: dr [switches] [list-of-files]\n");
   printf("Options -- see notes below -- [default]:\n");
   printf("   @<arg_file> file from which to read arguments and options\n");
   printf("   -a<format>  attribute format [%s]\n", ATTRIBUTE_FORMAT);
   printf("                  r=read only    a=archive     h=hidden\n");
   printf("                  d=directory    s=system      l=volume label\n");
   printf("   -d<format>  date format [%s]\n", DATE_FORMAT);
   printf("                  y=full year    Y=2-digit year  m=month number\n");
   printf("                  M=month name   d=day of month  j=Julian day\n");
   printf("                  w=day of week (0-6)     W=day of week name\n");
   printf("   -D[+/-]     include directories in list [%s]\n",
                             minus_plus[INCLUDING_DIRECTORIES]);
   printf("   -f<format>  main output format [%s]\n", FORMAT);
   printf("                  p=file path    n=file name     "
                            "e=file extension\n");
   printf("                  f=name+ext     s=file size     "
                            "a=file attributes\n");
   printf("                  d=file date    t=file time\n");
   printf("                  '%c' = space; use \"%%%c\" for '%c'; See -S\n",
                       SPACE_CHARACTER, SPACE_CHARACTER, SPACE_CHARACTER);
   printf("                  See also: -d, -t and -a switches\n");
   printf("   -L[+/-]     use lower case for everything [%s]\n",
                                  minus_plus[USE_LOWERCASE]);
   printf("   -O[+/-]     use octal for all numbers (except time)"
                              " [%s]\n", minus_plus[USE_OCTAL]);
   printf("   -s<order>   sort order (n,e,f,s,a,t,d), "
                                 "uppercase=reverse [%s]\n", ORDER);
   printf("                  where (n,e,f,s,a,t,d) are as in the -f option\n");
   printf("   -S<char>    character to translate into a space [%c]\n",
                                  SPACE_CHARACTER);
   printf("   -t<format>  time format [%s]\n", TIME_FORMAT);
   printf("                  h=24-hour clock (0-23)  m=minute   s=second\n");
   printf("                  H=12-hour clock (1-12)  a/p/A/P=am/pm (no m)\n");
   printf("   -T[+/-]     display totals [%s]\n", minus_plus[TOTAL]);
   printf("   -U[+/-]     use upper case for everything [%s]\n",
                                  minus_plus[USE_UPPERCASE]);
   printf("   -v[+/-]     verbose output [%s]\n", minus_plus[VERBOSE]);
   printf("   -W[+/-]     display warnings [%]\n",
                           minus_plus[WARNINGS_ARE_ENABLED]);
   printf("   -x<file>    file(s) to exclude\n");
   printf("   -X[+/-]     use hexadecimal for all numbers (except time)"
                              " [%s]\n", minus_plus[USE_HEXADECIMAL]);
   printf("Format qualifiers are like C printf format strings, and"
                               " may include:\n");
   printf("   '-'=left justification    '0'=zero-pad    "
             "'u'=upper case strings\n");
   printf("   'l'=lower case strings\n");
   printf("Examples:\n");
   printf("   dr \"-t%%2H:%%02m %%am\" -f%%-12f__%%t *.c\n");
   printf("      Output: fun.c          3:34 pm\n");
   printf("              leftjust.c    12:08 am\n");
   printf("   dr -S# -T- -ftlib#fun.lib#-+#obj_dir\\%%n.obj src_dir\\*.c\n");
   printf("      Output: tlib fun.lib -+obj_dir\\fun_util.obj\n");
   printf("              tlib fun.lib -+obj_dir\\othersrc.obj\n");
   printf("      This could be redirected to a batch file, then run.\n");
}   // Usage


/*****************************************************************************
*
*  TITLE:        Write Attribute To String
*
*****************************************************************************/

void Write_Attribute_To_String(

   uchar attribute,     // file attribute (as collection of bits)
   char* format,        // format descriptor for attribute
   char* target)        // target string to hold formatted attribute

{
   char ac;

   while (*format)
   {
      switch (*format)
      {
         case '%':
            format++;
            if (*format == '%')
            {
               *target++ = '%';
               break; //--------------------------------------> break!
            }

            ac = 0;

            if (strchr("rRaAhHdDsSlL",*format) == NULL)
            {
              if (*format == '_')
              {
                 ac = ' ';
              }
              else
              {
                 ac = *format;
              }
              format++;
            }

            switch (*format)
            {
               case 'd':
                  if (attribute & FA_DIREC)
                  {
                     ac = 'd';
                  }
                  break;

               case 'r':
                  if (attribute & FA_RDONLY)
                  {
                     ac = 'r';
                  }
                  break;

               case 'a':
                  if (attribute & FA_ARCH)
                  {
                     ac = 'a';
                  }
                  break;

               case 'h':
                  if (attribute & FA_HIDDEN)
                  {
                     ac = 'h';
                  }
                  break;

               case 's':
                  if (attribute & FA_SYSTEM)
                  {
                     ac = 's';
                  }
                  break;

               case 'l':
                  if (attribute & FA_LABEL)
                  {
                     ac = 'l';
                  }
                  break;

               case 'D':
                  if (attribute & FA_DIREC)
                  {
                     ac = 'D';
                  }
                  break;

               case 'R':
                  if (attribute & FA_RDONLY)
                  {
                     ac = 'R';
                  }
                  break;

               case 'A':
                  if (attribute & FA_ARCH)
                  {
                     ac = 'A';
                  }
                  break;

               case 'H':
                  if (attribute & FA_HIDDEN)
                  {
                     ac = 'H';
                  }
                  break;

               case 'S':
                  if (attribute & FA_SYSTEM)
                  {
                     ac = 'S';
                  }
                  break;

               case 'L':
                  if (attribute & FA_LABEL)
                  {
                     ac = 'L';
                  }
                  break;

               case 0:
                  format--;
                  break;

            }   // switch on character following '%'

            if (ac != 0) *target++ = ac;
            break;

         default:
           *target++ = *format;

      }   // main switch

      format++;

   }   // while

   *target = 0;

}   // Write_Attribute_To_String


/*****************************************************************************
*
*  TITLE:        Write File Information To String
*
*****************************************************************************/

void Write_File_Information_To_String(

   File_Info* file,     // file whose info is to be written
   char* format,        // format descriptor for output to target
   char* target)        // string to hold result

{
   char c;
   char width_is_negative;
   char field_is_hexadecimal;
   char field_is_lowercase;
   char field_is_numeric;
   char field_is_octal;
   char field_is_uppercase;
   unsigned long field_number;
   int field_width=0;
   char field[0x80];

   while (*format)
   {
      c = *format++;

      switch(c)
      {
         case '%':
            if (*format == '%')
            {
               *target++ = *format++;
               break; //--------------------------------------> break!
            }

            field[0] = 0;
            field_width = 0;
            width_is_negative = 0;
            field_is_hexadecimal = use_hexadecimal;
            field_is_lowercase = use_lowercase;
            field_is_numeric = 0;
            field_is_octal = 0;
            field_is_uppercase = use_uppercase;

            if (*format == '-')
            {
               width_is_negative = 1;
               format++;
            }

            while ((*format >= '0') && (*format <= '9'))
            {
               field_width *= 10;
               field_width += (*format - '0');
               format++;
            }

            if (width_is_negative)
            {
               field_width = -field_width;
            }

            if (*format == 'u')
            {
               field_is_uppercase = 1;
               format++;
            }
            else if (*format == 'l')
            {
               field_is_lowercase = 1;
               format++;
            }

            if (*format == 'x')
            {
               field_is_hexadecimal = 1;
               field_is_lowercase = 1;
            }
            else if (*format == 'X')
            {
               field_is_hexadecimal = 1;
               field_is_uppercase = 1;
            }
            else if (*format == 'o')
            {
               field_is_octal = 1;
            }

            switch (*format++)
            {
               case 0:
                  strcpy(field, "%");
                  break;

               case 'p':
                  strcpy(field, file->path);
                  break;

               case 'n':
                  strtocstr(field, sizeof(field), file->name);
                  break;

               case 'e':
                  strtocstr(field, sizeof(field), file->extension);
                  break;

               case 'f':
                  strtocstr(field, sizeof(field), file->name);

                  if (file->extension[0] != 0)  // need to write extension
                  {
                     int length_so_far = strlen(field);

                     if (length_so_far < sizeof(field))
                     {
                        field[length_so_far++] = '.';
                     }

                     if (length_so_far < sizeof(field))
                     {
                        strtocstr(&field[length_so_far],
                                  (sizeof(field) - length_so_far),
                                  file->extension);
                     }
                  }
                  break;

               case 's':
                  field_number = file->size;
                  field_is_numeric = 1;
                  break;

               case 'a':
                  Write_Attribute_To_String(
                       file->attribute,
                       attribute_format,
                       field);
                  break;

               case 'd':
                  file->date.Image(field, date_format);
                  break;

               case 't':
                 file->time.Image(field, time_format);
                 break;

               }   //switch on format type

               if (field_is_numeric)
               {
                  if (field_is_hexadecimal)
                  {
                     sprintf(field, "%lx", field_number);
                  }
                  else if (field_is_octal)
                  {
                     sprintf(field, "%lo", field_number);
                  }
                  else
                  {
                     sprintf(field, "%lu", field_number);
                  }
               }

               if (field_is_lowercase)
               {
                  strlwr(field);
               }
               else if (field_is_uppercase)
               {
                  strupr(field);
               }

               sprintf(target, "%*s", field_width, field);
               ADVANCE_TO_END(target);

               break;

         default:
            *target++ = c;

      }   //switch

   }   //while

   *target = 0;

}   // Write_File_Information_To_String



