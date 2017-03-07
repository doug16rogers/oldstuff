#include <stdio.h>

#include "icitypes.h"


#define CRC_12         0x00000F01UL    //1 + x1 + x2 + x3 + x11 + x12
#define CRC_16         0x0000A001UL    //1 + x2 + x15 + x16
#define CRC_CCITT      0x00008408UL    //1 + x5 + x12 + x16
#define CRC_CCITT_REV  0x00008810UL    //1 + x4 + x11 + x16
#define CRC_AFAPD      0xEDB88320UL    //1 + x1 + x2 + x4 + x5 + x7 + x8
                                       // + x10 + x11 + x12 + x16 + x22
                                       // + x23 + x26 + x32


#define POLYNOMIAL CRC_CCITT
UINT32 polynomial = POLYNOMIAL;

#define BUFFER_SIZE 0x8000
UINT8 buffer[BUFFER_SIZE];
UINT16 count = 0;

#define VERBOSE     0
char verbose = VERBOSE;
char* minus_plus[] = { "-", "+" };


UINT8 bit_reversed_UINT8[0x100] =
{
  /*0x00*/  0x00, 0x80, 0x40, 0xC0,  0x20, 0xA0, 0x60, 0xE0,
  /*0x08*/  0x10, 0x90, 0x50, 0xD0,  0x30, 0xB0, 0x70, 0xF0,
  /*0x10*/  0x08, 0x88, 0x48, 0xC8,  0x28, 0xA8, 0x68, 0xE8,
  /*0x18*/  0x18, 0x98, 0x58, 0xD8,  0x38, 0xB8, 0x78, 0xF8,
  /*0x20*/  0x04, 0x84, 0x44, 0xC4,  0x24, 0xA4, 0x64, 0xE4,
  /*0x28*/  0x14, 0x94, 0x54, 0xD4,  0x34, 0xB4, 0x74, 0xF4,
  /*0x30*/  0x0C, 0x8C, 0x4C, 0xCC,  0x2C, 0xAC, 0x6C, 0xEC,
  /*0x38*/  0x1C, 0x9C, 0x5C, 0xDC,  0x3C, 0xBC, 0x7C, 0xFC,
  /*0x40*/  0x02, 0x82, 0x42, 0xC2,  0x22, 0xA2, 0x62, 0xE2,
  /*0x48*/  0x12, 0x92, 0x52, 0xD2,  0x32, 0xB2, 0x72, 0xF2,
  /*0x50*/  0x0A, 0x8A, 0x4A, 0xCA,  0x2A, 0xAA, 0x6A, 0xEA,
  /*0x58*/  0x1A, 0x9A, 0x5A, 0xDA,  0x3A, 0xBA, 0x7A, 0xFA,
  /*0x60*/  0x06, 0x86, 0x46, 0xC6,  0x26, 0xA6, 0x66, 0xE6,
  /*0x68*/  0x16, 0x96, 0x56, 0xD6,  0x36, 0xB6, 0x76, 0xF6,
  /*0x70*/  0x0E, 0x8E, 0x4E, 0xCE,  0x2E, 0xAE, 0x6E, 0xEE,
  /*0x78*/  0x1E, 0x9E, 0x5E, 0xDE,  0x3E, 0xBE, 0x7E, 0xFE,
  /*0x80*/  0x01, 0x81, 0x41, 0xC1,  0x21, 0xA1, 0x61, 0xE1,
  /*0x88*/  0x11, 0x91, 0x51, 0xD1,  0x31, 0xB1, 0x71, 0xF1,
  /*0x90*/  0x09, 0x89, 0x49, 0xC9,  0x29, 0xA9, 0x69, 0xE9,
  /*0x98*/  0x19, 0x99, 0x59, 0xD9,  0x39, 0xB9, 0x79, 0xF9,
  /*0xA0*/  0x05, 0x85, 0x45, 0xC5,  0x25, 0xA5, 0x65, 0xE5,
  /*0xA8*/  0x15, 0x95, 0x55, 0xD5,  0x35, 0xB5, 0x75, 0xF5,
  /*0xB0*/  0x0D, 0x8D, 0x4D, 0xCD,  0x2D, 0xAD, 0x6D, 0xED,
  /*0xB8*/  0x1D, 0x9D, 0x5D, 0xDD,  0x3D, 0xBD, 0x7D, 0xFD,
  /*0xC0*/  0x03, 0x83, 0x43, 0xC3,  0x23, 0xA3, 0x63, 0xE3,
  /*0xC8*/  0x13, 0x93, 0x53, 0xD3,  0x33, 0xB3, 0x73, 0xF3,
  /*0xD0*/  0x0B, 0x8B, 0x4B, 0xCB,  0x2B, 0xAB, 0x6B, 0xEB,
  /*0xD8*/  0x1B, 0x9B, 0x5B, 0xDB,  0x3B, 0xBB, 0x7B, 0xFB,
  /*0xE0*/  0x07, 0x87, 0x47, 0xC7,  0x27, 0xA7, 0x67, 0xE7,
  /*0xE8*/  0x17, 0x97, 0x57, 0xD7,  0x37, 0xB7, 0x77, 0xF7,
  /*0xF0*/  0x0F, 0x8F, 0x4F, 0xCF,  0x2F, 0xAF, 0x6F, 0xEF,
  /*0xF8*/  0x1F, 0x9F, 0x5F, 0xDF,  0x3F, 0xBF, 0x7F, 0xFF,
};   //bit_reversed_UINT8


//===========================================================================
//  BitReversedWord
//  Reverses the bit order of the input UINT16.
//---------------------------------------------------------------------------
UINT16 BitReversedWord(  //value of input UINT16 with its bits in reverse order
  UINT16 value)          //input UINT16 to be reversed
{
  UINT16 reversed;

  ((UINT8*)&reversed)[0] = bit_reversed_UINT8[ ((UINT8*)&value)[1] ];
  ((UINT8*)&reversed)[1] = bit_reversed_UINT8[ ((UINT8*)&value)[0] ];
  return reversed;
}   //BitReversedWord


//===========================================================================
//  BitReversedLongWord
//  Reverses the bit order of the input longUINT16.
//---------------------------------------------------------------------------
UINT32 BitReversedLongWord(     //input longUINT16 with bits in reverse order
  UINT32 value)                 //input longUINT16 to be reversed
{
  UINT32 reversed;

  ((UINT8*)&reversed)[0] = bit_reversed_UINT8[ ((UINT8*)&value)[3] ];
  ((UINT8*)&reversed)[1] = bit_reversed_UINT8[ ((UINT8*)&value)[2] ];
  ((UINT8*)&reversed)[2] = bit_reversed_UINT8[ ((UINT8*)&value)[1] ];
  ((UINT8*)&reversed)[3] = bit_reversed_UINT8[ ((UINT8*)&value)[0] ];
  return reversed;
}   //BitReversedLongWord


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
//  ShowCRC
//  Displays a CRC value to the stdout.
//---------------------------------------------------------------------------
void ShowCRC(          //no output
  char* description,   //description string for the crc UINT16
  UINT32 crc)           //crc to display
{
  UINT32 rev = BitReversedLongWord(crc);   //crc with bits in reverse order
  printf("  %s:\n", description);
  printf("    CRC, normal  = 0x%08lX  Inverted = 0x%08lX\n", crc, ~crc);
  printf("    Bit-reversed = 0x%08lX  Inverted = 0x%08lX\n", rev, ~rev);
}   //ShowCRC


//===========================================================================
//  DumpData
//  Dumps data UINT8s, with description.
//---------------------------------------------------------------------------
void DumpData(         //no output
  char* description,   //description of data to be dumped
  UINT8* data,          //data to be dumped
  UINT16 count)          //number of UINT8s to dump
{
  UINT16 i;

  printf("    %s:\n", description);
  for (i = 0; (i < count); i++)
  {
    if ((i & 0x0F) == 0x00) printf("      %04X:", i);
    if ((i & 0x03) == 0x00) printf(" ");
    printf(" %02X", data[i]);
    if ((i & 0x0F) == 0x0F) printf("\n");
  }
  if ((i & 0x0F) != 0x00) printf("\n");
}   //DumpData


//===========================================================================
//  LoadBufferFromFile
//  Loads the internal UINT8 buffer from the specified file.
//---------------------------------------------------------------------------
UINT16 LoadBufferFromFile(       //number of UINT8s loaded
  char* filename)              //name of file to open
{
  FILE* file_buf;
  UINT16 UINT8_value;

  if ((filename[0] == '-') || (filename[0] == '/')) return 0;

  file_buf = fopen(filename, "rt");
  if (file_buf == NULL) return 0;

  count = 0;
  while (!feof(file_buf))
  {
    if (fscanf(file_buf, "%x", &UINT8_value) == 1)
    {
      if (count < sizeof(buffer))
        buffer[count++] = UINT8_value;
    }
    else
    {
      while (!feof(file_buf)) if (fgetc(file_buf) == '\n') break;
    }
  }   //while not at end of file
  fclose(file_buf);

  return count;
}   //LoadBufferFromFile


//===========================================================================
//  LoadFlag
//  Checks for a flag indicator.
//---------------------------------------------------------------------------
int  LoadFlag(         //1 if successful, 0 on error
  char* flag_name,     //name of flag (for error display)
  char* string,        //input switch string
  char* flag)          //pointer to flag variable
{
  switch (*string)
  {
  case 0:
  case '+':
  case 't':
  case 'T':
  case '1':
    *flag = 1;
    break;
  case '-':
  case 'f':
  case 'F':
  case '0':
    *flag = 0;
    break;
  default:
    printf("illegal flag value \"%s\" for %s.\n", string, flag_name);
    return 0;
  }   //switch
  return 1;
}   //LoadFlag


//===========================================================================
//  LoadArg
//  Checks to see if an argument is a switch; if so, parses it.
//---------------------------------------------------------------------------
int  LoadArg(          //returns 1 if all is ok, 0 if need to abort program
  char* s)             //argument string
{
  if ((*s != '-') && (*s != '/')) return 1;

  s++;
  switch (*s++)
  {
  case '?':
    return 0;
  case 'p':
    if (sscanf(s, "%lx", &polynomial) != 1)
    {
      printf("bad polynomial hex number \"%s\".\n", s);
      return 0;
    }
    break;
  case 'v':
    if (!LoadFlag("verbose", s, &verbose)) return 0;
    break;
  default:
    s--;
    s--;
    printf("unknown commandline switch \"%s\".\n", s);
    return 0;
  }   //switch
  return 1;
}   //LoadArg


//===========================================================================
//  Help
//  Displays the commandline structure for the program, including switch
//  values.
//---------------------------------------------------------------------------
int  Help(             //echo of value passed into routine
  int return_code)     //value to return, usually DOS return code
{
  printf(
    "\n"
    "Usage: crc [switches] <files>\n"
    "  -p<polynomial>  CRC polynomial to use in hex, here are some [%08lX]:\n"
    "                          F01 = CRC-12 = 1 + x1 + x2 + x3 + x11 + x12\n"
    "                         A001 = CRC-16 = 1 + x2 + x15 + x16\n"
    "                         8408 = CRC-CCITT = 1 + x5 + x12 + x16\n"
    "                         8810 = CRC-CCITT reversed = 1 + x4 + x11 + x16\n"
    "                     EDB88320 = AFAPD Frame Check Sequence\n"
    "  -v[-/+]         verbose output [%s]\n"
    ,POLYNOMIAL
    ,minus_plus[VERBOSE != 0]
  );
  return return_code;
}   //Help


//===========================================================================
//  main
//  Checks commandline arguments, then calculates CRC for input files.
//---------------------------------------------------------------------------
int  main(int argc,char* argv[])
{
  UINT32 crc = 0x00000000UL;    //crc value to calculate with
  UINT32 mask = 0UL;            //mask for presetting the crc

  UINT16 files = 0;
  UINT16 i,j;

  printf("crc -- calculates a specified CRC polynomial for input file(s)\n");

  if (argc < 2) return Help(0);

  for (i = 1; (i < argc); i++) if (!LoadArg(argv[i])) return Help(1);

  for (crc = polynomial; (crc != 0UL); crc >>= 1) mask |= crc;

  for (i = 1; (i < argc); i++)
  {
    if (!LoadBufferFromFile(argv[i])) continue;  //skip if just a switch
    files++;
    printf("File: %s\n", argv[i]);
    printf("  Bytes: 0x%04X\n", count);
    printf("  Polynomial: 0x%08lX,  Preset mask: 0x%08lX\n", polynomial, mask);

    if (verbose) DumpData("LSB first data", buffer, count);

    crc = 0UL;
    crc = CalculateCRC(crc, buffer, count);
    ShowCRC("Data LSB first, reset to all 0's", crc);
    crc = CalculateCRC(mask, buffer, count);
    ShowCRC("Data LSB first, preset to all 1's", crc);

    for (j = 0; (j < count); j++) buffer[j] = bit_reversed_UINT8[buffer[j]];
    if (verbose) DumpData("MSB first data", buffer, count);

    crc = 0UL;
    crc = CalculateCRC(crc, buffer, count);
    ShowCRC("Data MSB first, reset to all 0's", crc);
    crc = CalculateCRC(mask, buffer, count);
    ShowCRC("Data MSB first, preset to all 1's", crc);
  }   //for

  printf("Files processed: %d.\n", files);
  return 0;
}   //main
