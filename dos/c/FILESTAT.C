#include <dir.h>
#include <dos.h>
#include <stdio.h>
#include <string.h>


#define TERMINATE(s)   (s)[sizeof(s) - 1] = 0


int Show_Stats(char* path, struct ffblk* block)
{
  FILE* file;
  static char full_name[0x100];
  char attribute_string[0x40];
  char first_line[0x100];

  char* attributes;
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

  strcpy(full_name, path);
  strcat(full_name, block->ff_name);

  file = fopen(full_name, "rt");
  if (file == NULL)
  {
    printf("couldn't open file \"%s\".\n", full_name);
    return 0;
  }

  fgets(first_line, sizeof(first_line), file);
  fclose(file);

  TERMINATE(first_line);

  p = strchr(first_line, '\n');
  if (p != NULL) *p = 0;

  file_time = (FILE_TIME*) &(block->ff_ftime);
  file_date = (FILE_DATE*) &(block->ff_fdate);

  attributes = attribute_string;

  if (block->ff_attrib & FA_ARCH)
    *attributes++ = 'a';
  else
    *attributes++ = '-';

  if (block->ff_attrib & FA_RDONLY)
    *attributes++ = 'r';
  else
    *attributes++ = '-';

  if (block->ff_attrib & FA_HIDDEN)
    *attributes++ = 'h';
  else
    *attributes++ = '-';

  if (block->ff_attrib & FA_DIREC)
    *attributes++ = 'd';
  else
    *attributes++ = '-';

  if (block->ff_attrib & FA_SYSTEM)
    *attributes++ = 's';
  else
    *attributes++ = '-';

  if (block->ff_attrib & FA_LABEL)
    *attributes++ = 'l';
  else
    *attributes++ = '-';

  *attributes = 0;

  strlwr(block->ff_name);

  printf("%04u.%02u.%02u %02u:%02u:%02u %s %-12s %s\n",
       1980 + file_date->years_since_1980,
       file_date->month,
       file_date->day,
       file_time->hour,
       file_time->minute,
       2 * file_time->second_over_2,
       attribute_string,
       block->ff_name,
       first_line);

  return 1;

}   //Show_Stats



int Do_File(char* file_name)
{
  struct ffblk file_block;

  char path[MAXPATH];
  char drive[MAXDRIVE];
  char directory[MAXDIR];
  char name[MAXFILE];
  char extension[MAXEXT];

  printf("File(s): %s:\n", file_name);

  fnsplit(file_name, drive, directory, name, extension);
    
  strcpy(path, drive);
  strcat(path, directory);

  if (findfirst(file_name, &file_block, FA_ARCH |
                                        FA_RDONLY |
                                        FA_HIDDEN |
                                        FA_LABEL |
                                        FA_SYSTEM) != 0) return 0;
  do
  {
    Show_Stats(path, &file_block);
  } while (findnext(&file_block) == 0);

  return 1;
}   //Do_File


int main(
  int argc,
  char* argv[])
{
  int i;

  if (argc < 2)
  {
    Do_File("*.*");
  }
  else
  {
    for (i = 1; i < argc; i++) Do_File(argv[i]);
  }

  printf("\n");

  return 0;
}   //main
