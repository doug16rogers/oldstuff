#include <stdio.h>
#include <process.h>
#include <string.h>
#include <dos.h>
#include <conio.h>


//===========================================================================
//  Macro definitions...
//---------------------------------------------------------------------------
#ifndef byte
#define byte unsigned char
#endif

#ifndef word
#define word unsigned int
#endif

#define PROGRAM_NAME   "drclk"

#define VIDEO_INT      0x10
#define CLOCK_INT      0x1A
#define IDLE_INT       0x28
#define FIRST_USER_INT 0x60
#define LAST_USER_INT  0x66

#define CMOS_PORT      0x70
#define CMOS_DATA      0x71

#define SECOND         0x00
#define MINUTE         0x02
#define HOUR           0x04
#define WEEK_DAY       0x06
#define DAY            0x07
#define MONTH          0x08
#define YEAR           0x09

#define READ_CMOS(value,reg) \
  outportb(CMOS_PORT, reg);  \
  asm nop;                   \
  value = inportb(CMOS_DATA)

#define CLOCK_ATTR  (        YELLOW + (BLUE << 4))
#define ALARM_ATTR  (BLINK + YELLOW + (RED << 4))

enum
{
  ALARM_OFF = 0,
  ALARM_ON = 1,
  ALARM_TRIGGERED = 2,
} ALARM_STATES;

enum
{
  NO_COMMAND = 0x00,
  SET_CLOCK_ATTRIBUTE = 0x01,
  SET_ALARM_ATTRIBUTE,
  SET_ALARM_TIME,
  SET_ALARM_DATE,
  SET_ALARM_STATE,
  SET_DISPLAY,
  SET_CLOCK_POSITION,
  UNLOAD,
} USER_COMMANDS;



//===========================================================================
//  typedef definitions...
//---------------------------------------------------------------------------
typedef struct
{
  char c;
  unsigned char a;
} attr_char;

typedef void interrupt (*intr_function)();

typedef struct
{
  char name[12];
  word intr_num;
} Program_ID;

typedef struct
{
  word year;
  byte month;
  byte day;

  byte hour;
  byte minute;
  byte second;
  byte flag;
} Time;



typedef struct
{
  byte command;
  byte value;
} Set_Value;

typedef struct
{
  word ax;
  word bx;
  word cx;
  word dx;
} Register_Set;

typedef struct
{
  byte command;
  byte unused;
  byte hour;
  byte minute;
  byte second;
} Set_Alarm_Time;

typedef struct
{
  byte command;
  byte unused;
  word year;
  byte month;
  byte day;
} Set_Alarm_Date;

typedef struct
{
  byte command;
  byte unused;
  byte row;
  byte col;
} Set_Clock_Position;

typedef union
{
  Set_Value       cmd;
  Register_Set    regs;
  Set_Alarm_Time  a_time;
  Set_Alarm_Date  a_date;
  Set_Clock_Position pos;
} Command;


//===========================================================================
//  global variables...
//---------------------------------------------------------------------------
Program_ID program_id = { PROGRAM_NAME, 0 };

byte unload_clock = 0;
byte clock_loaded = 0;

void interrupt (*old_idle_func)() = NULL;

word name_intr = 0;
word user_intr = 0;

word vidseg = 0xB800;
word old_bp;

byte display_clock = 1;

byte clock_row = 0;
byte clock_col = 70;
byte clock_attr = CLOCK_ATTR;
byte alarm_attr = ALARM_ATTR;

Time tim;
byte old_second = 0xff;
Time alarm;
byte alarm_state = ALARM_OFF;
byte verbose = 0;

attr_char s[] =
{
  { ' ', CLOCK_ATTR },
  { '0', CLOCK_ATTR },
  { '0', CLOCK_ATTR },
  { ':', CLOCK_ATTR },
  { '0', CLOCK_ATTR },
  { '0', CLOCK_ATTR },
  { ':', CLOCK_ATTR },
  { '0', CLOCK_ATTR },
  { '0', CLOCK_ATTR },
  { ' ', CLOCK_ATTR },
  {0}
};   //s

Command command_list[20] = { 0 };
Command* command_ptr = command_list;



word ch_i;
byte ch_attr;
byte ch_display;
byte alarm_triggered;

//===========================================================================
//  ClockHandler ISR
//---------------------------------------------------------------------------
void interrupt ClockHandler(void)
{
  READ_CMOS(tim.second, SECOND);
  if (tim.second != old_second)
  {
    old_second = tim.second;
    READ_CMOS(tim.hour, HOUR);
    READ_CMOS(tim.minute, MINUTE);
    if (tim.hour >= 0x20)
    {
      tim.hour &= 0x0f;
      tim.hour += 0x12 + 0x06;
    }

    ch_display = display_clock;
    switch (alarm_state)
    {
    case ALARM_OFF:
      ch_attr = clock_attr;
      s[0].c = '\xFA';
      s[9].c = '\xFA';
      break;
    case ALARM_ON:
      alarm_triggered = 0;
      if (tim.hour > alarm.hour)
        alarm_triggered = 1;
      else if (tim.hour == alarm.hour)
      {
        if (tim.minute > alarm.minute)
          alarm_triggered = 1;
        else if (tim.minute == alarm.minute)
          alarm_triggered = (tim.second >= alarm.second);
      }
      if (!alarm_triggered)
      {
        ch_attr = clock_attr;
        s[0].c = '\x11';
        s[9].c = '\x10';
        break;
      }
      else
      {
        alarm_state = ALARM_TRIGGERED;    //fall through to triggered state
      }
    case ALARM_TRIGGERED:
      ch_display = 1;
      ch_attr = alarm_attr;
      s[0].c = '\x11';
      s[9].c = '\x10';
      break;
    default:
      alarm_state = ALARM_OFF;
      ch_attr = clock_attr;
      s[0].c = '\xFA';
      s[9].c = '\xFA';
      break;
    }   //switch

    if (ch_display)
    {
      for (ch_i = 0; (s[ch_i].c != 0); ch_i++) s[ch_i].a = ch_attr;
      s[1].c = '0' + ((tim.hour >> 4) & 3);
      s[2].c = '0' + (tim.hour & 15);
      s[4].c = '0' + (tim.minute >> 4);
      s[5].c = '0' + (tim.minute & 15);
      s[7].c = '0' + (tim.second >> 4);
      s[8].c = '0' + (tim.second & 15);
      old_bp = _BP;
      _ES = FP_SEG(s);
      _BP = FP_OFF(s);
      _AX = 0x01302;
      _BX = 0;
      _CX = 10;
      _DH = clock_row;
      _DL = clock_col;
      geninterrupt(VIDEO_INT);
      _BP = old_bp;
    }   ///if displaying clock
  }   //if changed

  if (old_idle_func != NULL) old_idle_func();
}   //DisplayClock



Command user;

//===========================================================================
//  UserInterface ISR
//---------------------------------------------------------------------------
void interrupt UserInterface(void)
{
#if 0
  user.regs.ax = _AX;
  user.regs.bx = _BX;
  user.regs.cx = _CX;
  user.regs.dx = _DX;
#else
  asm push di
  asm push si

  asm mov si,SEG(user)
  asm mov es,si
  asm mov si,OFFSET(user)
  asm mov es:[si+0],ax
  asm mov es:[si+2],bx
  asm mov es:[si+4],cx
  asm mov es:[si+6],dx

  asm pop si
  asm pop di
#endif

  switch (user.cmd.command)
  {
  case SET_DISPLAY:
    display_clock = user.cmd.value;
    break;
  case SET_CLOCK_ATTRIBUTE:
    clock_attr = user.cmd.value;
    break;
  case SET_ALARM_ATTRIBUTE:
    alarm_attr = user.cmd.value;
    break;
  case SET_ALARM_DATE:
    alarm.year  = user.a_date.year;
    alarm.month = user.a_date.month;
    alarm.day   = user.a_date.day;
    break;
  case SET_ALARM_TIME:
    alarm.hour   = user.a_time.hour;
    alarm.minute = user.a_time.minute;
    alarm.second = user.a_time.second;
    break;
  case SET_ALARM_STATE:
    switch (user.cmd.value)
    {
    case ALARM_ON:
      alarm_state = user.cmd.value;
      break;
    case ALARM_OFF:
      alarm_state = user.cmd.value;
      break;
    case ALARM_TRIGGERED:
      alarm_state = user.cmd.value;
      break;
    default:
      alarm_state = ALARM_OFF;
    }   //switch
    break;
  case SET_CLOCK_POSITION:
    clock_row = user.pos.row;
    clock_col = user.pos.col;
    break;
  case UNLOAD:
    disable();
    setvect(IDLE_INT, old_idle_func);
    setvect(user_intr, NULL);
    setvect(name_intr, NULL);
    enable();
    printf("%s unloaded.\n", PROGRAM_NAME);
    exit(0);
    break;
  }   //switch
}   //UserInterface


//===========================================================================
//  SendUserCommand
//---------------------------------------------------------------------------
void SendUserCommand(          //no output
  word user_interrupt,         //user interrupt number
  Command* to_user)            //command to send
{
#if 0
  _AX = to_user->regs.ax;
  _CX = to_user->regs.cx;
  _DX = to_user->regs.dx;
  _BX = to_user->regs.bx;
  geninterrupt(user_interrupt);
#else
  asm push di
  asm push si

  asm les si,to_user
  asm mov ax,es:[si+0]
  asm mov bx,es:[si+2]
  asm mov cx,es:[si+4]
  asm mov dx,es:[si+6]
  asm xor di,di
  asm mov es,di
  asm mov di,user_interrupt
  asm shl di,1
  asm shl di,1
  asm pushf
  asm cli
  asm call dword ptr es:[di]

  asm pop si
  asm pop di
#endif
}   //SendUserCommand


//===========================================================================
//  UnloadInterrupts
//---------------------------------------------------------------------------
int  UnloadInterrupts(void)    //1 on success, 0 otherwise
{
  word intr_num;
  Program_ID far* prog_id;
  int i;

  for (  intr_num = FIRST_USER_INT;
        (intr_num <= LAST_USER_INT) && (name_intr == 0);
         intr_num++)
  {
    prog_id = (Program_ID far*) getvect(intr_num);
    if (prog_id != NULL)
    {
      for (i = 0; (program_id.name[i] != 0); i++)
      {
        if (program_id.name[i] != prog_id->name[i]) break;
      }   //for
      if (program_id.name[i] == 0) name_intr = intr_num;   //found it!
    }
  }   //for each user interrupt

  if (name_intr == 0)
  {
    printf("%s is not loaded.\n", PROGRAM_NAME);
    return 0;
  }

  user_intr = prog_id->intr_num;

  _AH = UNLOAD;
  geninterrupt(user_intr);

  return 1;
}   //UnloadInterrupts


//===========================================================================
//  LoadInterrupts
//---------------------------------------------------------------------------
int  LoadInterrupts(void)      //1 on success, 0 otherwise
{
  word intr_num;
  Program_ID far* prog_id;
  int i;

  for (  intr_num = FIRST_USER_INT;
        (intr_num <= LAST_USER_INT) && (name_intr == 0);
         intr_num++)
  {
    prog_id = (Program_ID far*) getvect(intr_num);
    if (prog_id != NULL)
    {
      for (i = 0; (program_id.name[i] != 0); i++)
      {
        if (program_id.name[i] != prog_id->name[i]) break;
      }   //for
      if (program_id.name[i] == 0) name_intr = intr_num;   //found it!
    }
  }   //for each user interrupt

  if (name_intr != 0)
  {
    user_intr = prog_id->intr_num;
    return 1;
  }

    //------ not found, so look for available user interrupts... ------//

  for (intr_num = FIRST_USER_INT; intr_num <= LAST_USER_INT; intr_num++)
  {
    if (getvect(intr_num) == NULL)
    {
      if (name_intr == 0)
        name_intr = intr_num;
      else
      {
        user_intr = intr_num;
        break;
      }
    }
  }   //for each user interrupt

  if ((name_intr == 0) || (user_intr == 0))
  {
    printf("Not enough user interrupts to load TSR's.\n");
    return 0;
  }

  old_idle_func = getvect(IDLE_INT);
  setvect(IDLE_INT,  ClockHandler);
  setvect(name_intr, (intr_function) &program_id);
  setvect(user_intr, UserInterface);
  program_id.intr_num = user_intr;
  clock_loaded = 1;
  return 1;
}   //LoadInterrupts


//===========================================================================
//  LoadFlag
//---------------------------------------------------------------------------
int LoadFlag(          //1 if legal flag char, 0 otherwise
  char* name,          //name of flag being loaded
  char* s,             //string to be parsed
  char* flag)          //flag to be changed (set/reset)
{
  switch (*s)
  {
  case 0:
  case '+':
  case '1':
  case 't':
  case 'T':
    *flag = 1;
    break;
  case '-':
  case '0':
  case 'f':
  case 'F':
    *flag = 0;
    break;
  default:
    printf("illegal flag value \"%s\" for %s.\n", s, name);
    return 0;
  }
  return 1;
}   //LoadFlag


//===========================================================================
//  LoadArg
//---------------------------------------------------------------------------
int LoadArg(           //1 if argument was loaded ok
  char* s)             //argument to load
{
  word hour, minute, second = 0;

  if ((*s == '/') || (*s == '-')) s++;   //skip switch indicator

  switch (*s++)
  {
  case '?':
    return 0;
  case 'A':
    if (sscanf(s, "%x", &command_ptr->cmd.value) != 1)
    {
      printf("invalid attribute value \"%s\".\n", s);
      return 0;
    }
    command_ptr->cmd.command = SET_ALARM_ATTRIBUTE;
    command_ptr++;
    break;
  case 'C':
    if (sscanf(s, "%x", &command_ptr->cmd.value) != 1)
    {
      printf("invalid attribute value \"%s\".\n", s);
      return 0;
    }
    command_ptr->cmd.command = SET_CLOCK_ATTRIBUTE;
    command_ptr++;
    break;
  case 'P':
    if (sscanf(s, "%u:%u", &hour, &minute) != 2)
    {
      printf("invalid display position \"%s\".\n", s);
      return 0;
    }
    command_ptr->pos.command = SET_CLOCK_POSITION;
    command_ptr->pos.row = hour;
    command_ptr->pos.col = minute;
    command_ptr++;
    break;
  case 't':
    if (sscanf(s, "%x:%x:%x", &hour, &minute, &second) < 2)  //BCD, so hex
    {
      printf("invalid alarm time \"%s\".\n", s);
      return 0;
    }
    command_ptr->a_time.command = SET_ALARM_TIME;
    command_ptr->a_time.hour = hour;
    command_ptr->a_time.minute = minute;
    command_ptr->a_time.second = second;
    command_ptr++;
    command_ptr->cmd.command = SET_ALARM_STATE;
    command_ptr->cmd.value = ALARM_ON;
    command_ptr++;
    break;
  case 's':
    if (!LoadFlag("displaying clock", s, &command_ptr->cmd.value)) return 0;
    command_ptr->cmd.command = SET_DISPLAY;
    command_ptr++;
    break;
  case 'r':
    command_ptr->cmd.command = SET_ALARM_STATE;
    command_ptr->cmd.value = ALARM_OFF;
    command_ptr++;
    break;
#if 1
  case 'T':
    command_ptr->cmd.command = SET_ALARM_STATE;
    command_ptr->cmd.value = ALARM_TRIGGERED;
    command_ptr++;
    break;
#endif
  case 'u':
    unload_clock = 1;
    break;
  case 'v':
    if (!LoadFlag("verbose listing", s, &verbose)) return 0;
    break;
  default:
    s -= 2;
    printf("unknown commandline switch \"%s\".\n", s);
    return 0;
  }   //switch
  return 1;
}   //LoadArg


//===========================================================================
//  Help
//---------------------------------------------------------------------------
int  Help(             //whatever is passed in (typically DOS return code)
  int parameter)       //parameter to be returned after displaying help
{
  printf(
    "\n"
    "drclock [switches]\n"
    "Switches:\n"
    "  -A<attribute>  attribute for when alarm goes off\n"
    "  -C<attribute>  attribute for clock display\n"
    "  -P<row>:<col>  row:column for clock display (starts at 0,0)\n"
    "  -tHH:MM[:SS]   set alarm to time\n"
    "  -s[-/+]        show clock or not\n"
    "  -r             reset alarm once it goes off\n"
    "  -u             unload clock program\n"
    "  -v[-/+]        verbose listing\n"
  );   //printf

  return parameter;
}   //Help


//===========================================================================
//  main
//---------------------------------------------------------------------------
int  main(             //DOS program return code
  int argc,            //number of commandline arguments
  char* argv[])        //array of pointers to commandline arguments
{
  int i;

  printf("drclock -- loads an alarm-clock TSR\n");

//  _AH = 0xf;
//  geninterrupt(VIDEO_INT);
//  if (_AL == 0x07) vidseg = 0xB000;

  for (i = 1; (i < argc); i++) if (!LoadArg(argv[i])) return Help(1);

  if (unload_clock)
  {
    if (!UnloadInterrupts()) return 2;
    printf("%s unloaded.\n", PROGRAM_NAME);
    return 0;
  }

  if (!LoadInterrupts()) return 3;

  command_ptr = command_list;
  while (command_ptr->cmd.command != 0)
  {
    SendUserCommand(user_intr, command_ptr++);
  }   //while

  if (clock_loaded)
  {
    printf("%s installed.\n", PROGRAM_NAME);
    keep(0, _SS + 10 - _psp);
  }

  return 0;
}   //main




//===========================================================================
//
//---------------------------------------------------------------------------
