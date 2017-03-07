/***			    Issue: B
************************************************************************
* File:  serdef.h
*
************************************************************************
* Description:
*
* Contains the serial communications procedure definitions.
*
************************************************************************
* Revision History:
*
* Date     Issue        Author          Reason
*
* 08/13/89   A		Eric Harmon	New.
* 09/02/89   B		Eric Harmon	Added the clear_port function.
*
************************************************************************
***/
unsigned       changebaud(int, unsigned);
void	       clear_port(int);
void	       delay(unsigned);
void	       do_ivec(int, int);
unsigned char  get_rs232(int);
unsigned       getbaud(int);
void	       i_mask(int, int);
void	       rs232_off(int, unsigned char);
void           rs232_on(int, unsigned char);
int	       s_buffst(int);
void	       s_config(int);
void	       s_defintr(int, unsigned char);
int	       s_fgetc(int, unsigned);
int	       s_fgets(int, unsigned char *, int);
void	       s_fputs(int, unsigned char *);
unsigned char  s_getch(int);
unsigned char  s_getfmt(int);
int	       s_inchar(int);
unsigned char  s_intrid(int);
int	       s_intson(int, unsigned char);
void INTERRUPT s_intsrv1(void);
void	       s_putch(int, unsigned char);
unsigned char  s_rcv(int);
int	       s_rcvstat(int);
int	       s_rdbuff(int);
void	       s_setfmt(int, unsigned char);
void	       s_unconfig(int);
void	       s_ungetc(int, unsigned char);
void	       s_xmit(int, unsigned char);
int	       s_xmtstat(int);
void           setdtr(int, int);
void	       setbaud(int, unsigned);
void	       undo_ivec(int);
