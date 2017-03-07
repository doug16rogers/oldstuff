#ifndef __pc_info
#define __pc_info

/*****************************************************************************
*
*  TITLE:        PC Information
*
*  DESCRIPTION:  The header file "PC_Info"
*                provides types and constants that
*                are helpful for programming IBM PCs and
*                compatibles.
*
*  *k "%n"
*  FILE NAME:    ""
*
*  *k "%v"
*  VERSION:      ""
*
*  REFERENCE:    Peter Norton's _Programmer's Guide to the IDM PC and PS/2_.
*                Ray Duncan's _Programmer's Quick Reference_ series.
*                _Undocumented DOS_
*
*****************************************************************************/

#ifndef __icitypes
#include "icitypes.h"
#endif

#ifndef INTERRUPT_ARGUMENTS     // used for interrupt routine C compatibility
#ifdef __cplusplus
  #define INTERRUPT_ARGUMENTS void
#else
  #define INTERRUPT_ARGUMENTS ...
#endif
#endif


   // _FLAGS bit break-out...

#define FLAGS_CF        0x0001  // carry
#define FLAGS_PF        0x0004  // parity
#define FLAGS_AF        0x0010  // auxiliary
#define FLAGS_ZF        0x0040  // zero
#define FLAGS_SF        0x0080  // sign
#define FLAGS_TF        0x0100  // trap
#define FLAGS_IF        0x0200  // interrupt
#define FLAGS_DF        0x0400  // direction
#define FLAGS_OF        0x0800  // overflow


   // Interrupts...

#define ZERO_INT                0x00
#define SINGLE_STEP_INT         0x01
#define NON_MASKABLE_INT        0x02
#define NMI                     0x02
#define BREAKPOINT_INT          0x03
#define OVERFLOW_INT            0x04
#define PRTSCR_INT              0x05
#define TIMER_HW_INT            0x08
#define KEYBOARD_HW_INT         0x09
#define COM2_INT                0x0B
#define COM1_INT                0x0C
#define HARD_DISK_INT           0x0D
#define FLOPPY_DISK_INT         0x0E
#define PRINT_MANAGER_INT       0x0F
#define VIDEO_INT               0x10
#define EQUIPMENT_INT           0x11
#define MEMORY_SIZE_INT         0x12
#define DISKETTE_INT            0x13
#define COM_PORT_INT            0x14
#define SYSTEM_SERVICE_INT      0x15
#define KEYBOARD_INT            0x16
#define PRINTER_INT             0x17
#define ROM_BASIC_INT           0x18
#define WARM_BOOT_INT           0x19
#define CLOCK_INT               0x1A
#define CTRL_BREAK_INT          0x1B
#define TIMER_TICK_INT          0x1C
#define VIDEO_INIT_TABLE        0x1D
#define DISK_INIT_TABLE         0x1E
#define GRAPHICS_TABLE          0x1F
#define RETURN_TO_DOS_INT       0x20
#define DOS_SERVICES_INT        0x21
#define DOS_IDLE_INT            0x28
#define DOS_MULTIPLEX_INT       0x2F
#define DPMI_INT                0x31

#define FIRST_USER_INT          0x60
#define LAST_USER_INT           0x66

#define REAL_TIME_CLOCK_INT     0x70


   // IO Ports...

#define PIC_PORT        0x20
#define PIC1_PORT       0x20
#define PIC2_PORT       0xA0
   #define PIC_NSEOI            0x20

#define CMOS_PORT       0x70
#define CMOS_DATA       0x71

#define SECOND          0x00
#define MINUTE          0x02
#define HOUR            0x04
#define WEEK_DAY        0x06
#define DAY             0x07
#define MONTH           0x08
#define YEAR            0x09

#define READ_CMOS(value,reg) \
   outportb(CMOS_PORT, reg);  \
   asm nop;                   \
   value = inportb(CMOS_DATA)

   // Serial chip (8250) register definitions...

#define MSKVAL(a,m,v)  ( ((a)&(~(m))) | ((v)&(m)) )

#define SER_COM1INT    0x0C
#define SER_COM2INT    0x0B
#define SER_COM3INT    0x0D
#define SER_COM4INT    0x0D

#define SER_COM1IMR    0x10            /*IMR bit for intr 0x0C, above*/
#define SER_COM2IMR    0x08
#define SER_COM3IMR    0x20
#define SER_COM4IMR    0x20

#define SER_BAUD_MSK   0x01FF
#define SER_BAUD_300   0x0180
#define SER_BAUD_600   0x00C0
#define SER_BAUD_1200  0x0060
#define SER_BAUD_2400  0x0030
#define SER_BAUD_4800  0x0018
#define SER_BAUD_9600  0x000C
#define SER_BAUD_19200 0x0006

#define IOP_PIC        0x0020          /*programmable interrupt controller*/
#define IOP_IMR        0x0021          /*interrupt mask register I/O port*/

#define SIO_RBR        0x00            /*receiver buffer register, DLAB=0*/
#define SIO_THR        0x00            /*transmit hold register, DLAB=0*/
#define SIO_IER        0x01            /*interrupt enable register*/
#define   SIO_ERBFI    0x01            /*enable received data available int*/
#define   SIO_ETBEI    0x02            /*enable transmitter hold register empty int*/
#define   SIO_ELSI     0x04            /*enable receiver line status int*/
#define   SIO_EDSSI    0x08            /*enable modem status interrupt*/
#define SIO_IIR        0x02            /*interrupt identification register*/
#define   SIO_INTPEND  0x01            /*0 if interrupt pending*/
#define   SIO_INTID    0x06            /*interrupt ID bits*/
#define   SIO_INTRX    0x02            /*receive interrupt*/
#define   SIO_INTTX    0x04            /*transmit hold register empty int*/
#define SIO_LCR        0x03            /*line control register*/
#define   SIO_WLS      0x03            /*word length select bits: */
#define   SIO_WLS0     0x01            /*WLS bit 0       00-5  01-6  */
#define   SIO_WLS1     0x02            /*WLS bit 1       10-7  11-8  */
#define   SIO_DAT      0x03            /*more convenient names..*/
#define   SIO_DAT5     0x00
#define   SIO_DAT6     0x01
#define   SIO_DAT7     0x02
#define   SIO_DAT8     0x03
#define   SIO_STB      0x04            /*stop bits: 0-1  1-2*/
#define   SIO_STB1     0x00
#define   SIO_STB2     0x04
#define   SIO_PEN      0x08            /*parity enable:  0-off  1-on*/
#define   SIO_EPS      0x10            /*even parity select: 0-odd  1-even*/
#define   SIO_PAR      0x18
#define   SIO_PARN     0x00
#define   SIO_PARO     0x10
#define   SIO_PARE     0x18
#define   SIO_SPAR     0x20            /*stick parity*/
#define   SIO_SBRK     0x40            /*set break:  0-normal  1-set break*/
#define   SIO_DLAB     0x80            /*divisor latch access bit*/
#define SIO_MCR        0x04            /*modem control register*/
#define   SIO_DTR      0x01            /*data terminal ready, inverted*/
#define   SIO_RTS      0x02            /*request to send, inverted*/
#define   SIO_OUT1     0x04            /*user output 1, inverted*/
#define   SIO_OUT2     0x08            /*user output 2, inverted*/
#define   SIO_EINTS    0x08            /*OUT2=enable interrupts*/
#define   SIO_LPBK     0x10            /*loopback of TX to RX*/
#define SIO_LSR        0x05            /*line status register*/
#define   SIO_DR       0x01            /*data ready when 1*/
#define   SIO_OR       0x02            /*overrun error, bits 1-4 can gen ints*/
#define   SIO_PE       0x04            /*parity error*/
#define   SIO_FE       0x08            /*framing error (RX stop bit bad)*/
#define   SIO_BI       0x10            /*break interrupt (RX was BREAK)*/
#define   SIO_THRE     0x20            /*transmitter hold register empty*/
#define   SIO_TSRE     0x40            /*transmitter shift register empty*/
#define SIO_MSR        0x06            /*modem status register*/
#define   SIO_DCTS     0x01            /*delta clear to send, CTS changed*/
#define   SIO_DDSR     0x02            /*delta data set ready*/
#define   SIO_TERI     0x04            /*trailing edge ring indicator*/
#define   SIO_DRLSD    0x08            /*delta receive line signal detected*/
#define   SIO_CTS      0x10            /*clear to send*/
#define   SIO_DSR      0x20            /*data set ready*/
#define   SIO_RI       0x40            /*ring indicator*/
#define   SIO_RLSD     0x80            /*received line signal detected*/
#define SIO_DLL        0x00            /*divisor latch LSB, when DLAB=1*/
#define SIO_DLM        0x01            /*divisor latch MSB, when DLAB=1*/




typedef void interrupt (*INTERRUPT_HANDLER)();


typedef struct
{
   UINT8 length;
   UINT8 text[0x7f];

} COMMAND_LINE;


    /* the following are data block types... */

#define BUFFERS_AREA            'B'     /* BUFFERS= */
#define EMS_WORKSPACE_AREA      'C'     /* BUFFERS /X */
#define DEVICE_DRIVER           'D'     /* DEVICE= */
#define DEVICE_DRIVER_APPENDAGE 'E'
#define FILES_STORAGE_AREA      'F'     /* FILES=, if > 5 */
#define IFS_DRIVER              'I'     /* Installable File System driver */
#define LASTDRIVE_AREA          'L'     /* LASTDRIVE= */
#define MEMORY_BLOCK            'M'     /* memory block */
#define STACKS_AREA             'S'     /* STACKS= */
#define FCBS_AREA               'X'     /* FCBS= */
#define LAST_BLOCK              'Z'     /* last data block */

typedef struct
{
   UINT8  block_type;           /* see block types above */
   UINT16 start_segment;        /* subsegment start paragraph, usually next */
   UINT16 size;                 /* size in paragraphs */
   UINT8  unused_05[3];
   UINT8  driver_name[8];       /* 0-terminated if shorter than 8;
                                /*    provided for types 'D' & 'I' only */

} DATA_CONTROL_BLOCK;


typedef struct
{
   UINT8  block_type;           /* either 'M' or 'Z'; see define's above */
   UINT16 owner_psp_segment;    /* segment of owner's PSP */
   UINT16 size;                 /* size in paragraphs of memory block */
   UINT8  unused_05[3];
   UINT8  owner_name[8];        /* 0-terminated if less than 8 long; */
                                /*    unused before DOS 4.x */

} MEMORY_CONTROL_BLOCK;


     /* Program Segment Prefix */

typedef struct
{
   /*00*/ UINT8   int_20_instructions[0x02];   /* exit to DOS instructions */
   /*02*/ UINT16  memory_size;
   /*04*/ UINT8   reserved_04;
   /*05*/ UINT8   call_to_DOS_dispatcher[0x05];
   /*0a*/ UINT32  int_22_address;              /* Terminate */
   /*0e*/ UINT32  int_23_address;              /* Ctrl-C */
   /*12*/ UINT32  int_24_address;              /* Critical error */
   /*16*/ UINT16  parent_psp_segment;
   /*18*/ UINT8   open_file_table[0x14];
   /*2c*/ UINT16  environment_segment;
   /*2e*/ UINT32  process_ss_sp;        /* from last DOS call (int 0x21) */
   /*32*/ UINT16  max_open_files;
   /*34*/ UINT32  open_file_table_pointer;
   /*38*/ UINT32  previous_psp_pointer; /* DOS SHARE */
   /*3c*/ UINT8   reserved_3C[0x14];
   /*50*/ UINT8   int_21_retf_instructions[0x03];
   /*53*/ UINT8   reserved_53[0x09];
   /*5c*/ UINT8   fcb_1[0x10];
   /*6c*/ UINT8   fcb_2[0x14];
   /*80*/ union
          {
             COMMAND_LINE command_line;
             UINT8 default_dta[0x80];
          } the;

} PSP;


#endif
