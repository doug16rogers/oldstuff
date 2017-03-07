#include <stdio.h>
#include <process.h>
#include <string.h>
#include <dos.h>
#include <conio.h>


typedef unsigned char byte;
typedef struct { unsigned char c; unsigned char a; } attr_char;


void interrupt (*oldfunc)();
unsigned int vidseg=0xB800;
static byte _dh,_dl,_ch,_cl;
static union REGS r;
static unsigned char old_dh=0xff;


#define ATTR ((BLUE<<4)+YELLOW)

attr_char s[9]={
  {'0',ATTR},
  {'0',ATTR},
  {':',ATTR},
  {'0',ATTR},
  {'0',ATTR},
  {':',ATTR},
  {'0',ATTR},
  {'0',ATTR},
  {0}
};   //s



void interrupt DisplayClock(void) {
#if 0
  r.h.ah=0x02;
  int86(0x1A,&r,&r);
  if (r.h.dh!=old_dh) {
    old_dh=r.h.dh;
    s[0].c='0'+((r.h.ch>>4)&3);
    s[1].c='0'+(r.h.ch&15);
    s[3].c='0'+(r.h.cl>>4);
    s[4].c='0'+(r.h.cl&15);
    s[6].c='0'+(r.h.dh>>4);
    s[7].c='0'+(r.h.dh&15);
#else
  _AH=0x02;
  geninterrupt(0x1A);
  _dh=_DH; _dl=_DL; _ch=_CH; _cl=_CL;
  if (_dh!=old_dh) {
    old_dh=_dh;
    s[0].c='0'+((_ch>>4)&3);
    s[1].c='0'+(_ch&15);
    s[3].c='0'+(_cl>>4);
    s[4].c='0'+(_cl&15);
    s[6].c='0'+(_dh>>4);
    s[7].c='0'+(_dh&15);
#endif
#if 0              //direct screen access -- doesn't work in graphics
    pokeb(vidseg,72*2+0,(unsigned)s[0].c);
    pokeb(vidseg,72*2+1,(unsigned)s[0].a);
    pokeb(vidseg,73*2+0,(unsigned)s[1].c);
    pokeb(vidseg,73*2+1,(unsigned)s[1].a);
    pokeb(vidseg,74*2+0,(unsigned)s[2].c);
    pokeb(vidseg,74*2+1,(unsigned)s[2].a);
    pokeb(vidseg,75*2+0,(unsigned)s[3].c);
    pokeb(vidseg,75*2+1,(unsigned)s[3].a);
    pokeb(vidseg,76*2+0,(unsigned)s[4].c);
    pokeb(vidseg,76*2+1,(unsigned)s[4].a);
    pokeb(vidseg,77*2+0,(unsigned)s[5].c);
    pokeb(vidseg,77*2+1,(unsigned)s[5].a);
    pokeb(vidseg,78*2+0,(unsigned)s[6].c);
    pokeb(vidseg,78*2+1,(unsigned)s[6].a);
    pokeb(vidseg,79*2+0,(unsigned)s[7].c);
    pokeb(vidseg,79*2+1,(unsigned)s[7].a);
#else              //video services interrupt via ROM BIOS
    asm   push bp
    _ES=FP_SEG(s);
    _BP=FP_OFF(s);
    asm   mov  ax,0x01302
    asm   mov  bx,0
    asm   mov  cx,8
    asm   mov  dx,72
    asm   int  0x10
    asm   pop  bp
#endif
  }   //if changed
}   //DisplayClock


void main(void) {
  union REGS r;
  r.h.ah=0xf;
  int86(0x10,&r,&r);
  if (r.h.al==0x07) vidseg=0xB000;
  oldfunc=getvect(0x28);
  setvect(0x28,DisplayClock);
  keep(0,_SS+20-_psp);
}   //main


