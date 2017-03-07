#ifndef __BUFF_HPP
#define __BUFF_HPP


#include "alphabet.hpp"


#ifndef BYTE
#define BYTE unsigned char
#endif


class ByteBuffer {
protected:
  BYTE *buff;
  unsigned int bytecount[0x100];
  BYTE sorted[0x100];
  unsigned int different_bytes;
  unsigned int size;
  unsigned int count;
  char sort_performed;
public:
  ByteBuffer(unsigned int _size=0x4000);
  ~ByteBuffer() { if (buff) delete buff; }
  void Add(BYTE b) {
    if (count<size) {
      buff[count++]=b;
      if (!bytecount[b]++) different_bytes++;
      sort_performed=0;
    }  //if room
  }
  void Add(Alphabet& a,BYTE b) { if (a.Inv(b) != -1) Add(b); }
  void Add(char*s);
  void Add(Alphabet& a,char* s);
  int  Load(Alphabet& a,char* fn);
  unsigned int Count(void) { return count; }
  unsigned int Count(int k) {
    if ((k<0)||(k>0xFF)) return 0; else return bytecount[k];
  }
  BYTE Byte(int k) { if ((k<0)||(k>count)) return 0; else return buff[k]; }
  BYTE operator[](int k) {
    if ((k<0)||(k>count)) return 0; else return buff[k];
  }
  unsigned int Sort(void);             //returns different_bytes
  unsigned int Different(void) { return different_bytes; }
  BYTE SortedByte(int k) {
    if (!sort_performed) Sort();
    if ((k<0)||(k>count)) return 0; else return sorted[k];
  }
};   //class ByteBuffer


#endif
