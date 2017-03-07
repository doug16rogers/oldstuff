#if !defined (_SERCOM_INCLUDED_)
#define _SERCOM_INCLUDED_

#if defined (_SERCOM_GLOBAL_)
  #define SIO_decl               /*if compiling SERCOM.C, generate PUBLICs*/
#else
  #define SIO_decl extern        /*otherwise generate externals*/
#endif

#if !defined (byte)
  #define byte unsigned char
#endif
#if !defined (word)
  #define word unsigned int
#endif
#if !defined (dword)
  #define dword long unsigned int
#endif

/****************************************************************************/
/*  configuration information                                               */
/****************************************************************************/
#define USE_INTERRUPTS       1         /*specify use interrupt-driven code*/
#define MAX_PORTS            2         /*only 2 ports supported*/
#define SER_BUFF_SIZE       20         /*size of serial buffer*/
#define TERM_STR_SIZE       10         /*termination string size*/
#define COM1                 0         /*COM1 port definition*/
#define COM2                 1         /*COM2 port definition*/
#define COM3                 2         /*COM3 port definition*/
#define COM4                 3         /*COM4 port definition*/


/****************************************************************************/
/*  global constants for PC                                                 */
/****************************************************************************/
#define UART_TABLE     (dword)(0x00000400)  /*table of uart port bases*/
#define TIMER_LO       (dword)(0x0000046C)  /*pointer to LSW of system ticker*/
#define PIC_REG        0x0020               /*programmable interrupt controller*/
#define IMR_REG        0x0021               /*interrupt mask register*/
#define INT0BEN          0x08               /*enable bit for interrupt 0x0B*/
#define INT0CEN          0x10               /*enable bit for interrupt 0x0C*/


/****************************************************************************/
/*  uart register constants                                                 */
/****************************************************************************/
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
#define SIO_LCR        0x03            /*line control register*/
#define   SIO_WLS      0x03            /*word length select bits: */
#define   SIO_WLS0     0x01            /*WLS bit 0       00-5  01-6  */
#define   SIO_WLS1     0x02            /*WLS bit 1       10-7  11-8  */
#define   SIO_STB      0x04            /*stop bits: 0-1  1-2*/
#define   SIO_1STB     0x00            /*one stop bit*/
#define   SIO_2STB     0x04            /*two stop bits*/
#define   SIO_PEN      0x08            /*parity enable:  0-off  1-on*/
#define   SIO_EPS      0x10            /*even parity select: 0-odd  1-even*/
#define   SIO_PAR      0x18            /*mask for parity bits*/
#define   SIO_NONE     0x00            /*no parity*/
#define   SIO_ODD      0x08            /*odd parity*/
#define   SIO_EVEN     0x18            /*even parity*/
#define   SIO_SPAR     0x20            /*stick parity*/
#define   SIO_SBRK     0x40            /*set break:  0-normal  1-set break*/
#define   SIO_DLAB     0x80            /*divisor latch access bit*/
#define SIO_MCR        0x04            /*modem control register*/
#define   SIO_DTR      0x01            /*data terminal ready, inverted*/
#define   SIO_RTS      0x02            /*request to send, inverted*/
#define   SIO_OUT1     0x04            /*user output 1, inverted*/
#define   SIO_OUT2     0x08            /*user output 2, inverted*/
#define   SIO_INTEN    0x08            /*interrupt enables???*/
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


/****************************************************************************/
/*  error, call and return codes...                                         */
/****************************************************************************/
enum error_codes {
  S_ILLEGAL_PORT       =-100,
  S_NO_PORT_AVAILABLE,
  S_ALREADY_CONFIGURED,

  S_OK                 =0
};   /*error_codes*/
#define INQUIRE        0xFFFF          /*for inquiring info -- not set*/
#define UNKNOWN        0xFFFE          /*unknown setting was found*/
#define RESET          1               /*reset comm parameters on unconfig*/
#define NO_RESET       0               /*leave comm parameters on unconfig*/

enum parity_codes { NO_PARITY,ODD_PARITY,EVEN_PARITY };  /*for s_parity*/


/****************************************************************************/
/*  Structure:  serial                                                      */
/****************************************************************************/
/*  Description:                                                            */
/*                                                                          */
/*  Contains the information for the configuration and use of the serial    */
/*  ports.                                                                  */
/*                                                                          */
/****************************************************************************/
struct flag_rec {
  word init:1;                         /*has port been initialized*/
  word rxtimout:1;                     /*enable/disable receive timeouts*/
  word txtimout:1;                     /*enable/disable transmit timeouts*/
  word cnvrtcr:1;                      /*conversion flag for CR to CR+LF*/
  word rxframe:1;                      /*frame on reception*/
  word framefound:1;                   /*framing character found*/
  word rxterm:1;                       /*enable receive termination str*/
  word txterm:1;                       /*use termination str on string tx*/
};

struct serial_rec {
  struct flag_rec flags;               /*various flags*/
  word uartbase;                       /*base address of currant uart*/
  word startbaud;                      /*initial baud rate*/
  word startparity;                    /*initial parity*/
  word startdatabits;                  /*initial data bits*/
  word startstopbits;                  /*initial stop bits*/
  void (interrupt *startvect)();       /*initial interrupt vector*/
  int  intnum;                         /*interrupt number*/
  void (interrupt *rxvect)();          /*interrupt vector for port*/
  word commport;                       /*com port being addressed*/
  int  rxhead;                         /*head of buffer*/
  int  rxtail;                         /*tail of buffer*/
  int  rxcount;                        /*number of chars in buffer */
  char rxbuff[SER_BUFF_SIZE];          /*interrupt buffer*/
  char rxframechar;                    /*framing character for receive*/
  char rxtermstr[TERM_STR_SIZE];       /*receive termination string*/
  word rxtimout;                       /*receive char timeout in ticks*/
  char txtermstr[TERM_STR_SIZE];       /*transmit termination string*/
  word txtimout;                       /*transmit char timeout in ticks*/
};


/****************************************************************************/
/*  variable definitions...                                                 */
/****************************************************************************/
SIO_decl struct serial_rec s_ports[MAX_PORTS];         /*serial port records*/


/****************************************************************************/
/*  function prototypes...                                                  */
/****************************************************************************/
word s_config (word port);             /*configures a port*/
void s_unconfig (word port,int reset); /*resets port to original settings*/
int  s_rxready (word port);            /*returns 1 if rx char ready in port*/
int  s_txready (word port);            /*returns 1 if tx char ready for port*/
char s_getc (word port);               /*gets a character from port*/
int  s_putc (word port,char c);        /*puts a character to a port*/
int  s_puts (word port,char *s);       /*puts a string*/
word s_baudrate (word port,word baud); /*returns/sets baud rate for port*/
word s_databits (word port,word data); /*returns/sets data bits for port*/
word s_stopbits (word port,word bits); /*returns/sets stop bits for port*/
word s_parity (word port,word parity); /*return/set parity*/
word s_status (word port);             /*returns (line) status for port*/
void s_break (word port,word ticks);   /*holds break for ticks clock ticks*/
word s_modstatus (word port);          /*modem status for port*/

#endif                                 /*if first include of sercom.h*/
