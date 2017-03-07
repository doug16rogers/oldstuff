/*
      Declarations and prototype for the DHELP help utility.
*/

#ifndef DHELP_HEADER_LOADED

#define DHELP_HEADER_LOADED
#ifdef DHELP_MAIN
  #define HDecl
#else
  #define HDecl extern
#endif

#ifndef byte
  #define byte unsigned char
#endif

#ifndef word
  #define word unsigned int
#endif

#define MAX_KEY_SIZE     0x0020
#define MAX_BUFF_SIZE    0x0100
#define MAX_KEYS         0x0200
#define MAX_DEPTH        0x0008

#define BORDER_ATTR      BLACK+(LIGHTGRAY<<4)

typedef char keytype[MAX_KEY_SIZE];
typedef struct { keytype keyword; int page; } keyrec;

enum hmodes { COMMAND_MODE, DEFINE_MODE, TEXT_MODE, UNCOMPRESS_MODE };
HDecl char hfn[80];
HDecl FILE *hf;
HDecl byte attr[MAX_DEPTH];
HDecl byte buff[MAX_BUFF_SIZE];
HDecl word curattr;
HDecl word rowcol[MAX_DEPTH];
HDecl word currowcol;
HDecl keyrec keys[MAX_KEYS];
HDecl word numkeys;
HDecl int mode;

/* prototypes... ************************************************************/

void init_values (void);           /* initializes all variables */
void draw_border (void);           /* draw border on screen */

#endif      /* if header file not already loaded */