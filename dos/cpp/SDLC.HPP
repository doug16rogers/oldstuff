#ifndef __SDLC_H
#define __SDLC_H


#define SDLC_NAME              "SDLC STANDARD"

extern BYTE SDLC_tx_addr;              //transmit address
extern BYTE SDLC_rx_addr;              //receive address
extern double SDLC_baud;               //baud rate for port

int  SDLC_init(void);
WORD SDLC_rx(void);
WORD SDLC_tx(void);
void SDLC_key(int ch);
char* SDLC_stat(char* stat);

extern CFG_TYPE SDLC_cfg[];

#endif
