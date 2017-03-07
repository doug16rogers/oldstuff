#ifndef __BITSTR_HPP
#define __BITSTR_HPP


#ifndef SBIT8
  #define SBIT8 char
#endif

#ifndef UBIT8
  #define UBIT8 unsigned char
#endif

#ifndef SBIT16
  #define SBIT16 int
#endif

#ifndef UBIT16
  #define UBIT16 unsigned int
#endif

#ifndef SBIT32
  #define SBIT32 long int
#endif

#ifndef UBIT32
  #define UBIT32 unsigned long int
#endif

#ifndef HIBYTE
  #define HIBYTE(x)  *(((UBIT8*)&x)+1)
#endif

#ifndef LOBYTE
  #define LOBYTE(x)  *(UBIT8*)&x
#endif


//===========================================================================
//  class BIT_STREAM
//===========================================================================
//-----------------------------Description-----------------------------------
//  This class is used to manipulate a stream of bits.  It is set up like a
//  normal byte buffer, with elementary put/get functions.  The use of Put()
//  and Get() will implement a FIFO, whereas the use of Put() and UnPut()
//  (or UnGet() and Get()) will implement a LIFO/stack.  The bits may be
//  addressed as an array using [] and Set(), and simple rotations may then
//  be performed via BitPut(BitGet()) or BitUnGet(BitUnPut()).
//-----------------------------Members---------------------------------------
//-----------------------------Global constants------------------------------
//-------------------Mod-------Global variables------------------------------
//-----------------------------Functions called------------------------------
//-----------------------------Examples--------------------------------------
//-----------------------------Constraints/Gotchas---------------------------
//--Date--------Programmer----------Comments---------------------------------
//  1991.06.28  D. Rogers           initial code                            -
//===========================================================================

class BIT_STREAM {                     //bit stream manipulation
private:
  UBIT8  *base;                        //pointer to base of bit storage
  UBIT16 max;                          //number of bits maximum
  UBIT16 cnt;                          //number of bits available
  UBIT16 hed;                          //beginning (source) of stream
  UBIT16 tal;                          //pointer to end (sink) of stream
public:
  BIT_STREAM(int maxbits=0x800);       //constructor
//=======================  ~BIT_STREAM(); { delete base; }    //destructor
  int    Empty(void) { return cnt==0; }   //tells when buffer has run out
  int    Full(void) { return cnt>=max; }  //tells when buffer is full
  void   Flush(void) { cnt=hed=tal=0; }   //reset stream
  UBIT8  operator [] (UBIT16 k);       //reads from current offset ([0])
  void   Set(UBIT16 index,UBIT8 bit);  //sets from offset (can't use [])
  UBIT16 Length(void) { return cnt; }  //number of bits in stream
  UBIT16 Peek(UBIT16 off,UBIT16 bits); //peeks from offset, no bits removed

  void   BitPut(UBIT8 bit);            //inserts a single bit
  void   Put(UBIT16 word,UBIT16 bits); //add from a word
  void   Put(UBIT32 word,UBIT16 bits); //add from a long word
  void   Put(UBIT8* data,UBIT16 bits); //add from byte stream

  UBIT8  BitGet(void);                 //extracts a single bit
  UBIT16 Get(UBIT16 bits);             //get bits
  void   Get(UBIT8* data,UBIT16 bits); //get bits into byte buffer

  UBIT8  BitUnPut(void);               //un-inserts a single bit
  UBIT16 UnPut(UBIT16 bits);             //get bits from head end, not tail
  void   UnPut(UBIT8* data,UBIT16 bits); //get from head into byte buffer

  void   BitUnGet(UBIT8 bit);          //un-extracts a single bit
  void   UnGet(UBIT16 bits=1);         //ungets with current contents
  void   UnGet(UBIT16 word,UBIT16 bits);  //add to tail end from a word
  void   UnGet(UBIT32 word,UBIT16 bits);  //add to tail end from a long word
  void   UnGet(UBIT8* data,UBIT16 bits);  //add to tail from byte stream
};   //class BIT_STREAM


#endif
