#include <stdio.h>
#include <string.h>
#include <dos.h>


#define byte unsigned char


byte msg[320];
byte rpy[320];
byte err;


int  main(int argc,char* argv[])
{
  int i;
  if (argc!=3) {
    printf("tlog <name> <password>\n");
    return 1;
  }
  i=2;
  msg[i++]=0;
  msg[i++]=strlen(argv[1]);
  while (*argv[1]) msg[i++]=*argv[1]++;
  msg[i++]=strlen(argv[2]);
  while (*argv[1]) msg[i++]=*argv[2]++;
  *(int*)msg=i;
  rpy[0]=0;
  asm {
    push ds
    push si
    push di
    mov  di,OFFSET rpy
    mov  ax,SEG rpy
    mov  ds,ax
    mov  si,OFFSET msg
    mov  ax,SEG msg
    mov  ds,ax
    mov  ah,0xE3
    int  0x21
    pop  di
    pop  si
    pop  ds
    mov  err,al
  };   //asm
  printf(
    "code was %u, reply was \\x%02X\\x%02X\\x%02X\\x%02X.\n"
    ,err,rpy[0],rpy[1],rpy[2],rpy[3]
  );
  return err;
}   //main


