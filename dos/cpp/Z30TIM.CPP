#include <iostream.h>
#include <iomanip.h>
#include <dos.h>
#include <string.h>


#include "cmdline.hpp"
#include "kb.hpp"
#include "onebuff.hpp"
#include "script.hpp"
#include "timer.hpp"
#include "z30util.hpp"


//===========================================================================
//  class StatBuffer : OneWayBuffer
//---------------------------------------------------------------------------
class StatBuffer : public OneWayBuffer
{
public:
  long  begin_time;            //time when first char was received/sent
  long  end_time;              //time when frame closed
  long  min;
  long  max;
  double sum;
  double sum_of_squares;
  long  entries;
  byte error;                  //error flags
  byte sent;                   //data packet sent
  byte received;               //data packet received

  void Reset(void)
  {
    OneWayBuffer::Reset();
    received = 0;
    sent = 0;
    error = 0;
    begin_time = 0;
    end_time = 0;
  }
  void ResetStats(void)
  {
    min = 0x7fffffffL;
    max = (long) 0x80000000UL;
    sum = 0.0;
    sum_of_squares = 0.0;
    entries = 0;
  }
  void UpdateStats(long datum)
  {
    if (datum < min) min = datum;
    if (datum > max) max = datum;
    sum += (double)datum;
    sum_of_squares += ((double)datum * (double)datum);
    entries++;
  }

  word PutWithHeader(  //total number of bytes written
    byte* packet,      //pointer to byte
    word byte_count,   //number of bytes in packet
    byte destination); //destination address for packet header

  virtual ostream& out(ostream& o);

  StatBuffer(void)
  {
    Reset();
    ResetStats();
  }
};   //class StatBuffer


//===========================================================================
//  StatBuffer::out(ostream& o)
//  Dumps the contents of the buffer to an output stream.
//---------------------------------------------------------------------------
ostream& StatBuffer::out(ostream& o)
{
  long flags = o.flags() & ios::basefield;
  char fill = o.fill(' ');

  OneWayBuffer::out(o);
  o << dec;
  o << "Begin time: " << setw(10) << begin_time << " ms" << endl;
  o << "End Time  : " << setw(10) << end_time   << endl;
  o << "Error: 0x" << setfill('0') << hex << setw(2) << (word)error << endl;
  o << dec;

  o.setf(flags, ios::basefield);
  o.fill(fill);
  return o;
}   //ostream& StatBuffer::out(ostream& o)


//===========================================================================
//  Checksum
//  Calculates 1-byte additive checksum.
//---------------------------------------------------------------------------
byte Checksum(         //one-byte additive checksum on data
  byte* data,          //pointer to array of bytes
  word count)          //number of bytes in array
{
  byte checksum = 0;
  while (count--) checksum += *data++;
  return checksum;
}   //Checksum


//===========================================================================
//  StatBuffer::PutWithHeader
//  Adds the header to the packet to be sent; places the packet into the
//  buffer.
//---------------------------------------------------------------------------
word StatBuffer::PutWithHeader(  //total number of bytes written
  byte* packet,                //pointer to byte
  word byte_count,             //number of bytes in packet
  byte destination)            //destination address for packet header
{
  byte count_msb = (byte_count+4) >> 8;     //length includes checksum
  byte count_lsb = (byte_count+4) & 0xFF;
  byte checksum = Checksum(packet, byte_count);

  word previous_count = Count();

  checksum += count_msb + count_lsb + destination;
  Put(destination);
  Put(count_lsb);
  Put(count_msb);
  Put(packet, byte_count);
  Put(-checksum);
  return Count() - previous_count;
}   //StatBuffer::PutWithHeader


//===========================================================================
//  variables...
//---------------------------------------------------------------------------
#define DESTINATION 0x32
#define BAUD_RATE   19200.0
#define VERBOSE     0

char script_file[0x80] = "\0";
word destination = DESTINATION;
double baud_rate = BAUD_RATE;
int verbose = VERBOSE;
StatBuffer Rx;
StatBuffer Tx;

CommandLine command_line(
      + *new Parameter("script file", script_file),
      + *new Switch("destination address", 'a', &destination, "%x")
      + *new Switch("baud rate", 'b', &baud_rate, "%lf")
      + *new Switch("verbose", 'v', &verbose, NULL, ScanFlag) );


//===========================================================================
//  ScriptPacket::run
//  Sends the packet and waits for the response.
//---------------------------------------------------------------------------
int  ScriptPacket::run(void)
{
  static byte data[0x110];
  if (packet == NULL)
  {
    cout << "Packet data error for \"" << name << "\".\n";
    return 0;
  }

  word n = packet->getData(data);

  Rx.Reset();
  Tx.Reset();

  Tx.begin_time = GetTickerCount();
  Tx.PutWithHeader(data, n, destination);

  if (verbose) cout << "\nSending:\n" << (OneWayBuffer)Tx;

  if (PutFrameA(Tx.data, Tx.Count()) != Tx.Count())
  {
    cout << "Error transmitting packet \"" << name << "\".\n";
    return 0;
  }
  Tx.end_time = GetTickerCount();

  Rx.count = GetFrameA(Rx.data, sizeof(Rx.data), 72);  //4 seconds for timeout
  Rx.begin_time = GetTickerCount();
  Rx.end_time = GetTickerCount();

  if (Rx.Count() == 0)
  {
    cout << "\nDidn't receive any bytes!" << endl;
  }
  else if (verbose)
  {
    cout << '\n' << (OneWayBuffer) Rx << dec
         << "Tx started:     " << setw(10) << Tx.begin_time << " ms" << '\n'
         << "Tx complete:    " << setw(10) << Tx.end_time   << '\n'
         << "First Received: " << setw(10) << Rx.begin_time << '\n'
         << "Received:       " << setw(10) << Rx.end_time   << '\n'
         << flush;
  }

//  z30.Rx.Reset();
//  z30.Tx.Reset();
  return 1;
}   //ScriptPacket::run


//===========================================================================
//  ScriptDelay::run
//  Runs the delay command.
//---------------------------------------------------------------------------
int  ScriptDelay::run(void)
{
  lword start = GetTickerCount();
  if (verbose) cout << "Delaying " << dec << milliseconds << " ms...";
  while ((GetTickerCount() - start) < milliseconds)
  {
    if (*kbin) break;
  }   //while
  if (verbose) cout << endl;
  if (*kbin) return 0;
  return 1;
}   //ScriptDelay::run


//===========================================================================
//  SDLCScript::run
//  Runs the whole script.
//---------------------------------------------------------------------------
int  SDLCScript::run(void)
{
  Rx.ResetStats();
  Tx.ResetStats();
  while (1)
  {
    if (ScriptDataList::run() == 0) cout << "Error." << endl;
    Rx.UpdateStats(Rx.begin_time - Tx.end_time);
    cout << "\rthis=" << dec << Rx.begin_time - Tx.end_time
         << " min=" << Rx.min
         << " max=" << Rx.max
         << " avg=" << Rx.sum/Rx.entries
         << "               ";
    if (*kbin) if (kbin.Get() == ESC) break;
  }
  cout << endl
       << "Stats for script file..." << endl
       << "  Packets sent: " << dec << Rx.entries << endl
       << "  Min response time: " << Rx.min << endl
       << "  Max response time: " << Rx.max << endl
       << "  Avg response time: " << Rx.sum/Rx.entries << endl;
  Rx.Reset();
  Tx.Reset();
  return 1;
}   //SDLCScript::run


//===========================================================================
void z30setup(int c)                   //set up a channel
{
//modes...
  z30write(c,WR9 ,0xc0);               //hardware reset
//z30write(c,WR0 ,0x00);               //reset, cuz that's what book says
  z30write(c,WR4 ,0x20);               //x1 clock, SDLC mode
  z30write(c,WR1 ,0x00);               //no interrupts, reset DMA
  z30write(c,WR2 ,0x00);               //unused interrupt vector
  z30write(c,WR3 ,0xc0);//c4           //no rx 4 now,rx 8 data,addr search
  z30write(c,WR5 ,0x63);               //no DTR,tx 8 data,no tx 4 now,CRC on
  z30write(c,WR6 ,0x00);               //our address
  z30write(c,WR7 ,0x7e);               //sync character, 6 1s
  z30write(c,WR9 ,0x00);               //disable all interrupts -- chipwide
  z30write(c,WR10,0x00);               //NRZ, no loop mode
  z30write(c,WR11,0x16);               //tx clock=BR generator, rx clock=RTXC
  z30setbaud(c,1,baud_rate);           //set baud rate
  z30write(c,WR14,0x02);               //no BR 4 now, local loopback
  z30write(c,WR14,0x02);               //again? just follow the book, ma'am
//enables...
  z30write(c,WR14,0x07);//03           //enable BR generator, local loopback
  z30write(c,WR3 ,0xc1);//c5           //rx enable
  z30write(c,WR5 ,0x6b);               //no DTR,tx 8 data bits,enable tx,CRC on
  z30write(c,WR0 ,0x80);               //reset tx CRC
  z30write(c,WR1 ,0x00);               //no interrupts/dma
//interrupt...
  z30write(c,WR15,0x00);               //reset miscellaneous flags
  z30write(c,WR0 ,0x10);               //reset ext/status
  z30write(c,WR0 ,0x10);               //reset ext/status, again
  z30write(c,WR9 ,0x00);               //nothin, but it's in the book
}   //z30setup


//===========================================================================
//  Help
//  Shows commandline format of program.
//---------------------------------------------------------------------------
int  Help(                     //whatever was passed in as an argument
  int return_code)             //value to return, usually the DOS return code
{
  cout
    << "\n"
    << "Usage: z30tim [switches] <script-file>\n"
    << "Switches [default]:\n"
    << "  -a<address>   hex destination address ["
              << hex << setfill('0') << setw(2) << DESTINATION << "]\n"
    << "  -b<baudrate>  baud rate [" << BAUD_RATE << "]\n"
    << "  -v[0/1]       verbose output [" << (int) VERBOSE << "]\n"
    << flush;
  return return_code;
}   //Help


//===========================================================================
//  main
//---------------------------------------------------------------------------
int  main(                     //DOS return code
  int argc,                    //number of arguments from commandline
  char* argv[])                //pointer to array of argument pointers
{
  cout << "z30tim -- sends a script file and keeps track of timing.\n";

  if (!command_line.Load(argc, argv)) return Help(1);
  if (!script_file[0])
  {
    cout << "No script file specified.\n";
    return Help(1);
  }

  cout << "using script file \"" << script_file << "\"...\n";

  SDLCScript* script = new SDLCScript(script_file);

  if (script == NULL)
  {
    cout << "Out of memory opening \"" << script_file << "\"." << endl;
    return 2;
  }

  switch (script->error)
  {
  case SDLCScript::no_error:
    if (!z30check())
    {
      cout << "z8530 is NOT attached.\n";
      break;
    }
    InstallOneMillisecondTicker();
    z30setup(CHANA);
    script->run();
    RemoveOneMillisecondTicker();
    break;
  case SDLCScript::no_file:
    cout << "Could not open \"" << script_file << "\"." << endl;
    break;
  case SDLCScript::error_on_line:
    cout << "File \"" << script->filename
         << "\" line " << dec << script->error_line
         << ", Error: \"" << script->token_in_error << "\"." << endl;
    break;
  default:
    cout << "Weird error \"" << script->token_in_error << "\" on line "
         << dec << script->error_line << "." << endl;
    break;
  }   //switch

  delete script;

  return 0;
}   //main

