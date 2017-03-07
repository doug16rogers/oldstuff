#include <ctype.h>
#include <dir.h>
#include <stdio.h>
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
   could_not_open_image_file,
   could_not_open_map_file,

} DOS_Return_Codes;


typedef struct
{
   UINT16 length;
   char name;

} BIT_RECORD;

//===========================================================================
//
//  Global Data...
//
//===========================================================================

#define FORMAT          "M4Y12m6h5D5"

char run_file[MAXNAME];         // name for executing program
char run_path[MAXPATH];         // directory of file that was executed

BIT_RECORD bit[0x40];
UINT16 bit_records = 0;
char* format = FORMAT;
UINT16 last_bit = 0;

const char* minus_plus[] = { "-", "+" };  // text for flags


//===========================================================================
//
//  Function Prototypes...
//
//===========================================================================

/*****************************************************************************
*
*  TITLE:        Create Bit Records
*
*  DESCRIPTION:  The subprogram "Create_Bit_Records"
*                adds the bit fields specified in the
*                string to the global bit field records.
*
*                The string is incremented on each character
*                correctly scanned.
*
*                If errors are encountered, it returns
*                false, else true.
*
*  REFERENCE:    None.
*
*****************************************************************************/

BOOLEAN Create_Bit_Records(     // whether or not successful

     char** format);            // pointer to string holding format

/*****************************************************************************
*
*  TITLE:        Digit Value
*
*  DESCRIPTION:  The subprogram "Digit_Value"
*                returns the value of the character,
*                which can be '0'..'9', 'a'..'z' or 'A'..'Z'.
*                The values range from 0..35, with 36
*                indicating an error.
*
*  REFERENCE:    None.
*
*****************************************************************************/

inline UINT Digit_Value(        // value of digit, 0-35; 36 on error

     char character)            // character whose value should be taken
{
   UINT value = 36;

   if ((character >= '0') && (character <= '9'))
   {
      value = character - '0';
   }
   else if ((character >= 'a') && (character <= 'z'))
   {
      value = character - 'a' + 10;
   }
   else if ((character >= 'A') && (character <= 'A'))
   {
      value = character - 'A' + 10;
   }

   return value;

}   // Digit_Value

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
*  TITLE:        Read Number
*
*  DESCRIPTION:  The subprogram "Read_Number"
*                scans the given string (which may
*                begin with an 'x'/'X' to use hex),
*                reading in the integer.
*
*                It returns true on success, false otherwise.
*
*  REFERENCE:    None.
*
*****************************************************************************/

BOOLEAN Read_Number(    // whether successful

     char** string,     // pointer to string from which to read
     void* number,      // number to read
     UINT16 size = sizeof(UINT16));     // size of number: 1, 2 or 4 bytes

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
*  TITLE:        Create Bit Records
*
*****************************************************************************/

BOOLEAN Create_Bit_Records(     // whether or not successful

     char** format)             // pointer to string holding format

{
   const char* valid_field_names = "YMDhms";

   while ( (**format != 0) &&
           (bit_records < (sizeof(bit) / sizeof(bit[0]))) )
   {
      if (strchr(valid_field_names, **format) == NULL)
      {
         break; //--------------------------------------------> break!
      }

      BIT_RECORD* record = &bit[bit_records];
      record->name = **format;
      (*format)++;

      if (**format == 0)
      {
         return false; //-------------------------------------> return!
      }

      if (!Read_Number(format, &record->length))
      {
         break; //--------------------------------------------> break!
      }
      bit_records++;

   }   // while

   return (BOOLEAN) (**format == 0);

}   // Create_Bit_Records


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
      if (fgets(line, sizeof(line), file) == NULL)
      {
         break; //--------------------------------------------> break!
      }
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
            return commandline_error;

         case 'f':
            bit_records = 0;
            if (!Create_Bit_Records(&option))
            {
               printf("error in format bit field spec \"%s\"\n", option);
               return commandline_error;
            }
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

   Create_Bit_Records(&format);

   return_code = Initialize(count, argument_list);

   if (return_code != success)
   {
      Usage();
      return return_code; //----------------------------------> return!
   }

   printf("--------------------------------------\n");
   printf("bit records = 0x%x\n", bit_records);
   for (UINT16 i = 0; i < bit_records; i++)
   {
      printf("%x: %c %u\n", i, bit[i].name, bit[i].length);
   }

   //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
   //!!!!
   //!!!!  Insert your application's code here...
   //!!!!
   //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   return success;

}   // main


/*****************************************************************************
*
*  TITLE:        Read Number
*
*****************************************************************************/

BOOLEAN Read_Number(    // whether successful

     char** string,     // pointer to string from which to read
     void* number,      // number to read
     UINT16 size)       // size of number (in bytes): 1, 2 or 4
{
   UINT16 base = 10;

   if ( (*string == NULL) ||
        !((size == 1) || (size == 2) || (size == 4)) )
   {
      return false; //----------------------------------------> return!
   }

   if (tolower(**string) == 'x')
   {
      (*string)++;
      base = 16;
   }

   UINT16 digit_value = Digit_Value(**string);

   if (digit_value >= base)
   {
      return false; //----------------------------------------> return!
   }

   UINT32 value = 0;
   do
   {
      value *= base;
      value += digit_value;
      (*string)++;
      digit_value = Digit_Value(**string);

   } while (digit_value < base);

   switch (size)
   {
      case 1: *(UINT8*) number = value;  break;
      case 2: *(UINT16*) number = value;  break;
      case 4: *(UINT32*) number = value;  break;
   }

   return true;

}   // Read_Number

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
      "Usage: %s [options] <image_file>\n"
      "\n"
      "Options must begin with '-' or '/' (defaults in []):\n"
      "   @<file>       read more arguments and options from <file>\n"
      "   -?            display usage information\n"
      "   -d<date>      date to insert: 'YYYY.MM.DD', 'i' = image date [%s]\n"
      "   -f<format>    format of date/time: '<field><bits>' [%s]\n"
      "                    where field may be:\n"
      "                    'Y' = year     'M' = month      'D' = day\n"
      "                    'h' = hour     'm' = minute     's' = second\n"
      "                    '+' = set (1)  '-' = reset (0)  'x' = skip\n"
      "   -m[map_file]  use map file for offset [<image_file_name>.map]\n"
      "   -o[x]<offset> byte offset into image file where to place time\n"
      "   -t<time>      time to insert: 'hh:mm', 'i' = image time [%s]\n"

      ,run_file
      ,"i"
      ,FORMAT
      ,"i"

   );

}   // Usage

