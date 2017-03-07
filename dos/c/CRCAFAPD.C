#include <stdio.h>

//#include "strutil.h"


#ifndef UINT8
#define UINT8 unsigned char
#endif

#ifndef UINT16
#define UINT16 unsigned int
#endif

#ifndef UINT32
#define UINT32 unsigned long int
#endif

#ifndef BYTE
#define BYTE unsigned char
#endif

#ifndef UINT16
#define UINT16 unsigned int
#endif

#ifndef UINT32
#define UINT32 unsigned long int
#endif


#define CRC_AFAPD      0xEDB88320UL    //1 + x1 + x2 + x4 + x5 + x7 + x8
                                       // + x10 + x11 + x12 + x16 + x22
                                       // + x23 + x26 + x32


UINT32 polynomial = CRC_AFAPD;



//===========================================================================
//  CalculateCRC
//  Calculates the next value of CRC for the crc and data UINT8s passed into
//  the function.
//---------------------------------------------------------------------------
UINT32 CalculateCRC(    //new CRC calculated from the input UINT8s and CRC
  UINT32 crc,           //previous (input) CRC
  UINT8 *data,          //data to pass through CRC
  UINT16 count)          //count of UINT8s to process
{
  UINT16 i;
  UINT16 bit;
  UINT8 b;
  UINT8 lsb;

  for (i = 0; i < count; i++)
  {
    b = *data++;
    for (bit = 0; bit < 8; bit++)
    {
      lsb = (b ^ (UINT8)crc) & 1;
      crc >>= 1;
      if (lsb) crc ^= polynomial;
      b >>= 1;
    }
  }   //for thru UINT8 string
  return crc;
}   //crc


//===========================================================================
//  main
//  Checks commandline arguments, then calculates CRC for input files.
//---------------------------------------------------------------------------
int  main(void)
{
   UINT32 crc = 0x00000000UL;    //crc value to calculate with
   UINT32 mask = 0UL;            //mask for presetting the crc

   for (crc = 0; crc < 0x100UL; crc++)
   {
       if ((crc & 0x03) == 0) printf("     ");
       printf("16#%08lX#, ", CalculateCRC(0, &crc, 1));
       if ((crc & 0x03) == 3) printf("-- 16#%02lX# (%lu)\n", crc-3, crc-3);
   }
   printf("\n");

   return 0;
}   //main
