#ifndef __CARDUTIL_HPP
#define __CARDUTIL_HPP


#ifndef byte
#define byte unsigned char
#endif

#ifndef word
#define word unsigned int
#endif


#define PIP_MASK   0x0F
#define SUIT_MASK  0x70

#define PIP(c)     ((c)&PIP_MASK)
#define SUIT(c)    (((c)&SUIT_MASK)>>4)
#define DEALT(c)   ((c)&0x80)

enum SUITS {
  CLUBS=1,
  DIAMONDS,
  HEARTS,
  SPADES,
  JOKER_SUIT=0x07
};

enum PIPS {
  ACE=1,
  TWO,
  THREE,
  FOUR,
  FIVE,
  SIX,
  SEVEN,
  EIGHT,
  NINE,
  TEN,
  JACK,
  QUEEN,
  KING,
  ACE_HI,
  JOKER_PIP=0x0F,
  JOKER=0x7F
};

typedef struct {                       //Card type
  unsigned pip:4;                      //pip value of card
  unsigned suit:3;                     //suit of card
  unsigned dealt:1;                    //whether card was dealt
} Card;

extern byte NormalCardList[53];
extern char *card_name[];

char *CardName(byte card);


//===========================================================================
class CardRandom {
private:
  int  index;
  word r[250];
public:
  CardRandom(int seed=0);
  word Rnd(void);
  word Rnd(word n) { if (n>1) return Rnd()%n; else return Rnd()&1; }
  byte Card(int jokers=0);
};   //class CardRandom


extern CardRandom CardRand;


//===========================================================================
class CardHand {
protected:
  int size;
  int n;
  byte* card;
public:

  CardHand(int _size=13);
  ~CardHand() { delete card; }
  int  Count(void) { return n; }
  int  Size(void)  { return size; }
  int  operator *(void) { return n; }
  byte operator [] (int i) {
    if ((i<0)||(i>=n)) return 0;
    return card[i];
  }   //operator []
  byte Add(byte c,int i=-1);
  byte operator += (byte c) { return Add(c); }
  byte Del(int i=-1);
  byte operator -= (int i=-1) { return Del(i); }
  int  Find(byte c,int del=0);
  byte operator ! (void) { return Del(); }
  void Flush(void) { n=0; }
  void Shuffle(void);
  byte Move(int dest=0,int source=-1);
  enum { NUM_OVER_SUIT,SUIT_OVER_NUM };
  void Sort(int method=NUM_OVER_SUIT,int ace_hi=1);
};   //class CardHand


//===========================================================================
class CardDeck : public CardHand {
private:
  int decks;
  int jokers;
public:
  CardDeck(int _decks=1,int _jokers=0,int shuffle=1);
  void Refill(int shuffle=1);
};   //class CardDeck


#endif
