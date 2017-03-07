#include <stdio.h>
#include <stdlib.h>

#include "cardutil.hpp"


byte NormalCardList[53]={
  (CLUBS<<4)+TWO,
  (CLUBS<<4)+THREE,
  (CLUBS<<4)+FOUR,
  (CLUBS<<4)+FIVE,
  (CLUBS<<4)+SIX,
  (CLUBS<<4)+SEVEN,
  (CLUBS<<4)+EIGHT,
  (CLUBS<<4)+NINE,
  (CLUBS<<4)+TEN,
  (CLUBS<<4)+JACK,
  (CLUBS<<4)+QUEEN,
  (CLUBS<<4)+KING,
  (CLUBS<<4)+ACE,
  (DIAMONDS<<4)+TWO,
  (DIAMONDS<<4)+THREE,
  (DIAMONDS<<4)+FOUR,
  (DIAMONDS<<4)+FIVE,
  (DIAMONDS<<4)+SIX,
  (DIAMONDS<<4)+SEVEN,
  (DIAMONDS<<4)+EIGHT,
  (DIAMONDS<<4)+NINE,
  (DIAMONDS<<4)+TEN,
  (DIAMONDS<<4)+JACK,
  (DIAMONDS<<4)+QUEEN,
  (DIAMONDS<<4)+KING,
  (DIAMONDS<<4)+ACE,
  (HEARTS<<4)+TWO,
  (HEARTS<<4)+THREE,
  (HEARTS<<4)+FOUR,
  (HEARTS<<4)+FIVE,
  (HEARTS<<4)+SIX,
  (HEARTS<<4)+SEVEN,
  (HEARTS<<4)+EIGHT,
  (HEARTS<<4)+NINE,
  (HEARTS<<4)+TEN,
  (HEARTS<<4)+JACK,
  (HEARTS<<4)+QUEEN,
  (HEARTS<<4)+KING,
  (HEARTS<<4)+ACE,
  (SPADES<<4)+TWO,
  (SPADES<<4)+THREE,
  (SPADES<<4)+FOUR,
  (SPADES<<4)+FIVE,
  (SPADES<<4)+SIX,
  (SPADES<<4)+SEVEN,
  (SPADES<<4)+EIGHT,
  (SPADES<<4)+NINE,
  (SPADES<<4)+TEN,
  (SPADES<<4)+JACK,
  (SPADES<<4)+QUEEN,
  (SPADES<<4)+KING,
  (SPADES<<4)+ACE,
  JOKER
};   //NormalCardList


char *card_name[]={
"A_C","2_C","3_C","4_C","5_C","6_C","7_C","8_C","9_C","10C","J_C","Q_C","K_C",
"A_D","2_D","3_D","4_D","5_D","6_D","7_D","8_D","9_D","10D","J_D","Q_D","K_D",
"A_H","2_H","3_H","4_H","5_H","6_H","7_H","8_H","9_H","10H","J_H","Q_H","K_H",
"A_S","2_S","3_S","4_S","5_S","6_S","7_S","8_S","9_S","10S","J_S","Q_S","K_S",
};


char *CardName(byte card)
{
  if (!card)
    return "<0>";
  else if ((card & JOKER)==JOKER)
    return "JOK";
  else
    return card_name[(SUIT(card)-1)*13+(PIP(card)-1)];
}   //CardName(byt)



//===========================================================================
CardRandom::CardRandom(int seed)
{
  int i;
  word k=1;

  if (seed) srand(seed); else randomize();
  for (i=0;i<250;i++) {                //put in initial random numbers
    r[i]=rand();                       //..initialize each byte of array
  }   //for initial values
  for (i=0;i<(sizeof(r[0])*8);i++) {   //make sure we have word-sized basis..
    r[i] |= k;                         //..by setting each bit
    r[i+1] &= ~k;                      //..and resetting it, too
    k<<=1;                             //..go to next bit
  }   //for basis-ensuring
  index=0;                             //current location in array
  for (i=0;i<(sizeof(r[0])*8);i++) Rnd();
}   //CardRandom(void)


word CardRandom::Rnd(void)
{
  register word v;

  if (index>=103)
    v=r[index] ^ r[index-103];
  else
    v=r[index] ^ r[index+250-103];
  r[index++]=v;
  if (index>=250) index=0;
  return v;
}   //CardRandom::Rnd(void)


byte CardRandom::Card(int jokers)
{
  register word v;

  if (index>=103)
    v=r[index] ^ r[index-103];
  else
    v=r[index] ^ r[index+250-103];
  r[index++]=v;
  if (index>=250) index=0;
  v %= (52+jokers);
  if (v<52)
    return NormalCardList[v];
  else
    return JOKER;
}   //CardRandom::Rnd(void)


CardRandom CardRand;


//===========================================================================
CardHand::CardHand(int _size)
{
  n=0;
  card=new byte[size=_size];
  if (card==NULL) size=0;
}   //CardHard::CardHand(int)


byte CardHand::Add(byte c,int i)
{
  if (i==-1) i=n;
  if (n>=size) return 0;
  if ((i>n)||(i<0)) return 0;
  if (!c) return 0;
  n++;
  for (int k=n;k>i;k--) card[k]=card[k-1];
  card[i]=c;
  return c;
}   //CardHand::Add(byte,int)


byte CardHand::Del(int i)
{
  if (i==-1) i=n-1;
  if (!n) return 0;
  if ((i<0)||(i>=n)) return 0;
  byte c=card[i];
  n--;
  for (int k=i;k<n;k++) card[k]=card[k+1];
  return c;
}   //CardHand::Del(int)


int  CardHand::Find(byte c,int del)
{
  if (!n) return -1;
  for (int k=0;k<n;k++) if (card[k]==c) break;
  if (k>=n) return -1;
  if (del) Del(k);
  return k;
}   //CardHand::Del(byte)


void CardHand::Shuffle(void)
{
  int i;
  int r;
  register byte tmp;

  if (!card) return;
  for (i=0;i<n;i++) {
    r=CardRand.Rnd(n);
    tmp=card[i];
    card[i]=card[r];
    card[r]=tmp;
  }
}   //CardHand::Shuffle


byte CardHand::Move(int dest,int source)
{
  if (source==-1) source=n-1;
  if ((dest<0)||(dest>=n)||(source<0)||(source>=n)) return 0;
  if (source<dest)
    return Add(Del(source),dest-1);
  else
    return Add(Del(source),dest);
}   //CardHand::Move(int,int)


void CardHand::Sort(int method,int ace_hi)
{
  int i;
  if (ace_hi) for (i=0;i<n;i++) if (PIP(card[i])==ACE) {
      card[i]&=~PIP_MASK;
      card[i]|=ACE_HI;
  }   //if/for/if

  int swap=0;
  i=0;
  while (i<(n-1)) {
    swap=0;
    if (method==SUIT_OVER_NUM)
      swap = (card[i]>card[i+1]);
    else {
      if (PIP(card[i])>PIP(card[i+1])) swap=1;
      else if (PIP(card[i])<PIP(card[i+1])) swap=0;
      else swap = card[i]>card[i+1];
    }
    if (swap) {
      byte temp=card[i];
      card[i]=card[i+1];
      card[i+1]=temp;
      if (i) i--; else i=1;
    }
    else
      i++;
  }

  if (ace_hi) for (i=0;i<n;i++) if (PIP(card[i])==ACE_HI) {
      card[i]&=~PIP_MASK;
      card[i]|=ACE;
  }   //if/for/if
}   //CardHand::Sort(int,int)


//===========================================================================
CardDeck::CardDeck(int _decks,int _jokers,int shuffle) :
  CardHand(52*(_decks+_jokers))
{
  jokers=_jokers;
  decks=_decks;
  Refill(shuffle);
}   //CardDeck::CardDeck(int,int,int)


void CardDeck::Refill(int shuffle)
{
  Flush();
  int k=0;
  int j;
  for (int d=0;d<decks;d++) {
    for (j=0;j<52    ;j++) if (k<size) card[k++]=NormalCardList[j];
    for (j=0;j<jokers;j++) if (k<size) card[k++]=JOKER;
  }
  n=k;
  if (shuffle) Shuffle();
}   //CardDeck::Refill(int)

