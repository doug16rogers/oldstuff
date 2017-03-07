#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

#include "cutil.h"

#define COLOR_VIDEO   (char far *) (long unsigned) 0xB8000000
#define MONO_VIDEO    (char far *) (long unsigned) 0xB0000000
#define RMAX   25
#define CMAX   80
#define REPEAT_GENS 0x00A0

unsigned int state[RMAX][CMAX];
unsigned int temp[RMAX][CMAX];
unsigned int rmax,cmax,smax,roff,coff;
int dist;
long unsigned int gens;
int paused,finished,show_gens;
char far *vidptr;


main (int argc,char *argv[])
{
  init_vars (argc,argv);
  show_array ();
  while (!finished) {
    if (kbhit()) process_char ();
    if (!paused) {
      if (gens>REPEAT_GENS) random_gen (); else generation ();
      show_array ();
    }   /*if not paused*/
  }   /*while main loop*/
  do_help ();
}   /*main*/


do_help ()
{
  gotoxy (1,25);
  printf (
    "Commandline options:\n"
    "  -m  monocrhome video      -c  color video (default)\n"
    "  -p  start paused          -g  show generations\n"
    "Keystroke commands (first letter of each):\n"
    "  Pause  Monochrome  Color  Generations  Single-step  Randomize\n"
    "  ESC-quits\n"
  );
}   /*do_help*/


set_flags (char *s)
{
  for (;*s!=0;s++) {
    switch (upcase (*s)&0x00ff) {
      case '-':
        break;
      case 'M':
        vidptr=MONO_VIDEO;
        break;
      case 'C':
        vidptr=COLOR_VIDEO;
        break;
      case 'P':
        paused=1;
        break;
      case 'G':
        show_gens=1;
        break;
      default:
         break;
    }   /*switch*/
  }   /*for*/
}   /*set_flags*/


init_vars (int argc,char *argv[])
{
  char ch;
  int i;
  randomize ();
  vidptr=COLOR_VIDEO;
  paused=0;
  finished=0;
  gens=0;
  show_gens=0;
  rmax=RMAX;
  cmax=CMAX;
  smax=16;
  dist=1;
  clrscr ();
  textcolor (BLACK);
  textbackground (LIGHTGRAY);
  random_gen ();

  for (i=1;i<argc;i++) {
    switch (argv[i][0]) {
      case '-':
        set_flags (argv[i]);
        break;
      default:
        break;
    }   /*switch*/
  }   /*for argument list*/

  roff=(RMAX-rmax)/2;
  coff=(CMAX-cmax)/2;
}   /*init_vars*/


process_char ()
{
  unsigned char ch;
  ch=upcase (getkey ());
  switch (ch) {
    case 'P':
      paused=!paused;
      break;
    case 'R':
      random_gen ();
      break;
    case 'S':
      if (paused) {
        generation ();
        show_array ();
      }
      paused=1;
      break;
    case 'G':
      show_gens=!show_gens;
      break;
    case 'M':
      vidptr=MONO_VIDEO;
      break;
    case 'C':
      vidptr=COLOR_VIDEO;
      break;
    case _UPAR:
      dist=(dist+1)%rmax;
      break;
    case _DNAR:
      dist=(dist+rmax-1)%rmax;
      break;
    case ESC:
      finished=1;
      break;
    default:
      break;
  }   /*switch*/
}   /*process_char*/


generation ()
{
  int r,c;
  unsigned int s;
  int cm1,rm1,rp1,cp1;
  unsigned int sm1;

  sm1=((smax-1)<<8)|219;
  for (r=0;r<rmax;r++) {
    for (c=0;c<cmax;c++) {
      s=state[r][c];
      rp1=r+dist;
      rm1=r-dist;
      cp1=c+dist;
      cm1=c-dist;
      if (rm1<0) rm1+=rmax;
      if (r>=rmax) rp1-=rmax;
      if (cm1<0) cm1+=cmax;
      if (cp1>=cmax) cp1-=cmax;
      if (s==sm1) {
        if      (state[rm1][c]==219) s=219;
        else if (state[rp1][c]==219) s=219;
        else if (state[r][cm1]==219) s=219;
        else if (state[r][cp1]==219) s=219;
        else if (state[rp1][cp1]==219) s=219;
        else if (state[rp1][cm1]==219) s=219;
        else if (state[rm1][cp1]==219) s=219;
        else if (state[rm1][cm1]==219) s=219;
      }
      else {
        if      ((state[rm1][c]-s)==0x100) s+=0x100;
        else if ((state[rp1][c]-s)==0x100) s+=0x100;
        else if ((state[r][cm1]-s)==0x100) s+=0x100;
        else if ((state[r][cp1]-s)==0x100) s+=0x100;
        else if ((state[rm1][cm1]-s)==0x100) s+=0x100;
        else if ((state[rm1][cp1]-s)==0x100) s+=0x100;
        else if ((state[rp1][cm1]-s)==0x100) s+=0x100;
        else if ((state[rp1][cp1]-s)==0x100) s+=0x100;
      }   /*else this state ain't zero*/
      temp[r][c]=s;
    }   /*for column*/
  }   /*for row*/

  memcpy (&state[0][0],&temp[0][0],sizeof (state));
  gens++;
}   /*generation*/


show_array ()
{
  int r,c;
  char far *q;
/*
  textbackground (BLACK);
  for (r=0;r<rmax;r++) {
    gotoxy (1,r+1);
    for (c=0;c<cmax;c++) {
      textcolor (state[r][c]%smax);
      cprintf ("\xdb");
    }   /*for c*/
  }   /*for r*/
*/
  q=vidptr;
  for (r=0;r<rmax;r++) {
    memcpy (q,&state[r][0],cmax+cmax);
    q+=160;
  }
  if (show_gens) {
    gotoxy (60,25);
    cprintf (" GENS: %08X ",gens);
  }
}   /*show_array*/


random_gen ()
{
  int r,c;
  gens=0;
  for (r=0;r<rmax;r++)
    for (c=0;c<cmax;c++)
      state[r][c]=(random (smax)<<8)|219;
}   /*random_gen*/