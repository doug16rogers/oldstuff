#include <stdio.h>

#define MAIN
#include "consts.h"
#include "cutil.h"


#define MAX_LEN 32

char infn[80];
char oufn1[]="_temp_1_.des";
char oufn2[]="_temp_2_.des";
FILE *inf,*ouf1,*ouf2;

main (int argn,char *argv[])
{
  infn[0]=0;
  printf ("Output will be to \"%s\" and \"%s\".\n",oufn1,oufn2);
  if (argn>1)
    strcpy (infn,argv[1]);
  else {
    do {
      printf ("Was/Is filename: ");
      gets (infn);
      strip_white (infn);
    } while (infn[0]==0);
  }   /* else read filename from command line */
  open_files ();
  read_in ();
}   /* main */


open_files ()
{
  inf=fopen (infn,"rt");
  if (inf==NULL) {
    printf ("Could not open \"%s\" for read.",infn);
    exit (1);
  }
  ouf1=fopen (oufn1,"wt");
  if (ouf1==NULL) {
    printf ("Could not open \"%s\" for write.",oufn1);
    exit (1);
  }
  ouf2=fopen (oufn2,"wt");
  if (ouf2==NULL) {
    printf ("Could not open \"%s\" for write.",oufn2);
    exit (1);
  }
}   /* open_files */


read_in ()
{
  int line;
  int num_des;
  char s[255];
  char des[255];
  line=0;
  num_des=0;
  while (!feof (inf)) {
    line++;
    fgets (s,255,inf);
    strip_white (s);
    parse_str (s);
    if (num_params>0) {
      if (num_params!=2) {
        printf ("Parameter count wrong, [%d]: \"%s\"\n",line,s);
      }
      else {
        if ((param_len[0]>MAX_LEN)||(param_len[1]>MAX_LEN))
          printf ("Parameter too long, [%d]: \"%s\"\n",line,s);
        else {
          num_des++;
          sub_str (s,param_pos[0],param_len[0],des);
          fprintf (ouf1,"%s TEMP_%d\n",des,num_des);
          sub_str (s,param_pos[1],param_len[1],des);
          fprintf (ouf2,"TEMP_%d %s\n",num_des,des);
        }   /* if lengths ok */
      }   /* if param num ok */
    }   /* if there are parameters */
  }   /* while */
  fclose (inf);
  fclose (ouf1);
  fclose (ouf2);
  printf ("Processed %d designators in %d lines.\n",num_des,line);
}   /* read_in */