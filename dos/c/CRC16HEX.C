/* CRC16.C  -  by Rob Duff, Vancouver, BC, Canada.  Not copyrighted. */
#include <fcntl.h>
#include <sys\stat.h>
#include <io.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/*
    This algorithm is adapted from the classic paper
    "Byte-wise CRC Calculations", Aram Perez
    IEEE Micro June 1983 pp 40-50

    CRC16 is used by the popular ARC archiving program.
    CCITT is used by XMODEM, SDLC by IBM corp.

    gen16(0) will create table for CRC16
    gen16(1) for CCITT and start crc at -1 for SDLC
*/

extern findfile(char*, char*, int);

int usage(void);
void gen16(unsigned);
unsigned crc16(unsigned char*, unsigned, unsigned);

int gen_tab[2][8] = {
    { 0xC0C1, 0xC181, 0xC301, 0xC601, 0xCC01, 0xD801, 0xF001, 0xA001 },
    { 0x1189, 0x2312, 0x4624, 0x8C48, 0x1081, 0x2102, 0x4204, 0x8408 }
};

int crc_tab[256];

void
gen16(p)
unsigned p;
{
    int     i, j, val;

    for (i = 0; i < 256; i++) {
        val = 0;
        for (j = 0; j < 8; j++)
            if (i & (1<<j))
                val ^= gen_tab[p][j];
        crc_tab[i] = val;
    }
}

unsigned
crc16(data, count, old_crc)
register unsigned char *data;
register unsigned count, old_crc;
{
    union {
        unsigned w;
    struct {
        unsigned char lo;
        unsigned char hi;
    } b;
    } crc;

    crc.w = old_crc;
    while (count-- > 0)
      crc.w = crc_tab[*data++ ^ crc.b.lo] ^ crc.b.hi;
    return crc.w;
}

int usage()
{
    fprintf(stderr, "usage: crc16 [-c][-s] file...\n");
    fprintf(stderr, "       wildcards are permitted\n");
    fprintf(stderr, "       default is CRC16 (ARC)\n");
    fprintf(stderr, "       -c gives CCITT   (XMODEM)\n");
    fprintf(stderr, "       -s gives SDLC    (IBM)\n");
    return 1;
}

char    fname[64];

main(argc, argv)
char *argv[];
{
    FILE* fd;
    unsigned crc, k;
    int     poly, init;

    argc--; argv++;
    if (argv > 0 && argv[0][0] == '-') {
      if (argv[0][1] == 'c') {
        poly = 1;
        init = 0;
      }
      if (argv[0][1] == 's') {
        poly = 1;
        init = -1;
      }
      argc--; argv++;
    }
    else {
      poly = 0;
      init = 0;
    }
    if (argc < 1)
    return usage();
    gen16(poly);
    while (argc) {
      strcpy(fname,*argv);
      fd = fopen(fname,"rt");
      if (!fd) break;
      crc = init;
      while (fscanf(fd,"%x",&k)==1) {
    crc = crc16((char*)&k, 1, crc);
    printf("%02X  %04X\n",k,crc);
      }
      fclose(fd);
      printf("crc of %s is %04X\n", fname, crc);
      argc--; argv++;
    }
    return 0;
}

