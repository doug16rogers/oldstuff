#include <stdio.h>
#include <process.h>
#include <string.h>
#include <dos.h>
#include <conio.h>

//***************************************************************************
//  This program is a TSR that will display the current time in the upper
//  right corner of the screen.  You can specify a time for an alarm on
//  the command line in the format "HH:MM".  The colons will flash if the
//  alarm is set correctly.  The time will flash (and the attributes will
//  change to yellow on red) when the alarm time is reached.  The alarm may
//  be reset by pressing BOTH shift keys and the control key.  Make sure you
//  hold them down until the next second ticks.
//
//  To set alarm:    clocktsr HH:MM            (colons will flash)
//  To reset alarm:  Ctrl+LShift+RShift
//
//***************************************************************************




typedef unsigned char byte;
typedef struct { byte c; byte a; } attr_char;


void interrupt (*oldfunc)();
static union REGS r;
static byte old_dh=0xff;

#define ARRAY_SIZE(a)      (sizeof(a)/sizeof(a[0]))

#define CLOCK_ID           0xC1

#define SHIFT_STATUS_PTR   ((byte far*)0x00400017UL)
#define IPCA               ((byte far*)0x004000F0UL)  //intra-proc comm area
#define CLOCK_ATTR         ((BLUE<<4)+YELLOW)
#define ALARM_ATTR         (((RED<<4)+YELLOW)+BLINK)

enum CLOCK_CMDS {
  ALARM_SET,
  ALARM_OFF,
};


attr_char clock_time[8]={
  {'0',CLOCK_ATTR},
  {'0',CLOCK_ATTR},
  {':',CLOCK_ATTR},
  {'0',CLOCK_ATTR},
  {'0',CLOCK_ATTR},
  {':',CLOCK_ATTR},
  {'0',CLOCK_ATTR},
  {'0',CLOCK_ATTR},
};   //clock_time

attr_char alarm_time[8]={
  {'0',0},
  {'0',0},
  {'0',0},
  {'0',0},
};   //alarm_time


static unsigned int old_BP;
static byte alarm_on=0;
static byte alarm_set=0;


void interrupt DisplayClock(void)
{
  if (IPCA[0]==CLOCK_ID) &&            //must have ID pointer
     ((IPCA[ 1]+IPCA[ 2]+IPCA[ 3]+     //checksum of rest must be CLOCK_ID
       IPCA[ 4]+IPCA[ 5]+IPCA[ 6]+
       IPCA[ 7]+IPCA[ 8]+IPCA[ 9]+
       IPCA[10]+IPCA[11]+IPCA[12]+
       IPCA[13]+IPCA[14]+IPCA[15])==CLOCK_ID) )
  {
    switch (IPCA[1]) {
    case ALARM_SET:
      alarm_time[0].c=IPCA[2];
      alarm_time[1].c=IPCA[3];
      alarm_time[2].c=IPCA[4];
      alarm_time[3].c=IPCA[5];
      alarm_on=1;
      alarm_reset=0;
      break;
    case ALARM_OFF:
      alarm_on=0;
      break;
    default:
      ;
    }   //switch on command
    IPCA[0]=~CLOCK_ID;
  }   //if IPCA message is in

  r.h.ah=0x02;
  int86(0x1A,&r,&r);
  if (r.h.dh!=old_dh) {
    old_dh=r.h.dh;
    clock_time[0].c='0'+((r.h.ch>>4)&3);
    clock_time[1].c='0'+(r.h.ch&15);
    clock_time[3].c='0'+(r.h.cl>>4);
    clock_time[4].c='0'+(r.h.cl&15);
    clock_time[6].c='0'+(r.h.dh>>4);
    clock_time[7].c='0'+(r.h.dh&15);
    if (alarm_on) {
      s[2].c^=':';
      s[5].c^=':';
      if (alarm_set) {
        if ((*SHIFT_STATUS_PTR & 7)==7) {    //Ctrl+LShift+RShift
          for (alarm_on=0;alarm_on<8;alarm_on++)
            s[alarm_on].a=CLOCK_ATTR;
          clock_time[2].c=':';
          clock_time[5].c=':';
          alarm_on=0;
          alarm_set=0;
        }
      }
      else {
        if ((clock_time[0].c==alarm_time[0].c)  &&
            (clock_time[1].c==alarm_time[1].c)  &&
            (clock_time[3].c==alarm_time[2].c)  &&
            (clock_time[4].c==alarm_time[3].c)) {
          for (alarm_on=0;alarm_on<8;alarm_on++)
            clock_time[alarm_on].a=ALARM_ATTR;
          alarm_on=1;
          alarm_set=1;
        }
      }   //else alarm hasn't been set on yet
    }   //if there's an alarm to check
    old_BP=_BP;
    _ES=FP_SEG(s);
    _BP=FP_OFF(s);
    _AX=0x1302;
    _BX=0;
    _CX=8;
    _DX=72;
    geninterrupt(0x10);
    _BP=old_BP;
  }   //if changed
  oldfunc();
}   //DisplayClock


void loadtime(char* s) {
  int hour=-1,min=-1;
  if (sscanf(s,"%u:%u",&hour,&min)==2) {
    if ((hour<0)||(hour>23)) return;
    if ((min<0)||(min>59)) return;
    alarm_hour_msd='0'+(hour/10);
    alarm_hour_lsd='0'+(hour%10);
    alarm_min_msd ='0'+(min/10);
    alarm_min_lsd ='0'+(min%10);
    alarm_on=1;
  }
}   //loadtime


void main(int argc,char* argv[]) {
  if (argc>1) loadtime(argv[1]);
  oldfunc=getvect(0x28);
  setvect(0x28,DisplayClock);
  keep(0,_SS+1-_psp);
}   //main


