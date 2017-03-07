#include <stdio.h>
#include <dos.h>

#define BASIC_INT      0x18

#define VIDSEG         0xB8000000UL

#define FIRST_CHAR     (0x1E00 + '0')
#define LAST_CHAR      (0x1E00 + '9')

unsigned far* screen = (unsigned far*)VIDSEG;

int i = 0;
unsigned int val = FIRST_CHAR;


void interrupt dr_basic(void)
{
  if (i++ > 1000)
  {
    i = 0;
    screen = (unsigned far*)VIDSEG;
  }
  *screen = val++;
  if (val > LAST_CHAR) val = FIRST_CHAR;
}


void main(void)
{
}

