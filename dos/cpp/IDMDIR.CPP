//===========================================================================
//  IDM Directory Generator
//  Generates a directory from input checksum (.sum) files from csum960.
//---------------------------------------------------------------------------
#include <stdiostr.h>
#include <iomanip.h>
#include <string.h>
#include <fstream.h>
#include <dir.h>


#include "cmdline.hpp"


//===========================================================================
//  global constants...
//---------------------------------------------------------------------------
#define FILENAME_SIZE          0x80

#define VERBOSE                1
#define USING_ARMY_DATA        0

#define DEFAULT_EXTENSION      ".sum"

#define DIRECTORY_FILE         "direct.sum"
#define GPP_CODE_FILE          "gpprom.sum"
#define GPP_POWERUP_FILE       "datarom.sum"
#define GPP_BASELINE_FILE      "datarom.sum"
#define DSP_CODE_FILE          "dsprom.sum"
#define IOP_CODE_FILE          "ioprom.sum"
#define ARMY_DATA_FILE         "#0"

#define PACKED_TO_LAST_ADDRESS 0xFFFFFFFFUL
#define GPP_CODE_ADDRESS       0x0000000BUL
#define GPP_POWERUP_ADDRESS    0x0000B0C0UL
#define GPP_BASELINE_ADDRESS   0x0000B1C0UL
#define DSP_CODE_ADDRESS       0x00043000UL
#define IOP_CODE_ADDRESS       0x00020000UL
#define ARMY_DATA_ADDRESS      PACKED_TO_LAST_ADDRESS


//===========================================================================
//  type definitions...
//---------------------------------------------------------------------------
typedef unsigned long U32;

typedef struct IDM_Directory_Entry_str
{
public:
  U32 start;                   //starting address of entry's data/code
  U32 length;                  //length, in 32-bit words, of data/code

public:
  int  Load(char* filename);
} IDM_Directory_Entry;

typedef struct IDM_Directory_str
{
public:
  IDM_Directory_Entry gpp_code;
  IDM_Directory_Entry gpp_powerup_data;
  IDM_Directory_Entry gpp_baseline_data;
  IDM_Directory_Entry dsp_code;
  IDM_Directory_Entry iop_code;
  U32 base_checksum;

  IDM_Directory_Entry army_data;       //to be used for army directories
  U32 army_checksum;

public:
  void FillInChecksums(void);
  int  WriteToFile(char* filename, char include_army);
  friend ostream& operator << (ostream& o, IDM_Directory_str& d);
} IDM_Directory;


//===========================================================================
//  global variables...
//---------------------------------------------------------------------------
char verbose = VERBOSE;
char using_army_data = USING_ARMY_DATA;

char directory_file[FILENAME_SIZE] = DIRECTORY_FILE;
char gpp_code_file[FILENAME_SIZE] = GPP_CODE_FILE;
char gpp_powerup_file[FILENAME_SIZE] = GPP_POWERUP_FILE;
char gpp_baseline_file[FILENAME_SIZE] = GPP_BASELINE_FILE;
char dsp_code_file[FILENAME_SIZE] = DSP_CODE_FILE;
char iop_code_file[FILENAME_SIZE] = IOP_CODE_FILE;
char army_data_file[FILENAME_SIZE] = ARMY_DATA_FILE;

IDM_Directory idm_dir = {
  { GPP_CODE_ADDRESS,     0UL },
  { GPP_POWERUP_ADDRESS,  0UL },
  { GPP_BASELINE_ADDRESS, 0UL },
  { DSP_CODE_ADDRESS,     0UL },
  { IOP_CODE_ADDRESS,     0UL },
  0UL
};   //idm_directory

char* minus_plus[] = { "-", "+" };


//===========================================================================
//  ScanFlag (void*,char*)
//  Scans a character string as if it were a flag ('+', '-', "on", "off",
//  etc.).
//  Returns 1 on success, 0 on failure.  data should point to a character.
//---------------------------------------------------------------------------
int  ScanFlag(                 //success = 1, failure = 0
  void* data,                  //pointer to flag data (a byte) to change
  char* argument)              //commandline argument, starting past switch
{
  switch (*argument)
  {
  case 0:
  case '+':
  case '1':
  case 't':
  case 'T':
    *(char*)data = 1;
    break;
  case '-':
  case '0':
  case 'f':
  case 'F':
    *(char*)data = 0;
    break;
  default:
    if      (stricmp(argument, "on" ) == 0) *(char*)data = 1;
    else if (stricmp(argument, "off") == 0) *(char*)data = 0;
    else
      return 0;
  }   //switch
  return 1;
}   //ScanFlag (void*,char*)


//===========================================================================
//  LoadEntry (char*)
//  Loads the length of the entry according to the length of the specified
//  file.  If the filename begins with '#', the length is loaded as the
//  hexadecimal number following the '#'.
//---------------------------------------------------------------------------
int  IDM_Directory_Entry_str::Load(    //success = 1, failure = 0
  char* filename)                      //name of file from which to load
{
  if (filename[0] == '#')
  {
    if (sscanf(&filename[1], "%X", &length) != 1)
    {
      cout << "Illegal specified hexadecimal length \"" << filename << "\".\n";
      return 0;
    }
  }
  else
  {
    static char local_filename[FILENAME_SIZE];
    struct ffblk file_block;
    char* extension;

    strcpy(local_filename, filename);
    extension = strchr(local_filename, '.');
    if (extension == NULL) strcat(local_filename, DEFAULT_EXTENSION);

    if (findfirst(local_filename, &file_block, 0) != 0)
    {
      cout << "Could not open \"" << filename << "\" to get size.\n";
      return 0;
    }
    length = ((U32) file_block.ff_fsize) / 4;
  }   //else it's a filename

  return 1;
}   //IDM_Directory_Entry::Load (U32,char*)


//===========================================================================
//  FillInChecksums (void)
//  Fills in the checksums for the IDM directory object.  Checksums are to
//  sum-to-zero, so to make them sum-to-n, just add n to each checksum.
//---------------------------------------------------------------------------
void IDM_Directory_str::FillInChecksums(void)  //no output
{
  base_checksum  = 0UL;
  base_checksum -= gpp_code.start;
  base_checksum -= gpp_code.length;
  base_checksum -= gpp_powerup_data.start;
  base_checksum -= gpp_powerup_data.length;
  base_checksum -= gpp_baseline_data.start;
  base_checksum -= gpp_baseline_data.length;
  base_checksum -= dsp_code.start;
  base_checksum -= dsp_code.length;
  base_checksum -= iop_code.start;
  base_checksum -= iop_code.length;

  army_checksum  = 0UL;
  army_checksum -= army_data.start;
  army_checksum -= army_data.length;
}   //IDM_Directory::FillInChecksums (void)


//===========================================================================
//  operator << (FILE*,U32)
//  Writes an unsigned long (32 bits) to a FILE* buffer.
//---------------------------------------------------------------------------
ofstream& operator << (        //file for output
  ofstream& out_file,          //file for output
  U32 value)                   //value to write binarially
{
  out_file.write((unsigned char *)&value, sizeof(value));
  return out_file;
}   //operator << (FILE*,U32)



//===========================================================================
//  WriteToFile (char*,char)
//  Writes the IDM directory object out to the given file.
//---------------------------------------------------------------------------
int  IDM_Directory_str::WriteToFile(   //success = 1, failure = 0
  char* filename,              //output filename
  char  include_army)          //include/don't include Army fields
{
  ofstream output_file;

  output_file.open(filename, ios::binary);
  if (!output_file)
  {
    cout << "Could not open output file \"" << filename << "\".\n";
    return 0;
  }

  output_file << gpp_code.start;
  output_file << gpp_code.length;
  output_file << gpp_powerup_data.start;
  output_file << gpp_powerup_data.length;
  output_file << gpp_baseline_data.start;
  output_file << gpp_baseline_data.length;
  output_file << dsp_code.start;
  output_file << dsp_code.length;
  output_file << iop_code.start;
  output_file << iop_code.length;
  output_file << base_checksum;

  if (include_army)
  {
    output_file << army_data.start;
    output_file << army_data.length;
    output_file << army_checksum;
  }

  output_file.close();

  return 1;
}   //IDM_Directory::WriteToFile (char*,char)


//===========================================================================
//  operator << (ostream&,IDM_Directory&)
//  Writes a text listing of the IDM directory object to the given output
//  stream.
//---------------------------------------------------------------------------
ostream& operator <<(          //output stream used
  ostream& o,                  //output stream to use
  IDM_Directory_str& d)        //IDM directory to dump
{
  o << setfill('0') << setbase(16);
  o << "GPP code              = (" << setw(8) << d.gpp_code.start
               << "," << setw(8) << d.gpp_code.length << ")\n";
  o << "GPP powerup defaults  = (" << setw(8) << d.gpp_powerup_data.start
               << "," << setw(8) << d.gpp_powerup_data.length << ")\n";
  o << "GPP baseline defaults = (" << setw(8) << d.gpp_baseline_data.start
               << "," << setw(8) << d.gpp_baseline_data.length << ")\n";
  o << "DSP code              = (" << setw(8) << d.dsp_code.start
               << "," << setw(8) << d.dsp_code.length << ")\n";
  o << "IOP code              = (" << setw(8) << d.iop_code.start
               << "," << setw(8) << d.iop_code.length << ")\n";
  o << "Base checksum = " << setw(8) << d.base_checksum << "\n";

  o << "\n";
  o << "Army data             = (" << setw(8) << d.army_data.start
               << "," << setw(8) << d.army_data.length << ")\n";
  o << "Army checksum = " << setw(8) << d.army_checksum << "\n";

  return o;
}   //operator << (ostream&,IDM_Directory&)


//===========================================================================
//  Help (int)
//  Prints help message and returns with its one parameter.
//---------------------------------------------------------------------------
int  Help(                     //copy of parameter
  int DOS_return_code)         //dummy parameter, usually DOS return code
{
  cout << setfill('0') << setbase(16);

  cout << "Usage: idmdir [switches]";
  cout << "Switches [default]:\n";
  cout << "  -o<file>    output file for directory [" DIRECTORY_FILE "]\n";
  cout << "  -v[+/-]     verbose output (to screen) ["
                           << minus_plus[VERBOSE != 0] << "]\n";
  cout << "  -u[+/-]     use (include) Army data ["
                           << minus_plus[USING_ARMY_DATA != 0] << "]\n";
  cout << "  -g<file>    GPP code file [" GPP_CODE_FILE "]\n";
  cout << "  -p<file>    GPP powerup default file [" GPP_POWERUP_FILE "]\n";
  cout << "  -b<file>    GPP baseline default file [" GPP_BASELINE_FILE "]\n";
  cout << "  -d<file>    DSP code file [" << DSP_CODE_FILE << "]\n";
  cout << "  -i<file>    IOP code file [" << IOP_CODE_FILE << "]\n";
  cout << "  -a<file>    Army data file [" << ARMY_DATA_FILE << "]\n";
  cout << "  -G<address> base address for GPP code ["
                           << setw(8) << idm_dir.gpp_code.start << "]\n";
  cout << "  -P<address> base address for GPP powerup defaults ["
                           << setw(8) << idm_dir.gpp_powerup_data.start << "]\n";
  cout << "  -B<address> base address for GPP baseline defaults ["
                           << setw(8) << idm_dir.gpp_baseline_data.start << "]\n";
  cout << "  -D<address> base address for DSP code ["
                           << setw(8) << idm_dir.dsp_code.start << "]\n";
  cout << "  -I<address> base address for IOP code ["
                           << setw(8) << idm_dir.iop_code.start << "]\n";
  cout << "  -A<address> base address for Army data ["
                           << setw(8) << idm_dir.army_data.start << "]\n";
  cout << "When specifying <file>, you may use \"#<length>\", in hex.\n";

  return DOS_return_code;
}   //Help (int)


//===========================================================================
//  Variable command_line
//  This variable contains all of the commandline switch options.
//---------------------------------------------------------------------------
CommandLine command_line(      //--invoking constructor with switches...
  + *new Switch("Output filename", 'o', directory_file, "%s")

  + *new Switch("use Army data", 'u', &using_army_data, NULL, ScanFlag)
  + *new Switch("verbose output", 'v', &verbose, NULL, ScanFlag)

  + *new Switch("GPP code filename", 'g', gpp_code_file, "%s")
  + *new Switch("GPP powerup data filename", 'p', gpp_powerup_file, "%s")
  + *new Switch("GPP baseline data filename", 'b', gpp_baseline_file, "%s")
  + *new Switch("DSP code filename", 'd', dsp_code_file, "%s")
  + *new Switch("IOP code filename", 'i', iop_code_file, "%s")
  + *new Switch("Army data filename", 'a', army_data_file, "%s")

  + *new Switch("GPP code start", 'G', &idm_dir.gpp_code.start, "%X")
  + *new Switch("GPP powerup data start", 'P',
                         &idm_dir.gpp_powerup_data.start, "%X")
  + *new Switch("GPP baseline data start", 'B',
                         &idm_dir.gpp_baseline_data.start, "%X")
  + *new Switch("DSP code start", 'D', &idm_dir.dsp_code.start, "%X")
  + *new Switch("IOP code start", 'I', &idm_dir.iop_code.start, "%X")
  + *new Switch("Army data start", 'A', &idm_dir.army_data.start, "%X")
);   //command_line


//===========================================================================
//  main (int,char**)
//  Main program.
//---------------------------------------------------------------------------
int  main(                     //DOS return code, success = 0
  int argc,                    //count of arguments from the commandline
  char* argv[])                //pointer to table of argument pointers
{
  cout << "idmdir -- creates an IDM EEPROM directory file\n\n";

  if (!command_line.Load(argc,argv)) return Help(1);

  if (!idm_dir.gpp_code.Load(gpp_code_file)) return 2;
  if (!idm_dir.gpp_powerup_data.Load(gpp_powerup_file)) return 2;
  if (!idm_dir.gpp_baseline_data.Load(gpp_baseline_file)) return 2;
  if (!idm_dir.dsp_code.Load(dsp_code_file)) return 2;
  if (!idm_dir.iop_code.Load(iop_code_file)) return 2;

  if (using_army_data)
  {
    if (!idm_dir.army_data.Load(army_data_file)) return 2;
  }

  idm_dir.FillInChecksums();
  if (verbose)
  {
    cout << idm_dir;
    cout << "\n";
    if (!using_army_data) cout << "Army information will NOT be written.\n";
  }
  if (!idm_dir.WriteToFile(directory_file, using_army_data)) return 3;

  cout << "Output written to \"" << directory_file << "\".\n";
}   //main (int,char**)

