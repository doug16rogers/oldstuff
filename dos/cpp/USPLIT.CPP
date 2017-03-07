#include <dir.h>
#include <dos.h>
#include <string.h>
#include <stdiostr.h>
#include <stdlib.h>


typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned long ulong;

class File_Name
{
public:
  char drive[MAXDRIVE];
  char dir[MAXDIR];
  char name[MAXFILE];
  char ext[MAXEXT];

  static char class_full_name[MAXPATH];

  int Split(char* full_name)
  {
    return fnsplit(full_name, drive, dir, name, ext);
  }

  char* Merge(char* full_name = NULL)
  {
    char* return_pointer;
    if (full_name != NULL)
      fnmerge(return_pointer = full_name, drive, dir, name, ext);
    else
      fnmerge(return_pointer = class_full_name, drive, dir, name, ext);
    return return_pointer;
  }

  char* path(void)
  {
    fnmerge(class_full_name, drive, dir, name, ext);
    return class_full_name;
  }

  File_Name(void) {}

  File_Name(char* _drive, char* _dir, char* _name, char* _ext)
  {
    strcpy(drive, _drive);
    strcpy(dir, _dir);
    strcpy(name, _name);
    strcpy(ext, _ext);
  }

  File_Name(char* full_name)
  {
    Split(full_name);
  }
};   //class File_Name


char File_Name::class_full_name[MAXPATH] = "";



//===========================================================================
//  Dump_File
//---------------------------------------------------------------------------
//  Dumps a file to the end of the given, already-opened, output file.
//===========================================================================
ulong Dump_File(                //number of bytes actually written
  char* input_filename,         //input filename
  FILE* output_file)            //output file from fopen()
{
  ulong bytes_read_from_input_so_far = 0UL;
  ulong bytes_read_this_time;
  FILE* input_file;

  static uchar local_buffer[0x8000];

  input_file = fopen( input_filename, "rb" );
  if (input_file == NULL) return 0UL;
  if (feof(input_file)) return 0UL;

  while (!feof(input_file))
  {
    bytes_read_this_time = fread( local_buffer,
                                  1, sizeof(local_buffer),
                                  input_file );
    if (bytes_read_this_time == 0) break;
    fwrite( local_buffer, 1, bytes_read_this_time, output_file );
    bytes_read_from_input_so_far += bytes_read_this_time;
  }

  fclose( input_file );
  return bytes_read_from_input_so_far;
}   //Dump_File


//===========================================================================
//  main
//---------------------------------------------------------------------------
//  Main program.  Checks parameters, then calls Dump_Block until there's
//  nothing left to dump.
//===========================================================================
int main(                       //DOS return code (0 = success)
  int number_of_arguments,      //number of commandline arguments
  char* commandline_argument[]) //list of commandline argument pointers
{
  File_Name input;
  File_Name output;

  cout << "usplit -- combines files previously split with split" << endl;

  if (number_of_arguments != 2)
  {
    cout << "Usage: usplit <filename_of_.000_file>" << endl;
    return 1;
  }

  output.Split(commandline_argument[1]);
  FILE* output_file = fopen( output.path(), "wb" );
  if (output_file == NULL)
  {
    cout << "file \"" << output.path() << "\" could not be opened" << endl;
    return 2;
  }

  input = output;

  int current_file_number = 0;
  ulong bytes_written;
  while (1)
  {
    if (current_file_number > 999) break;
    sprintf( input.ext, ".%03u", current_file_number );
    bytes_written = Dump_File( input.path(), output_file );
    if (bytes_written == 0) break;

    cout << "file \"" << input.path() << "\": "
         << bytes_written << " bytes." << endl;
    current_file_number++;
  }

  cout << current_file_number << " files used." << endl;
  fclose( output_file );
  return 0;
}   //main


