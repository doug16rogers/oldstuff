#include <stdio.h>

#include "tbutil.h"

#define Rows 5
#define Cols 20

void main(void)
{
  TBuf tb(Rows,Cols,ATTR(WHITE,BLUE));
  WORD* w;

  tb << "Test message\n";

  printf("Rows=%04X,Row=%04X\nCols=%04X,Col=%04X\n",
    tb.getrows(),tb.getrow(),tb.getcols(),tb.getcol());

  tb.printf("This is line nl\n%04X <-- test hex\ro\n^overwrite\n",0x1234);
  tb << "This line should force a newline and scroll.  Let\'s be hopin!!";

  w=tb.gettxt();
  for (int i=0;i<Rows;i++) {
    for (int j=0;j<Cols;j++) {
      textattr(*w>>8);
      cprintf("%c",*w&0x00ff);
      w++;
    }
    cprintf("\r\n");
  }   //for
}   //main

