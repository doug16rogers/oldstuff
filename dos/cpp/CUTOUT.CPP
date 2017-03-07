#include <ctype.h>
#include <dir.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "icitypes.h"


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
   success = 0,         // program ran successfully

   commandline_error,   // error in a commandline parameter or switch
   could_not_open_input_file,   // input file not openable
   could_not_open_output_file,  // output file not openable
   could_not_write_all_units,   // could not write all requested units
   could_not_goto_offset,       // could not reach desired offset

} DOS_Return_Codes;

//===========================================================================
//
//  Global Data...
//
//===========================================================================

char run_file[MAXNAME];         // name for executing program
char run_path[MAXPATH];         // directory of file that was executed

#define BYTES_PER_UNIT  1
#define FILE_OFFSET     0UL
#define ALL_UNITS       0xFFFFFFFFUL
#define NUMBER_OF_UNITS ALL_UNITS

FILE* input_file = NULL;
BOOLEAN offset_changed = false;
UINT16 output_file_count = 0;

UINT16 bytes_per_unit = BYTES_PER_UNIT;
UINT32 file_offset = FILE_OFFSET;
UINT32 number_of_units = NUMBER_OF_UNITS;

const char* endian_name[] = { "m", "i" };   // Motorola/Intel byte order
const char* minus_plus[] = { "-", "+" };    // text for flags


//===========================================================================
//
//  Function Prototypes...
//
//===========================================================================

/*****************************************************************************
*
*  TITLE:        Extract To File
*
*  DESCRIPTION:  The subprogram "Extract_To_File"
*                extracts from the input file into a
*                new file of the given name.
*
*                It returns one of the program return codes,
*                success if ok.
*
*  REFERENCE:    None.
*
*****************************************************************************/

DOS_Return_Codes Extract_To_File(       // whether successful

     char* file_name);          // name of target (output) file

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


//===========================================================================
//
//  Function Bodies...
//
//===========================================================================

/*****************************************************************************
*
*  TITLE:        Extract To File
*
*  DESCRIPTION:  The subprogram "Extract_To_File"
*                extracts from the input file into a
*                new file of the given name.
*
*                It returns one of the program return codes,
*                success if ok.
*
*  REFERENCE:    None.
*
*****************************************************************************/

DOS_Return_Codes Extract_To_File(       // whether successful

     char* file_name)           // name of target (output) file
{
   UINT32 byte_offset = file_offset * (UINT32) bytes_per_unit;

   if (offset_changed)
   {
      fseek(input_file, byte_offset, SEEK_SET);
      if (ftell(input_file) != byte_offset)
      {
         printf("could not set unit/byte offset 0x%lx/0x%lx in file \"%s\"\n",
              file_offset, byte_offset, file_name);
         return could_not_goto_offset; //---------------------> return!
      }
      offset_changed = false;
   }

   FILE* output_file = fopen(file_name, "wb");

   if (output_file == NULL)
   {
      printf("couldn't open output file \"%s\"\n", file_name);
      return could_not_open_output_file;
   }

   output_file_count++;

   const UINT16 max_buffer_size = 0x4000;
   static UINT8 buffer[max_buffer_size];

   UINT16 units_allowed_per_read = max_buffer_size / bytes_per_unit;

   UINT16 units_read;
   UINT32 units_read_so_far = 0UL;
   UINT16 units_to_read;
   UINT16 units_written;

   while (!feof(input_file) &&
          (units_read_so_far < number_of_units))
   {
      units_to_read = units_allowed_per_read;
      if (((UINT32) units_to_read) > (number_of_units - units_read_so_far))
      {
         units_to_read = (UINT16) (number_of_units - units_read_so_far);
      }
      units_read = fread(buffer, bytes_per_unit, units_to_read, input_file);

      if (units_read == 0) break; //--------------------------> break!

      units_written = fwrite(buffer, bytes_per_unit, units_read, output_file);
      units_read_so_far += units_written;

      if (units_written != units_read)
      {
         printf("could not write to output file \"%s\", "
                "at unit/byte 0x%lx/0x%lx\n",
                file_name,
                (UINT32) units_read_so_far,
                (UINT32) units_read_so_far * (UINT32) bytes_per_unit);
         break; //--------------------------------------------> break!
      }
   }

   fclose(output_file);

   if ( (units_read_so_far < number_of_units) &&
        (number_of_units != ALL_UNITS) )
   {
      printf("only 0x%lx/0x%lx units/bytes (of 0x%lx/0x%lx)"
             " were written to \"%s\"\n",
            units_read_so_far,
            units_read_so_far * (UINT32) bytes_per_unit,
            number_of_units,
            number_of_units * (UINT32) bytes_per_unit,
            file_name);
      return could_not_write_all_units; //--------------------> return!
   }

   return success;

}   // Extract_To_File

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

   if (input_file != NULL)
   {
      return Extract_To_File(argument);
   }

   input_file = fopen(argument, "rb");
   if (input_file == NULL)
   {
      printf("could not open input file \"%s\"\n", argument);
      return could_not_open_input_file;
   }

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
   int line_length;
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
      if (fgets(line, sizeof(line), file) == NULL)
      {
         break; //--------------------------------------------> break!
      }
      TERMINATE(line);

      line_length = strlen(line);
      if (line_length == 0) continue;

      if (line[line_length - 1] == '\n')
      {
         line_length--;
         if (line_length == 0) continue;
         line[line_length] = 0;
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
   while (*option)
   {
      switch (*option++)
      {
         case '?':
            return commandline_error;

         case '1':
            bytes_per_unit = 1;
            break;

         case '2':
            bytes_per_unit = 2;
            break;

         case '4':
            bytes_per_unit = 4;
            break;

         case 'b':
            if (*option == 'x')
            {
               option++;
               if ( (sscanf(option, "%x", &bytes_per_unit) != 1) ||
                    (bytes_per_unit < 1) ||
                    (bytes_per_unit > 32) )
               {
                  printf("bad hex number of bytes \"%s\"\n", option);
                  return commandline_error;
               }
               while (isdigit(*option)) option++;
            }
            else
            {
               if ( (sscanf(option, "%u", &bytes_per_unit) != 1) ||
                    (bytes_per_unit < 1) ||
                    (bytes_per_unit > 32) )
               {
                  printf("bad decimal number of bytes \"%s\"\n", option);
                  return commandline_error;
               }
               while (isdigit(*option)) option++;
            }
            break;

         case 'f':
            if (input_file != NULL)
            {
               fclose(input_file);
            }

            input_file = fopen(option, "rb");
            if (input_file == NULL)
            {
               printf("could not open input file \"%s\"\n", option);
               return could_not_open_input_file;
            }
            ADVANCE_TO_END(option);
            break;

         case 'n':
            if (*option == '*')
            {
               option++;
               number_of_units = ALL_UNITS;
               break;
            }
            if (*option == 'x')
            {
               option++;
               if (sscanf(option, "%lx", &number_of_units) != 1)
               {
                  printf("bad hex number of units \"%s\"\n", option);
                  return commandline_error;
               }
               while (isxdigit(*option)) option++;
            }
            else
            {
               if (sscanf(option, "%lu", &number_of_units) != 1)
               {
                  printf("bad decimal number of units \"%s\"\n", option);
                  return commandline_error;
               }
               while (isdigit(*option)) option++;
            }
            break;

         case 'o':
            if (*option == 'x')
            {
               option++;
               if (sscanf(option, "%lx", &file_offset) != 1)
               {
                  printf("bad hex file offset (in units) \"%s\"\n", option);
                  return commandline_error;
               }
               while (isxdigit(*option)) option++;
            }
            else
            {
               if (sscanf(option, "%lu", &file_offset) != 1)
               {
                  printf("bad decimal file offset (in units) \"%s\"\n",
                        option);
                  return commandline_error;
               }
               while (isdigit(*option)) option++;
            }
            offset_changed = true;
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
      if (return_code == commandline_error)
      {
         Usage();
      }
      return return_code; //----------------------------------> return!
   }

   if (input_file == NULL)
   {
      printf("no input file given\n");
      Usage();
      return commandline_error; //----------------------------> return!
   }

   if (output_file_count == 0)
   {
      printf("no output files specified\n");
      Usage();
      return commandline_error;
   }

   fclose(input_file);

   return success;

}   // main


/*****************************************************************************
*
*  TITLE:        Usage
*
*****************************************************************************/

void Usage(void)

{
   char number_of_units_image[0x10] = "*";

   #if (NUMBER_OF_UNITS != ALL_UNITS)
      sprintf("x%08lx", (UINT32) NUMBER_OF_UNITS);
   #endif

   printf(

      "\n"
      "Usage: %s [options] <input_file> [options and <output_files...>]\n"
      "Where <input_file> is the file from which to extract, and\n"
      "[options and <output_files...>] specify target files and options.\n"
      "The input file may be changed on the fly by using -f, which will\n"
      "also reset the offset to 0.\n"
      "NOTE: Each output file is written with the most recent options.\n"
      "\n"
      "Options must begin with '-' or '/' (defaults in []):\n"
      "   @<file>       read more arguments and options from <file>\n"
      "   -?            display usage information\n"
      "   -1            one byte per unit (-b4)\n"
      "   -2            two bytes per unit (-b2)\n"
      "   -4            four bytes per unit (-b1)\n"
      "   -f<input>     new input file to use\n"
      "   -b[x]<bytes>  bytes per unit [%u]\n"
      "   -n[x]<count>  number of units to extract "
                             "('*'='x%lx'=all) [%s]\n"
      "   -o[x]<offset> starting unit offset into file [x%lx]\n"
      "Specify 'x' after -b, -n or -o for hexadecimal instead of decimal.\n"

      ,run_file

      ,BYTES_PER_UNIT
      ,(UINT32) ALL_UNITS, number_of_units_image
      ,(UINT32) FILE_OFFSET
   );

}   // Usage


