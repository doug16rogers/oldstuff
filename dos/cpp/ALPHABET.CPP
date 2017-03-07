#include <stdiostr.h>
#include <ctype.h>

#include "alphabet.hpp"


//***************************************************************************
void Alphabet::Purge(void)
{
  for (int i=0;i<MAX_ALPHA;i++) {
    alpha[i] = 0;
    inver[i] = -1;
  }   //for
  count = 0;
}   //Alphabet::Purge(void)


//***************************************************************************
int  Alphabet::Add(BYTE ch)
{
  if (inver[ch] != -1) return 0;
  if (count >= MAX_ALPHA) return 0;
  inver[ch] = count;
  alpha[count++] = ch;
  return count;
}   //Alphabet::Add(BYTE)


//***************************************************************************
int  Alphabet::Add(char* s)
{
  while (*s) Add(*(BYTE*)s++);
  return count;
}   //Alphabet::Add(char*)


//***************************************************************************
void Alphabet::Show(char* intro,char* ending)
{
  if (intro!=NULL) printf("%s",intro);
  for (int i=0;i<count;i++) {
    if (!isprint(alpha[i]))
      printf("\x%02X",alpha[i]);
    else if ((alpha[i]=='\\')||(alpha[i]=='\"'))
      printf("\%c",alpha[i]);
    else
      printf("%c",alpha[i]);
  }   //for
  if (ending!=NULL) printf("%s",ending);
}   //Alphabet::Show(char* intro)


