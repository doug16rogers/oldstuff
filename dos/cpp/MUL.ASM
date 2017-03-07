	.386
	ifndef  ??version
?debug  macro
	endm
publicdll macro name
	public  name
	endm
$comm   macro   name,dist,size,count
	comm    dist name:BYTE:count*size
	endm
	else
$comm   macro   name,dist,size,count
	comm    dist name[size]:BYTE:count
	endm
	endif
	?debug  V 300h
	?debug  S "mul.cpp"
	?debug  C E986595418076D756C2E637070
	?debug  C E9A1595418076D756C2E687070
MUL_TEXT        segment byte public 'CODE'
MUL_TEXT        ends
DGROUP  group   _DATA,_BSS
	assume  cs:MUL_TEXT,ds:DGROUP
_DATA   segment word public 'DATA'
d@      label   byte
d@w     label   word
_DATA   ends
_BSS    segment word public 'BSS'
b@      label   byte
b@w     label   word
_BSS    ends
MUL_TEXT        segment byte public 'CODE'
   ;    
   ;    lword Mul32(lword a, lword b)
   ;    
	assume  cs:MUL_TEXT
@Mul32$qulul    proc    far
	push    bp
	mov     bp,sp
   ;    
   ;    {
   ;      return a*b;
   ;    
;       mov     cx,word ptr [bp+8]
;       mov     bx,word ptr [bp+6]
;       mov     dx,word ptr [bp+12]
;       mov     ax,word ptr [bp+10]
;       call    far ptr F_LXMUL@
;       jmp     short @1@58
	  mov eax, dword ptr [bp+6]
	  mul dword ptr [bp+10]
	  ror eax,16
	  mov dx,ax
	  ror eax,16
@1@58:
   ;
   ;    }
   ;
	pop     bp
	ret
@Mul32$qulul    endp
	?debug  C E9
	?debug  C FA00000000
MUL_TEXT        ends
_DATA   segment word public 'DATA'
s@      label   byte
_DATA   ends
MUL_TEXT        segment byte public 'CODE'
MUL_TEXT        ends
	extrn   F_LXMUL@:far
	public  @Mul32$qulul
_s@     equ     s@
	end
