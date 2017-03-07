#include <iostream.h>
#include <iomanip.h>


//===========================================================================
class ReverseBits
{
private:
  static unsigned char *revbits;
public:
  ReverseBits(void);
 ~ReverseBits() { if (revbits != NULL) delete revbits; }
  unsigned char operator [](int i) { return revbits[i&0x00FF]; }
};   //ReverseBits

unsigned char* ReverseBits::revbits = NULL;

ReverseBits::ReverseBits(void)
{
  if (revbits != NULL) return;
  revbits = new unsigned char[0x100];

  int i,k,b,j;
  for (i = 0; (i < 0x100); i++)
  {
    k = i;
    b = 0;
    for (j = 0; (j < 8); j++)
    {
      b <<= 1;
      b  |= k & 1;
      k >>= 1;
    }
    revbits[i] = b;
  }
}   //ReverseBits::ReverseBits(void)


ReverseBits rev;


void main(void)
{
  cout << setbase(16) << setfill('0') << setiosflags(ios::uppercase);
  for (int i = 0; (i < 0x100); i++)
  {
    cout << "0x";
    cout << setw(2);
    cout << (unsigned int)rev[i] << ", ";
    if ((i & 0x03) == 0x03) cout << ' ';
    if ((i & 0x07) == 0x07) cout << '\n';
  }   //for
}   //main

