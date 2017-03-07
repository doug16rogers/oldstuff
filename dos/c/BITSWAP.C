#include <stdio.h>


#define byte unsigned char


byte reverse_bits(     //value of byte with bit-order reversed
  byte value)          //value to be reversed
{
  byte bit = 0x01;
  byte rev = 0x00;

  for (bit = 0x01; bit != 0; bit <<= 1)
  {
    rev <<= 1;
    if (value & bit) rev |= 0x01;
  }
  return rev;
}   //reverse_bits


void main(void)
{
  int k;

  printf("{\n");
  for (k = 0; k < 0x100; k++)
  {
    if ((k & 0x07) == 0x00) printf("  /*0x%02X*/  ", k);
    printf("0x%02X, ", reverse_bits(k));
    if ((k & 0x03) == 0x03) printf(" ");
    if ((k & 0x07) == 0x07) printf("\n");
  }   //for
  printf("};   //bits_reversed\n");
}   //main
