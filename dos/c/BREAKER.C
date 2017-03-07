#include <conio.h>
#include <dos.h>
#include <stdio.h>
#include <stdlib.h>


int break_handler(void)
{
   printf("breaking...\n");
   exit(1);
}



void main(void)
{
   ctrlbrk(break_handler);
   while (1)
   {
      printf("killing 5 seconds...\n");
      delay(5000);
   }
}
