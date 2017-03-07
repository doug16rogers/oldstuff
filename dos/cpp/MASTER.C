#include <dos.h>
#include <stdio.h>
#include "icitypes.h"
#include "pc_info.h"

char* master_env(
     UINT16* size)
{
   UINT16 env_seg = 0;

   asm mov ax, 0x352e            // get INT 2Eh vector
   asm int 0x21
   asm mov dx, es:[0x002C]       // environment segment
   asm mov env_seg, dx
   if (size != NULL)
   {
      *size = ((MEMORY_CONTROL_BLOCK*) MK_FP(env_seg - 1, 0))->size * 0x10;
   }

   return (char*) MK_FP(env_seg, 0);
}

void main(void)
{
   UINT16 size = 0xffff;
   char* master = master_env(&size);

   printf("%p\n%s\n0x%x\n", master, master, size);
}


