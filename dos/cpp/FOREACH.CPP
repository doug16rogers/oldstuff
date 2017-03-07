#include <dir.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define TERMINATE(s) s[sizeof(s) - 1] = 0




void Make_Command(char* cmd, char* arg, char* fn)
{
  char volume[MAXDRIVE+1];
  char dir[MAXDIR+1];
  char name[MAXFILE+1];
  char ext_with_dot[MAXEXT+1];
  char* ext = &ext_with_dot[1];

#define ADVANCE(s)  while (*s) s++

  fnsplit(fn, volume, dir, name, ext_with_dot);

  TERMINATE(volume);
  TERMINATE(dir);
  TERMINATE(name);
  TERMINATE(ext_with_dot);

  while (*arg)
  {
    if (*arg == '@')
    {
      arg++;
      switch (*arg++)
      {
        case 0:
          strcpy(cmd, fn);
          ADVANCE(cmd);
          break;

        case '@':
          *cmd++ = '@';
          break;

        case 'd':
          strcpy(cmd, dir);
          ADVANCE(cmd);
          break;

        case 'e':
          strcpy(cmd, ext);
          ADVANCE(cmd);
          break;

        case 'f':
          strcpy(cmd, name);
          ADVANCE(cmd);
          strcpy(cmd, ext_with_dot);
          ADVANCE(cmd);
          break;

        case 'n':
          strcpy(cmd, name);
          ADVANCE(cmd);
          break;

        case 'p':
          strcpy(cmd, volume);
          ADVANCE(cmd);
          strcpy(cmd, dir);
          ADVANCE(cmd);
          break;

        case 'v':
          strcpy(cmd, volume);
          ADVANCE(cmd);
          break;

      }
    }
    else
    {
      *cmd++ = *arg++;
    }
  }

  *cmd = 0;
}



int main(
  int argument_count,
  char* argument[])
{
  if (argument_count < 2)
  {
    printf(
      "Usage:  foreach <file_containing_file_list> \"<command>\"\n"
      "Put @X in your command to:\n"
      "  X = @, volume, drive, name, extension, filename, path\n"
    );
    return 1;
  }

  FILE* list_file = fopen(argument[1], "rt");

  if (list_file == NULL)
  {
    printf("could not open file \"%s\" to get list of files.\n", argument[1]);
    return 2;
  }

  int file_count = 0;
  char file_name[0x100];
  char command[0x100];

  while (fscanf(list_file, "%s", file_name) == 1)
  {
    if (strlen(file_name) > 0x40) break;

    file_count++;

    Make_Command(command, argument[2], file_name);

    printf("executing \"%s\"\n", command);

    system(command);
  }

  fclose(list_file);

  return 0;
}   // main
