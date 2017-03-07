/*****************************************************************************
*
*  TITLE:       Copy List
*
*  DESCRIPTION: This module copies a list of files (which are listed in a
*               single file) into a directory.  The first argument on the
*               commandline is the name of the file containing the list; the
*               second argument is the directory in which to place them.
*
*  *k "%n"
*  FILE NAME: "COPYLIST.CPP"
*
*  *k "%v"
*  VERSION:   "1"
*
*****************************************************************************/

#include <stdiostr.h>
#include <dos.h>
#include <dir.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>


FILE* fin = NULL;       //pointer to file list file... global for Ctrl-Break


//===========================================================================
//  Isnt_Blank
//---------------------------------------------------------------------------
//  Checks for non-whitespace characters in a string.  Returns 0 if the
//  string contains only whitespace characters; returns 1 otherwise.
//===========================================================================
int  Isnt_Blank(        //1 is non-whitespace is present, 0 otherwise
  char* s)              //string to check
{
  while (*s) if (!isspace(*s++)) return 1;
  return 0;
}   //Isnt_Blank


//===========================================================================
//  Handle_Control_Break
//---------------------------------------------------------------------------
//  This function handles a Ctrl-Break key if it's pressed.  It exits the
//  program.
//===========================================================================
int  Handle_Control_Break(void)   //return value is unused
{
  if (fin != NULL) fclose(fin);
  exit(3);
  return 0;
}   //Handle_Control_Break


//===========================================================================
//  main
//---------------------------------------------------------------------------
//  Opens input file, checks that the output is a directory, then calls DOS
//  to copy each file.
//===========================================================================
int main(               //DOS return code, 0 on success
  int argc,             //count of arguments on commandline
  char* argv[])         //pointer to list of argument pointers
{
  char file_list[0x100];
  char directory[0x100] = "\0";

  printf("copylist -- copies a list of files into a directory\n");

  if ((argc < 2) || (argc > 3))
  {
    printf("Usage: copylist <list_of_files> [<destination_directory>]\n");
    return 1;
  }

  ctrlbrk(Handle_Control_Break);

  strcpy(file_list, argv[1]);

  fin = fopen(file_list, "rt");
  if (fin == NULL)
  {
    printf("couldn't open \"%s\"\n", file_list);
    return 1;
  }

  if (argc > 2)
  {
    strcpy(directory, argv[2]);

    if ((strchr(directory, '?') != NULL) ||
        (strchr(directory, '*') != NULL) )
    {
      fclose(fin);
      printf("wildcards not allowed in destination \"%s\"\n", directory);
      return 1;
    }

    struct ffblk dir;

    if (findfirst(directory, &dir, -1) != 0)
    {
      fclose(fin);
      printf("couldn't find \"%s\"\n", directory);
      return 1;
    }

    if (!(dir.ff_attrib & FA_DIREC))
    {
      fclose(fin);
      printf("\"%s\" not a directory\n", dir.ff_name);
      return 1;
    }
  }   //if directory specified

  char s[0x100];
  char command[0x100];
  char* p;
  int copy_count = 0;

  while (!feof(fin))
  {
    fgets(s, sizeof(s), fin);

    p = strstr(s, "--");        //kill comment
    if (p != NULL) *p = 0;

    p = strchr(s, '\n');        //nix newline
    if (p != NULL) *p = 0;

    if (Isnt_Blank(s))          //blast blanks
    {
      sprintf(command, "copy %s %s > NUL", s, directory);
      if (system(command) != 0)
        printf("ERROR: couldn't copy \"%s\"\n", s);
      else
      {
        printf("%s\n", s);
        copy_count++;
      }
    }
  }
  fclose(fin);
  printf("%u files copied.\n", copy_count);

  if (copy_count == 0) return 2;
  return 0;
}   //main
