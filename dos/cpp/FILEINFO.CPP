#include <string.h>

#include "fileinfo.hpp"


void File_Info:: Extract_Name_And_Extension(
   const char* from_string)
{
   int i;

   name[0] = 0;
   extension[0] = 0;

   for (i = 0;
        (i < 8) && (*from_string != 0) && (*from_string != '.');
        i++)
   {
      name[i] = *from_string++;
   }

   name[i] = 0;

   if (*from_string != 0)
   {
      from_string++;
   }

   for (i = 0;
        (i < 3) && (*from_string != 0);
        i++)
   {
      extension[i] = *from_string++;
   }

   extension[i] = 0;

}   // File_Info:: Extract_Name_And_Extension


void File_Info:: Load(

   struct ffblk* file_block)

{

   date.Set( (file_block->ff_fdate >> 9) + 1980,
             (file_block->ff_fdate >> 5) & 0x0f,
             (file_block->ff_fdate)      & 0x1f);

   time.Set( (file_block->ff_ftime >> 11),
             (file_block->ff_ftime >> 5) & 0x3f,
             (file_block->ff_ftime << 1) & 0x3f);

   size = file_block->ff_fsize;

   attribute = file_block->ff_attrib;

   name[0] = 0;
   extension[0] = 0;

   if (attribute & FA_DIREC)
   {
      if (strcmp(file_block->ff_name, "." ) == 0)
      {
         strcpy(name, "." );
      }
      else if (strcmp(file_block->ff_name, "..") == 0)
      {
         strcpy(name, "..");
      }
   }

   if (name[0] == 0)    // that is, if name wasn't already set to "." or ".."
   {
      Extract_Name_And_Extension(file_block->ff_name);
   }

}   // File_Info:: Load


const char* File_Info:: Name_And_Extension(void)

{

   static char name_and_extension[sizeof(name) + sizeof(extension)];

   if (extension[0] == 0)
   {
      strcpy(name_and_extension, name);
   }
   else
   {
      sprintf(name_and_extension, "%s.%s", name, extension);
   }

   return name_and_extension;

}   // File_Info:: Name_And_Extension


void File_Info:: Update_Stats(

   UINT8 only_attributes)

{
   char full_name[MAXPATH] = "";

   if (name[0] == 0)
   {
      return; //----------------------------------------------> return!
   }

   if (path != NULL)
   {
      strcpy(full_name, path);
   }

   strcat(full_name, name);

   struct ffblk file_block;

   if (findfirst(full_name, &file_block, only_attributes) == 0)
   {
      Load(&file_block);
   }

}   // File_Info:: Update_Stats

