/***************************************************************************
/*
/*        TOOLS.C: The expression parser used by grep.
/*
/*               Copyright (c) 1984 Allen Holub
/*     Copyright (c) 1984 Software Engineering Consultants
/*                         P.O. Box 5679
/*                     Berkeley, CA,  94705
/*
/*                     All rights reserved.
/*
/**************************************************************************/

#include <stdio.h>
#include <string.h>
#include <dos.h>
#include "tools.h"

        /*      This module contains the various routines needed by grep  */
        /* to match regular expressions. To find an expression in a       */
        /* string, only use makepat() to make a pattern template,         */
        /* matchs() to match the string, and include "tools.h".           */
        /* for example,                                                   */
        /*                                                                */
        /*         #include "tools.h"                                     */
        /*         TOKEN *template; char *ptr;                            */
        /*         template = makepat("456",'\0');                        */
        /*         ptr = matchs("1234567890", template, 0);               */
        /*                                                                */
        /*      Use stoupper() to set lines to upper case for case-       */
        /* insensitive searches.                                          */

int amatch(char *lin, TOKEN *pat, char *boln)
{

       /*      Scans through the pattern template looking for a match     */
       /* with lin. Each element of lin is compared with the template     */
       /* until either a mis-match is found or the end of the template is */
       /* reached.  In the former case a 0 is returned; in the latter, a  */
       /* pointer into lin (pointing to the last character in the matched */
       /* pattern) is returned.                                           */

   register char   *bocl, *rval, *strstart;

   if (pat == 0)
      return(0);

   strstart = lin;

   while(pat)
   {
      if (pat->tok == CLOSURE  &&  pat->next)
      {

       /* Process a closure: first skip over the closure token to the     */
       /*    object to be repeated.  This object can be a character class.*/

         pat = pat->next;

       /* Now match as many of the possible occurrences of the closure    */
       /*    pattern as possible.                                         */

         bocl = lin;

         while (*lin  &&  omatch(&lin, pat, boln) );

       /* lin now points to the character that made us fail.  Now go on   */
       /*    Now go on to process the rest of the string.  A problem here */
       /*    is a character following the closure which could have been   */
       /*    in the closure.  We will repeat the process of trying to     */
       /*    match the rest of the string until we get back to the        */
       /*    closure.                                                     */

         if (pat = pat->next)
         {
            while (bocl <= lin)
            {
               if (rval = amatch(lin, pat, boln) )
                  return(rval);      /* success */
               else
                  --lin;
            }
            return(0);               /* match failed */
         }
      }
      else if (omatch(&lin, pat, boln))
         pat = pat->next;
      else
         return(0);
   }
   return(max(strstart, --lin));
}


delete(int ch, register char *str)
{

       /*      Delete the first occurrence of character from string      */
       /* moving everything else over a notch to fill the hole.          */

   ch &= 0xff;

   while (*str && *str != ch)
      str++;

   while (*str)
   {
      *str = *(str+1);
      str++;
   }
}


int dodash(int delim, char **src, char *dest, int maxccl)
{

       /*      Expand the set pointed to by *src into dest, stopping at   */
       /* delim.  Return 0 on error or size of character class on         */
       /* success. Update *src to point at delim.  The maximum number of  */
       /* chars in a class is defined by maxccl.                          */

   register char   *dstart;
   register int    k, at_begin;
   char            *sptr;

   dstart = dest;
   sptr = *src;
   at_begin = 1;

   while (*sptr  &&  (*sptr != delim)  &&  (dstart-dest < maxccl))
   {
      if (*sptr == ESCAPE)
      {
         *dest++ = esc(&sptr);
         sptr++;
      }
      else if (*sptr != '-')
              *dest++ = *sptr++;
      else if (at_begin  ||  *(sptr+1) == delim)
              *dest++ = '-';
      else if (*(sptr-1) <= *(sptr+1))
      {
              sptr++;
              for (k= *(sptr-2); ++k <= *sptr; )
                 *dest++ = k;
              sptr++;
      }
      else
              return(0);

      at_begin = 0;
   }
   *dest++ = '\000';
   *src = sptr;

   return(dest - dstart);
}


int esc(char **s)
{

       /*      Map escape characters into their equivalent symbols.       */
       /* Returns the correct ASCII character.  If no escape prefix is    */
       /* present then s is untouched and *s is returned, otherwise **s   */
       /* is advanced to point at the escaped character and the trans-    */
       /* lated character is returned.                                    */

   register int rval;

   if (**s != ESCAPE)
      rval = **s;
   else
   {
      (*s)++;

      switch(toupper(**s))
      {
         case '\000':   rval = ESCAPE; break;
         case 'S':      rval = ' ';    break;
         case 'N':      rval = '\n';   break;
         case 'T':      rval = '\t';   break;
         case 'B':      rval = '\b';   break;
         case 'R':      rval = '\r';   break;
         default:       rval = **s;    break;
      }
   }

   return(rval);
}



TOKEN *getpat(char *arg)
{

       /* Translate arg into a TOKEN string.                              */

   return(makepat(arg, '\000'));
}



insert(int ch, char *str)
{

       /*      Insert ch into str at the place pointed to by str.  Move   */
       /* everything else over a notch.                                   */

   register char  *bp;

   bp = str;

   while (*str)
      str++;

   do
   {
      *(str+1) = *str;
      str--;
   } while (str >= bp);

   *bp = ch;
}



char *in_string(register int delim, register char *str)
{

       /* Return a pointer to delim if it is in the string, 0 if not.     */

   delim &= 0x7f;

   while (*str  &&  *str != delim)
      str++;

   return(*str ? str:0);
}



int isalphanum(int c)
{

       /* Return true if c is an alphanumeric, false otherwise.           */

   return(('a' <= c  &&  c <= 'z')  ||
          ('A' <= c  &&  c <= 'Z')  ||
          ('0' <= c  &&  c <= '9')    );
}



TOKEN *makepat(char *arg, int delim)
{

       /*      Make a pattern template from the string pointed to by arg. */
       /* Stop when delim or '\000' or '\n' is found.  Return a pointer   */
       /* to the pattern template.                                        */

   TOKEN *head, *tail;
   TOKEN *ntok;
   char buf[CLS_SIZE];
   int error;

       /* Check for characters that aren't legal at the beginning of a    */
       /*    template.                                                    */

   if (*arg == '\0'  ||  *arg == delim  ||  *arg == '\n'  ||  *arg == CLOSURE)
      return(0);

   error = 0;
   head = 0;
   tail = 0;

   while (*arg  &&  *arg != delim  &&  *arg != '\n'  &&  !error)
   {
      error = allocmem(TOKSIZE, &ntok);
      if (error != -1)
         printf("allocmem failure\n");

      error = 0;

      ntok->string = &(ntok->lchar);
      ntok->lchar = '\000';
      ntok->next = 0;

      switch(*arg)
      {
         case ANY:
            ntok->tok = ANY;
            break;

         case BOL:
            if (head == 0)         /* then this is the first symbol */
               ntok->tok = BOL;
            else
               error = 1;
            break;

         case EOL:
            if (*(arg+1) == delim  ||  *(arg+1) == '\000'  ||  *(arg+1) == '\n')
               ntok->tok = EOL;
            else
               error = 1;
            break;

         case CCL:
            if (*(arg+1) == NEGATE)
            {
               ntok->tok = NCCL;
               arg += 2;
            }
            else
            {
               ntok->tok = CCL;
               arg++;
            }

            error = dodash(CCLEND, &arg, buf, CLS_SIZE);
            if (error != 0)
            {
               error = allocmem(error, &(ntok->string));
               if (error != -1)
                  printf("error in allocmem\n");

               strcpy(ntok->string, buf);
               error = 0;
            }

            break;

         case CLOSURE:
            if (head != 0)
            {
               switch(tail->tok)
               {
                  case BOL:
                  case EOL:
                  case CLOSURE:
                     return(0);
                  default:
                     ntok->tok = CLOSURE;
               }
            }

            break;

         default:
            ntok->tok = LITCHAR;
            ntok->lchar = esc(&arg);
      }

      if (error  ||  ntok == 0)
      {
         unmakepat(head);
         return(0);
      }
      else if (head == 0)
      {
         ntok->next = 0;
         head = tail = ntok;
      }
      else if (ntok->tok != CLOSURE)
      {
         tail->next = ntok;
         ntok->next = tail;
         tail = ntok;
      }
      else if (head != tail)            /* More than one node in chain.   */
      {
         (tail->next)->next = ntok;     /* Insert the CLOSURE node        */
         ntok->next = tail;             /*    immediately before tail.    */
      }
      else                              /* Only one node in chain.        */
      {
         ntok->next = head;             /* Insert CLOSURE node at head    */
         tail->next = ntok;             /*    of linked list.             */
         head = ntok;
      }

      arg++;
   }
   tail->next = 0;
   return(head);
}


char *matchs(char *line, TOKEN *pat, int ret_endp)
{

       /*      This procedure will return a pointer to the location of    */
       /* the item searched for in the string.  Use makepat to make the   */
       /* pattern template out of a string, then call this routine.  When */
       /* ret_endp = 0, the ptr returned is the pointer to the  start of  */
       /* the string.  If ret_endp = 1, the pointer points to the end.    */

   char *rval, *bptr;

   bptr = line;

   while (*line)
   {
      if ((rval = amatch(line, pat, bptr)) == 0)
         line ++;
      else
      {
         rval = ret_endp ? rval : line;
         break;
      }
   }

   return(rval);
}



stoupper(char *str)
{

       /*      Map the entire string pointed to by str to upper case.     */
       /* Return str.                                                     */

   char *rval;

   rval = str;

   while (*str)
   {
      if ('a' <= *str  &&  *str <= 'z')
         *str -= ('a' - 'A');

      str++;
   }

   return(rval);
}



int max(int x, int y)
{
   return( (x>y) ? x : y );
}



int omatch(char **linp, TOKEN *pat, char *boln)
{

       /*      Match one pattern element, pointed to by pat, with the     */
       /* char at **linp.  Return non-zero on match, otherwise 0.  *Linp  */
       /* is advanced to skip over the matched character; it is not       */
       /* advanced on failure.  The amount of advance is 0 for patterns   */
       /* that match null strings, 1 otherwise.  "boln" should point at   */
       /* at the position that will match a BOL token.                    */

   register int advance;

   advance = -1;

   if (**linp)
   {
      switch(pat->tok)
      {
         case LITCHAR:
            if (**linp == pat->lchar)
               advance = 1;
            break;

         case BOL:
            if (*linp == boln)
               advance = 0;
            break;

         case ANY:
            if (**linp != '\n')
               advance = 1;
            break;

         case EOL:
            if (**linp == '\n')
               advance = 0;
            break;

         case CCL:
            if (in_string(**linp, pat->string))
               advance = 1;
            break;

         case NCCL:
            if (!in_string(**linp, pat->string))
               advance = 1;
            break;

         default:
            printf("omatch: can't happen\n");
      }
   }

   if (advance >= 0)
      *linp += advance;

   return(++advance);
}



pr_line(register char *ln)
{

       /* Print out ln.  If a non-printing char is found, print hex value */

   for ( ; *ln; ln++)
   {
      if ((' ' <= *ln)  &&  (*ln <= '~'))
         putchar(*ln);
      else
      {
         printf("\\0x%02x", *ln);

         if (*ln == '\n')
            putchar('\n');
      }
   }
}



pr_tok(TOKEN *head)
{

       /*      Print out the pattern template (linked list of TOKENs)     */
       /* pointed to by head.                                             */

   register char *str;

   for ( ; head; head = head->next)
   {
      switch(head->tok)
      {
         case BOL:
            str = "BOL OR NEGATE";
            break;

         case EOL:
            str = "EOL";
            break;

         case ANY:
            str = "ANY";
            break;

         case LITCHAR:
            str = "LITCHAR";
            break;

         case ESCAPE:
            str = "ESCAPE";
            break;

         case CCL:
            str = "CCL";
            break;

         case CCLEND:
            str = "CCLEND";
            break;

         case NCCL:
            str = "NCCL";
            break;

         case CLOSURE:
            str = "CLOSURE";
            break;

         default:
            str = "**** unknown ****";
      }

      printf("%-8s at: 0x%x, ", str, head);

      if (head->tok == CCL  ||  head->tok == NCCL)
         printf("string =[%s]=, ", head->string);

      else if (head->tok == LITCHAR)
         printf("lchar = %c, ", head->lchar);

      printf("next = 0x%x\n", head->next);
   }
   putchar('\n');
}



unmakepat(TOKEN *head)
{

        /* Free up the memory used for the token string                   */

   register TOKEN *old_head;

   while (head)
   {
      switch (head->tok)
      {
         case CCL:
         case NCCL:
            free(head->string);
      }
      old_head = head;
      head = head->next;
      free(old_head);
   }
}
