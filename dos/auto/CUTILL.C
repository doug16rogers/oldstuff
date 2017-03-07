/*
  This module contains c utility routines.
*/
#include <stdio.h>
#include <conio.h>
#include <string.h>

#define CUTIL_MAIN

#include "cutil.h"
#include "consts.h"


/* upcase *******************************************************************/
/*                                                                          */
/*  Returns the uppercase equivalent of the character c.                    */
/*                                                                          */
/*************************************************** Doug Rogers 1989.01.12 */
char upcase (char c)
{
  if ((c<'a')||(c>'z'))
    return (c);
  else
    return (c-0x20);
}   /* upcase */


/* getkey *******************************************************************/
/*                                                                          */
/*  This function returns the ASCII code of the key returned by getch ().   */
/*  If a special key was pressed (such as Page Up, F1, etc), it returns     */
/*  the codes specified in the header file cutil.h.  Most of the time the   */
/*  code is simply the second code + 0x80 (sign-extended).  See the header  */
/*  file for the VERY convenient code names (_LTAR, _UPAR, A_F11, etc).     */
/*                                                                          */
/*************************************************** Doug Rogers 1989.01.12 */
unsigned char getkey ()
{
  unsigned char c;
  c=getch ();
  if (c==0) {
    c=getch ();
    switch (c) {
      case 0x03 : c=NUL;
      case 0x84 : c=C_PGUP;  /* C_PgUp */
      case 0x85 : c=_F11;    /* _F11   */
      case 0x86 : c=_F12;    /* _F12   */
      case 0x87 : c=S_F11;   /* S_F11  */
      case 0x88 : c=S_F12;   /* S_F12  */
      case 0x89 : c=C_F11;   /* C_F11  */
      case 0x8A : c=C_F12;   /* C_F12  */
      case 0x8B : c=A_F11;   /* A_F11  */
      case 0x8C : c=A_F12;   /* A_F12  */
      default   :
	if ((c>=0x78)&&(c<=0x83))
	  c+=0x09;
	else
	  c+=0x80;
	break;
    }   /* CASE */
  }   /* IF a special function key was pressed */
  return (c);
}   /* getkey */


/* sub_str ******************************************************************/
/*                                                                          */
/*  This function takes a substring of the argument char *s, starting at    */
/*  the position pos, of length len, and places the substring in the char   */
/*  *t string.                                                              */
/*                                                                          */
/*************************************************** Doug Rogers 1989.01.12 */
char *sub_str (char *s,int pos,int len,char *t)
{
  int i,j;

  j=0;
  for (i=pos;(s[i]!=0)&&(j<len);i++) t[j++]=s[i];
  t[j]=0;
  return (t);
}   /* sub_str */


/* white_char ***************************************************************/
/*                                                                          */
/*  This function returns TRUE (1) if the char c argument is one of the     */
/*   whitespace characters, otherwise it returns FALSE (0).                 */
/*                                                                          */
/*************************************************** Doug Rogers 1989.01.12 */
int white_char (char c)
{
  return (
		  (c==' ')  ||
		  (c==0x09) ||
		  (c==0x0A) ||
		  (c==0x0D) ||
		  (c==0x0C)
		 );
}   /* white_char */


/* alphanumeric *************************************************************/
/*                                                                          */
/*  This function returns TRUE if its char parameter is alphanumeric.       */
/*                                                                          */
/*************************************************** Doug Rogers 1989.01.12 */
int alphanumeric (char c)
{
  if (((c>='0')&&(c<='9')) ||
	  ((c>='A')&&(c<='Z')) ||
	  ((c>='a')&&(c<='z')))
	return (TRUE);
  else
	return (FALSE);
}   /* alphanumeric */


/* alphanumeric_str *********************************************************/
/*                                                                          */
/*  This function returns TRUE if its *char parameter contains only         */
/*  alphanumeric characters.                                                */
/*                                                                          */
/*************************************************** Doug Rogers 1989.01.12 */
int alphanumeric_str (char *s)
{
  int i;
  int an;

  an=TRUE;
  for (i=0;(s[i]!=0)&&(an);i++) an=alphanumeric (s[i]);
  return (an);
}   /* alphanumeric_str */


/* short_eq *****************************************************************/
/*                                                                          */
/*  This routine returns TRUE if one of longer string is equal to the       */
/*  shorter string up to the length of the shorter string.                  */
/*                                                                          */
/*************************************************** Doug Rogers 1989.01.12 */
int short_eq (char *s, char *t)
{
  int i;
  int eq;

  if ((s[0]==0)||(t[0]==0))
	return (FALSE);
  else {
	eq=TRUE;
	for (i=0;(s[i]!=0)&&(t[i]!=0)&&(eq);i++) eq=(s[i]==t[i]);
	return (eq);
  }
}   /* short_eq */


/* strip_white **************************************************************/
/*                                                                          */
/*  This routine strips all redundant white characters from the specified   */
/*  string.                                                                 */
/*                                                                          */
/*************************************************** Doug Rogers 1989.01.12 */
char *strip_white (char *s)
{
  enum {START,NORMAL,WHITE,QUOTE,END};
  int i,j,state;
  char q;

  state=START;
  for (i=j=0;s[i]!=0;i++) {
	switch (state) {
	  case START:
		if ((s[i]=='\"') || (s[i]=='\''))
		{
		  q=s[i];
		  s[j++]=s[i];
		  state=QUOTE;
		}   /* if 1 */
		else if (!white_char(s[i]))
		{
		  s[j++]=s[i];
		  state=NORMAL;
		}   /* else 1 */
		break;     /* case START */
	  case NORMAL:
		if ((s[i]=='\"') || (s[i]=='\'')) {
		  q=s[i];
		  s[j++]=s[i];
		  state=QUOTE;
		}
		else if (white_char (s[i])) {
		  s[j++]=' ';
		  state=WHITE;
		}
		else {
		  s[j++]=s[i];
		}
		break;     /* case NORMAL */
	  case WHITE:
		if ((s[i]=='\"') || (s[i]=='\'')) {
		  q=s[i];
		  s[j++]=s[i];
		  state=QUOTE;
		}
		else if (!white_char(s[i])) {
		  s[j++]=s[i];
		  state=NORMAL;
		}
		break;     /* case WHITE */
	  case QUOTE:
		if (s[i]==q) {
		  s[j++]=s[i];
		  state=NORMAL;
		}
		else
		  s[j++]=s[i];
		break;     /* case QUOTE */
	  default : state=NORMAL; break;
	}   /* switch */
  }   /* for */
  if (state==WHITE) j--;
  s[j]=0;
  return (s);
}   /* strip_white */


/* strip_comment ************************************************************/
/*                                                                          */
/*  This routine places a NUL at the position of the first comment char.    */
/*                                                                          */
/*************************************************** Doug Rogers 1989.01.12 */
char *strip_comment (char *s,char comchar)
{
  enum {START,NORMAL,WHITE,QUOTE,END};
  int i,state;
  char q;

  state=START;
  for (i=0;(s[i]!=0)&&(state!=END);i++) {
	switch (state) {
	  case START:
		if (s[i]==comchar) {
		  s[i]=0;
		  state=END;
		}
		else if ((s[i]=='\"') || (s[i]=='\''))
		{
		  q=s[i];
		  state=QUOTE;
		}   /* if 1 */
		else if (!white_char(s[i]))
		{
		  state=NORMAL;
		}   /* else 1 */
		break;     /* case START */
	  case NORMAL:
		if (s[i]==comchar) {
		  s[i]=0;
		  state=END;
		}
		else if ((s[i]=='\"') || (s[i]=='\'')) {
		  q=s[i];
		  state=QUOTE;
		}
		else if (white_char (s[i])) {
		  state=WHITE;
		}
		else {
		}
		break;     /* case NORMAL */
	  case WHITE:
		if (s[i]==comchar) {
		  s[i]=0;
		  state=END;
		}
		else if ((s[i]=='\"') || (s[i]=='\'')) {
		  q=s[i];
		  state=QUOTE;
		}
		else if (!white_char(s[i])) {
		  state=NORMAL;
		}
		break;     /* case WHITE */
	  case QUOTE:
		if (s[i]==q) {
		  state=NORMAL;
		}
		break;     /* case QUOTE */
	  default : state=END; break;
	}   /* switch */
  }   /* for */
  return (s);
}   /* strip_comment */


/* c_chr_str ****************************************************************/
/*                                                                          */
/*  This procedure converts the given string in a C-like manner.  The       */
/*  special character '\' can be followed with a DECIMAL ASCII number (up   */
/*  to three digits), 'r' (CR), 'n' (LF) or 'e' (ESC).  These should all    */
/*  be specified in lower case.  Use "\\" for '\'.                          */
/*                                                                          */
/*  Returns the length of the new string.                                   */
/*************************************************** Doug Rogers 1989.08.15 */
int c_chr_str (char *s)
{
  enum {NORMAL,BACKSLASH,GOTONE,GOTTWO};
  int i,j,n;
  int state;
  char c;

  state=NORMAL;
  i=0;
  j=0;
  for (i=0;s[i]!=0;i++) {
    c=s[i];
    switch (state) {
      case NORMAL:
        if (c=='\\') state=BACKSLASH; else s[j++]=c;
        break;
      case BACKSLASH:
        state=NORMAL;
        if      (c=='\\') s[j++]='\\';
        else if (c=='\"') s[j++]='\"';
        else if (c=='\'') s[j++]='\'';
        else if (c=='r') s[j++]=0x0d;
        else if (c=='n') s[j++]=0x0a;
        else if (c=='e') s[j++]=0x1b;
        else if ((c>='0')&&(c<='9')) { n=c-'0'; state=GOTONE; }
        else { s[j++]='\\'; s[j++]=c; }
        break;
      case GOTONE:
        if ((c>='0')&&(c<='9')) {
          n=10*n+(c-'0');
          state=GOTTWO;
        }
        else {
           s[j++]=n;
           if (c=='\\') state=BACKSLASH;
           else {
             s[j++]=c;
             state=NORMAL;
           }
        }
        break;
      case GOTTWO:
        if ((c>='0')&&(c<='9')) {
          n=10*n+(c-'0');
          s[j++]=n;
          state=NORMAL;
        }
        else {
           s[j++]=n;
           if (c=='\\') state=BACKSLASH;
           else {
             s[j++]=c;
             state=NORMAL;
           }
        }
        break;
      default: state=NORMAL; break;
    }   /* switch on state */
  }   /* for */
  switch (state) {
    case NORMAL:    break;
    case BACKSLASH: s[j++]='\\';  break;
    case GOTONE:
    case GOTTWO:    s[j++]=n;  break;
    default: break;
  }   /* switch on final state */
  s[j]=0;
  return (j);
}   /* c_chr_str */


/* parse_str ****************************************************************/
/*                                                                          */
/*  This routine parses its char *s parameter.  It places the number of     */
/*  parameters present in the global int num_params and places the          */
/*  starting position and length of each parameter in the global arrays     */
/*  int param_pos[MAX_PARAMS],param_len[MAX_PARAMS].                        */
/*                                                                          */
/*************************************************** Doug Rogers 1989.01.12 */
int parse_str (char *s)
{
  enum {START,NORMAL,WHITE,QUOTE,END};
  int i,state;
  char q;

  state=START;
  num_params=0;
  for (i=0;s[i]!=0;i++) {
	switch (state) {
	  case START:
		if ((s[i]=='\"') || (s[i]=='\''))
		{
		  if (num_params<MAX_PARAMS)
		  {
			param_len[num_params]=0;
			param_pos[num_params]=i+1;
		  }
		  q=s[i];
		  state=QUOTE;
		}   /* if 1 */
		else if (!white_char(s[i]))
		{
		  if (num_params<MAX_PARAMS)
		  {
			param_len[num_params]=1;
			param_pos[num_params]=i;
		  }
		  state=NORMAL;
		}   /* else 1 */
		break;     /* case START */
	  case NORMAL:
		if ((s[i]=='\"') || (s[i]=='\''))
		{
		  num_params++;
		  if (num_params<MAX_PARAMS)
		  {
			param_len[num_params]=0;
			param_pos[num_params]=i+1;
		  }
		  q=s[i];
		  state=QUOTE;
		}
		else if (white_char (s[i]))
		  state=WHITE;
		else
		{
		  if (num_params<MAX_PARAMS) param_len[num_params]++;
		}
		break;     /* case NORMAL */
	  case WHITE:
		if ((s[i]=='\"') || (s[i]=='\'')) {
		  num_params++;
		  if (num_params<MAX_PARAMS) {
			param_len[num_params]=0;
			param_pos[num_params]=i+1;
		  }
		  q=s[i];
		  state=QUOTE;
		}
		else if (!white_char(s[i])) {
		  num_params++;
		  if (num_params<MAX_PARAMS) {
			param_len[num_params]=1;
			param_pos[num_params]=i;
		  }
		  state=NORMAL;
		}
		break;     /* case WHITE */
	  case QUOTE:
		if (s[i]==q)
		  state=WHITE;
		else
		  if (num_params<MAX_PARAMS) param_len[num_params]++;
		break;     /* case QUOTE */
	  default : state=WHITE; break;
	}   /* switch */
  }   /* for */
  if (state!=START) num_params++;
  return (num_params);
}   /* parse_str */


/* param_int ****************************************************************/
/*                                                                          */
/*  This function returns TRUE if parameter param of string s (presumably   */
/*  parsed already by parse_str) is converted into an integer without       */
/*  error by sscanf.                                                        */
/*                                                                          */
/*************************************************** Doug Rogers 1989.03.10 */
int param_int (char s[],int param,int *i)
{
  if ((param>=num_params)||(param>=MAX_PARAMS)) return (FALSE);
  return (sscanf (&s[param_pos[param]],"%d",i)==1);
}   /* param_int */


/* show_params **************************************************************/
/*                                                                          */
/*  This function simply lists the parameters produced by parse_str.        */
/*                                                                          */
/*************************************************** Doug Rogers 1989.01.12 */
int show_params (char *s)
{
  int i;
  char t[STR_LEN];

  printf ("num_params=%d\n",num_params);
  for (i=0;i<min (num_params,MAX_PARAMS);i++) {
	sub_str (s,param_pos[i],param_len[i],t);
	printf ("param%d=\"%s\"\n",i,t);
  }
  return (num_params);
}   /* show_params */


/* cinit ********************************************************************/
/*                                                                          */
/*  This function writes len1 characters of the string s using attr1, then  */
/*  writes the remaining characters using attr2.                            */
/*                                                                          */
/*************************************************** Doug Rogers 1989.01.12 */
void cinit (char *s,int len1,int attr1,int attr2)
{
  int i;
  textattr (attr1);
  for (i=0;(i<len1)&&(s[i]!=0);i++) cprintf ("%c",s[i]);
  textattr (attr2);
  for (;s[i]!=0;i++) cprintf ("%c",s[i]);
}   /* cinit */