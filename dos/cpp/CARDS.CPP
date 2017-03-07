#include <stdio.h>
#include <stdlib.h>

#include "cardutil.hpp"


#define NOTHING            0x0000
#define ONE_PAIR           0x0001
#define TWO_PAIR           0x0002
#define PAIRS              0x0003
#define THREE_OF_A_KIND    0x0004
#define STRAIGHT           0x0008
#define FLUSH              0x0010
#define FULL_HOUSE         (ONE_PAIR|THREE_OF_A_KIND)
#define FOUR_OF_A_KIND     0x0020
#define STRAIGHT_FLUSH     (STRAIGHT|FLUSH)
#define FIVE_OF_A_KIND     0x0040


class PokerHand : public CardHand {
public:
  PokerHand(void) : CardHand(5) { return; }
  word Type(void);
  char* TypeName(void);
};   //PokerHand


//===========================================================================
word PokerHand::Type(void)
{
  if (n==0) return NOTHING;
  if (n==1) return (FLUSH | STRAIGHT);

  Sort(0);                             //sort, but keep ace low
  word val=NOTHING;
  int i;
  for (i=0;i<n;i++) if (SUIT(card[i])!=SUIT(card[0])) break;
  if (i>=n) val |= FLUSH;
  for (i=1;i<n;i++) if (PIP(card[i])!=(PIP(card[i-1])+1)) break;
  if (i>=n) val |= STRAIGHT;
  if ((PIP(card[0])==ACE) && (PIP(card[n-1])==KING)) {
    for (i=n-1;i>1;i--) if (PIP(card[i])!=(PIP(card[i-1])+1)) break;
    if (i<=1) val |= STRAIGHT;
  }
  if (val & STRAIGHT) return val;

  int cnt[20];
  for (i=0;i<n;i++) cnt[i]=1;
  int grp=0;
  for (i=1;i<n;i++) {
    if (PIP(card[i])==PIP(card[i-1])) cnt[grp]++; else grp++;
  }
  for (i=0;i<=grp;i++) {
    switch (cnt[i]) {
    case 2: val += ONE_PAIR; break;
    case 3: val |= THREE_OF_A_KIND; break;
    case 4: val |= FOUR_OF_A_KIND; break;
    case 5: val |= FIVE_OF_A_KIND; break;
    }
  }   //for
  return val;
}   //PokerHand::Type(void)


//===========================================================================
char* PokerHand::TypeName(void)
{
  switch (Type()) {
  case NOTHING:          return "Nothing";
  case ONE_PAIR:         return "One Pair";
  case TWO_PAIR:         return "Two Pair";
  case THREE_OF_A_KIND:  return "Three-of-a-kind";
  case STRAIGHT:         return "Straight";
  case FLUSH:            return "Flush";
  case FULL_HOUSE:       return "Full House";
  case FOUR_OF_A_KIND:   return "Four-of-a-kind";
  case STRAIGHT_FLUSH:   return "Straight Flush";
  case FIVE_OF_A_KIND:   return "Five-of-a-kind";
  }   //switch
  return "Unknown";
}   //PokerHand::TypeName(void)


//===========================================================================
void Show(PokerHand& hand)
{
  int i;
  for (i=0;i<*hand;i++) printf("--------");  printf("\n");
  for (i=0;i<*hand;i++) printf("\t%u",i);  printf("\n");
  for (i=0;i<*hand;i++) printf("\t%s",CardName(hand[i]));
  printf("\t%s\n",hand.TypeName());
}   //Show


//===========================================================================
void Draw(PokerHand& hand,CardDeck& deck,int k)
{
  char s[80];
  int i;

  Show(hand);
  printf("Player %u, which ones to replace?: ",k); gets(s);
  for (i=0;s[i];i++) if ((s[i]>='0')&&(s[i]<='4')) {
    if (*deck) { hand.Del(s[i]-'0'); hand.Add(!deck,s[i]-'0'); }
  }
}   //Draw


//===========================================================================
#define HANDS 2

void main(void)
{
  CardDeck deck(1);
  PokerHand hand[HANDS];
  char s[80];

  int i,h;
  while (1) {
    printf("==================================================\n");
    deck.Refill();
    for (h=0;h<HANDS;h++) hand[h].Flush();
    for (i=0;i<5;i++) for (h=0;h<HANDS;h++) hand[h] += !deck;

    for (h=0;h<HANDS;h++)
      hand[h].Sort();

    for (h=0;h<HANDS;h++) { Draw(hand[h],deck,h); hand[h].Sort(); }

    for (h=0;h<HANDS;h++) Show(hand[h]);

    s[0]=0;
    printf("press <CR>"); gets(s);
    if (s[0]) break;
  }   //while

}   //main

