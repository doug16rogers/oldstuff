#include <stdiostr.h>
#include <dos.h>
//#include "mul.hpp"

#ifndef lword
#define lword unsigned long
#endif

//#ifndef word
//#define word unsigned
//#endif


#define ex_reg     __emit__(0x66);
#define ex_add     __emit__(0x67);


lword Mul32(lword a, lword b)
{
  ex_reg  asm mov ax,word ptr a
  ex_reg  asm mul word ptr b
          asm mov dx,ax
          asm mov cl,0x10
  ex_reg  asm shr ax,cl
          asm xchg ax,dx
  return (lword) MK_FP(_DX,_AX);
}   //Mul32


void main(void)
{
  lword a;
  lword b;
  lword c;

  cout << hex;
  while (1)
  {
    cout << "Enter a: ";  cin >> hex >> a;
    cout << "Enter b: ";  cin >> hex >> b;
    c = Mul32(a, b);
    cout << "0x" << a << " * 0x" << b << " = 0x" << c << '\n';
    if ((a == 0) && (b == 0)) break;
  }
}   //main
