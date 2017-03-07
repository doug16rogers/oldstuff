#include <stdio.h>

typedef unsigned char      BIT_CHAR;
typedef unsigned short int BIT_SHORT;
typedef unsigned long int  BIT_LONG;
typedef unsigned int       BIT_INT;

typedef struct {
  unsigned char  f1:1;
  unsigned char  f2:2;
  unsigned char  f3:8;
  unsigned char  f4:9;
} CHAR_BITS;

typedef struct {
  BIT_SHORT f1:1;
  BIT_SHORT f2:2;
  BIT_SHORT f3:8;
  BIT_SHORT f4:9;
} SHORT_BITS;

typedef struct {
  BIT_INT   f1:1;
  BIT_INT   f2:2;
  BIT_INT   f3:8;
  BIT_INT   f4:9;
} INT_BITS;

/*
typedef struct {
  BIT_LONG  f1:1;
  BIT_LONG  f2:2;
  BIT_LONG  f3:8;
  BIT_LONG  f4:9;
} LONG_BITS;
*/

void main(void)
{
  CHAR_BITS  c = {0,0,0,0};
  SHORT_BITS s = {0,0,0,0};
  INT_BITS   i = {0,0,0,0};
  int k;

  c.f1=1; c.f2=2; c.f3=3; c.f4=4;
  s.f1=1; s.f2=2; s.f3=3; s.f4=4;
  i.f1=1; i.f2=2; i.f3=3; i.f4=4;

  printf("=====================================================\n");

  printf("char  (%x) = ",sizeof(CHAR_BITS));
  for (k = 0; (k < sizeof(CHAR_BITS)); k++)
    printf("0x%02X ",((unsigned char*)&c)[k]);
  printf("\n");

  printf("short (%x) = ",sizeof(SHORT_BITS));
  for (k = 0; (k < sizeof(SHORT_BITS)); k++)
    printf("0x%02X ",((unsigned char*)&s)[k]);
  printf("\n");

  printf("int   (%x) = ",sizeof(INT_BITS));
  for (k = 0; (k < sizeof(INT_BITS)); k++)
    printf("0x%02X ",((unsigned char*)&i)[k]);
  printf("\n");
}
