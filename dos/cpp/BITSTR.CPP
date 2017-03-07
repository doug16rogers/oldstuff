#include "bitstr.hpp"


static UBIT16 bittab[0x10]={
  0x0001,0x0002,0x0004,0x0008, 0x0010,0x0020,0x0040,0x0080,
  0x0100,0x0200,0x0400,0x0800, 0x1000,0x2000,0x4000,0x8000
};   //bittab


//===========================================================================
BIT_STREAM::BIT_STREAM(int maxbits)    //constructor
{
  max=maxbits;
#if 0
  if (max>(sizeof(base)<<3)) max=(sizeof(base)<<3);
#else
  if (max&7)
    base=new UBIT8 [(max>>3)+1];       //get byte array -- leave room for end
  else
    base=new UBIT8 [max>>3];           //get byte array
#endif
  Flush();
}   //BIT_STREAM::BIT_STREAM


//===========================================================================
UBIT8 BIT_STREAM::operator [] (UBIT16 index)
{
  if (index>=cnt) return 0;
  UBIT16 off=tal+index;
  if (off>=max) off-=max;
  UBIT16 offbyt=off>>3;
  UBIT16 offbit=off&7;
  if (base[offbyt]&bittab[offbit]) return 1;
  return 0;
}   //[](UBIT16 k)


//===========================================================================
void BIT_STREAM::Set(UBIT16 index,UBIT8 bit)   //sets from current offset
{
  if (index>=cnt) return;
  UBIT16 off=tal+index;
  if (off>=max) off-=max;
  UBIT16 offbyt=off>>3;
  UBIT16 offbit=off&7;
  if (bit)
    base[offbyt]|=bittab[offbit];
  else
    base[offbyt]&=~bittab[offbit];
}   //Set(UBIT16,UBIT8)


//===========================================================================
UBIT16 BIT_STREAM::Peek(UBIT16 off,UBIT16 bits)   //scans bits from offset
{
  register UBIT16 bit=1;
  register UBIT16 val=0;
  if (bits>16) bits=16;
  for (UBIT16 i=0;i<bits;i++) {
    if ((*this)[off+i]) val|=bit;
    bit<<=1;
  }   //for
  return val;
}   //BIT_STREAM::Peek(UBIT16,UBIT16)


//===========================================================================
void BIT_STREAM::BitPut(UBIT8 bit)     //inserts a single bit
{
  if (cnt>=max) return;                //make sure there's room
  UBIT16 hedbyt=hed>>3;
  UBIT16 hedbit=hed&7;
  if (bit)
    base[hedbyt]|=bittab[hedbit];
  else
    base[hedbyt]&=~bittab[hedbit];
  cnt++;
  hed++;
  if (hed>=max) hed=0;
}   //BitPut


//===========================================================================
void BIT_STREAM::Put(UBIT16 word,UBIT16 bits)  //add from a word
{
  UBIT16 register bit=1;
  while (bits--) {
    if (word & bit)
      BitPut(1);                       //..if so, set a bit
    else
      BitPut(0);                       //..otherwise, reset a bit
    if ((bit<<=1)==0) bit=1;           //..shift up
  }
}   //Put(UBIT16,UBIT16)


//===========================================================================
void BIT_STREAM::Put(UBIT32 word,UBIT16 bits)  //add from a long word
{
  UBIT32 register bit=1;
  while (bits--) {
    if (word & bit)
      BitPut(1);                       //..if so, set a bit
    else
      BitPut(0);                       //..otherwise, reset a bit
    if ((bit<<=1)==0) bit=1;           //..shift up
  }
}   //Put(UBIT32,UBIT16)


//===========================================================================
void BIT_STREAM::Put(UBIT8* data,UBIT16 bits)  //add from byte stream
{
  register UBIT8 bit=1;
  while (bits-- && !Full()) {
    if (*data & bit)
      BitPut(1);                       //..if so, set a bit
    else
      BitPut(0);                       //..otherwise, reset a bit
    if ((bit<<=1)==0) { bit=1; data++; }   //..shift up
  }
}   //Put(UBIT8*,UBIT16)


//===========================================================================
UBIT8 BIT_STREAM::BitGet(void)         //extracts a single bit
{
  if (!cnt) return 0;                  //make sure there's some
  UBIT16 talbyt=tal>>3;
  UBIT16 talbit=tal&7;
  cnt--;
  tal++;
  if (tal>=max) tal=0;
  if (base[talbyt]&bittab[talbit]) return 1;
  return 0;
}   //BitGet


//===========================================================================
UBIT16 BIT_STREAM::Get(UBIT16 bits)    //get bits
{
  UBIT16 val=0;
  if (bits>32) bits=32;
  UBIT16 register bit=1;
  while (bits--) {
    if (BitGet())
      val|=bit;                        //..if so, set a bit
    else
      val&=~bit;                       //..otherwise, reset a bit
    if ((bit<<=1)==0) bit=1;           //..shift up
  }
  return val;
}   //Get(UBIT16)


//===========================================================================
void BIT_STREAM::Get(UBIT8* data,UBIT16 bits)  //get bits into byte buffer
{
  register UBIT8 bit=1;
  while (bits-- && !Empty()) {
    if (BitGet())
      *data|=bit;                      //..if so, set a bit
    else
      *data&=~bit;                     //..otherwise, reset a bit
    if ((bit<<=1)==0) { bit=1; data++; }   //..shift up
  }
}   //BIT_STREAM::Get(UBIT8*,UBIT16)


//===========================================================================
UBIT8 BIT_STREAM::BitUnPut(void)       //un-inserts a single bit
{
  if (!cnt) return 0;                  //make sure there's something
  cnt--;
  if (hed) hed--; else hed=max-1;
  UBIT16 hedbyt=hed>>3;
  UBIT16 hedbit=hed&7;
  if (base[hedbyt]&bittab[hedbit]) return 1;
  return 0;
}   //BitUnPut


//===========================================================================
UBIT16 BIT_STREAM::UnPut(UBIT16 bits)  //unput bits
{
  UBIT16 val=0;
  if (bits>32) bits=32;
  UBIT16 register bit=1;
  while (bits--) {
    if (BitUnPut())
      val|=bit;                        //..if so, set a bit
    else
      val&=~bit;                       //..otherwise, reset a bit
    if ((bit<<=1)==0) bit=1;           //..shift up
  }
  return val;
}   //UnPut(UBIT16)


//===========================================================================
void BIT_STREAM::UnPut(UBIT8* data,UBIT16 bits)  //unput bits into byte buff
{
  register UBIT8 bit=1;
  while (bits-- && !Empty()) {
    if (BitUnPut())
      *data|=bit;                      //..if so, set a bit
    else
      *data&=~bit;                     //..otherwise, reset a bit
    if ((bit<<=1)==0) { bit=1; data++; }   //..shift up
  }
}   //BIT_STREAM::UnPut(UBIT8*,UBIT16)


//===========================================================================
void BIT_STREAM::BitUnGet(UBIT8 bit)   //un-extracts a single bit
{
  if (cnt>=max) return;                //make sure it's not full
  cnt++;
  if (tal) tal--; else tal=max-1;      //make sure will work with BitGet
  UBIT16 talbyt=tal>>3;
  UBIT16 talbit=tal&7;
  if (bit)
    base[talbyt]|=bittab[talbit];
  else
    base[talbyt]&=~bittab[talbit];
}   //BitUnGet


//===========================================================================
void BIT_STREAM::UnGet(UBIT16 bits)    //ungets with current contents
{
  while (bits--) {
    if (cnt>=max) return;              //make sure it's not full
    cnt++;                             //increment available bits count
    if (tal) tal--; else tal=max-1;
  }   //while
}   //BIT_STREAM::UnGet(UBIT16)


//===========================================================================
void BIT_STREAM::UnGet(UBIT16 word,UBIT16 bits)  //unget from a word
{
  UBIT16 register bit=1;
  while (bits--) {
    if (word & bit)
      BitUnGet(1);                     //..if so, set a bit
    else
      BitUnGet(0);                     //..otherwise, reset a bit
    if ((bit<<=1)==0) bit=1;           //..shift up
  }
}   //UnGet(UBIT16,UBIT16)


//===========================================================================
void BIT_STREAM::UnGet(UBIT32 word,UBIT16 bits)  //unget from a long word
{
  UBIT32 register bit=1;
  while (bits--) {
    if (word & bit)
      BitUnGet(1);                     //..if so, set a bit
    else
      BitUnGet(0);                     //..otherwise, reset a bit
    if ((bit<<=1)==0) bit=1;           //..shift up
  }
}   //UnGet(UBIT32,UBIT16)


//===========================================================================
void BIT_STREAM::UnGet(UBIT8* data,UBIT16 bits)  //unget from byte stream
{
  register UBIT8 bit=1;
  while (bits-- && !Full()) {
    if (*data & bit)
      BitUnGet(1);                     //..if so, set a bit
    else
      BitUnGet(0);                     //..otherwise, reset a bit
    if ((bit<<=1)==0) { bit=1; data++; }   //..shift up
  }
}   //UnGet(UBIT8*,UBIT16)


