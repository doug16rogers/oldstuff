/***
************************************************************************
* File:  serial.h
*
************************************************************************
* Description:
*
* Contains the system wide structure definitions.
*
************************************************************************
* Revision History:
*
* Date     Issue        Author          Reason
*
* 08/13/89   A          Eric Harmon     New
*
************************************************************************
***/
/***
************************************************************************
* Description:
*
* Contains the serial port constant definitions.
*
************************************************************************
***/

/***
************************************************************************
* Data format masks
************************************************************************
***/

#define PARITY_MASK	0x18	/* 00011000  Parity field		*/
#define LEN_MASK	0x3	/* 00000011  Data length field		*/
#define NONE_MASK	0x00	/* 00000000  No parity setting		*/
#define ODD_MASK	0x08	/* 00001000  Odd parity 		*/
#define EVEN_MASK	0x18	/* 00011000  even parity		*/
#define STOP_MASK	0x4	/* 00000100  Stop bit field		*/
#define ONE_MASK	0x00	/* 00000000  1 stop bit 		*/
#define TWO_MASK	0x04	/* 00000100  2 stop bits		*/
#define FIVE_MASK       0x00    /* 00000000  5 data bits                */
#define SIX_MASK        0x01    /* 00000001  6 data bits                */
#define SEVEN_MASK	0x02	/* 00000010  7 data bits		*/
#define EIGHT_MASK	0x03	/* 00000011  8 data bits		*/

/***
************************************************************************
* Timer counts for function delay()
************************************************************************
***/

#define _0_SEC_05	1		/* 50 ms 		*/
#define _0_SEC_1	2		/* 1.8 			*/
#define _0_SEC_15	3		/* 150 ms		*/
#define _0_SEC_2	4		/* 3.6			*/
#define _0_SEC_25	5		/* 250 ms		*/
#define _0_SEC_3	6		/* 5.5			*/
#define _0_SEC_4	7		/* 7.3			*/
#define _0_SEC_5	9		/* 9.1			*/
#define _0_SEC_6       11		/* 11.9			*/
#define _0_SEC_7       13		/* 12.7			*/
#define _0_SEC_8       15		/* 14.6			*/
#define _0_SEC_9       16		/* 16.4			*/
#define _1_SEC_0       18		/* 18.2			*/
#define _1_SEC_5       29


/***
************************************************************************
* usart register constants
************************************************************************
***/
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
#define   SIO_PEN      0x08            /*parity enable:  0-off  1-on*/
#define   SIO_EPS      0x10            /*even parity select: 0-odd  1-even*/
#define   SIO_SPAR     0x20            /*stick parity*/
#define   SIO_SBRK     0x40            /*set break:  0-normal  1-set break*/
#define   SIO_DLAB     0x80            /*divisor latch access bit*/
#define SIO_MCR        0x04            /*modem control register*/
#define   SIO_DTR      0x01            /*data terminal ready, inverted*/
#define   SIO_RTS      0x02            /*request to send, inverted*/
#define   SIO_OUT1     0x04            /*user output 1, inverted*/
#define   SIO_OUT2     0x08            /*user output 2, inverted*/
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


/***
************************************************************************
* Miscellaneous constants and masks
************************************************************************
***/

#define BAD_ITYPE	0x100	/* Unchanged int flag			*/
#define BAUD_LSB	0	/* Baud offset - Most Sig Bit		*/
#define BAUD_MSB	1	/* Baud offset - Least Sig Bit		*/
#define CONTROL_232	4       /*modem control register*/
#define COMM1		0	/* comm ports				*/
#define COMM2		1
#define COMM3		2
#define COMM4		3
#define DATA_FORMAT	3	/* Offset for the data format register	*/
#define DTROFF		0xFE	/* DTR off mask 			*/
#define DTRON		0x01	/* DTR on mask				*/
#define DTR_MASK	1	/* Data Transmit Ready mask		*/
#define ENABLE_ERR	4	/* bit 2 - Int on error?		*/
#define ENABLE_RCV	1	/* Bit 0 - Int on receive		*/
#define ENABLE_XMIT	2	/* Bit 1 - Int on empty trans buff	*/
#define ENABLE_232	8	/* bit 3 - RS232 ?			*/
#define I_DISABLE	1	/* Disable interrupt flag		*/
#define I_ENABLE	0	/* Enable interrupt flag		*/
#define IMR_PORT	0x21	/* Interrupt mask register port 	*/
#define INTON_MASK	8	/* Enable RS232 interupts mask		*/
#define INTR_ENABLE	1	/* Offset of int enable reg		*/
#define INTR_ID 	2	/* Offset of interrupt ID reg		*/
#define MAX_INTR        10      /* Max number of ints can swtch         */
#define NO_INIT         -1      /* Structure not initialized            */
#define NO_INTR         -2      /* None existing interrupt              */
#define RCV		0	/* Offset of register from uart base	*/
#define RCV_MASK	1	/* Bit 0				*/
#define RTS_MASK        2       /* Ready To Send mask                   */
#define SER_BUFF        10000   /* Serial buffer size                   */
#define SIO_INIT_BAD	0	/* MY bad init flag for serial		*/
#define SIO_INIT_OK	1	/* MY Init flag for serial port 	*/
#define SIO_STAT	5	/* Offset for status register		*/
#define TIMER_LO	0x46C	/* Location of timer tick buffer	*/
#define TIMEOUT 	-1	/* Timeout error flag			*/
#define UART_PTR	0x400	/* Offset of the address to the 	*/
#define XMIT		0	/* Offset of register from uart base	*/
#define XMIT_MASK	0x20	/* Bit 6 of register			*/

/***
************************************************************************
* Structure:  serial
*
************************************************************************
* Description:
*
* Contains the information for the configuration and use of the serial
* ports.
*
************************************************************************
***/
struct serial
{
	unsigned uart_base;		/* base address of currant uart */
	unsigned char usrfmt;		/* data format upon entry	*/
	unsigned usrbaud;		/* baud rate upon entry		*/
	unsigned startbaud;		/* default baud rate		*/
	unsigned char startfmt;		/* default data format		*/
	unsigned char start232;		/* default rs232 pins to turn on*/
	unsigned init_flag;		/* initialization OK flag	*/
	unsigned char usr232;		/* rs2323 pins upon entry	*/
        int      int_num;               /* Interrupt number             */
	void (INTERRUPT *oldvect)();    /* Old interrupt                */
	void (INTERRUPT *newvect)();    /* Interrupt vector for port    */
	int	 commport;		/* com port being addressed	*/
	int 	 head;			/* Head of buffer		*/
	int      tail;			/* Tail of buffer		*/
	int      buffcnt;		/* Number of chars in buffer	*/
	char	 buffer[SER_BUFF];	/* Interrupt buffer		*/
};
