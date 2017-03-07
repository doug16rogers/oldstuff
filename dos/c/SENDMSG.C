#include <stdio.h>
#include <stdlib.h>

#include "typdef.h"
#include "msgapi.h"
#include "msglib.h"


int main(int argc,char* argv[])
{
  word sta=0;
  word err;
  if (argc!=3) {
    printf(
      "sendmsg <station> <message>\n"
      "  put <message> in quotes if it's got whitespace in it\n"
    );   //printf
    return 1;
  }   //if bad arg count

  if (!sscanf(argv[1],"%u",&sta) || !sta) {
    printf("bad station number \"%s\"\n",argv[1]);
    return 2;
  }

  err=broadcast_message(sta,argv[2]);
  printf("sent \"%s\" to %u.  err=%u\n",argv[2],sta,err);
  return err;
}   //main

