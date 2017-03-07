//***************************************************************************
//  Link8530
//***************************************************************************
//-----------------------------Description-----------------------------------
//  This program integrates various (low-level) link protocols for use in a
//  single package.  Other protocols may be added easily, BUT SEE LINK8530.H
//  FOR CERTAIN RESTRICTIONS WHICH ARE PLACED ON OTHER MODULES!
//  Currently supported protocols are:
//    SDLC STANDARD      standard SDLC link
//    KEY MASTER         asserts key (RTS) to transmit
//    CLOCK MASTER       sends clock on rx/tx
//    DSP ROM ACCESS     gets/loads DSP EEPROM addresses
//-----------------------------Constraints/Gotchas---------------------------
//  See LINK8530.H.
//--Date--------Programmer----------Comments---------------------------------
//  1991.07.24  D. Rogers           initial code+testing
//***************************************************************************


#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <ctype.h>
#include <process.h>

#include "strutil.h"
#include "scrutil.h"
#include "wutil.h"
#include "z30util.h"
#include "cfgutil.h"

#include "z30.h"

#include "sdlc.h"
#include "keymastr.h"
#include "clkmastr.h"
#include "dsprom.h"


#define LINK8530_NAME          "Link8530"
#define LINK8530_HLP           "z30.hlp"
#define LINK8530_CFG           "z30.cfg"
#define LINK8530_LOG           "z30.log"

#define RLOG           "Receive buffer logging (F2)"
#define RXSHOW         "Receive buffer displaying (Ctrl-F1)"
#define TXSHOW         "Transmit buffer displaying (Ctrl-F2)"
#define HIST           "8530 Read/Write tracking (Ctrl-F3)"


#define Z30        1

BYTE rxbuf[RX_SIZE];                   //receive buffer
WORD rxn=0;                            //count of received bytes
BYTE txbuf[TX_SIZE];                   //transmit buffer
WORD txn=0;                            //count of bytes to send

char running=1;                        //program is running
char rx_show=1;                        //display received bytes
char tx_show=1;                        //display transmit data
char rlog=0;                           //logging to file
char rlogname[80]=LINK8530_LOG;        //logfile name
FILE* rlogfile=NULL;                   //file to which we log
char cfgname[80]=LINK8530_CFG;         //config file
char hlpname[80]=LINK8530_HLP;         //help file
char z30=Z30;                          //use z30

#define MESSAGES       4
#define MESSAGE_SIZE   200
char message[MESSAGES][MESSAGE_SIZE]={
  "\\x00\\x07\\x0C\\x00\\x00\\x00\\x00",
  "123456789ABCDEF012345678",
  "abcdefghijklmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ",
  "Test message",
};
int  curmsg=0;
char z30_string[80]="CA R0 R1";        //string for reading/writing chip

WIND *cw,*rw,*tw,*iw,*hw;              //command/receive/transmit/input/help
char scrap[0x80];                      //scrap space for link protocols
char* onoff[]={ "off", "on" };         //flag to ascii
char* minus_plus[]={ "-", "+" };       //minus/plus flag settings

LinkProtocol linkprot[]={
  { SDLC_NAME,                         //SDLC link protocol
    SDLC_cfg,
    SDLC_init,
    SDLC_rx,
    SDLC_tx,
    SDLC_key,
    SDLC_stat,                         //no status information
  },
  { KEY_MASTER_NAME,                   //asserts key to transmit
    key_master_cfg,
    key_master_init,
    key_master_rx,
    key_master_tx,
    key_master_key,
    key_master_stat,
  },
  { CLK_MASTER_NAME,                   //accepts key to receive, clocks data
    clk_master_cfg,
    clk_master_init,
    clk_master_rx,
    clk_master_tx,
    clk_master_key,
    clk_master_stat,
  },
  { DSPROM_NAME,                       //does load/get to/from DSP EEPROMs
    rom_cfg,
    rom_init,
    NULL,
    NULL,
    rom_key,
    rom_stat,
  },
};   //linkprot[]
#define LINKPROTS  (sizeof(linkprot)/sizeof(linkprot[0]))

LinkProtocol* currprot=linkprot;       //current link protocol is first one


int  cfg_link(int item,void* val,char* str,int loadsave)
{
  if (val != NULL) return 0;
  if (loadsave==CFG_LOAD) {
    for (item=0;item<LINKPROTS;item++) {
      if (*str=='\"') str++;
      if (stricmpshort(str,linkprot[item].name)==0) break;
    }   //for
    if (item<LINKPROTS) currprot=&linkprot[item];
  }
  else {
    sprintf(str,"\"%s\"",currprot->name);
  }   //else saving
  return 1;
}   //cfg_link


CFG_TYPE link8530_cfg[]={
  { "logfile",   CFG_STRING,  &rlogname,   NULL },
  { "cfgfile",   CFG_STRING,  &cfgname,    NULL },
  { "hlpfile",   CFG_STRING,  &hlpname,    NULL },
  { "rx_show",   CFG_BYTE,    &rx_show,    NULL },
  { "tx_show",   CFG_BYTE,    &tx_show,    NULL },
  { "message0",  CFG_QSTRING, message[0],  NULL },
  { "message1",  CFG_QSTRING, message[1],  NULL },
  { "message2",  CFG_QSTRING, message[2],  NULL },
  { "message3",  CFG_QSTRING, message[3],  NULL },
  { "message#",  CFG_INT,     &curmsg,     NULL },
  { "z30_string",CFG_QSTRING, z30_string,  NULL },
  { "z30_hist",  CFG_BYTE,    &z30histon,  NULL },
  { "link",      CFG_CUSTOM,  NULL,        cfg_link },
  { NULL }                             //mark end of list
};   //link8530_cfg


int  loadarg(char* s);
int  help(int k);


//===========================================================================
int  main(int argc,char* argv[])
{
  WORD n;

  for (n=1;n<argc;n++) if (!loadarg(argv[n])) help(1);
  if (!link8530_init()) return 2;

  statusline();                        //make sure status is displayed
  while (running) {
    if (KBhit()) handlekey(wgetc(cw));
    if (currprot->rxfunc) n=currprot->rxfunc(); else n=0;
    if (n) {
      wprintf(rw,"Received %04X bytes.\n",rxn);
      if (rx_show) { showbuf(rw,rxbuf,rxn); rxn=0; }
    }
    if (txn && currprot->txfunc) {
      wprintf(tw,"Transmitting %04X bytes.\n",txn);
      n=currprot->txfunc();
      if (tx_show) {
        wprintf(tw,"%04X bytes transmitted.\n",n);
        if (n) showbuf(tw,txbuf,n);
      }
      txn=0;
    }
  }   //while program still running

  link8530_end();
  return 0;
}   //main


//===========================================================================
int  charflag(char* f,char fc)
{
  switch (fc) {
  case  0 :   *f=1; break;
  case '+':   *f=1; break;
  case '-':   *f=0; break;
  case '1':   *f=1; break;
  case '0':   *f=0; break;
  default:
    return 0;
  }   //switch
  return 1;
}   //charflag


//===========================================================================
int  help(int k)
{
  printf(
    "\n"
    "Usage: link8530 [switches]\n"
    "Switches [default]:\n"
    "  -z[+/-]      use z8530 chip [%s]\n"
    ,minus_plus[Z30!=0]
  );
  return k;
}   //help


//===========================================================================
int  loadarg(char* s)
{
  if ((*s!='-')&&(*s!='/')) {
    printf("illegal switch \"%s\".\n",s);
    return 0;
  }   //if not switch

  s++;
  switch (*s++) {
  case '?':
    return 0;
  case 'z':
    if (!charflag(&z30,*s)) printf("illegal -z value \"%s\".\n",s);
    break;
  default:
    s--;
    printf("unknown commandline switch \"%s\".\n",s);
    return 0;
  }   //switch
  return 1;
}   //loadarg


//===========================================================================
int  link8530_init(void)
{
  int rows,cols;
  int rxsiz,txsiz;
  int i;

  if (z30 && !z30check()) {
    printf("Got no response from 8530 card at address %04X.\n",z30base);
    return 0;
  }

  currprot=linkprot;

  rows=getsrows();
  cols=getscols();
  rxsiz=rows/2;                        //make rx bigger than rest
  txsiz=rows-rxsiz-6;                  //subtract rx window and borders
  cw=wopen(0,NULL,              0,0,      1,cols,    ATTR(BLACK,LIGHTGRAY));
  iw=wopen(0,NULL,         rows-1,0,      1,cols,    ATTR(BLACK,LIGHTGRAY));
  hw=wopen(2," Help ",          3,10,rows-6,cols-20, ATTR(WHITE,BROWN));
  rw=wopen(1," Receive ",       2,1,  rxsiz,cols-2,  ATTR(WHITE,CYAN));
  tw=wopen(1," Transmit ",rxsiz+4,1,  txsiz,cols-2,  ATTR(WHITE,BLUE));
  if ((iw==NULL)||(rw==NULL)||(tw==NULL)||(cw==NULL)||(hw==NULL)) {
    wpurge();
    printf("Could not open all windows.\n");
    return 0;
  }   //if a wopen failed*/
  whide(hw);
  wputshot(cw,
    "  &F&1-Help &F&2-Log &F&3-WRxRRx &F&4-Reset"
    " &F&5-Send &F&6-Buffer &F&8-DOS &F&9-Link &F&1&0-e&Xit\r"
    ,ATTR(RED,LIGHTGRAY),'&'
  );   //wputshot*/
  wfront(tw);
  wfront(rw);
  wupdate(NULL);

  cfg_load(cfgname,LINK8530_NAME,link8530_cfg,0);
  for (i=0;i<LINKPROTS;i++) {
    if (linkprot[i].cfg)
      cfg_load(cfgname,linkprot[i].name,linkprot[i].cfg,0);
  }   //for

  if (currprot->initfunc) currprot->initfunc();
  showflag(rw,RLOG,rlog);
  showflag(rw,RXSHOW,rx_show);
  showflag(tw,TXSHOW,tx_show);

  return 1;
}   //link8530_init


//===========================================================================
void link8530_end(void)
{
  int i;

  cfg_save(cfgname,LINK8530_NAME,link8530_cfg,0,CFG_WRITE);
  for (i=0;i<LINKPROTS;i++) {
    if (linkprot[i].cfg)
      cfg_save(cfgname,linkprot[i].name,linkprot[i].cfg,0,CFG_APPEND);
  }   //for
  wpurge();                            //close down the windows
}   //link8530_end


//===========================================================================
int  KBhit(void)
{
  union REGS r;
  r.h.ah=0x0b;                         //keyboard status function*/
  int86(0x21,&r,&r);                   //DOS services interrupt*/
  return (r.h.al != 0);                //0xff if key waiting, 0x00 if not*/
}   //KBhit*/


//===========================================================================
BYTE csum(BYTE* b,WORD n)
{
  BYTE cs=0;
  while (n--) cs+=*b++;
  return cs;
}   //csum


//===========================================================================
WORD moveup(BYTE* b,WORD n,WORD up)
{
  BYTE* t;                             //pointer to new top of array
  t=&b[n-1+up];                        //set top pointer
  b=&b[n-1];                           //set bottom pointer
  while (n--) *t--=*b--;               //start copying
  return up;                           //allow easy count update
}   //moveup


//===========================================================================
void handlekey(int ch)
{
  WORD a;
  int i;

  switch (ch) {
  case _F1:                            //help
    sprintf(scrap,"Help filename [none=%s]: ",LINK8530_HLP);
    if (inputs(scrap,hlpname,sizeof(hlpname))==CR) {
      if (hlpname[0]==0) strcpy(hlpname,LINK8530_HLP);
      if (!strchr(hlpname,'.')) strcat(hlpname,".hlp");
      a=wfile(hw,hlpname);
      whide(hw);
      if (!a) wprintf(rw,"Could not open \"%s\" for read.\n",hlpname);
    }   //if really want help
    break;
  case _F2:
    showflag(rw,RLOG,setrlog(!rlog));  //put receive buffer to file
    break;
  case _F3:                            //write/read to/from register
    writread();                        //prompt for and execute WR/RR string
    break;
  case S_F3:                           //show history
    wprintf(rw,"\nHistory of reads/writes to 8530:");
    for (i=0;i<z30histcnt;i+=2) {
      if ((i&0x0F)==0) wprintf(rw,"\n%03X: ",i/2);
      if ((i&0x0F)==8) wputc(rw,' ');
      wprintf(rw,"%s ",HistoryString(z30history[i],z30history[i+1]));
      if (KBhit()) {
        wupdate(NULL);
        a=wgetc(rw);
        if (a==ESC) break;
        else if (a==C_S) while (wgetc(rw)!=C_Q) ;
        else if (a==' ') while (wgetc(rw)!=' ') ;
      }
    }
    wputc(rw,'\n');
    if (rlog) {
      fprintf(rlogfile,"\nHistory of reads/writes to 8530:");
      for (i=0;i<z30histcnt;i+=2) {
        if ((i&0x0F)==0) fprintf(rlogfile,"\n%03X: ",i/2);
        if ((i&0x0F)==8) fprintf(rlogfile," ");
        fprintf(rlogfile,"%s ",HistoryString(z30history[i],z30history[i+1]));
      }
      fprintf(rlogfile,"\n");
    }
    ResetHistory();
    break;
  case C_F3:                           //turn z30 history on/off
    showflag(rw,HIST,z30histon=!z30histon);
    if (z30histon) ResetHistory();     //reset when turned on
    break;
  case _F4:
    if (currprot->initfunc) currprot->initfunc();  //initialization function
    wputs(rw,"Link reset.\n");
    break;
  case _F5:                            //send string over SDLC
    send_packet();
    break;
  case _F6:                            //show receive buffer
    wprintf(rw,"There were %04X byte(s) in the buffer.\n",rxn);
    if (rxn>0) showbuf(rw,rxbuf,rxn);
    rxn=0;
    break;
  case _F7:
    getfromfile();
    break;
  case _F8:
    if (rlog) showflag(rw,RLOG,setrlog(0));
    wsetDOSscr();
    wprintf(wscr," ");
    a=wgetattr(wscr);
    wsetattr(wscr,ATTR(YELLOW,RED));
    wprintf(wscr," Enter ");
    wsetattr(wscr,BLINK+ATTR(YELLOW,RED));
    wprintf(wscr,"EXIT");
    wsetattr(wscr,ATTR(YELLOW,RED));
    wprintf(wscr," to return from DOS... ");
    wsetattr(wscr,a);
    wprintf(wscr,"\n");
    system("COMMAND");
    wgetDOSscr();
    break;
  case _F9:
    changelink();
    break;
  case S_F9:
    sprintf(scrap,"Config file to load [none=%s]: ",LINK8530_CFG);
    if (inputs(scrap,cfgname,sizeof(cfgname))==CR) {
      if (cfgname[0]==0) strcpy(cfgname,LINK8530_CFG);
      cfg_load(cfgname,LINK8530_NAME,link8530_cfg,0);
      for (i=0;i<LINKPROTS;i++) {
        if (linkprot[i].cfg)
          cfg_load(cfgname,linkprot[i].name,linkprot[i].cfg,0);
      }   //for
    }
    break;
  case C_F9:
    sprintf(scrap,"Config file to save [none=%s]: ",LINK8530_CFG);
    if (inputs(scrap,cfgname,sizeof(cfgname))==CR) {
      if (cfgname[0]==0) strcpy(cfgname,LINK8530_CFG);
      cfg_save(cfgname,LINK8530_NAME,link8530_cfg,0,CFG_WRITE);
      for (i=0;i<LINKPROTS;i++) {
        if (linkprot[i].cfg)
          cfg_save(cfgname,linkprot[i].name,linkprot[i].cfg,0,CFG_APPEND);
      }   //for
    }
    break;
  case _F10:
  case A_X:
    running=0;
    break;
  case S_F1:
    wclr(rw);
    break;
  case S_F2:
    wclr(tw);
    break;
  case C_F1:
    showflag(rw,RXSHOW,rx_show=!rx_show);
    break;
  case C_F2:
    showflag(tw,TXSHOW,tx_show=!tx_show);
    break;
  default:
    if (currprot->keyfunc) currprot->keyfunc(ch);
  }   //switch

  statusline();                        //make sure status is always right
}   //handlekey


//===========================================================================
void statusline(void)
{
  static char stat[0x80];
  wupd=0;
  wclr(iw);
  wprintf(iw,"%s  ",currprot->name);
  if (currprot->statfunc) {
    currprot->statfunc(stat);
    wputshot(iw,stat,ATTR(RED,LIGHTGRAY),'&');
  }
  wupdate(iw);
}   //statusline


//===========================================================================
int  inputc(char *p,char *clist)
{
  int ch;
  WORD a;

  a=wgetattr(iw);
  wsetattr(iw,ATTR(WHITE,MAGENTA));
  wclr(iw);
  wputs(iw,p);
  do {
    ch=wgetc(iw);
  } while (!strchr(clist,ch));
  wsetattr(iw,a);
  statusline();
  return ch;
}   //inputc


//===========================================================================
int  inputs(char *p,char *b,int n)
{
  int ch;
  BYTE twupd;

  twupd=wupd;
  wupd=0;
  wclr(iw);
  wputs(iw,p);
  ch=wgetsfield(iw,b,n,0,ATTR(WHITE,MAGENTA));
  wupd=twupd;
  if (wupd) wupdate(iw);
  statusline();
  return ch;
}   //inputs


//===========================================================================
void changelink(void)
{
  static link=0;
  int ch;
  char getting=1;

  wupd=0;
  while (getting) {
    strcpy(scrap,linkprot[link].name);
    ch=inputs("Use up/down arrows to select link: ",scrap,sizeof(scrap));
    switch (ch) {
    case CR:    getting=0; break;
    case _UPAR: if (link) link--; else link=LINKPROTS-1; break;
    case _DNAR: link++; if (link>=LINKPROTS) link=0; break;
    }   //switch
  }   //while
  wupdate(iw);
  currprot=&linkprot[link];
  if (currprot->initfunc) currprot->initfunc();
  statusline();
}   //changelink


//===========================================================================
char* hexstring(char* d,BYTE* s,int n)    //dest must have 71 chars available
{
#define BLANK \
  "                                                    |                |"
#define DOTOFF   53      //offset of first dot
  static WORD offs[]={0,3,6,9, 13,16,19,22, 26,29,32,35, 39,42,45,48};
  static BYTE htab[]="0123456789ABCDEF";
  static char dest[80];
  BYTE c;
  int i;

  if (d==NULL) d=dest;                 //use my own space if told to
  strcpy(d,BLANK);
  if ((n<0)||(n>16)) n=16;
  for (i=0;i<n;i++) {
    c=s[i];
    d[offs[i]]=htab[c>>4];
    d[offs[i]+1]=htab[c&0x0F];
    if (isprint(c)) d[DOTOFF+i]=c; else d[DOTOFF+i]='.';
  }   //for
  return d;
}   //hexstring


//===========================================================================
void showflag(WIND *w,char *nam,char f)
{
  wprintf(w,"%s is %s.\n",nam,onoff[f!=0]);
}   //showflag


//===========================================================================
void showbuf(WIND* w,BYTE* bb,int n)
{
  int i;
  static char s[80];

  wprintf(w,"%04X bytes in buffer.\n",n);
  if (rlog && rlogfile) fprintf(rlogfile,"%04X bytes in buffer.\n",n);
  for (i=0;i<n;i+=16) {
    hexstring(s,&bb[i],n-i);
    wprintf(w,"%04X: %s\n",i,s);
    if (rlog && rlogfile) fprintf(rlogfile,"%04X: %s\n",i,s);
  }   //for
}   //showbuf


//===========================================================================
int  setrlog(char rl)                  //turns on/off receiver log
{
  char s[sizeof(rlogname)];            //local var
  if (rl && rlog) return rlog=1;       //already set
  if (!rl && !rlog) rlog=0;            //already reset
  if (rl) {
    strcpy(s,rlogname);                //copy name
    if (inputs("Enter logfile name: ",s,sizeof(s))!=CR) {
      wputs(rw,"Logging aborted.\n");
      return rlog=0;                   //don't log
    }   //if CR not pressed
    strcpy(rlogname,s);                //restore name
    rlogfile=fopen(rlogname,"at");     //open for append
    if (!rlogfile) {                   //oops! couldn't open..
      wprintf(rw,"Couldn't open \"%s\" for append.\n",rlogname);
      return rlog=0;
    }   //if no open
    return rlog=1;
  }   //if want to turn on
//..turning off..
  fclose(rlogfile);                    //close log file
  rlogfile=NULL;                       //reset pointer
  return rlog=0;                       //reset flag
}   //setrlog


//===========================================================================
void send_packet(void)
{
  BYTE s[MESSAGE_SIZE];
  int n;                               //local
  char getting=1;                      //getting input from user

  wupd=0;                              //reset update to make it smoother
  while (getting) {
    strcpy(s,message[curmsg]);         //retrieve last string used
    n=inputs("Enter Tx message: ",s,sizeof(s));    //get user input
    switch (n) {
    case CR:
      getting=0;
      break;
    case _UPAR:
      if (curmsg) curmsg--; else curmsg=MESSAGES-1;
      break;
    case _DNAR:
      curmsg++;
      if (curmsg>=MESSAGES) curmsg=0;
      break;
    default:
      wupdate(iw);
      wprintf(tw,"Message transmission aborted.\n");
      return;
    }   //switch on character pressed to end string editing
  }
  wupdate(iw);
  strcpy(message[curmsg],s);                     //save new string
  txn=strcstr(txbuf,message[curmsg],sizeof(s));  //convert \x's and stuffcopy
}   //send_packet


//===========================================================================
void writreadstr(char *p)
{
  WORD v;
  WORD r;
  static WORD c=A;
  int  st=0;

  strupr(p);                           //set to upper
  c&=B;                                //make sure legal channel
  while (*p) {
    switch (st) {
    case 00:
      r=0;
      v=0;
      switch (*p) {
      case 'C': st=10; break;
      case 'R': st=20; break;
      case 'W': st=30; break;
      case ' ': case ',': case ';':
      case ':': case '/': case '.': break;
      default:
        if (!isspace(*p)) {
          wprintf(rw,"Expected R or W (%c)\n",*p);
          return;
        }   //if weird char
      }   //switch
      break;
    case 10:
      if (*p=='A') c=A;
      else if (*p=='B') c=B;
      else {
        wprintf(rw,"Expected channel A or B (%c)\n",*p);
        return;
      }   //else bad channel
      wprintf(rw,"Channel %c...\n",*p);
      st=0;
      break;
    case 20:
      if (isdigit(*p)) { r*=10;  r+=HEXVAL(*p); }   //if register number
      else {
        r&=0x0f;
        v=z30read(c,r);
        wprintf(rw,"  RR%d=>%02X\n",r,v);
        p--;
        st=0;
      }   //else end of read
      break;
    case 30:
      if (isdigit(*p)) { r*=10;  r+=HEXVAL(*p); }   //if register number
      else if (*p=='=') st=31;
      else { wprintf(rw,"\'=\' expected (%c)\n",*p); return; }
      break;
    case 31:
      if (isxdigit(*p)) { v*=16; v+=HEXVAL(*p); }   //if value
      else {
        r&=0x0f;  v&=0xff;
        z30write(c,r,v);
        wprintf(rw,"WR%d<=%02X\n",r,v);
        p--;
        st=0;
      }   //else end of write
      break;
    default:
      st=0;
    }   //switch
    p++;                               //go to next char
  }   //while

  switch (st) {                        //switch on final state
  case 00:
  case 10: break;
  case 20:
    r&=0x0f;
    v=z30read(c,r);
    wprintf(rw,"  RR%d=>%02X\n",r,v);
    break;
  case 30:
    wprintf(rw,"\'=\' expected (<EOL>)\n");
    break;
  case 31:
    r&=0x0f;  v&=0xff;
    z30write(c,r,v);
    wprintf(rw,"WR%d<=%02X\n",r,v);
    break;
  default: ;
  }   //switch on final state
}   //writreadstr


//===========================================================================
void writread(void)
{
#define PROMPT "C<AorB>,R<dec>,W<dec>=<hex>: "

  if (inputs(PROMPT,z30_string,sizeof(z30_string))!=CR) return;
  writreadstr(z30_string);
}   //writread


//===========================================================================
void getfromfile(void)
{
  static char fn[80]="data.dat";
  FILE *f;
  WORD v;

  if (inputs("Enter file to send: ",fn,sizeof(fn))!=CR) {
    wprintf(tw,"File transmission aborted.\n");
    return;
  }
  f=fopen(fn,"rt");
  if (!f) {
    wprintf(tw,"Could not open file \"%s\" for read.\n",fn);
    return;
  }

  txn=0;
  while (fgets(scrap,sizeof(scrap),f)) {
    if (sscanf(scrap,"%x",&v)) txbuf[txn++]=v;
  }   //while
  fclose(f);

  if (!txn) {
    wprintf(tw,"no (legal) bytes in file \"%s\".\n",fn);
    return;
  }
  wprintf(tw,"Sending %d bytes from \"%s\"...\n",txn,fn);
}   //getfromfile


