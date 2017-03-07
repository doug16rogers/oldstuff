
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

