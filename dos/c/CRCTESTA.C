#include <stdio.h>

#include "icitypes.h"


#define CRC_AFAPD      0xEDB88320UL    //1 + x1 + x2 + x4 + x5 + x7 + x8
                                       // + x10 + x11 + x12 + x16 + x22
                                       // + x23 + x26 + x32


#define POLYNOMIAL CRC_AFAPD
UINT32 polynomial = CRC_AFAPD;
UINT32 mask = 0xFFFFFFFFUL;            //mask for presetting the crc
UINT32 high_bit = 0x80000000UL;

#define BUFFER_SIZE 0x8000
UINT8 buffer[BUFFER_SIZE];
UINT16 count = 0;

#define VERBOSE     0
char verbose = VERBOSE;
char* minus_plus[] = { "-", "+" };


UINT32 quick_lookup[0x100] =
{
   0x00000000UL, 0x77073096UL, 0xEE0E612CUL, 0x990951BAUL, // 0x00 (0)
   0x076DC419UL, 0x706AF48FUL, 0xE963A535UL, 0x9E6495A3UL, // 0x04 (4)
   0x0EDB8832UL, 0x79DCB8A4UL, 0xE0D5E91EUL, 0x97D2D988UL, // 0x08 (8)
   0x09B64C2BUL, 0x7EB17CBDUL, 0xE7B82D07UL, 0x90BF1D91UL, // 0x0C (12)
   0x1DB71064UL, 0x6AB020F2UL, 0xF3B97148UL, 0x84BE41DEUL, // 0x10 (16)
   0x1ADAD47DUL, 0x6DDDE4EBUL, 0xF4D4B551UL, 0x83D385C7UL, // 0x14 (20)
   0x136C9856UL, 0x646BA8C0UL, 0xFD62F97AUL, 0x8A65C9ECUL, // 0x18 (24)
   0x14015C4FUL, 0x63066CD9UL, 0xFA0F3D63UL, 0x8D080DF5UL, // 0x1C (28)
   0x3B6E20C8UL, 0x4C69105EUL, 0xD56041E4UL, 0xA2677172UL, // 0x20 (32)
   0x3C03E4D1UL, 0x4B04D447UL, 0xD20D85FDUL, 0xA50AB56BUL, // 0x24 (36)
   0x35B5A8FAUL, 0x42B2986CUL, 0xDBBBC9D6UL, 0xACBCF940UL, // 0x28 (40)
   0x32D86CE3UL, 0x45DF5C75UL, 0xDCD60DCFUL, 0xABD13D59UL, // 0x2C (44)
   0x26D930ACUL, 0x51DE003AUL, 0xC8D75180UL, 0xBFD06116UL, // 0x30 (48)
   0x21B4F4B5UL, 0x56B3C423UL, 0xCFBA9599UL, 0xB8BDA50FUL, // 0x34 (52)
   0x2802B89EUL, 0x5F058808UL, 0xC60CD9B2UL, 0xB10BE924UL, // 0x38 (56)
   0x2F6F7C87UL, 0x58684C11UL, 0xC1611DABUL, 0xB6662D3DUL, // 0x3C (60)
   0x76DC4190UL, 0x01DB7106UL, 0x98D220BCUL, 0xEFD5102AUL, // 0x40 (64)
   0x71B18589UL, 0x06B6B51FUL, 0x9FBFE4A5UL, 0xE8B8D433UL, // 0x44 (68)
   0x7807C9A2UL, 0x0F00F934UL, 0x9609A88EUL, 0xE10E9818UL, // 0x48 (72)
   0x7F6A0DBBUL, 0x086D3D2DUL, 0x91646C97UL, 0xE6635C01UL, // 0x4C (76)
   0x6B6B51F4UL, 0x1C6C6162UL, 0x856530D8UL, 0xF262004EUL, // 0x50 (80)
   0x6C0695EDUL, 0x1B01A57BUL, 0x8208F4C1UL, 0xF50FC457UL, // 0x54 (84)
   0x65B0D9C6UL, 0x12B7E950UL, 0x8BBEB8EAUL, 0xFCB9887CUL, // 0x58 (88)
   0x62DD1DDFUL, 0x15DA2D49UL, 0x8CD37CF3UL, 0xFBD44C65UL, // 0x5C (92)
   0x4DB26158UL, 0x3AB551CEUL, 0xA3BC0074UL, 0xD4BB30E2UL, // 0x60 (96)
   0x4ADFA541UL, 0x3DD895D7UL, 0xA4D1C46DUL, 0xD3D6F4FBUL, // 0x64 (100)
   0x4369E96AUL, 0x346ED9FCUL, 0xAD678846UL, 0xDA60B8D0UL, // 0x68 (104)
   0x44042D73UL, 0x33031DE5UL, 0xAA0A4C5FUL, 0xDD0D7CC9UL, // 0x6C (108)
   0x5005713CUL, 0x270241AAUL, 0xBE0B1010UL, 0xC90C2086UL, // 0x70 (112)
   0x5768B525UL, 0x206F85B3UL, 0xB966D409UL, 0xCE61E49FUL, // 0x74 (116)
   0x5EDEF90EUL, 0x29D9C998UL, 0xB0D09822UL, 0xC7D7A8B4UL, // 0x78 (120)
   0x59B33D17UL, 0x2EB40D81UL, 0xB7BD5C3BUL, 0xC0BA6CADUL, // 0x7C (124)
   0xEDB88320UL, 0x9ABFB3B6UL, 0x03B6E20CUL, 0x74B1D29AUL, // 0x80 (128)
   0xEAD54739UL, 0x9DD277AFUL, 0x04DB2615UL, 0x73DC1683UL, // 0x84 (132)
   0xE3630B12UL, 0x94643B84UL, 0x0D6D6A3EUL, 0x7A6A5AA8UL, // 0x88 (136)
   0xE40ECF0BUL, 0x9309FF9DUL, 0x0A00AE27UL, 0x7D079EB1UL, // 0x8C (140)
   0xF00F9344UL, 0x8708A3D2UL, 0x1E01F268UL, 0x6906C2FEUL, // 0x90 (144)
   0xF762575DUL, 0x806567CBUL, 0x196C3671UL, 0x6E6B06E7UL, // 0x94 (148)
   0xFED41B76UL, 0x89D32BE0UL, 0x10DA7A5AUL, 0x67DD4ACCUL, // 0x98 (152)
   0xF9B9DF6FUL, 0x8EBEEFF9UL, 0x17B7BE43UL, 0x60B08ED5UL, // 0x9C (156)
   0xD6D6A3E8UL, 0xA1D1937EUL, 0x38D8C2C4UL, 0x4FDFF252UL, // 0xA0 (160)
   0xD1BB67F1UL, 0xA6BC5767UL, 0x3FB506DDUL, 0x48B2364BUL, // 0xA4 (164)
   0xD80D2BDAUL, 0xAF0A1B4CUL, 0x36034AF6UL, 0x41047A60UL, // 0xA8 (168)
   0xDF60EFC3UL, 0xA867DF55UL, 0x316E8EEFUL, 0x4669BE79UL, // 0xAC (172)
   0xCB61B38CUL, 0xBC66831AUL, 0x256FD2A0UL, 0x5268E236UL, // 0xB0 (176)
   0xCC0C7795UL, 0xBB0B4703UL, 0x220216B9UL, 0x5505262FUL, // 0xB4 (180)
   0xC5BA3BBEUL, 0xB2BD0B28UL, 0x2BB45A92UL, 0x5CB36A04UL, // 0xB8 (184)
   0xC2D7FFA7UL, 0xB5D0CF31UL, 0x2CD99E8BUL, 0x5BDEAE1DUL, // 0xBC (188)
   0x9B64C2B0UL, 0xEC63F226UL, 0x756AA39CUL, 0x026D930AUL, // 0xC0 (192)
   0x9C0906A9UL, 0xEB0E363FUL, 0x72076785UL, 0x05005713UL, // 0xC4 (196)
   0x95BF4A82UL, 0xE2B87A14UL, 0x7BB12BAEUL, 0x0CB61B38UL, // 0xC8 (200)
   0x92D28E9BUL, 0xE5D5BE0DUL, 0x7CDCEFB7UL, 0x0BDBDF21UL, // 0xCC (204)
   0x86D3D2D4UL, 0xF1D4E242UL, 0x68DDB3F8UL, 0x1FDA836EUL, // 0xD0 (208)
   0x81BE16CDUL, 0xF6B9265BUL, 0x6FB077E1UL, 0x18B74777UL, // 0xD4 (212)
   0x88085AE6UL, 0xFF0F6A70UL, 0x66063BCAUL, 0x11010B5CUL, // 0xD8 (216)
   0x8F659EFFUL, 0xF862AE69UL, 0x616BFFD3UL, 0x166CCF45UL, // 0xDC (220)
   0xA00AE278UL, 0xD70DD2EEUL, 0x4E048354UL, 0x3903B3C2UL, // 0xE0 (224)
   0xA7672661UL, 0xD06016F7UL, 0x4969474DUL, 0x3E6E77DBUL, // 0xE4 (228)
   0xAED16A4AUL, 0xD9D65ADCUL, 0x40DF0B66UL, 0x37D83BF0UL, // 0xE8 (232)
   0xA9BCAE53UL, 0xDEBB9EC5UL, 0x47B2CF7FUL, 0x30B5FFE9UL, // 0xEC (236)
   0xBDBDF21CUL, 0xCABAC28AUL, 0x53B39330UL, 0x24B4A3A6UL, // 0xF0 (240)
   0xBAD03605UL, 0xCDD70693UL, 0x54DE5729UL, 0x23D967BFUL, // 0xF4 (244)
   0xB3667A2EUL, 0xC4614AB8UL, 0x5D681B02UL, 0x2A6F2B94UL, // 0xF8 (248)
   0xB40BBE37UL, 0xC30C8EA1UL, 0x5A05DF1BUL, 0x2D02EF8DUL, // 0xFC (252)
};



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
     UINT32 crc,        //previous (input) CRC
     UINT8 *data,       //data to pass through CRC
     UINT16 bits)       //count of UINT8s to process
{
   UINT8 data_byte;
   UINT8 lsb;

   while (bits >= 8)
   {
      data_byte = *data++;
      crc = (crc >> 8) ^ quick_lookup[(UINT8) data_byte ^ (UINT8) crc];
      bits -= 8;
   }

   data_byte = *data;

   while (bits--)
   {
      lsb = ((UINT8) crc) ^ data_byte;
      crc >>= 1;
      if (lsb & 0x01)
      {
         crc ^= polynomial;
      }
      data_byte >>= 1;
   }

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
  UINT32 crc;
  if ((*s != '-') && (*s != '/')) return 1;

  s++;
  switch (*s++)
  {
  case '?':
    return 0;
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
    "  -v[-/+]         verbose output [%s]\n"
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

  UINT16 files = 0;
  UINT16 i,j;

  printf("crc -- calculates a specified CRC polynomial for input file(s)\n");

  if (argc < 2) return Help(0);

  for (i = 1; (i < argc); i++) if (!LoadArg(argv[i])) return Help(1);

  for (i = 1; (i < argc); i++)
  {
    if (!LoadBufferFromFile(argv[i])) continue;  //skip if just a switch
    files++;
    printf("File: %s\n", argv[i]);
    printf("  Bytes: 0x%04X\n", count);
    printf("  Polynomial: 0x%08lX,  Preset mask: 0x%08lX\n", polynomial, mask);

    if (verbose) DumpData("LSB first data", buffer, count);

    crc = 0UL;
    crc = CalculateCRC(crc, buffer, 8 * count);
    ShowCRC("Data LSB first, reset to all 0's", crc);
    crc = CalculateCRC(mask, buffer, 8 * count);
    ShowCRC("Data LSB first, preset to all 1's", crc);

    for (j = 0; (j < count); j++) buffer[j] = bit_reversed_UINT8[buffer[j]];
    if (verbose) DumpData("MSB first data", buffer, count);

    crc = 0UL;
    crc = CalculateCRC(crc, buffer, 8 * count);
    ShowCRC("Data MSB first, reset to all 0's", crc);
    crc = CalculateCRC(mask, buffer, 8 * count);
    ShowCRC("Data MSB first, preset to all 1's", crc);
  }   //for

  printf("Files processed: %d.\n", files);
  return 0;
}   //main

