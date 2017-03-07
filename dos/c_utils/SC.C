#include <stdio.h>

#include "cutil.h"
#include "sercom.h"

int  com1_on,com2_on;
word port,rport;
int finished,convert_cr;
char *cnam[]={"COM1","COM2","COM3","COM4"};


main ()
{
  unsigned char c;
  char s[256];

  setup_ports ();
  finished=0;
  convert_cr=1;
  rport=COM1;
  printf ("\n<<Receiving over COM1>>\n");
  while (!finished) {
    if (kbhit ()) {
      c=getkey ();
      switch (c) {
        case _F1:
          do_help ();
          break;
        case S_F1:
          if (!com1_on) printf ("\n<<COM1 not available>>\n");
          else {
            port=COM1;
            printf ("\n<<COM1>>\n");
          }
          break;
        case S_F2:
          if (!com2_on) printf ("\n<<COM2 not available>>\n");
          else {
            port=COM2;
            printf ("\n<<COM2>>\n");
          }
          break;
        case C_F1:
          do_setup (COM1);
          break;
        case C_F2:
          do_setup (COM2);
          break;
        case _F5:
          printf ("\n<<Sendstring>>\n");
          gets (s);
          if (!s_puts (port,s)) printf ("<<Timeout putting string>>\n");
          printf ("<<End Sendstring>>\n");
          break;
        case _F8:
          convert_cr=0;
          break;
        case S_F8:
          convert_cr=1;
          break;
        case _F9:
          send_file (port);
          break;
        case S_F9:
          sendx_file (port);
          break;
        case _F10:
          printf ("\n<<Exit? (Y/N)>>");
          finished=upcase (getkey())=='Y';
          printf ("\n");
          break;
        case S_F10:
          finished=1;
          break;
        case CR:
          if (!s_putc (port,CR)) printf ("<<SorryCR!>>");
          if (convert_cr) if (!s_putc (port,LF)) printf ("<<SorryLF>>");
          break;
        default:
          if (!s_putc (port,c)) printf ("<<Sorry!>>");
          break;
      }   /*switch*/
    }   /*if keyboard hit*/
    while (s_rxready (COM1)) {
      if (rport!=COM1) {
        printf ("\n<<Receiving over COM1>>\n");
        rport=COM1;
      }   /*if new port*/
      printf ("c=%d h=%d t=%d %c\n",
        s_ports[port].rxcount,
        s_ports[port].rxhead,
        s_ports[port].rxtail,
        s_getc (COM1)
      );
    }   /*while characters ready on COM1*/
    while (s_rxready (COM2)) {
      if (rport!=COM2) {
        printf ("\n<<Receiving over COM2>>\n");
        rport=COM2;
      }   /*if new port*/
      printf ("%c",s_getc (COM2));
    }   /*while characters ready on COM2*/
  }   /*while*/
  s_unconfig (COM1,NO_RESET);
  s_unconfig (COM2,NO_RESET);
}   /*main*/


setup_ports ()
{
  com1_on=com2_on=1;
  if (s_config (COM1)!=S_OK) {
    printf ("Could not initialize COM1\n");
    com1_on=0;
  }
  else {
    com1_on &= (s_baudrate (COM1,9600)==9600);
    com1_on &= (s_databits (COM1,8)==8);
    com1_on &= (s_parity (COM1,NO_PARITY)==NO_PARITY);
    com1_on &= (s_stopbits (COM1,1)==1);
    if (!com1_on) printf ("Could not configure COM1\n");
  }   /*if com1 initialized*/
  if (s_config (COM2)!=S_OK) {
    printf ("Could not initialize COM2\n");
    com2_on=0;
  }
  else {
    com2_on &= (s_baudrate (COM2,9600)==9600);
    com2_on &= (s_databits (COM2,8)==8);
    com2_on &= (s_parity (COM2,NO_PARITY)==NO_PARITY);
    com2_on &= (s_stopbits (COM2,1)==1);
    if (!com2_on) printf ("Could not configure COM2\n");
  }   /*if com2 initialized*/

  if (!(com1_on||com2_on)) {
    printf ("Halting since neither COM port is available (or settable)\n");
    exit (1);
  }
  if (com1_on) port=COM1; else port=COM2;
}   /*setup_ports*/


do_help ()
{
  printf (
"\n"
"ÉÍÍÍÍÍËÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÑÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÑÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÑÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ»\n"
"ºF-keyº    Function    ³ Shift-Function ³Control-Function³  Alt-Function  º\n"
"ÌÍÍÍÍÍÎÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍØÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍØÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍØÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¹\n"
"º F1  º      HELP      ³  COM1 for Tx   ³ Setup for COM1 ³                º\n"
"ÇÄÄÄÄÄ×ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÅÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÅÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÅÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¶\n"
"º F2  º                ³  COM2 for Tx   ³ Setup for COM2 ³                º\n"
"ÇÄÄÄÄÄ×ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÅÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÅÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÅÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¶\n"
"º F3  º                ³                ³                ³                º\n"
"ÇÄÄÄÄÄ×ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÅÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÅÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÅÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¶\n"
"º F4  º                ³                ³                ³                º\n"
"ÇÄÄÄÄÄ×ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÅÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÅÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÅÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¶\n"
"º F5  º                ³                ³                ³                º\n"
"ÇÄÄÄÄÄ×ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÅÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÅÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÅÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¶\n"
"º F6  º                ³                ³                ³                º\n"
"ÇÄÄÄÄÄ×ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÅÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÅÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÅÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¶\n"
"º F7  º                ³                ³                ³                º\n"
"ÇÄÄÄÄÄ×ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÅÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÅÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÅÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¶\n"
"º F8  º    CR = CR     ³   CR = CR+LF   ³                ³                º\n"
"ÇÄÄÄÄÄ×ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÅÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÅÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÅÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¶\n"
"º F9  º   Send File    ³ Send HEX File  ³                ³                º\n"
"ÇÄÄÄÄÄ×ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÅÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÅÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÅÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¶\n"
"º F10 º      EXIT      ³ Immediate EXIT ³                ³                º\n"
"ÈÍÍÍÍÍÊÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÏÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÏÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÏÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¼\n"
  );
}   /*do_help*/


do_setup (word port)
{
  word baud,parity,databits,stopbits;
  int done=0;
  byte c;

  printf ("\n<<Setup for %s>>\n",cnam[port]);
  while (!done) {
    baud=s_baudrate (port,INQUIRE);
    parity=s_parity (port,INQUIRE);
    databits=s_databits (port,INQUIRE);
    stopbits=s_stopbits (port,INQUIRE);
    printf (
      "Set baud:       F1=19200 F2=9600 F3=2400 F4=1200 F5=300\n"
      "Set parity:     Even Odd None\n"
      "Set data bits:  7 or 8 bits\n"
      "Set stop bits:  1 or 2 bits\n"
    );
    if (parity==NO_PARITY) c='N'; else if (parity==ODD_PARITY) c='O'; else c='E';
    printf ("%s currently set to %d %c%d%d\n",cnam[port],baud,c,databits,stopbits);
    printf ("<<Press key of choice, ESC to end>>\n");
    c=upcase (getkey());
    switch (c) {
      case ESC: done=1; break;
      case _F1: s_baudrate (port,19200); break;
      case _F2: s_baudrate (port,9600); break;
      case _F3: s_baudrate (port,2400); break;
      case _F4: s_baudrate (port,1200); break;
      case _F5: s_baudrate (port,300); break;
      case 'E': s_parity (port,EVEN_PARITY); break;
      case 'O': s_parity (port,ODD_PARITY); break;
      case 'N': s_parity (port,NO_PARITY); break;
      case '7': s_databits (port,7); break;
      case '8': s_databits (port,8); break;
      case '1': s_stopbits (port,1); break;
      case '2': s_stopbits (port,2); break;
      default: break;
    }   /*switch*/
  }   /*while*/
  printf ("<<Finished setup for %s>>\n",cnam[port]);
}   /*setup*/


send_file (word port)
{
  char s[256];
  FILE *f;
  word l;
  char *t;
  int ok;

  ok=1;
  printf ("\n<<Sendfile>>\n");
  printf ("Enter filename: ");
  s[0]=0;
  gets (s);
  if (s[0]!=0) {
    if ((f=fopen (s,"rt"))==NULL)
      printf ("Could not open \"%s\"\n",s);
    else {
      l=0;
      while (ok&&(fgets (s,255,f)!=NULL)) {
        l++;
        ok&=s_puts (port,s);
      }   /*while not EOF*/
      if (!ok) printf ("<<Timeout on transmit>>\n");
      printf ("<<wrote %d lines, hit a key>>\n",l);
      while (getkey()=='');
    }   /*else file opened ok*/
  }   /*if filename was entered*/
  printf ("<<Leaving Sendfile>>\n");
}   /*send_file*/


sendx_file (word port)
{
  port++;
}   /*sendx_file*/