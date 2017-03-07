#include <stdio.h>

#include "icitypes.h"



UINT8 buffer[0x8000];

UINT32 bytes = 0UL;
UINT8 sum8 = 0;
UINT16 sum16 = 0;
UINT32 sum32 = 0UL;

int main(
     int argc,
     char* argv[])
{
   if (argc != 2)
   {
      printf("Usage: SUM32.EXE <file_to_sum>\n");
      return 1;
   }

   FILE* file = fopen(argv[1], "rb");

   while (!feof(file))
   {
      UINT16 read = fread(buffer, 1, sizeof(buffer), file);
      UINT16 i, last;

      last = read;
      for (i = 0; i < last; i++)
      {
         sum8 += buffer[i];
      }

      last = read / sizeof(UINT16);
      for (i = 0; i < last; i++)
      {
         sum16 += ((UINT16*) buffer)[i];
      }

      last = read / sizeof(UINT32);
      for (i = 0; i < last; i++)
      {
         sum32 += ((UINT32*) buffer)[i];
      }

      bytes += read;

   }

   fclose(file);

   printf("sum8  =       0x%02x / 0x%02x       on 0x%lx bytes\n", sum8, (-sum8) & 0xff, bytes);
   printf("sum16 =     0x%04x / 0x%04x     on 0x%lx words\n", sum16, -sum16, bytes / 2);
   printf("sum32 = 0x%08lx / 0x%08lx on 0x%lx long words\n", sum32, -sum32, bytes / 4);

   return 0;
}
