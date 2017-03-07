#define NAME_LEN      13
#define ADDR_LEN      5
#define MAX_DEVICES   20

typedef enum {
  ieee_dev,
  qadcom_dev,
  qadpar_dev,
  doscom_dev,
  rtscom_dev,
  ioport_dev
 } dev_type;
typedef enum { p_none, p_even, p_odd } parity_type;
typedef struct {
  char      dev_name[NAME_LEN];
  dev_type  dev_dev;
  int       dev_com,dev_baud,dev_prty,dev_dbit,dev_sbit,dev_rts;
  char      dev_addr[ADDR_LEN];
  int		 dev_padr;
  int       dev_in,dev_out,dev_ctrl;
} dev_rec;

#ifdef SENDREC_MAIN
int snd_msg_on=FALSE;
int snd_error=FALSE;
int num_dev=0;
dev_rec dev_list[MAX_DEVICES];
int STAT[34];

#else
extern int snd_msg_on,snd_err,num_dev;
extern dev_rec dev_list[];
extern int STAT[34];

#endif

extern int ERCODE,RCVLEN,YEAR,DELAYCONST,CLKADR,PRIMARYADR,SECONDARYADR;
