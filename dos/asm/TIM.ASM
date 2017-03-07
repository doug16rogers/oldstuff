
PAGE  59,132

;==========================================================================
;==					                                 ==
;==				TIM	                                 ==
;==					                                 ==
;==      Created:   8-Nov-90		                                 ==
;==      Passes:    5          Analysis	Options on: U                    ==
;==      (c) dr 1990			                                 ==
;==					                                 ==
;==========================================================================


seg_a		segment	byte public
		assume	cs:seg_a, ds:seg_a


		org	100h

tim		proc	far

start::
		jmp	loc_1
		db	0Dh, '   ', 0Dh, 'tim -- display '
		db	'date/time -- '
copyright	db	'(c) dr 1990'
		db	 0Dh, 0Ah, 1Ah
data_2		dw	0			; Data table (indexed access)
		db	 1Fh, 00h, 3Bh, 00h, 5Ah, 00h
		db	 78h, 00h, 97h, 00h,0B5h, 00h
		db	0D4h, 00h,0F3h, 00h, 11h, 01h
		db	 30h, 01h, 4Eh, 01h, 6Dh, 01h
		db	 53h, 75h
		db	'n$'
		db	'Mon$'
		db	'Tue$'
		db	'Wed$'
		db	'Thu$'
		db	'Fri$'
		db	'Sat$'
data_4		db	'Jan$'
		db	'Feb$'
		db	'Mar$'
		db	'Apr$'
		db	'May$'
		db	'Jun$'
		db	'Jul$'
		db	'Aug$'
		db	'Sep$'
		db	'Oct$'
		db	'Nov$'
		db	'Dec$'
data_5		db	0
data_6		db	0
data_7		dw	0
data_8		dw	0
loc_1::
		mov	ah,4
		int	1Ah			; Real time clock   ah=func 04h
						;  get date  cx=year, dx=mon/day
		push	cx
		mov	data_5,cl
		mov	data_6,dl
		mov	cl,4
		mov	al,ch
		shr	al,cl			; Shift w/zeros fill
		add	al,30h			; '0'
		push	ax
		push	dx
		mov	dl,al
		mov	ah,2
		int	21h			; DOS Services  ah=function 02h
						;  display char dl
		pop	dx
		pop	ax
		mov	al,ch
		and	al,0Fh
		add	al,30h			; '0'
		push	ax
		push	dx
		mov	dl,al
		mov	ah,2
		int	21h			; DOS Services  ah=function 02h
						;  display char dl
		pop	dx
		pop	ax
		pop	ax
		mov	ah,al
		shr	al,cl			; Shift w/zeros fill
		add	al,30h			; '0'
		push	ax
		push	dx
		mov	dl,al
		mov	ah,2
		int	21h			; DOS Services  ah=function 02h
						;  display char dl
		pop	dx
		pop	ax
		mov	al,ah
		and	al,0Fh
		add	al,30h			; '0'
		push	ax
		push	dx
		mov	dl,al
		mov	ah,2
		int	21h			; DOS Services  ah=function 02h
						;  display char dl
		pop	dx
		pop	ax
		push	ax
		push	dx
		mov	dl,2Eh			; '.'
		mov	ah,2
		int	21h			; DOS Services  ah=function 02h
						;  display char dl
		pop	dx
		pop	ax
		push	dx
		cmp	dh,10h
		jl	loc_2			; Jump if <
		sub	dh,6
loc_2::
		dec	dh
		mov	bl,dh
		xor	bh,bh			; Zero register
		mov	data_8,bx
		shl	bx,1			; Shift w/zeros fill
		mov	data_7,bx
		shl	bx,1			; Shift w/zeros fill
		mov	dx,offset data_4	; ('Jan')
		add	dx,bx
		mov	ah,9
		int	21h			; DOS Services  ah=function 09h
						;  display char string at ds:dx
		push	ax
		push	dx
		mov	dl,2Eh			; '.'
		mov	ah,2
		int	21h			; DOS Services  ah=function 02h
						;  display char dl
		pop	dx
		pop	ax
		pop	ax
		mov	ah,al
		shr	al,cl			; Shift w/zeros fill
		add	al,30h			; '0'
		push	ax
		push	dx
		mov	dl,al
		mov	ah,2
		int	21h			; DOS Services  ah=function 02h
						;  display char dl
		pop	dx
		pop	ax
		mov	al,ah
		and	al,0Fh
		add	al,30h			; '0'
		push	ax
		push	dx
		mov	dl,al
		mov	ah,2
		int	21h			; DOS Services  ah=function 02h
						;  display char dl
		pop	dx
		pop	ax
		push	ax
		push	dx
		mov	dl,20h			; ' '
		mov	ah,2
		int	21h			; DOS Services  ah=function 02h
						;  display char dl
		pop	dx
		pop	ax
		mov	cl,4
		mov	al,data_6
		xor	ah,ah			; Zero register
		shl	ax,cl			; Shift w/zeros fill
		shr	al,cl			; Shift w/zeros fill
		aad				; Ascii adjust
		mov	bx,data_7
		mov	dx,data_2[bx]
		add	dx,ax
		mov	al,data_5
		shl	ax,cl			; Shift w/zeros fill
		shr	al,cl			; Shift w/zeros fill
		aad				; Ascii adjust
		test	al,3
		jnz	loc_3			; Jump if not zero
		cmp	data_8,2
		jl	loc_3			; Jump if <
		inc	dx
loc_3::
		mov	ax,dx
		xor	dx,dx			; Zero register
		mov	bx,0Ah
		div	bx			; ax,dx rem=dx:ax/reg
		push	dx
		xor	dx,dx			; Zero register
		div	bx			; ax,dx rem=dx:ax/reg
		add	al,30h			; '0'
		push	ax
		push	dx
		mov	dl,al
		mov	ah,2
		int	21h			; DOS Services  ah=function 02h
						;  display char dl
		pop	dx
		pop	ax
		mov	ax,dx
		add	al,30h			; '0'
		push	ax
		push	dx
		mov	dl,al
		mov	ah,2
		int	21h			; DOS Services  ah=function 02h
						;  display char dl
		pop	dx
		pop	ax
		pop	ax
		add	al,30h			; '0'
		push	ax
		push	dx
		mov	dl,al
		mov	ah,2
		int	21h			; DOS Services  ah=function 02h
						;  display char dl
		pop	dx
		pop	ax
		push	ax
		push	dx
		mov	dl,20h			; ' '
		mov	ah,2
		int	21h			; DOS Services  ah=function 02h
						;  display char dl
		pop	dx
		pop	ax
		mov	ah,2
		int	1Ah			; Real time clock   ah=func 02h
						;  read clock cx=hrs/min, dh=sec
						;   dl=0 standrd,=1 daylight sav
		push	cx
		mov	cl,4
		mov	al,ch
		shr	al,cl			; Shift w/zeros fill
		add	al,30h			; '0'
		push	ax
		push	dx
		mov	dl,al
		mov	ah,2
		int	21h			; DOS Services  ah=function 02h
						;  display char dl
		pop	dx
		pop	ax
		mov	al,ch
		and	al,0Fh
		add	al,30h			; '0'
		push	ax
		push	dx
		mov	dl,al
		mov	ah,2
		int	21h			; DOS Services  ah=function 02h
						;  display char dl
		pop	dx
		pop	ax
		push	ax
		push	dx
		mov	dl,3Ah			; ':'
		mov	ah,2
		int	21h			; DOS Services  ah=function 02h
						;  display char dl
		pop	dx
		pop	ax
		pop	ax
		mov	ah,al
		shr	al,cl			; Shift w/zeros fill
		add	al,30h			; '0'
		push	ax
		push	dx
		mov	dl,al
		mov	ah,2
		int	21h			; DOS Services  ah=function 02h
						;  display char dl
		pop	dx
		pop	ax
		mov	al,ah
		and	al,0Fh
		add	al,30h			; '0'
		push	ax
		push	dx
		mov	dl,al
		mov	ah,2
		int	21h			; DOS Services  ah=function 02h
						;  display char dl
		pop	dx
		pop	ax
		push	ax
		push	dx
		mov	dl,3Ah			; ':'
		mov	ah,2
		int	21h			; DOS Services  ah=function 02h
						;  display char dl
		pop	dx
		pop	ax
		mov	al,dh
		shr	al,cl			; Shift w/zeros fill
		add	al,30h			; '0'
		push	ax
		push	dx
		mov	dl,al
		mov	ah,2
		int	21h			; DOS Services  ah=function 02h
						;  display char dl
		pop	dx
		pop	ax
		mov	al,dh
		and	al,0Fh
		add	al,30h			; '0'
		push	ax
		push	dx
		mov	dl,al
		mov	ah,2
		int	21h			; DOS Services  ah=function 02h
						;  display char dl
		pop	dx
		pop	ax
		int	20h			;  ?? undocumented sub-function

tim		endp

seg_a		ends



		end	start
