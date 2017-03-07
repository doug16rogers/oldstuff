/*
DOSVER.C -- set different DOS version numbers 
an alternate to patching programs such as EXE2BIN
*/

#include <stdlib.h>
#include <stdio.h>
#include <process.h>
#include <dos.h>
#include <string.h>

#pragma pack(1)

void (interrupt far *old)();
unsigned dosver, old_bx, old_cx;

typedef struct {
    unsigned es,ds,di,si,bp,sp,bx,dx,cx,ax,ip,cs,flags;
    } REG_PARAMS;

void interrupt far dos(REG_PARAMS r)
{
    if ((r.ax >> 8) == 0x30)
    {
        r.ax = dosver;
        r.bx = old_bx;
        r.cx = old_cx;
    }
    else
        _chain_intr(old);
}

void fail(char *s) { puts(s); exit(1); }

main(int argc, char *argv[])
{
    int major, minor;
    char command_line[0x100] = "\0";
    int i;

    if (argc < 4)
        fail( "usage: dosver <major> <minor> <command...>\n"
              "example: dosver 3 31 exe2bin devlod.exe devlod.com");

    strcpy(command_line, argv[3]);
    for (i = 4; (i < argc); i++)
    {
      strcat(command_line, " ");
      strcat(command_line, argv[i]);
    }

    if (! (major = atoi(argv[1])))
        fail("bad version number");
    if ((minor = atoi(argv[2])) < 10)       /* e.g. 3.1 to 3.10 */
        minor *= 10;
    dosver = (minor << 8) + major;

    _asm mov ax, 3000h
    _asm int 21h
    _asm mov old_cx, cx                     /* OEM, serial# */
    _asm mov old_bx, bx

    old = _dos_getvect(0x21);               /* save INT 21h */
    _dos_setvect(0x21, dos);                /* hook INT 21h */
    system(command_line);                   /* run command */
    _dos_setvect(0x21, old);                /* unhook INT 21h */
    return 0;
}
