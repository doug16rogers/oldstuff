#include <stdio.h>


#ifndef uchar
#define uchar unsigned char
#endif

#ifndef ushort
#define ushort unsigned short
#endif

#ifndef ulong
#define ulong unsigned long
#endif


typedef enum
{
   none,

   character_group,
   character_set
   single_character,

} ATOM_TYPES;


typedef struct
{
   void* next;
   uchar type;

} GENERIC_EXPRESSION;


typedef struct
{
   void* next;
   uchar type;          /* single_character */

   uchar character;

} SINGLE_CHARACTERS;


typedef struct
{
   void* next;
   uchar type;          /* character_group */

   uchar first;
   uchar last;

} CHARACTER_GROUPS;


typedef struct
{
   void* next;
   uchar type;          /* character_set */

   void* set;

} CHARACTER_SETS;




void* Make_Expression(
   char* string)
{
}   // Make_Expression


void Show_Expression(
   void* any_expression)
{
   GENERIC_EXPRESSIONS* expression = any_expression;

   while (expression != NULL)
   {
      switch (expression->type)
      {
         case none:
            printf("No expression\n");
            break;

         case character_group:
            printf("Character group from '%c' to '%c'\n",
                 ((CHARACTER_GROUPS*)expression)->first,
                 ((CHARACTER_GROUPS*)expression)->last);
            break;

         case character_set:
            printf("
      }   // switch
   }   // while
}



void main(
   int argc,
   char* argv[])
{
   if (argc != 2) return;

   Show_Expression( Make_Expression(argv[1]) );

}   // main
