# define HOME		0
# define WORK		1

# include <dos.h>

/************************************************************************/
/*									*/
/* local interrupt handler for re-directed clock tick interrupt		*/
/*									*/
void interrupt newtimer(void)
{

extern unsigned myss;
extern unsigned stack;
extern unsigned timer_tick;
extern int tick_flag;
extern int stopflag;
extern int flagflag;
extern int count1;
extern int count2;
extern char switches;
extern void interrupt (*oldtimer)(void);
extern int flipflop;


int saveax;		/* a place to save the				*/
int savebx;		/*   registers that this			*/
int savecx;		/*     interrupt handler			*/
int savedx;		/*	 modifies				*/
unsigned intsp;		/* a place to save the stack pointer		*/
unsigned intss;		/*   used by the interrupted routine		*/

disable();		/* disable interrupts while messing with stack	*/
intsp = _SP;		/* save stack offset and segment from		*/
intss = _SS;		/*   the interrupted routine			*/
_SP = stack;		/* set the stack offset and segment to use	*/
_SS = myss;		/*   the local values so as to not blow		*/
			/*     somebody else's stack			*/
enable();		/* re-enable the interrupts			*/

	asm	mov	saveax,ax	/* save all			*/
	asm	mov	savebx,bx	/*   CPU register		*/
	asm	mov	savecx,cx	/*     needed by		*/
	asm	mov	savedx,dx	/*	 this routine		*/

	asm	mov	bx,0		/* init local counter to 0	*/
	asm	mov	dx,0201h	/* trigger the game		*/
	asm	out	dx,al		/*   port one-shot		*/
lab1:	asm	in	ax,dx		/* read game port		*/
	asm	inc	bx		/* increment local counter	*/

# if HOME
	asm	and	al,3		/* if neither of the one-shots	*/
	asm	cmp	al,3

# elif WORK
	asm	and	al,5
	asm	cmp	al,5
#endif

	asm	je	lab1		/*   are finished, loop again	*/
	asm	test	al,1		/* if it was pin 3 that fell,	*/
	asm	jz	lab2		/*   go on to lab2		*/
	asm	mov	count2,bx	/* else, save local counter as	*/
					/*   game port value for pin 6	*/
lab4:	asm	in	ax,dx		/* read game port		*/
	asm	inc	bx		/* increment local counter	*/
	asm	test	al,1		/* if the pin 3 one-shot is not	*/
	asm	jnz	lab4		/*   finished, loop again	*/
	asm	mov	count1,bx	/* save local counter as	*/
					/*   game port value		*/
	asm	jmp	lab5

lab2:	asm	mov	count1,bx	/* save local counter as game	*/
					/*   port value for pin 3	*/
lab3:	asm	in	ax,dx		/* read game port		*/
	asm	inc	bx		/* increment local counter	*/

# if HOME
	asm	test	al,2		/* if the pin 6 one-shot is not	*/

# elif WORK
	asm	test	al,4
#endif

	asm	jnz	lab3		/*   finished, loop again	*/
	asm	mov	count2,bx	/* save local counter as	*/
					/*   game port value		*/

lab5:	asm	mov	switches,al

	asm	mov	ax,saveax	/* restore			*/
	asm	mov	bx,savebx	/*   all			*/
	asm	mov	cx,savecx	/*     saved			*/
	asm	mov	dx,savedx	/*	 registers		*/

tick_flag = 1;
timer_tick++;

if (stopflag)		/* if the main routine has signaled a shutdown:	*/
{ disable();		/* disable interrupts while messing with	*/
			/*   the interrupt vector (and the stack)	*/
  setvect(0x1c,		/* restore the timer interrupt vector to 	*/
	  oldtimer);	/*   the standard DOS interupt handler		*/
  _SP = intsp;		/* restore the stack segment and offset		*/
  _SS = intss;		/*   from the interrupted routine		*/
  enable();		/* re-enable the interrupts			*/
  flagflag = 1;		/* show that interrupt handler has shut down	*/
}			/* end if					*/

else			/* otherwise: just restore stack & exit		*/
{ disable();		/* disable interrupts while messing with stack	*/
  _SP = intsp;		/* restore the stack segment and offset		*/
  _SS = intss;		/*   from the interrupted routine		*/
  enable();		/* re-enable the interrupts			*/
  flipflop ^= 1;	/* flip flop between 0 and 1			*/
  if (flipflop)		/* every other time throught this interrupt	*/
    (*oldtimer)();	/*   handler, go off and do the DOS timer	*/
			/*     interrupt so DOS won't get too confused	*/
}			/* end else					*/

}			/* end interrupt handler			*/
