#ifndef __DRPC_H
#define __DRPC_H
//===========================================================================
//  header drpc.h
//  PC-related definitions.
//---------------------------------------------------------------------------


//===========================================================================
//  Macro definitions...
//---------------------------------------------------------------------------
#ifndef byte
#define byte unsigned char
#endif

#ifndef word
#define word unsigned int
#endif

#ifndef lword
#define lword unsigned long int
#endif

#define ZERO_INT               0x00
#define SINGLE_STEP_INT        0x01
#define NON_MASKABLE_INT       0x02
#define NMI                    0x02
#define BREAKPOINT_INT         0x03
#define OVERFLOW_INT           0x04
#define PRTSCR_INT             0x05
#define TIMER_INT              0x08
#define KEYBOARD_HW_INT        0x09
#define COM2_INT               0x0B
#define COM1_INT               0x0C
#define HARD_DISK_INT          0x0D
#define FLOPPY_DISK_INT        0x0E
#define PRINT_MANAGER_INT      0x0F
#define VIDEO_INT              0x10
#define EQUIPMENT_INT          0x11
#define MEMORY_SIZE_INT        0x12
#define DISKETTE_INT           0x13
#define COM_PORT_INT           0x14
#define SYSTEM_SERVICE_INT     0x15
#define KEYBOARD_INT           0x16
#define PRINTER_INT            0x17
#define ROM_BASIC_INT          0x18
#define WARM_BOOT_INT          0x19
#define CLOCK_INT              0x1A
#define CTRL_BREAK_INT         0x1B
#define TIMER_TICK_INT         0x1C
#define VIDEO_INIT_TABLE       0x1D
#define DISK_INIT_TABLE        0x1E
#define GRAPHICS_TABLE         0x1F
#define RETURN_TO_DOS_INT      0x20
#define DOS_INT                0x21
#define DOS_IDLE_INT           0x28

#define FIRST_USER_INT         0x60
#define LAST_USER_INT          0x66

#define REAL_TIME_CLOCK_INT    0x70

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


//===========================================================================
//  typedef definitions...
//---------------------------------------------------------------------------
typedef void interrupt (*intr_function)();

typedef struct
{
  byte  int_20_instructions[0x02];
  word  memory_size;
  byte  reserved_04;
  byte  call_to_DOS_displatcher[0x05];
  lword int_22_address;                //Terminate
  lword int_23_address;                //Ctrl-C
  lword int_24_address;                //Critical error
  byte  reserved_16[0x16];
  word  environment_segment;
  byte  reserved_2E[0x22];
  byte  int_21_retf_instructions[0x03];
  byte  reserved_53[0x09];
  byte  fcb_1[0x10];
  byte  fcb_2[0x14];
  union
  {
    byte commandline_data[0x80];
    byte default_dta[0x80];
  } u;
} PSP;


#endif
