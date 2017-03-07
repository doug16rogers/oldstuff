#ifndef __R250_HPP
#define __R250_HPP


#define SIZE           250
#define PREV           103

#ifndef WORD
  typedef unsigned int WORD;
#endif
#ifndef DWORD
  typedef unsigned long int DWORD;
#endif


class R250 {                           //pseudorandom number generator
private:
  int  index;                          //index of second array element
  DWORD r[SIZE];                       //pointer to array
public:
  R250(void);                          //fill randomly lau time
  R250(int seed);                      //seed for initialization
//~R250();                             //destructor doesn't do anything
  DWORD rnd(void);                     //returns random on [0,0xFFFFFFFF]
  WORD rnd(WORD topval) {              //returns random on [0,topval-1]
    if (topval>1)
      return ((WORD)rnd()) % topval;
    else
      return ((WORD)rnd()) & 1;
  };
  double frnd(void) {                  //doublizer, random on [0.0,1.0]
    return ((double)(DWORD)rnd())/((double)0xFFFFFFFFUL);
  };
  double frnd(double lo,double hi) {   //doublizer, random on [lo,hi]
    return (hi-lo)*frnd()+lo;          //put it in range
  };
};   //class R250


#endif
