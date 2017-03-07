#include <stdio.h>

#include <consts.h>

#define DO_FILE_MAIN
#include "do_file.h"
#include "cutil.h"
#include "rcl.h"

int do_file (char *fn)
{
  int i,done;

  if (!(do_level<MAX_DO_LEVEL))
	return (MAX_DO_LEVEL_EXCEEDED);
  else {
	if ((dofile[do_level]=fopen (fn,"r t"))==NULL)
	  return (DO_FILE_NOT_FOUND);
	else {
	  strcpy (fname[do_level],fn);
	  done=FALSE;
	  do {
		if (fgets (cmdline,128,dofile[do_level])==NULL)
		  done=TRUE;
		else {
		  if (echo_on) cprintf ("%s\r",cmdline);
		  analyze_cmd ();
		}   /* else not eof */
	  } while ((!done)&&(!rcl_error)&&(!rcl_break));
	  fclose (dofile[do_level]);
	}   /* if opened ok */
  }   /* if not max do levels */
  return (0);
}   /* do_file */