#ifndef __ALPHABET_HPP
#define __ALPHABET_HPP


#ifndef BYTE
#define BYTE unsigned char
#endif


//***************************************************************************
//  class Alphabet
//***************************************************************************
//-----------------------------Description-----------------------------------
//  This class keeps track of a changing alphabet.
//-----------------------------Arguments-------------------------------------
//-----------------------------Return value----------------------------------
//-----------------------------Global constants------------------------------
//-------------------Mod-------Global variables------------------------------
//-----------------------------Functions called------------------------------
//-----------------------------Examples--------------------------------------
//-----------------------------Constraints/Gotchas---------------------------
//--Date--------Programmer----------Comments---------------------------------
//  1990.01.01  A. Turing           initial code
//***************************************************************************
class Alphabet {
protected:
#define MAX_ALPHA      0x100
  BYTE alpha[MAX_ALPHA];
  int inver[MAX_ALPHA];
  int count;
public:
  int  Count(void) { return count; }
  BYTE Char(int i) { if ((i<0)||(i>=count)) return 0; else return alpha[i]; }
  BYTE operator [](int i) { return Char(i); }
  int  Inv(BYTE ch) { return inver[ch]; }
  int  Add(BYTE ch);
  int  Add(char *s);
  void Purge(void);
#define INTRO      "Alphabet=\""
#define ENDING     "\"\n"
  void Show(char* intro=INTRO,char* ending=ENDING);

  Alphabet() { Purge(); }
  ~Alphabet() {};
};   //Alphabet


#endif
