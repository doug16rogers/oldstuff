#include <stdio.h>
#include <conio.h>
#include <dos.h>
#include <ctype.h>
#include <string.h>


char get_char_in(char* list, char* prompt)
{
  char ch;

  printf("%s", prompt);

  do
  {
    ch = tolower(getch());
  } while (strchr(list, ch) == NULL);

  printf("%s\n", ch);
  return ch;
}


void get_hex_value(unsigned int* port, char* prompt)
{
  char s[0x100];

  while (1)
  {
    printf(prompt);
    gets(s);
    if (sscanf(s, "%x", port) == 1) break;
    printf("ouch. do it right.\n");
  }
}



void main(void)
{
  char cmd;
  char type;
  unsigned int value, port;


  while (1)
  {
    cmd = get_char_in("ioq", "\nIn Out Quit -> ");

    if (cmd == 'q') break;

    type = get_char_in("bwc", "Byte Word Cancel -> ");

    if (type == 'c') continue;

    get_hex_value(&port, "Enter port (in hex): ");

    switch (cmd)
    {
      case 'i':
       if (type == 'b')
         value = inportb(port);
       else
         value = inport(port);
       printf("Read from port %x the value %x.\n", port, value);
       break;

      case 'o':
        get_hex_value(&value, "Value to write (in hex): ");
        if (type == 'b')
          outportb(port, value);
        else
          outport(port, value);
        printf("Wrote to port %x the value %x.\n", port, value);
        break;

      default:
        printf("What?\n");

    }   //switch
  }   //while

}
