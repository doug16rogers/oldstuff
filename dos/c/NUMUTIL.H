#ifndef __NUMUTIL_H
#define __NUMUTIL_H


#ifndef byte
  #define byte unsigned char
#endif


int   asciitonum(char* s,byte* n,int len);
char* numtoascii(byte *n,int len,char *s);

byte *add(byte *n1,byte *n2,byte *n3,int len);
byte *sub(byte *n1,byte *n2,byte *n3,int len);
byte *mul(byte *n1,byte *n2,byte *n3,int len);
byte *div(byte *n1,byte *n2,byte *n3,int len);


#endif
