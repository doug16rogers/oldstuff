#ifndef __MORSUTIL_H
#define __MORSUTIL_H


typedef void (*MorseON)(int freq,char ditdah);
typedef void (*MorseOFF)(void);


class Morse {
private:
  int    ditlen;
  int    dahlen;
  int    ditdahdel;
  int    letterdel;
  int    delscale;
  int    freq;
  MorseON morseon;
  MorseOFF morseoff;
public:
  Morse (MorseON mon,MorseOFF moff);
  Morse (void);
  ~Morse (void) { return; }
  char* morsestring(char c);
  char* playmorsestring(char* s);
  char* playchar(char c);
  char* playstring(char* s);
  char  charof(char *mstring);
  int   scale(void)  { return delscale; }
  int   scale(int s);
  int   frequency(void)     { return freq; }
  int   frequency(int f) { if (f>0) freq=f; return freq; }
  MorseON  ON (MorseON  mon)  { if (mon)  morseon=mon;   return morseon;  }
  MorseOFF OFF(MorseOFF moff) { if (moff) morseoff=moff; return morseoff; }
  Morse& operator<<(char c);
  Morse& operator<<(char* s);
};   //class Morse

#endif
