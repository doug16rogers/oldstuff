;============================================================================
;  rowcol.asm 
;
;  Sets the cursor's row and column to the decimal values provided on the
;  commandline:
; 
;    rowcol <row> <column> <page>
;
;  Any unspecified values are assumed to be 0 (the first row/column/page).
;
;----------------------------------------------------------------------------
        .model tiny
        .code

        org 100h

start:  mov si,080h             ;point to start of commandline
        mov cl,ds:[si]          ;get count of chars on commandline
        inc si                  ;go to next character
        xor ch,ch               ;clear msb of count
        mov bx,10               ;use decimal
        call get_number         ;get row argument
        push ax                 ;save it
        call get_number         ;get column argument
        pop dx                  ;get row into dx
        mov dh,dl               ;move row into dh
        mov dl,al               ;move column into dl
        push dx                 ;save row/col info
        call get_number         ;get page number (default to zero)
        mov bh,al               ;get page number
        pop dx                  ;restore row/column
        mov ah,002h             ;indicate set-cursor-position function
        int 10h                 ;call video services interrupt

        int 20h                 ;return to DOS

;============================================================================
;  get_number 
;
;  Gets a number of the indicated base from a string.  Initial white-space
;  is automatically scanned.  
;
;  register usage:
;  in   ds:si   pointer to string from which to read the number in ASCII
;  in   cx      length of input string
;  in   bx      base to use (2-36)
;
;  out  ax      number value of the string
;  out  cx      number of characters not scanned
;  out  si      incremented for each character read
;  out  cf      carry flag set if no information was read.
;
;  registers disrupted:  ax, cx, dx, si, di, flags(c,d)
;
;----------------------------------------------------------------------------
proc    get_number near

find_digit: 
        xor ax,ax               ;initialize to zero
        jcxz bad_result         ;nothing to read!
        cld                     ;set the direction flag to forward
        
initial_loop:
        lodsb                   ;get a character into al
        dec cx                  ;decrement count of characters scanned

        cmp al,' '              ;see if a space
        je scan_next           
        cmp al,09h              ;see if a tab
        je scan_next
        call char_to_digit      ;convert to a digit
        jc unget_and_bad        ;unget character, but return as bad
        cmp al,bl               ;see if it's within our base specification
        jge unget_and_bad       ;oops! greater than (or equal to) base
        jmp load_loop           ;got one digit, now continue

scan_next:
        jcxz bad_result         ;well, nothing but white-space
        jmp initial_loop        ;check for more white-space
         
load_loop:
        mov di,ax               ;save current value
        lodsb                   ;get next character
        dec cx                 
        call char_to_digit      ;convert to digit
        jc restore_unget_and_good   ;unget char and quit
        push ax                 ;save digit
        mov ax,di               ;get old value
        mul bx                  ;multiply by base
        pop dx                  ;get digit
        xor dh,dh               ;make sure MSB is clear
        add ax,dx               ;add in digit
        jcxz good_result        ;see if at end of string
        jmp load_loop           ;go back for more

restore_unget_and_good:
        mov ax,di               ;get value of number from save register
        inc cx                  ;unget the character
        dec si

good_result:
        clc                     ;tell world data was ok
        retn                    ;return to sender, address unknown

unget_and_bad:
        inc cx                  ;unget the character
        dec si
bad_result:
        stc                     ;tell world data was bad
        retn                    ;return to sender, address unknown

endp    get_number


;============================================================================
;  char_to_digit 
;
;  Converts the character to a digit as follows:
;    '0'..'9' => 000h..009h, no carry
;    'A'..'Z' => 00ah..023h, no carry
;    'a'..'z' => 00ah..023h, no carry
;    others   => carry flag set
;
;  register usage:
;  in   al      character to convert
;
;  out  al      digit after conversion
;  out  cf      carry flag set if invalid
;
;  registers affected: al, flags
;
;----------------------------------------------------------------------------
proc    char_to_digit near

        cmp al,'0'              ;check '0'..'9'
        jl not_a_digit
        cmp al,'9'
        jg check_uppercase
        sub al,'0'
        jmp a_digit

check_uppercase:                ;check 'A'..'Z'
        cmp al,'A'
        jl not_a_digit
        cmp al,'Z'
        jg check_lowercase
        sub al,('A'-10)
        jmp a_digit

check_lowercase:                ;check 'a'..'z'
        cmp al,'a'
        jl not_a_digit
        cmp al,'z'
        jg check_lowercase
        sub al,('a'-10)

a_digit:
        clc                     ;tell world digit was ok
        retn                    ;return to sender

not_a_digit:
        stc                     ;tell world the digit was bad
        retn                    ;return to sender

endp    char_to_digit


end      start

