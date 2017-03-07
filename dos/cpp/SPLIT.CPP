#include <conio.h>
#include <ctype.h>
#include <dir.h>
#include <dos.h>
#include <string.h>
#include <stdiostr.h>
#include <stdlib.h>


typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned long ulong;


char* strNcpy(
  char* target,
  char* source,
  int length)
{
  strncpy(target, source, length);
  target[length - 1] = 0;
  return target;
}   //strNcpy


class File_Name
{
private:
  char class_full_name[MAXPATH];

public:
  char drive[MAXDRIVE];
  char dir[MAXDIR];
  char name[MAXFILE];
  char ext[MAXEXT];

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

  void clear(void)
  {
    drive[0] = 0;
    dir[0] = 0;
    name[0] = 0;
    ext[0] = 0;
  }

  File_Name(void) { clear(); }

  File_Name(char* _drive, char* _dir, char* _name, char* _ext)
  {
    clear();
    if (_drive != NULL) strNcpy(drive, _drive, sizeof(drive));
    if (_dir   != NULL) strNcpy(dir,   _dir,   sizeof(dir));
    if (_name  != NULL) strNcpy(name,  _name,  sizeof(name));
    if (_ext   != NULL) strNcpy(ext,   _ext,   sizeof(ext));
  }

  File_Name(char* full_name)
  {
    Split(full_name);
  }
};   //class File_Name


//===========================================================================
//  Disk_Space
//---------------------------------------------------------------------------
//  Returns the disk space on the given drive.  Use 0 or ' ' for current
//  drive.
//===========================================================================
ulong Disk_Space(
  char on_drive)
{
  struct dfree disk_space;
  ulong space = 0L;
  uchar drive;

  on_drive = toupper(on_drive);

  if (!isalpha(on_drive))
  {
    drive = 0;
  }
  else
  {
    drive = on_drive - 'A' + 1;
  }

  getdfree(drive, &disk_space);

  space =   (ulong) disk_space.df_avail
          * (ulong) disk_space.df_bsec
          * (ulong) disk_space.df_sclus;

  return space;

}   //Disk_Space


//===========================================================================
//  Dump_Block
//---------------------------------------------------------------------------
//  Dumps a block from an (already opened) input file into an output file.
//===========================================================================
ulong Dump_Block(               //number of bytes actually written
  FILE* input_file,             //input file from fopen()
  ulong block_length,           //bytes to write from input file
  char* output_filename)        //name of output file for block
{
  ulong bytes_read_from_input_so_far = 0UL;
  ulong bytes_left_to_read;
  ulong bytes_to_read_this_time;
  ulong bytes_read_this_time;
  FILE* output_file;

  static uchar local_buffer[0x8000];

  if (feof(input_file)) return 0UL;

  output_file = fopen( output_filename, "wb" );
  if (output_file == NULL) return 0UL;

  while (!feof(input_file) && (bytes_read_from_input_so_far < block_length))
  {
    bytes_left_to_read = block_length - bytes_read_from_input_so_far;
    if (bytes_left_to_read > sizeof(local_buffer))
      bytes_to_read_this_time = sizeof(local_buffer);
    else
      bytes_to_read_this_time = bytes_left_to_read;
    bytes_read_this_time = fread( local_buffer,
                                  1, (size_t) bytes_to_read_this_time,
                                  input_file );
    if (bytes_read_this_time == 0) break;
    fwrite( local_buffer, 1, (size_t) bytes_read_this_time, output_file );
    bytes_read_from_input_so_far += bytes_read_this_time;
  }

  fclose( output_file );
  return bytes_read_from_input_so_far;
}   //Dump_Block


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

  ulong length = 0UL;

  cout << "split -- splits a file into multiple files" << endl;

  if (number_of_arguments < 3)
  {
    cout << "Usage: split <filename> <block_size_in_decimal> [dest]" << endl;
    return 1;
  }

  input.Split( commandline_argument[1] );

  FILE* input_file = fopen( input.path(), "rb" );
  if (input_file == NULL)
  {
    cout << "file \"" << input.path() << "\" could not be opened" << endl;
    return 2;
  }

  length = atol( commandline_argument[2] );
  if (length == 0UL)
  {
    fclose( input_file );
    cout << "invalid block length \"" << commandline_argument[2] << "\"."
         << endl;
    return 1;
  }

  if (number_of_arguments <= 3)
  {
    output = input;
  }
  else
  {
    output.Split(commandline_argument[3]);
    if (output.name[0] == 0) strcpy(output.name, input.name);
  }

  int current_file_number = 0;
  ulong bytes_written;

  while (1)
  {
    sprintf( output.ext, ".%03u", current_file_number );

    char c = 0;
    while ((Disk_Space(output.drive[0]) < length) && (c != 0x1b))
    {
      cout << "Not enough room on drive;  ";
      cout << "  Swap disks or hit <ESC> to quit";
      c = getch();
      cout << "\n";
    }
    if (c == 0x1b) break;

    cout << "file \"" << output.path() << "\": ";

    bytes_written = Dump_Block( input_file, length, output.path());

    cout << bytes_written << " bytes." << endl;

    if (feof(input_file)) break;

    current_file_number++;
  }

  cout << current_file_number << " files created." << endl;
  fclose( input_file );
  return 0;
}   //main


