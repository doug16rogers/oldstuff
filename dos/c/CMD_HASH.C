#include <stdio.h>

typedef unsigned char byte;
typedef unsigned long uint;
typedef void (*Function_Pointer)(void);


  /*-- note: these are only prototypes, so linker must fill in table --*/

void command_error(void);

void command_01(void);
void command_05(void);
void command_0A(void);
void command_0C(void);
void command_0E(void);
void command_0F(void);
void command_13(void);



Function_Pointer command_function_lookup[] =
{
  command_error,    /*00 - for error message*/
  command_01,       /*01*/
  command_05,       /*02*/
  command_0A,       /*03*/
  command_0C,       /*04*/
  command_0E,       /*05*/
  command_0F,       /*06*/
  command_13,       /*07*/
};

#define UNUSED 0x00
#define MAKE_ENTRY(a,b,c,d)  ( ((((uint)a)<< 0) & 0x000000FFUL) | \
                               ((((uint)b)<< 8) & 0x0000FF00UL) | \
                               ((((uint)c)<<16) & 0x00FF0000UL) | \
                               ((((uint)d)<<24) & 0xFF000000UL) )

uint command_lookup[] =   /*indexed by command number*/
{
  MAKE_ENTRY( UNUSED,      /*command 00*/
              0x01,        /*command 01*/
              UNUSED,      /*command 02*/
              UNUSED  ),   /*command 03*/
  MAKE_ENTRY( UNUSED,      /*command 04*/
              0x02,        /*command 05*/
              UNUSED,      /*command 06*/
              UNUSED  ),   /*command 07*/
  MAKE_ENTRY( UNUSED,      /*command 08*/
              UNUSED,      /*command 09*/
              0x03,        /*command 0A*/
              UNUSED  ),   /*command 0B*/
  MAKE_ENTRY( 0x04,        /*command 0C*/
              UNUSED,      /*command 0D*/
              0x05,        /*command 0E*/
              0x06    ),   /*command 0F*/
  MAKE_ENTRY( UNUSED,      /*command 10*/
              UNUSED,      /*command 11*/
              UNUSED,      /*command 12*/
              0x07    ),   /*command 13*/
};


void call_command(byte command_number)
{
  uint lookup_word;                    /*pointer into command hash table*/
  uint amount_to_shift;                /*used to extract byte from word*/
  uint function_index;                 /*at last, index into function table*/

  if (command_number >= sizeof(command_lookup))
  {
    printf("command number too large.\n");
    return;
  }

  lookup_word     = command_lookup[command_number/4];
  amount_to_shift = 8 * (command_number & 3);
  function_index  = (lookup_word >> amount_to_shift) & 0x00FF;

/*  if (function_index == UNUSED) return;  -- using error function instead*/
  command_function_lookup[function_index]();
}    /*call_command(int command_number)*/


  /*-- these functions could be in a completely different file --*/

void command_error(void) { printf("Sorry, command not implemented.\n"); }
void command_01(void) { printf("I'm in the command 01 function.\n"); }
void command_05(void) { printf("I'm in the command 05 function.\n"); }
void command_0A(void) { printf("I'm in the command 0A function.\n"); }
void command_0C(void) { printf("I'm in the command 0C function.\n"); }
void command_0E(void) { printf("I'm in the command 0E function.\n"); }
void command_0F(void) { printf("I'm in the command 0F function.\n"); }
void command_13(void) { printf("I'm in the command 13 function.\n"); }



void main(void)
{
  char s[0x80];
  uint command;

  while (1)
  {
    printf("\nEnter a command number (hex): ");
    gets(s);
    if ((s[0] == 0) || (s[0] == '\n')) break;
    if (sscanf(s,"%X",&command) != 1)
      printf("BAD COMMAND NUMBER \"%s\".\n");
    else
      call_command((byte) command);
  }
}   //main
