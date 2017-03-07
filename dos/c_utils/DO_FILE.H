#define MAX_DO_LEVEL 6
#define FNAM_LEN 40
#define MAX_DO_LEVEL_EXCEEDED 1
#define DO_FILE_NOT_FOUND     2

#ifdef DO_FILE_MAIN
int do_level=0;
char fname[MAX_DO_LEVEL][FNAM_LEN];
FILE *dofile[MAX_DO_LEVEL];

#else
extern int do_level;
extern char fname[][FNAM_LEN];
extern FILE *dofile[];

#endif