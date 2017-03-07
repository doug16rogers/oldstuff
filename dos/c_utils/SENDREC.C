#include <stdio.h>
#include <conio.h>

#define SENDREC_MAIN
#include "consts.h"
#include "sendrec.h"
#include "rcl.h"
#include "cutil.h"


error (char *s)
{
  cprintf ("%%SND-%s\r\n",s);
  snd_error=TRUE;
}


int addrok (char *adr)
{
  int i;

  i=strlen (adr);
  if (sscanf (adr,"%d",&i)!=1)
    return (FALSE);
  else if ((i<1)||(i>31))
    return (FALSE);
  else {
    sprintf (adr,"%d ",i);
    return (TRUE);
  }
}   /* addrok */


send (char *devadr,char *devstr)
{
  SENDST (devadr,devstr);
}   /* send */


recv (char *devadr, char *rcvstr)
{
  SETLEN (rcvstr,STR_LEN);
  RECVST (devadr,rcvstr);
}   /* recv */


/* swrite *******************************************************************/
/*                                                                          */
/*  This routine writes a string to the specified serial port.              */
/*                                                                          */
/************************************** Mike Dillon, Doug Rogers 1988.12.15 */
swrite (int board, char *ostring)
{
  int j;

  j=0;
  while (ostring[j]!=0) serial_out (board,ostring[j++]);
}	/* swrite */


/* sread ********************************************************************/
/*                                                                          */
/*  This routine reads a string from the specified serial port.             */
/*                                                                          */
/************************************** Mike Dillon, Doug Rogers 1988.12.15 */
sread (int board, char *istring)
{
#define SERTIMOUT 5

  int i;
  int frame;
  unsigned char temp;
  int timout;
  long int timtim;

  timtim=18 * SERTIMOUT;    /* 18 timer ticks per second */
  timout=FALSE;
  istring[0]=0;
  settimer ();
  i=0;

  temp=' ';
  for (i=0;(i<STR_LEN)&&(!timout)&&(temp!='\n');i++)
  {
	temp=serial_in (board);
	if ((temp==0)||(temp==255))
	  timout=(gettimer ()>timtim);
	else
	  istring[i]=temp;
  } /* for */

  if (i>=STR_LEN) i=STR_LEN-1;
  istring[i]=0;
}   /* sread */


/* com_write ****************************************************************/
/*                                                                          */
/*  Writes a string to the specified DOS COM port.                          */
/*                                                                          */
/*************************************************** Doug Rogers 1988.12.15 */
com_write (int prt, char *s)
{
 int i;

 for (i=0;(s[i]!=0)&&(i<STR_LEN);i++) outch (prt,s[i]);
 outch (prt,'\r');
 outch (prt,'\n');
}   /* com_write */


/* com_read *****************************************************************/
/*                                                                          */
/*  Reads a string from the specified DOS COM port.                         */
/*                                                                          */
/*************************************************** Doug Rogers 1988.12.15 */
com_read (int prt, char *s)
{
#define SERTIMOUT 5

  char c;
  int b;
  int timout;
  long int timtim;
  char sertermstr[5];

  timtim=18 * SERTIMOUT;          /* 18 timer ticks per second */
  settimer ();                    /* start the timout clock */
  timout=FALSE;
  c=' ';
  for (b=0;(b<STR_LEN)&&(!timout)&&(c!='\n');b++) {
	if (!char_ready (prt))
	  timout=(gettimer ()>timtim);
	else {
	  c=inch (prt);
	  s[b]=c;
	}   /* else char ready */
  }   /* for */
  if (b>=STR_LEN) b=STR_LEN-1;
  s[b]=0;
}   /* com_read */


/* initialize_bus ***********************************************************/
/*                                                                          */
/*  This routine initializes the Ziatech IEEE-488 bus.  DO NOT CALL THIS    */
/*  ROUTINE if THE ZIATECH CARD IS NOT PRESENT!!!                           */
/*                                                                          */
/*************************************************** Doug Rogers 1988.12.13 */
int initialize_bus ()
{
  unsigned char devstr[5];

  snd_msg_on=FALSE;
  YEAR=0x3938;  /* one's digit, ten's digit */
  DELAYCONST=54;
  CLKADR=0x0240;
  PRIMARYADR=0x0210;
  SECONDARYADR=0x0220;
  TIMEDY (3000);
  INIT (1);
  if (ERCODE==5)
	return (FALSE);
  else {
	TIMEDY (0);
	devstr[0]='\n';
	devstr[1]=0;
	TERM (devstr);
  }
  return (TRUE);
}   /* initialize_bus */


/* baud_bits ****************************************************************/
/*                                                                          */
/*  This function takes a baud rate as its parameter and returns the        */
/*  appropriate bit settings for a call to interrupt 14H.                   */
/*                                                                          */
/*************************************************** Doug Rogers 1988.12.15 */
int baud_bits (int baud)
{
  switch (baud) {
	case  110 : return (0x0000);
	case  150 : return (0x0020);
	case  300 : return (0x0040);
	case  600 : return (0x0060);
	case 1200 : return (0x0080);
	case 2400 : return (0x00A0);
	case 4800 : return (0x00C0);
	case 9600 : return (0x00E0);
	default   : return (-1);
  }   /* switch */
}   /* baud_bits */


/* dev_num ******************************************************************/
/*                                                                          */
/*  This function returns the device number for the device indicated in     */
/*  string parameter.  Devices are stored in a PUBLIC array of records.     */
/*  The routine returns 0 if the corresponding device is not found.         */
/*                                                                          */
/*************************************************** Doug Rogers 1988.12.13 */
int dev_num (char *dname)
{
  int i;
  int found;
  char nam[NAME_LEN];

  sub_str (dname,0,NAME_LEN,nam);
  found=FALSE;
  for (i=0;(i<num_dev)&&(!found);i++) {
	found=(stricmp (nam,dev_list[i].dev_name)==0);
  }
  if (!found)
	return (MAX_DEVICES);
  else
	return (i);
}   /* dev_num */


/* send_str *****************************************************************/
/*                                                                          */
/*  This routine sends the string specified by msg to the device specified  */
/*  by dev.  The routine determines whether to use a serial port or the     */
/*  IEEE-488 bus.                                                           */
/*                                                                          */
/*************************************************** Doug Rogers 1988.12.13 */
send_str (char *devstr,char *msg)
{
  int dnum;
  dev_rec *dev;

  dnum=dev_num (devstr);
  if (dnum==MAX_DEVICES)
	error ("SNDBADDEV, illegal device name, make sure device is installed");
  else {
	dev=&dev_list[dnum];
	if (snd_msg_on) cprintf ("sending \"%s\" to %s\r\n",msg,dev->dev_name);
	if (dev->dev_dev==rtscom_dev) setRTS (dev->dev_com,dev->dev_rts);
	switch (dev->dev_dev) {
	  case ieee_dev   : send (dev->dev_addr,msg);
						break;
	  case rtscom_dev :
	  case qadcom_dev : swrite (dev->dev_com,msg);
						break;
	  case doscom_dev : com_write (dev->dev_com,msg);
						break;
	  case qadpar_dev : break;
	  case ioport_dev : write_port (dev->dev_out,msg[0]);
						break;
	  default    	  : cprintf ("send_str not yet available for this type\r\n");
						break;
	}   /* switch */
  }   /* else device was found */
}   /* send_str */


/* recv_str *****************************************************************/
/*                                                                          */
/*  This routine receives a string from the device specified by the string  */
/*  variable devstr and places it in the VAR string parameter rec.  This    */
/*  routine will receive as many characters, up to 255, as the sending      */
/*  device sends.                                                           */
/*                                                                          */
/*************************************************** Doug Rogers 1988.12.14 */
recv_str (char *devstr,char *rec)
{
  int dnum;
  dev_rec *dev;

  dnum=dev_num (devstr);
  if (dnum==MAX_DEVICES)
	error ("RCVBADDEV, illegal device name; make sure device is installed");
  else {
	dev=&dev_list[dnum];
	switch (dev->dev_dev) {
	  case ieee_dev   : recv (dev->dev_addr,rec);
						break;
	  case rtscom_dev :
	  case qadcom_dev : sread  (dev->dev_com,rec);
						break;
	  case doscom_dev : com_read (dev->dev_com,rec);
						break;
	  case qadpar_dev : break;
	  case ioport_dev : rec[0]=read_port (dev->dev_in);
						rec[1]=0;
						break;
	  default    	  : cprintf ("recv_str not yet available for this type\r\n");
						break;
	}   /* switch */
  }   /* else device was found */
}	/* recv_str */


/* sendrec_str **************************************************************/
/*                                                                          */
/*  This routine sends then receives a string to/from the device specified  */
/*  by the string variable devstr.                                          */
/*                                                                          */
/*************************************************** Doug Rogers 1988.12.14 */
sendrec_str (char *devstr, char *rec)
{
  int dnum;
  dev_rec *dev;

  dnum=dev_num (devstr);
  if (dnum==MAX_DEVICES)
	error ("SRCBADDEV, illegal device name; make sure device is installed");
  else {
	dev=&dev_list[dnum];
	switch (dev->dev_dev) {
	  case ieee_dev   : send (dev->dev_addr,rec);
						if (!snd_error) recv (dev->dev_addr,rec);
						break;
	  case rtscom_dev :
	  case qadcom_dev : sread  (dev->dev_com,rec);  break;
	  case doscom_dev : com_read (dev->dev_com,rec);  break;
	  case qadpar_dev : break;
	  case ioport_dev : rec[0]=read_port (dev->dev_in);
						rec[1]=0;
						break;
	  default    	  : cprintf ("recv_str not yet available for this type\r\n");
						break;
	}   /* switch */
  }   /* else device was found */
}	/* sendrec_str */


/* install_device ***********************************************************/
/*                                                                          */
/*  This routine enters a device into the current device list.  if a        */
/*  device with the given name already exists, an error is reported via     */
/*  the EXTERN BOOLEAN variable snd_error.                                  */
/*                                                                          */
/*************************************************** Doug Rogers 1988.12.13 */
install_device (dev_rec *dev, char *initst)
{
  char nname[NAME_LEN];
  int i,j,b;

  if ((num_dev<0)||(num_dev>MAX_DEVICES)) num_dev=0;
  if (num_dev==MAX_DEVICES)
	error ("MAXDEV, maximum number of devices already defined");
  else {
	strcpy (nname,dev->dev_name);
	if (dev_num (nname)<MAX_DEVICES) {
	  cprintf ("%%RCL-DEVEXIST, device %s already exists. Delete it first.\r\n",nname);
	  snd_error=TRUE;
	}
	else {
/*
	  switch (dev->dev_dev) {
		case ieee_dev   : ;
		case qadcom_dev :
		case rtscom_dev :
		case doscom_dev : b=baud_bits (dev->dev_baud);
						  if (b==-1) {
							cprintf ("Invalid baud rate, setting it to 9600\r\n");
							dev->dev_baud=9600;
							b=baud_bits (dev->dev_baud);
						  } /* if invalid baud rate */
						  b=(b & 0x00E0);   /* leave only parity bits */
						  switch (dev->dev_prty) {
							case p_none : break;
							case p_odd  : b=(b | 0x0008);  break;
							case p_even : b=(b | 0x0018);  break;
							default     : break;
						  }
						  if (dev->dev_sbit==2) b=(b | 0x0004);
						  if (dev->dev_dbit==7)
							b=(b | 0x0002);
						  else
							b=(b | 0x0003);
						  if (dev->dev_dev!=doscom_dev)
							serial_init (dev->dev_com,b);
						  else
							com_init (dev->dev_com,b);
						  break;   /* qadcom_dev,doscom_dev CASE */
		case qadpar_dev : break;
		case ioport_dev : write_port (initst[0],dev->dev_ctrl);
						  break;
		default         : cprintf ("Weird device type in install_device\r\n");
						  break;
	  } /* switch */
	  if ((initst[0]!=0)&&(dev->dev_dev!=rtscom_dev))
		send_str (dev->dev_name,initst);
*/
	  memcpy (&dev_list[num_dev++],dev,sizeof (dev_rec));
	  if (snd_msg_on) cprintf ("Device %s installed.\r\n",dev->dev_name);
	}   /* else device is not yet in list */
  }   /* else device list is not full */
}   /* install_device */


/* delete_device ************************************************************/
/*                                                                          */
/*  This routine deletes the device with the given name from the device     */
/*  list.  if the specified device is not present, an error is reported     */
/*  via the EXTERN BOOLEAN variable snd_error.                              */
/*                                                                          */
/*************************************************** Doug Rogers 1988.12.13 */
delete_device (char *dname)
{
  char nname[NAME_LEN];
  int i;

  if ((num_dev<0)||(num_dev>MAX_DEVICES)) num_dev=0;
  if (num_dev==0)
	error ("NODEV, no devices available to delete");
  else {
	i=dev_num (dname);
	if (i==MAX_DEVICES) {    /* if device not in list */
	  cprintf ("%%RCL-BADDEV, no such device \"%s\"\r\n",dname);
	  snd_error=TRUE;
	}
	else {
	  if (snd_msg_on) cprintf ("Deleting device \"%s\"\r\n",dname);
	  memcpy (dev_list[i],dev_list[num_dev--],sizeof (dev_rec));
	}   /* else device was indeed in the list */
  }   /* else # of devices > 0 */
}   /* delete_device */


/* list_devices *************************************************************/
/*                                                                          */
/*  This routine lists the current devices available in the device list.    */
/*                                                                          */
/*************************************************** Doug Rogers 1988.12.13 */
list_devices ()
{
  int i;
  dev_rec *d;

  if ((num_dev<0)||(num_dev>MAX_DEVICES)) num_dev=0;
  if (num_dev==0)
	cprintf ("No devices have been defined.\r\n");
  else {
	if (num_dev==1)
	  cprintf ("There is one device defined.\r\n");
	else
	  cprintf ("There are %d devices defined.\r\n",num_dev);
	for (i=0;i<num_dev;i++) {
	  d=&dev_list[i];
	  cprintf ("%s, ",d->dev_name);
	  switch (d->dev_dev) {
		case ieee_dev:
		  cprintf ("IEEE-488 address \"%s\"\r\n",d->dev_addr);
		  break;
		case rtscom_dev:
		case qadcom_dev:
		  cprintf ("QuadPort Serial device, COM%d",d->dev_com);
		  if (d->dev_dev==rtscom_dev) cprintf (" using RTS = %d",d->dev_rts);
		  cprintf ("\r\n");
		  cprintf ("  %d baud, %d data bits, ",d->dev_baud,d->dev_dbit);
		  if (d->dev_sbit==1)
			cprintf ("1 stop bit, ");
		  else
			cprintf ("2 stop bits, ");
		  switch (d->dev_prty) {
			case p_none:
			  cprintf ("no parity\r\n");
			  break;
			case p_even:
			  cprintf ("even parity\r\n");
			  break;
			case p_odd:
			  cprintf ("odd parity\r\n");
			  break;
			default:
			  break;
		  } /* inner switch */
		  break;
		case doscom_dev:
		  cprintf ("DOS Serial device, COM%d\r\n",d->dev_com+1);
		  cprintf ("  %d baud, %d data bits, ",d->dev_baud,d->dev_dbit);
		  if (d->dev_sbit==1)
			cprintf ("1 stop bit, ");
		  else
			cprintf ("2 stop bits, ");
		  switch (d->dev_prty) {
			case p_none:
			  cprintf ("no parity\r\n");
			  break;
			case p_even:
			  cprintf ("even parity\r\n");
			  break;
			case p_odd:
			  cprintf ("odd parity\r\n");
			  break;
			default:
			  break;
		  } /* inner switch */
		  break;
		case qadpar_dev:
		  cprintf ("QuadPort parallel port, no description yet\r\n");
		  break;
		case ioport_dev:
		  cprintf ("IO Port (PPI), input %Xh, output %Xh, control %Xh\r\n",
			d->dev_in,d->dev_out,d->dev_ctrl);
		  break;
		default:
		  cprintf ("unkown device type - update list_devices routine\r\n");
		  break;
	  }   /* switch */
	}   /* for through list */
  }   /* else a list */
}  /* list_devices */


/* load_devfile *************************************************************/
/*                                                                          */
/*  This routine loads devices from a text file.  The format for each line  */
/*  in the file, whose name is provided by the string parameter, is as      */
/*  follows:                                                                */
/*                                                                          */
/*  <init_str> is a string parameter which is sent to the device to         */
/*  initialize it.  Strings containing whitespace, quotation marks or       */
/*  apostrophes should be enclosed in apostrophes (') or quotes (").        */
/*                                                                          */
/*************************************************** Doug Rogers 1988.12.13 */

do_ieee (char *line, dev_rec *dev, char *initst,char *ss)
{
  int j;

  dev->dev_dev=ieee_dev;
  if (num_params!=4)
	error ("LODINUMP, invalid number of parameters for IEEE device");
  else {
	sub_str (line,param_pos[1],param_len[1],ss);
	if (sscanf (ss,"%d",&j)!=1)
	  error ("LODIBADDR, bad IEEE address");
	else if ((j<1)||(j>31))
	  error ("LODIBADDR, bad IEEE address");
	else {
	  sub_str (line,param_pos[2],min (NAME_LEN,param_len[2]),dev->dev_name);
	  if (!alphanumeric_str (dev->dev_name))
		error ("LODINONA, device names must contain only alphanumerics");
	  else {
		sub_str (line,param_pos[3],min (STR_LEN,param_len[3]),initst);
		sprintf (ss,"%d ",j);
		strcpy (dev->dev_addr,ss);
		install_device (dev,initst);
	  }   /* else all alphanumerics */
	}   /* else address is ok */
  }   /* else num_params ok */
}   /* do_ieee */


do_com (dev_type dt, char *line, dev_rec *dev, char *initst, char *ss)
{
  int j;

  dev->dev_dev=dt;
  if (num_params!=6)
	error ("LODCNUMP, invalid number of parameters for COM specification");
  else {
	sub_str (line,param_pos[1],param_len[1],ss);
	if (sscanf (ss,"%d",&j)!=1)
	  error ("LODCPRTN, bad serial port number");
	else if ( (((j<0)||(j>7)) && ((dt==qadcom_dev)||(dt==rtscom_dev))) ||
			  (((j<1)||(j>2)) && (dt==doscom_dev)) )
	  error ("LODCPRTN, bad serial port number");
	else {
	  if (dt==doscom_dev)
		dev->dev_com=j-1;
	  else
		dev->dev_com=j;
	  sub_str (line,param_pos[3],param_len[3],ss);
	  if (sscanf (ss,"%d",&j)!=1)
		error ("LODCBADUD, bad baud rate number for COM device");
	  else if (baud_bits (j)==-1)
		error ("LODCBADUD, bad baud rate number for COM device");
	  else {
		dev->dev_baud=j;
		j=param_pos[4];
		line[j]=upcase (line[j]);
		if ( !( ((param_len[4]==3)&&(dt!=rtscom_dev)) ||
				((param_len[4]==4)&&(dt==rtscom_dev)) ) )
		  error ("LODCBADPDSL, bad parity-databits-stopbits parameter");
		else if ( !( ((line[j]=='N')||(line[j]=='O')||(line[j]=='E'))
				  && ((line[j+1]=='8')||(line[j+1]=='7'))
				  && ((line[j+2]=='1')||(line[j+2]=='2'))) )
		  error ("LODCBADPDS, bad parity-databits-stopbits parameter");
		else {
		  switch (line[j]) {
			case 'N':
			  dev->dev_prty=p_none;
			  break;
			case 'E':
			  dev->dev_prty=p_even;
			  break;
			case 'O':
			  dev->dev_prty=p_odd;
			  break;
			dfault:
			  break;
		  }   /* switch */
		  switch (line[j+1]) {
			case '8':
			  dev->dev_dbit=8;
			  break;
			case '7':
			  dev->dev_dbit=7;
			  break;
			default:
			  break;
		  }   /* switch */
		  switch (line[j+2]) {
			case '1':
			  dev->dev_sbit=1;
			  break;
			case '2':
			  dev->dev_sbit=2;
			  break;
			default:
			  break;
		  }   /* switch */
		  if (dt==rtscom_dev) {
			if (line[j+3]=='0')
			  dev->dev_rts=0;
			else
			  dev->dev_rts=1;
		  }
		  sub_str (line,param_pos[5],min (STR_LEN,param_len[5]),initst);
		  sub_str (line,param_pos[2],min (param_len[2],NAME_LEN),dev->dev_name);
		  if (!alphanumeric_str (dev->dev_name))
			error ("LODCNONA, device names must contain only alphanumerics");
		  else {
			install_device (dev,initst);
		  }   /* else name is ok */
		}   /* else parity-databits-stopbits ok */
	  }   /* if baud rate is ok */
	}   /* else com number ok */
  }   /* else param count ok */
}   /* do_com */


do_parallel (char *line, dev_rec *dev, char *initst, char *ss)
{
  dev->dev_dev=qadpar_dev;
}


do_ioport (char *line, dev_rec *dev, char *initst, char *ss)
{
  int j;

  dev->dev_dev=ioport_dev;
  if (num_params!=6)
	error ("LODPNUMP, invalid number of parameters for IOPORT device");
  else {
	sub_str (line,param_pos[2],param_len[2],ss);
	if (sscanf (ss,"%d",&j)!=1)
	  error ("LODPBADIN, bad input port number");
	else {
	  dev->dev_in=j;
	  sub_str (line,param_pos[3],param_len[3],ss);
	  if (sscanf (ss,"%d",&j)!=1)
		error ("LODPBADIN, bad input port number");
	  else {
		dev->dev_out=j;
		sub_str (line,param_pos[4],param_len[4],ss);
		if (sscanf (ss,"%d",&j)!=1)
		  error ("LODPBADIN, bad input port number");
		else {
		  dev->dev_ctrl=j;
		  sub_str (line,param_pos[5],param_len[5],ss);
		  if (sscanf (ss,"%d",&j)!=1)
			error ("LODPBADIN, bad input port number");
		  else {
			sub_str (line,param_pos[1],min (param_len[1],NAME_LEN),dev->dev_name);
			if (!alphanumeric_str (dev->dev_name))
			  error ("LODPBADNAM, device name must be alphanumeric");
			else {
			  install_device (dev,initst);
			}   /* else name is ok */
		  }   /* else initialization number ok */
		}   /* else control port number ok */
	  }   /* else output port number ok */
	}   /* else input port number ok */
  }   /* else param count ok */
}   /* do_ioport */


int load_devfile (char *fname)
{
  char line[STR_LEN];
  FILE *dfile;
  int  i,j,lnum;
  char initst[STR_LEN];
  char ss[STR_LEN];
  dev_rec dev;

  if ((dfile=fopen (fname,"rt"))==NULL) {
	cprintf ("%%RCL-BADDEVFIL, error opening device file \"%s\"\r\n",fname);
	snd_error=TRUE;
	return (FALSE);
  }
  else {
	num_dev=lnum=0;
	while (!feof (dfile)) {
	  fgets (line,STR_LEN-1,dfile);
	  lnum++;
      strip_white (line);   /* get rid of white space */
	  if (snd_msg_on) cprintf ("%3d: %s\r\n",lnum,line);
	  if (alphanumeric (line[0])) {
		parse_str (line);  /* set up parse table */
		if (num_params>0) {
		  sub_str (line,param_pos[0],param_len[0],ss);
		  strupr (ss);
		  if      (short_eq (ss,"IEEE-488")) do_ieee (line,&dev,initst,ss);
		  else if (short_eq (ss,"DOSCOM")  ) do_com (doscom_dev,line,&dev,initst,ss);
		  else if (short_eq (ss,"QUADCOM") ) do_com (qadcom_dev,line,&dev,initst,ss);
		  else if (short_eq (ss,"COMQUAD") ) do_com (qadcom_dev,line,&dev,initst,ss);
		  else if (short_eq (ss,"QUADPAR") ) do_parallel (line,&dev,initst,ss);
		  else if (short_eq (ss,"PARQUAD") ) do_parallel (line,&dev,initst,ss);
		  else if (short_eq (ss,"RTSCOM")  ) do_com (rtscom_dev,line,&dev,initst,ss);
		  else if (short_eq (ss,"IOPORT")  ) do_ioport (line,&dev,initst,ss);
		  else {
			if (!snd_msg_on) cprintf ("%3d: %s\r\n",lnum,line);
			cprintf ("--- unknown command \"%s\" in device file \"%s\"\r\n",ss,fname);
          }   /* else NOT a command */
		}   /* if not blank line */
      }   /* if line contains a record, not a comment */
	}   /* while still reading file */
	fclose (dfile);
	return (TRUE);
  }   /* else file opened ok */
}   /* load_devfile */
