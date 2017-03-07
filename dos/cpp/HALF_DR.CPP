#include <stdio.h>
#include <dir.h>
#include <dos.h>
#include <io.h>
#include <string.h>

#include <ttypes.h>     // from Turbo Vision

#include "datetime.hpp"
#include "fileinfo.hpp"
#include "templist.hpp"


typedef Sorted_List<File*> File_Table;

typedef Sorted_List<String*> String_Table;



#define DEFAULT_FILES  "*.*"

#define ATTRIBUTE_FORMAT        "%-d%-r%-a%-h"
#define DATE_FORMAT    "%4Y.%3M.%02d"
#define FORMAT         "%-8n.%-3e_%7s_%d_%t_%a_%p"
#define LOWERCASE      1
#define ORDER          "nepdts"
#define TIME_FORMAT    "%2H:%02m:%02s"
#define TOTAL          1
#define VERBOSE        0

char lowercase = LOWERCASE;
char format[0x80] = FORMAT;
char order[0x80] = ORDER;
char attribute_format[0x80] = ATTRIBUTE_FORMAT;
char date_format[0x80] = DATE_FORMAT;
char time_format[0x80] = TIME_FORMAT;
char total=TOTAL;
char verbose=VERBOSE;
int  args=0;

File_Table files;
File_Table excludes;
String_Table paths;

const char* lowercase_str[] = { "C",  "c" };
const char* minus_plus[] = { "-",  "+" };
const char* name_of_month[] =
{
   "???", 
   "January", 
   "February",
   "March",
   "April",
   "May",
   "June",
   "July",
   "August",
   "September",
   "October",
   "November",
   "December"
};   //month


//===========================================================================
void ToFile(File* f,struct ffblk* ff,char* path)
{
  int i;
  char *s;

  f->path = path;
  f->attr = ff->ff_attrib;
  f->size = ff->ff_fsize;
  f->year = ((struct ftime*)&ff->ff_ftime)->ft_year + 1980;
  f->mon  = ((struct ftime*)&ff->ff_ftime)->ft_month;
  f->day  = ((struct ftime*)&ff->ff_ftime)->ft_day;
  f->hour = ((struct ftime*)&ff->ff_ftime)->ft_hour;
  f->min  = ((struct ftime*)&ff->ff_ftime)->ft_min;
  f->sec  = ((struct ftime*)&ff->ff_ftime)->ft_tsec << 1;

  f->name[0] = 0;
  f->ext[0] = 0;
  s = ff->ff_name;
  if (f->attr & FA_DIREC) {
    if      (strcmp(s,"." ) == 0) strcpy(f->name,"." );
    else if (strcmp(s,"..") == 0) strcpy(f->name,"..");
  }
  if (f->name[0] == 0) {
    for (i=0;(i<8) && *s && (*s != '.');i++) f->name[i] = *s++;
    f->name[i] = 0;
    s++;
    for (i=0;(i<3) && *s;i++) f->ext[i] = *s++;
    f->ext[i] = 0;
  }
//  if (lowercase) { strlwr(f->name);  strlwr(f->ext); }
  if ((f->attr & FA_DIREC) == 0) {
    strlwr(f->name);
    strlwr(f->ext);
  }
}   //ToFile


//===========================================================================
int  LoadFileSpecs(FileTable& Table,char* s)
{
  struct ffblk ff;
  File file;
  int done;
  char drv[MAXDRIVE];
  char dir[MAXDIR];
  char nam[MAXFILE];
  char ext[MAXEXT];
  char path[0x80]="";
  char* path_ptr;
  int  flags;

  flags = fnsplit(s,drv,dir,nam,ext);
  if (flags & DRIVE)     strcat(path,drv);
  if (flags & DIRECTORY) strcat(path,dir);
  path_ptr = Paths.Insert(path);
  if (verbose) {
    printf(
      "argument:  \"%s\"\n"
      "  flags:     drv%c dir%c nam%c ext%c wildcards%c\n"
      "  drive:     \"%s\"\n"
      "  directory: \"%s\"\n"
      "  name:      \"%s\"\n"
      "  extension: \"%s\"\n"
      ,s
      ,(flags & DRIVE)     ? '+' : '-'
      ,(flags & DIRECTORY) ? '+' : '-'
      ,(flags & FILENAME)  ? '+' : '-'
      ,(flags & EXTENSION) ? '+' : '-'
      ,(flags & WILDCARDS) ? '+' : '-'
      ,drv,dir,nam,ext
    );
  }   //if verbose

  done=findfirst(s,&ff,0xFF);
  while (!done) {
    ToFile(&file,&ff,path_ptr);
    if (Excludes.Find(&file) == NULL) Table.Insert(&file);
    done = findnext(&ff);
  }
  return 1;
}   //LoadFileSpecs


############################################################################

#include <ctype.h>
#include <dir.h>
#include <stdio.h>
#include <string.h>


#define ADVANCE_TO_END(s)       while (*s) s++
#define MAXNAME                 (MAXFILE + MAXEXT)
#define TERMINATE(s)            s[sizeof(s)-1] = 0


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

   BYTE attribute,      // file attribute (as collection of bits)
   char* format,        // format descriptor for attribute
   char* target);       // target string to hold formatted attribute

/*****************************************************************************
*
*  TITLE:        Write Date To String
*
*  DESCRIPTION:  The procedure "Write_Date_To_String"
*                writes the given year, month and day of
*                month to the target string with the
*                specified format.
*
*  REFERENCE:    None.
*
*****************************************************************************/

void Write_Date_To_String(

   int year,            // year (including century, like 1999, not 99)
   int month,           // month of year (1-12)
   int day,             // day of month (1-31)
   char* format,        // source format descriptor for date
   char* target);       // target string to hold formatted date

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

int  Sorted_List<File*>:: Compare(

   File* file_1,
   File* file_2)

{
   int comparison = 0;

   for (int i = 0;
        (comparison == 0) && (order[i] != 0);
        i++)
   {
      switch (order[i])
      {
         case 'n':
            comparison = strcmp(file_1->name, file_2->name);
            break;

         case 'e':
            comparison = strcmp(file_1->ext, file_2->ext);
            break;

         case 'p':
            comparison = strcmp(file_1->path, file_2->path);
            break;

         case 'd':
            comparison = (file_1->year - file_2->year);
            if (comparison) break;

            comparison = (file_1->mon - file_2->mon);
            if (comparison) break;

            comparison = (file_1->day - file_2->day);
            break;

         case 't':
            comparison = (file_1->hour - file_2->hour);
            if (comparison) break;

            comparison = (file_1->min - file_2->min);
            if (comparison) break;

            comparison = (file_1->sec - file_2->sec);
            break;

         case 'f':
            comparison = stricmp(file_1->name, file_2->name);
            if (comparison) break;

            comparison = stricmp(file_1->ext, file_2->ext);
            break;

         case 's':
            if      (file_1->size < file_2->size) comparison = -1;
            else if (file_1->size > file_2->size) comparison = +1;
            break;

         case 'N':
            comparison = -strcmp(file_1->name, file_2->name);
            break;

         case 'E':
            comparison = -strcmp(file_1->ext, file_2->ext);
            break;

         case 'D':
            comparison = -(file_1->year - file_2->year);
            if (comparison) break;

            comparison = -(file_1->mon - file_2->mon);
            if (comparison) break;

            comparison = -(file_1->day - file_2->day);
            break;

         case 'T':
            comparison = -(file_1->hour - file_2->hour);
            if (comparison) break;

            comparison = -(file_1->min - file_2->min);
            if (comparison) break;

            comparison = -(file_1->sec - file_2->sec);
            break;

         case 'F':
            comparison = -stricmp(file_1->name, file_2->name);
            if (comparison) break;

            comparison = -stricmp(file_1->ext, file_2->ext);
            break;

         case 'S':
            if      (file_1->size < file_2->size) comparison = +1;
            else if (file_1->size > file_2->size) comparison = -1;
            break;

      }
   }   //for each sort field

   return comparison;

}   // Sorted_List<File*>:: Compare

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

int Sorted_List::<String*>:: Compare(

   String* string_1,
   String* string_2)

{

   return strcmp(*string_1, *string_2);

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

   if (!n) *ord++ = 'n';
   if (!e) *ord++ = 'e';
   if (!p) *ord++ = 'p';
   if (!d) *ord++ = 'd';
   if (!t) *ord++ = 't';
   if (!s) *ord++ = 's';

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
            return commandline_error;   -- forces printing of usage info

         case 'c':
            lowercase = 1;
            break;

         case 'C':
            lowercase = 0;
            break;

         case 'd':
            strNcpy(date_fmt, option, sizeof(date_fmt));
            ADVANCE_TO_END(option);
            break;

         case 't':
            strNcpy(time_fmt, option, sizeof(time_fmt));
            ADVANCE_TO_END(option);
            break;

         case 'a':
            strNcpy(attr_fmt, option, sizeof(attr_fmt));
            ADVANCE_TO_END(option);
            break;

         case 'f':
            strNcpy(format, option, sizeof(format));
            ADVANCE_TO_END(option);
            break;

         case 's':
            strNcpy(order,s,sizeof(order));
            Complete_Order_List(order);
            ADVANCE_TO_END(option);
            break;

         case 'x':
            if (verbose)
            {
               printf("loading exclude files...\n");
            }
            Load_File_Specs(Excludes, option);
            ADVANCE_TO_END(option);
            break;

         case 'T':
            Load_Flag(&total, &option);
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

   if (verbose)
   {
      printf("order for sort: \"%s\"\n", order);
      printf("full format:    \"%s\"\n", format);
      printf("date format:    \"%s\"\n", date_fmt);
      printf("time format:    \"%s\"\n", time_fmt);
      printf("attr format:    \"%s\"\n", attr_fmt);
   }

   //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
   //!!!!
   //!!!!  Insert your application's code here...
   //!!!!
   //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   if (verbose)
   {
      printf("loading filespecs...\n");
   }

   return success;

}   // main
{

  char outstring[0x80];
  for (File* f=Files.First();f!=NULL;f=Files.Next()) {
    ShowToString(outstring,f,format);
    printf("%s\n",outstring);
  }
  if (total) printf("  Files=%u  Bytes=%lu\n",Files.Count(),Files.Size());
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
      "\n"
      "Usage: dr [switches] [list-of-files]\n"
      "Switches [default]:\n"
      "   -f<format>  main output format (\"%%[-][0][<#>]<item>\"): [%s]\n"
      "                  where the field specifier (after the '%') can be:\n"
      "                     p=file path    n=file name  e=file extension\n"
      "                     f=name+ext     s=file size  a=file attributes\n"
      "                     d=file date    t=file time\n"
      "                  Use '_' for a space; use \"%_\" for '_'.\n"
      "                  See also: -d, -t and -a switches\n"
      "   -d<format>  date format:  Y=YYYY y=YY m=MM M=Mon d=DD [%s]\n"
      "   -t<format>  time format:  H=24 h=12 m=MM s=S a=a/p(am/pm) [%s]\n"
      "   -a<format>  attribute format (\"%%[<altchar>]<item>\"): [%s]\n"
      "                  where the field specifier (after the '%') can be:\n"
      "                     r=read only    a=archive     h=hidden\n"
      "                     d=directory    s=system      l=volume label\n"
      "   -s<order>   sort order (n,e,f,s,a,t,d), uppercase=reverse [%s]\n"
      "                  where (n,e,f,s,a,t,d) are as in the -f option\n"
      "   -T[+/-]     display totals [%s]\n"
      "   -x<file>    file(s) to exclude\n"
//      "   -c/C        use lower/upper case [%s]\n"
      "   -v[+/-]     verbose output [%s]\n"
      ,FORMAT
      ,DATE_FMT
      ,TIME_FMT
      ,ATTR_FMT
      ,ORDER
      ,minus_plus[TOTAL!=0]
//      ,lowercase_str[LOWERCASE!=0]
      ,minus_plus[VERBOSE!=0]
   );   //printf

}   // Usage


/*****************************************************************************
*
*  TITLE:        Write Attribute To String
*
*****************************************************************************/

void Write_Attribute_To_String(

   BYTE attribute,      // file attribute (as collection of bits)
   char* format,        // format descriptor for attribute
   char* target)        // target string to hold formatted attribute

{
   char ac;

   while (*format)
   {
      switch (*format)
      {
         case '_':
            *target++ = ' ';
            break;

         case '%':
            format++;
            if (*format == '%')
            {
               *target++ = '%';
               break; //--------------------------------------> break!
            }
            else if (*format == '_')
            {
               *target++ = '_';
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
                  if (attr & FA_DIREC)
                  {
                     ac = 'd';
                  }
                  break;

               case 'r':
                  if (attr & FA_RDONLY)
                  {
                     ac = 'r';
                  }
                  break;

               case 'a':
                  if (attr & FA_ARCH)
                  {
                     ac = 'a';
                  }
                  break;

               case 'h':
                  if (attr & FA_HIDDEN)
                  {
                     ac = 'h';
                  }
                  break;

               case 's':
                  if (attr & FA_SYSTEM)
                  {
                     ac = 's';
                  }
                  break;

               case 'l':
                  if (attr & FA_LABEL)
                  {
                     ac = 'l';
                  }
                  break;

               case 'D':
                  if (attr & FA_DIREC)
                  {
                     ac = 'D';
                  }
                  break;

               case 'R':
                  if (attr & FA_RDONLY)
                  {
                     ac = 'R';
                  }
                  break;

               case 'A':
                  if (attr & FA_ARCH)
                  {
                     ac = 'A';
                  }
                  break;

               case 'H':
                  if (attr & FA_HIDDEN)
                  {
                     ac = 'H';
                  }
                  break;

               case 'S':
                  if (attr & FA_SYSTEM)
                  {
                     ac = 'S';
                  }
                  break;

               case 'L':
                  if (attr & FA_LABEL)
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
*  TITLE:        Write Date To String
*
*  DESCRIPTION:  The procedure "Write_Date_To_String"
*                writes the given year, month and day of
*                month to the target string with the
*                specified format.
*
*  REFERENCE:    None.
*
*****************************************************************************/

void Write_Date_To_String(

   int year,            // year (including century, like 1999, not 99)
   int month,           // month of year (1-12)
   int day,             // day of month (1-31)
   char* format,        // source format descriptor for date
   char* target)        // target string to hold formatted date

{
   int field_size=0;
   char width_is_negative;
   char padding_with_zero;
   char c;
   char scrap[0x40];            // for local string manipulation

   if ((month < January) || (month > December))
   {
      month = 0;
   }

   while (*format)
   {
      switch (c=*format++) {
         case '_':
            *d++ = ' ';
            break;

         case '%':
            if (*format == '%') { *d++ = '%'; break; }
            field_size = 0;
            width_is_negative = 0;
            padding_with_zero = 0;

            if (*format == '-')
            {
               width_is_negative = 1;
               format++;
            }

            if (*format == '0')
            {
               padding_with_zero = 1;
               format++;
            }

            while ((*format >= '0') && (*format <= '9'))
            {
               field_size *= 10;
               field_size += (*format++ - '0');
            }

            if (field_is_negative)
            {
               field_size = -field_size;
            }

            switch (*format++)
            {
               case 'y':
                  if (padding_with_zero)
                  {
                     sprintf(target, "%0*u", field_size, year%100);
                  }
                  else
                  {
                     sprintf(target, "%*u", field_size, year%100);
                  }
                  ADVANCE_TO_END(target);
                  break;

               case 'Y':
                  if (padding_with_zero)
                  {
                     sprintf(target, "%0*u", field_size, year);
                  }
                  else
                  {
                     sprintf(target, "%*u", field_size, year);
                  }
                  ADVANCE_TO_END(target);
                  break;

               case 'm':
                  if (padding_with_zero)
                  {
                     sprintf(target, "%0*u", field_size, month);
                  }
                  else
                  {
                     sprintf(target, "%*u", field_size, month);
                  }
                  ADVANCE_TO_END(target);
                  break;

               case 'M':
                  strcpy(scrap, name_of_month[month]);
                  sprintf(target, "%*s", field_size, name_of_month[month]);
                  while (*d) d++;
                  break;

               case 'd':
                  if (padding_with_zero)
                  {
                     sprintf(target, "%0*u", field_size, day);
                  }
                  else
                  {
                     sprintf(target, "%*u", field_size, day);
                  }
                  ADVANCE_TO_END(target);
                  break;

               default:
                  *target++ = *format;

            }   // switch on field specifier
            break;

         default:
           *target++ = c;

      }   //switch

   }   //while

   *target = 0;

}   // Write_Date_To_String


void TimeToString(char* d,int hour,int min,int sec,char* format)
{
  int field_size=0;
  char neg;
  char zero;
  char c;

  while (*format) {
    switch (c=*format++) {
    case '_': *d++ = ' '; break;
    case '%':
      if (*format == '%') { *d++ = '%'; break; }
      field_size = 0;
      neg = 0;
      zero = 0;
      if (*format == '-') { neg  = 1; format++; }
      if (*format == '0') { zero = 1; format++; }
      while ((*format >= '0') && (*format <= '9')) {
        field_size *= 10;
        field_size += (*format++ - '0');
      }
      if (neg) field_size = -field_size;
      switch (*format++) {
      case 'H':
        if (zero)
          sprintf(target,"%0*u",field_size,hour);
        else
          sprintf(target,"%*u",field_size,hour);
        while (*d) d++;
        break;
      case 'h':
        int h = hour;
        if (h > 12) h -= 12;
        if (h == 0) h  = 12;
        if (zero)
          sprintf(target,"%0*u",field_size,h);
        else
          sprintf(target,"%*u",field_size,h);
        while (*d) d++;
        break;
      case 'm':
 ÿ      else
          sprintf(target,"%*u",field_size,min);
        while (*d) d++;
        break;
      case 's':
        if (zero)
          sprintf(target,"%0*u",field_size,sec);
        else
          sprintf(target,"%*u",field_size,sec);
        while (*d) d++;
        break;
      case 'a':
      case 'p':
        if (hour < 12) *d++ = 'a'; else *d++ = 'p';
        break;
      case 'A':
      case 'P':
        if (hour < 12) *d++ = 'A'; else *d++ = 'P';
        break;
      default:
        *d++ = *format;
      }   //switch on format
      break;
    default:
      *d++ = c;
    }   //switch
  }   //while
  *d=0;
}   //TimeToString


void ShowToString(char* d,File* f,char* format)
{
  char c;
  char neg;
  int field_size=0;
  char str[0x80];

  while (*format) {
    switch(c=*format++) {
    case '%':
      if (*format == '%') { printf("%%"); break; }
      field_size = 0;
      neg = 0;
      if (*format == '-') { neg = 1; format++; }
      while ((*format >= '0') && (*format <= '9')) {
        field_size *= 10;
        field_size += (*format - '0');
        format++;
      }
      if (neg) field_size = -field_size;
      switch (*format++) {
      case 0:
        *d++ = '%';
        break;
      case 'p':
        sprintf(target,"%*s",field_size,f->path);
        while (*d) d++;
        break;
      case 'n':
        strtocstr(str, sizeof(str), f->name, strlen(f->name));
        sprintf(target,"%*s",field_size,str);
        while (*d) d++;
        break;
      case 'e':
        strtocstr(str, sizeof(str), f->ext, strlen(f->ext));
        sprintf(target,"%*s",field_size,str);
        while (*d) d++;
        break;
      case 'f':
        strtocstr(str, sizeof(str), f->name, strlen(f->name));
        int len = strlen(str);
        if (len < sizeof(str)) str[len++] = '.';
        if (len < sizeof(str))
          strtocstr(&str[len], (sizeof(str) - len), f->ext, strlen(f->ext));
        sprintf(target,"%*s",field_size,str);
        while (*d) d++;
        break;
      case 's':
        sprintf(target,"%*lu",field_size,f->size);
        while (*d) d++;
        break;
      case 'a':
        AttrToString(str,f->attr,attr_format);
        sprintf(target,"%*s",field_size,str);
        while (*d) d++;
        break;
      case 'd':
        DateToString(str,f->year,f->mon,f->day,date_format);
        sprintf(target,"%*s",field_size,str);
        while (*d) d++;
        break;
      case 't':
        TimeToString(str,f->hour,f->min,f->sec,time_format);
        sprintf(target,"%*s",field_size,str);
        while (*d) d++;
        break;
      }   //switch on format type
      break;
    case '_':
      *d++ = ' ';
      break;
    default:
      *d++ = c;
    }   //switch
  }   //while
  *d = 0;
}   //ShowToString



