#if !defined (_SYSDEF_INCLUDED_)
#define _SYSDEF_INCLUDED_

#if defined (TURBOC)
  #define INTERRUPT   interrupt
#else     /*assume MSC*/
  #define INTERRUPT   interrupt far
#endif

#define VIRGIN      0
#define NIL         NULL

#define TRUE        1
#define FALSE       0
#define ON          1
#define OFF         0

#endif            /*if not included already*/
