#include <dir.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
  
main(int argc, char *argv[])
{
   typedef enum {
      false,
      true
   }BOOLEAN;
   FILE *source_print_batch_file;
   FILE *source_list;
   FILE *new_string_file;
   char default_string[100];
   char source_print_string[100];
   char drive[MAXDRIVE];
   char dir[MAXDIR];
   char file[MAXFILE];
   char ext[MAXEXT];
   int flags;
   char filename[40];
   char temp_str[80];
   BOOLEAN done = false;
   BOOLEAN filelist = false;
   BOOLEAN print_file = true;
   int i, j;
   char ch;

   strcpy(default_string, "/l78f0#w115o2,27,38,108,48,111,56,68,27,40,115,48,112,49,54,46,54,55,72");
/*   strcpy(default_string, "/cl72ki2g4#w132o8"); */
   if(*argv[1] == '?')
      {
      printf("\nSource print style output program\n\n");
      printf("Enter file name only for standard output\n");
      printf("\nEnter file name followed by 'n' to only create spbat.bat file\n");
      printf("\nTo change spbat.bat parameters create a spbat.bat file\n");
      printf("with new defaults then enter file name 'y/n' to print and 'y/n'\n");
      printf("to use new defaults\n\n");
      exit(0);
      }
   if(argc > 3)
      if(*argv[3] == 'y')
      /*check for new string */
      {
         if ((new_string_file = fopen("spbat.bat", "r")) != NULL)
         {
            fgets(temp_str, 80, new_string_file);
            fgets(temp_str, 80, new_string_file);
            for(i=0; ((ch = temp_str[i]) != '/'); i++);
            for(j=0; (ch = temp_str[i]) != '\n'; j++, i++)
               default_string[j] = temp_str[i];

            default_string[j] = '\0';

            fclose(new_string_file);
         }
      }
   if(argc > 2)
      if(*argv[2] == 'n')
         print_file = false;
   flags = fnsplit(argv[1],drive,dir,file,ext);
   strcpy(filename,argv[1]);
   if(strcmp(ext, ".lst"))
      done = true;
   else
   {
      filelist = true;
      if ((source_list = fopen(filename, "r")) == NULL)
      {
         printf("unable to open list file\n");
         exit(1);
      }
      else
      {
         if((fgets(temp_str, 14, source_list) == NULL) || (temp_str[0] == '\n'))
         {
            done = true;
            exit(0);
         }
         else
         {
            for(i=0; ((ch = temp_str[i]) != '\n'); i++)
               filename[i] = ch;
            filename[i] = '\0';
         }
      }
   }
   do
   {
      flags = fnsplit(filename,drive,dir,file,ext);
      strcpy(source_print_string,"sp ");
      strcat(source_print_string,filename);
      strcat(source_print_string,"$");
      strcat(source_print_string,file);
      strcat(source_print_string,".PRN");
      strcat(source_print_string,default_string);
      if (print_file)
      {
         strcat(source_print_string,"\nnprint ");
         strcat(source_print_string,file);
         strcat(source_print_string,".PRN /NOTIFY j=l_c");
      }
      if ((source_print_batch_file = fopen("temp.bat", "w")) != NULL)
      {
         fprintf(source_print_batch_file, source_print_string);
         fclose(source_print_batch_file);
         system("temp.bat");
      }
      else
         printf("unable to open temp.bat file\n");
      if(filelist)
      {
         if(fgets(temp_str, 14, source_list) == NULL)
            done = true;
         else
         {
            for(i=0; (ch = temp_str[i]) != '\n'; i++)
               filename[i] = ch;
            filename[i] = '\0';
         }
      }
   }while (!done);
   if(filelist)
      fclose(source_list);

}
