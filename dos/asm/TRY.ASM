
PAGE  59,132

;==========================================================================
;==								         ==
;==			        KBF				         ==
;==								         ==
;==      Created:   20-May-92					         ==
;==      Passes:    5	       Analysis Options on: AU		         ==
;==								         ==
;==========================================================================

keybd_q_tail	equ	1Ch
psp_envirn_seg	equ	2Ch

seg_a		segment	byte public
		assume	cs:seg_a, ds:seg_a


		org	100h

kbf		proc	far

start:
		jmp	short loc_2
		nop
		nop
		nop
		nop
		nop
		nop

kbf		endp

;==========================================================================
;
;			External Entry Point
;
;==========================================================================

int_16h_entry	proc	far
		push	ax
		push	bx
		push	cx
		push	dx
		push	es
		push	ds
		push	si
		push	di
		pushf				; Push flags
		call	dword ptr cs:[104h]
		mov	ax,40h
		mov	es,ax
		mov	si,es:keybd_q_tail
		mov	ax,es:[si]
		cmp	al,0Dh
		jne	loc_1			; Jump if not equal
		mov	bx,0
		mov	ah,3
		int	16h			; ??INT Non-standard interrupt
loc_1:
		pop	di
		pop	si
		pop	es
		pop	ds
		pop	dx
		pop	cx
		pop	bx
		pop	ax
		iret				; Interrupt return
int_16h_entry	endp

loc_2:
		mov	es,ds:psp_envirn_seg
		mov	ah,49h			; 'I'
		int	21h			; DOS Services  ah=function 49h
						;  release memory block, es=seg
		mov	ax,3516h
		int	21h			; DOS Services  ah=function 35h
						;  get intrpt vector al in es:bx
		mov	word ptr ds:[106h],es
		mov	word ptr ds:[104h],bx
		mov	dx,offset int_16h_entry
		mov	ax,2516h
		int	21h			; DOS Services  ah=function 25h
						;  set intrpt vector al to ds:dx
		mov	dx,140h
		int	27h			; Terminate & stay resident
						;  dx=offset last byte+1, cs=PSP
		db	0EBh,0ADh,0CDh, 20h

seg_a		ends



		end	start
