;============================================================================
;  kbfast.asm 
;
;  Sets the typematic rate on the keyboard to maximum whenever Ctrl-Enter
;  is pressed.  This is a TSR.  It doesn't yet unload itself.
;
;----------------------------------------------------------------------------
        .286
        .model tiny
        startupcode
        .code
        
        assume cs:DGROUP,ds:DGROUP,es:DGROUP

KB_INT          equ 009h
KB_SERVICES_INT equ 016h
DOS_INT         equ 021h
TSR_INT         equ 027h

KB_SHIFT        equ 00017h      ;offset of shift status byte in seg 0040
KB_TAIL         equ 0001ch      ;offset (in seg 0040) of keyboard buffer tail

CR              equ 00dh

;============================================================================
;  start
;----------------------------------------------------------------------------

        org 100h

start:  jmp load_start          ;skip past data and resident portion of code


;============================================================================
;  data
;----------------------------------------------------------------------------

        codeseg
debug_char      db '0'          ;for debug -- appears on second row of screen
old_kb_isr      dd 0            ;old keyboard interrupt service routine (ISR)


;============================================================================
;  kb_fast_int
;
;  Sets the typematic rate on the keyboard to maximum whenever <Ctrl-Enter>
;  is pressed.  This is a TSR.  It doesn't yet unload itself.
;
;----------------------------------------------------------------------------
kb_fast_int:

        pusha

        mov ax,cs               ;use our own data segment
        mov ds,ax

        mov ah,01fh
        mov al,debug_char
        inc al
        mov debug_char,al

        mov dx,0b800h
        mov es,dx
        mov es:[160],ax

        popa

        iret

        pusha

        mov ax,0b800h
        mov ds,ax
        mov ax,01f00h + '#'
        mov ds:[160],ax

        mov ax,cs               ;use our own data segment
        mov ds,ax

        pushf                           ;iret will pop flags in old ISR
        call dword ptr old_kb_isr       ;call the old ISR

        popa
        iret

        mov ax,00040h           ;low DOS memory locations
        mov es,ax
        mov si,es:[KB_TAIL]     ;get tail pointer
        mov ax,es:[si]          ;get scan code at tail pointer
        cmp al,00ah             ;see if it's a <Ctrl-Enter> = LF
        jne nothing_to_do       ;skip if it's not

        mov cs:debug_char,'*'      ;have debug section display a star

        mov bh,0                ;minimum delay of 250 ms
        mov bl,0                ;maximum typematic rate of 30 keys per second
        mov ah,003h             ;set typematic rate service
        int KB_SERVICES_INT     ;call keyboard services interrupt

nothing_to_do:
debug:
        mov ax,0b800h           ;screen segment (color, text)
        mov es,ax
        mov al,cs:debug_char       ;current debug character
        mov ah,01fh             ;white on blue
        mov es:[160],ax         ;write to first char in second line
        inc al                  ;go to next char
        cmp al,'9'              ;see if need to wrap
        mov cs:debug_char,al       ;dump it out before test
        jle end_isr             ;if still ok to show, skip to end
        mov cs:debug_char,'0'      ;reset to 0

end_isr:
        popa                    ;restore registers

        iret

;endp    kb_fast_int


;============================================================================
;  transient portion of code -- loads interrupt service routine (ISR).
;----------------------------------------------------------------------------

load_start:
        mov al,KB_INT           ;ask DOS for KB interrupt vector
        mov ah,035h             ;get interrupt vector
        int DOS_INT             ;call DOS services interrupt

        mov word ptr old_kb_isr+2,es    ;store old interrupt vector, segment
        mov word ptr old_kb_isr,bx      ;offset

        mov ax,SEG(kb_fast_int)         ;load pointer to our vector
        mov ds,ax
        mov dx,OFFSET(kb_fast_int)      ;...

        mov al,KB_INT           ;tell'm we're replacing the KB interrupt
        mov ah,025h             ;set interrupt vector service
        int DOS_INT             ;call DOS services interrupt

        mov dx,OFFSET(load_start)       ;get end of resident part
        add dx,00100h                   ;add in a safety margin
        and dx,0fffeh                   ;make even
        mov sp,dx                       ;set the stack pointer
        int TSR_INT                     ;terminate-and-stay-resident

end      start

