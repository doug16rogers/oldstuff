;*****************************************************************************
;**                                                                         **
;**                 #          #####      #####      #####                  **
;**                 #            #        #          #                      **
;**                 #            #        ###        ###                    **
;**                 #            #        #          #                      **
;**                 #####      #####      #          #####                  **
;**                                                                         **
;*****************************************************************************
;
;  This program runs LIFE on whatever happens to be on the screen.
;  The following control keys are available:
;
;     ESC        quits
;     F2         toggles random mode (random living cells appear)
;     F3         increments delay time by 1/18th of a second ==> slower
;     F4         decrements delay time by 1/18th of a second ==> faster
;     F5         decrements threshhold for random mode (decreases probability)
;     F6         increments threshhold for random mode (increases probability)
;     F8         clears entire screen
;     F9         pauses (toggle)
;     F10        single-steps a generation and turns on pause
;     Arrow keys move the cursor
;     All other keys are inserted at the current cursor position
;
;*****************************************************************************

.MODEL     TINY

.CODE
           ORG     0100h

WRD        EQU     WORD PTR            ;shorthand
BYT        EQU     BYTE PTR            ;ditto

_ESC       EQU     01Bh                ;ESC key
_F1        EQU     0BBh                ;F1 key, second scan code OR 80h
_F2        EQU     0BCh                ;F2 key
_F3        EQU     0BDh                ;F3 key
_F4        EQU     0BEh                ;F4 key
_F5        EQU     0BFh                ;F5 key
_F6        EQU     0C0h                ;F6 key
_F7        EQU     0C1h                ;F7 key
_F8        EQU     0C2h                ;F8 key
_F9        EQU     0C3h                ;F9 key
_F10       EQU     0C4h                ;F10 key
_HOME      EQU     0C7h                ;Home key
_UPAR      EQU     0C8h                ;Up arrow key
_PGUP      EQU     0C9h                ;Page up key
_LTAR      EQU     0CBh                ;Left arrow key
_RTAR      EQU     0CDh                ;Right arrow key
_END       EQU     0CFh                ;End key
_DNAR      EQU     0D0h                ;Down arrow key
_PGDN      EQU     0D1h                ;Page down key
_INS       EQU     0D2h                ;Insert key
_DEL       EQU     0D3h                ;Delete key


TICKER     EQU     0046Ch              ;system timer location
COUNTINC   EQU     00001h              ;amount to inc/dec delay counter
RANDINC    EQU     00001h              ;amount to raise/lower randthresh
PAUSED     EQU     08000h              ;pause flag
RANDOM     EQU     04000h              ;random flag
MOUSINT    EQU     033h                ;mouse interrupt
MOUSVECT   EQU     (MOUSINT*4)         ;mouse interrupt vector table position


DO_NEIGHBOR MACRO                      ;macro to increment neighbor count, etc
           stosw                       ;save this char to take over
           inc     BYTE PTR [es:di]    ;increment neighbor count
           ENDM

GET_CURSOR MACRO                       ;calls video services interrupt
           xor     bh,bh               ;page 0
           mov     ah,03h              ;specify read-cursor-position function
           int     10h                 ;call video services interrupt
           ENDM

start:     jmp     realstart           ;skip data and go to real code

lifemsg:   db      13,'    ',13,10
           db      ' LIFE screen contents, dr.o. (c) 1989'             ,13,10
           db      '   ESC - quit '                                    ,13,10
           db      '   F2  - toggle random '                           ,13,10
           db      '   F3  - slow down '                               ,13,10
           db      '   F4  - speed up '                                ,13,10
           db      '   F5  - decrease probability '                    ,13,10
           db      '   F6  - increase probability '                    ,13,10
           db      '   F8  - clears screen '                           ,13,10
           db      '   F9  - toggle pauses '                           ,13,10
           db      '   F10 - single-step '                             ,13,10
           db      ' Arrow keys move the cursor '                      ,13,10
           db      ' All other keys are inserted at cursor position '  ,13,10
           db      26      ;EOF char

;
;  The following table is used for looking up whether a cell is to live in the
;  next generation.  It is 8 times larger than necessary for future add-/ed-
;  itions.
;
           db      'Life lookup table; '
           db      'Upper nybble, 0=dead, 1=alive; '
           db      'Lower nybble holds number of neighbors, inclusive; '
           db      ' jen la tablo:'
;                  0 1 2 3  4 5 6 7  8 9 A B  C D E F  ;xX
lifelookup db      0,0,0,1, 0,0,0,0, 0,0,0,0, 0,0,0,0  ;0x
           db      0,0,0,1, 1,0,0,0, 0,0,0,0, 0,0,0,0  ;1x
           db      0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0  ;2x
           db      0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0  ;3x
           db      0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0  ;4x
           db      0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0  ;5x
           db      0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0  ;6x
           db      0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0  ;7x
           db      0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0  ;8x
           db      0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0  ;9x
           db      0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0  ;Ax
           db      0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0  ;Bx
           db      0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0  ;Cx
           db      0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0  ;Dx
           db      0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0  ;Ex
           db      0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0  ;Fx
vidseg     dw      0B800h              ;start with color video memory
mouse      dw      0                   ;default is no mouse
curs_pos   dw      1 DUP(?)            ;initial cursor position
curs_siz   dw      1 DUP(?)            ;initial cursor size
rows       db      1 DUP (?)           ;number of rows on screen
cols       db      1 DUP (?)           ;number of columns on screen
rowsxcols  dw      1 DUP (?)           ;product of rows and cols
randthresh dw      00020h              ;random threshhold is 80 dec
r250       dw      250 DUP (?)         ;r250 algorithm table
r250indx   dw      498-206             ;r250 p-pointer, in bytes (going down)
r250prev   dw      498                 ;r250 q-pointer, in bytes
_rc        dw      1 DUP (?)           ;rows*cols*2 address offset
_c         dw      1 DUP (?)           ;cols*2
_2c        dw      1 DUP (?)           ;2*cols*2
lflags     dw      00000h              ;initially all lflags disabled
counter    dw      00000h              ;counter for end of each generation
screenbak  dw      50 DUP (80 DUP (0720H))   ;backup copy of initial screen
lifetab    db      50 DUP (80 DUP (20H,07H,000H))   ;table for life prop


;============================================================================
rnd        PROC    NEAR                ;gets next r250 value
           mov     bx,r250prev         ;get location of q-pointer
           mov     ax,[bx+OFFSET r250] ;get value
           sub     bx,2                ;go to next entry
           jge     gotprev             ;if not, skip
           mov     bx,498              ;otherwise, q-pointer to start
gotprev:   mov     r250prev,bx         ;save q-pointer
           mov     bx,r250indx         ;get previous pointer (q-pointer)
           xor     ax,[bx+OFFSET r250] ;calc next random value
           mov     [bx+OFFSET r250],ax ;save new random number
           sub     bx,2                ;go to next entry
           jge     gotindx             ;if not, skip
           mov     bx,498              ;otherwise, p-pointer to start
gotindx:   mov     r250indx,bx         ;save index
           ret                         ;return to sender, address unknown
rnd        ENDP


;============================================================================
rnd_init   PROC    NEAR                ;initialize r250 values

LC_CONST   EQU     02F41h              ;add-in constant for linear congruent.
LC_MULTI   EQU     0A00Dh              ;multiplier for linear congruential

           xor     ax,ax               ;make es point
           mov     es,ax               ;  to low memory
           cli                         ;clear interrupts for reading timer
           mov     ax,es:TICKER        ;get system timer for random seed
           sti                         ;restore interrupts
           mov     r250[0],ax          ;store ticker value

           mov     bx,2                ;starting location
           mov     cx,249              ;number of times to go

;initialize the array...
           mov     si,LC_MULTI         ;load multiplier
rinitlup:  mul     si                  ;multiply, ignore dx result
           add     ax,LC_CONST         ;add in constant
           mov     [bx+OFFSET r250],ax ;store value
           add     bx,2                ;go to next
           loop    rinitlup            ;do for each guy

;force linear independence...
           mov     bx,498              ;start back at beginning
           mov     ax,0ffffh           ;AND flag
           mov     dx,1h               ;OR flag
           mov     cx,16               ;we have 16 bits

indloop:   and     [bx+OFFSET r250],ax ;leave off lower bits
           shl     ax,1                ;ready for next bit/word
           or      [bx+OFFSET r250],dx ;force current lowest to 1
           shl     dx,1                ;ready for next bit/word
           sub     bx,2                ;go on to next one
           loop    indloop             ;looperoo

           mov     cx,17               ;load it up with number of bits
rndloop:   call    rnd                 ;call r250 to get past false-start
           loop    rndloop             ;go 'til done

           ret                         ;return to sender, address unknwon
rnd_init   ENDP



;============================================================================
realstart: cld                         ;make sure we're going forward
           call    rnd_init            ;initialize the r250 generator
           xor     ax,ax               ;clear ax
           mov     es,ax               ;set extra segment to 0
           mov     ah,0Fh              ;specify get-video-mode
           int     10h                 ;call video services interrupt
           mov     cols,ah             ;set number of columns
           cmp     al,07h              ;see if monochrome mode
           jne     getcolor            ;skip if not monochrome
           mov     rows,25             ;specify 25 rows
           mov     vidseg,0B000H       ;set to monochrome video memory
           jmp     initstuff           ;skip complex video card stuff

getcolor:  mov     ax,1130h            ;specify get-extended-video-mode
           mov     bh,03H              ;give it a bogus task, 8x8 font pointer
           int     10h                 ;call video services interrupt
           inc     dl                  ;make last row into number of rows
           mov     rows,dl             ;store number of rows
           mov     ax,0040h            ;load in low memory location segment
           mov     es,ax               ;put into extra segment
           cli                         ;turn off interrupts during read
           mov     bx,es:[004eh]       ;get offset into video page
           sti                         ;restore interrupts
           mov     cl,4                ;get ready for shift
           shr     bx,cl               ;shift it (hopefully on paragraph)
           and     bx,0fffh            ;keep only lower
           add     vidseg,bx           ;add page in to video segment

initstuff: mov     al,rows             ;get number of rows into accumulator
           xor     ah,ah               ;clear top bits
           mul     BYT cols            ;multiply by columns
           mov     rowsxcols,ax        ;store product in memory

           mov     _rc,ax              ;put in 1x
           shl     ax,1                ;shift up for address offset
           add     _rc,ax              ;add in 2x=3x for later

           xor     ah,ah               ;clear upper byte
           mov     al,cols             ;put columns in al
           mov     _c,ax               ;put in 1x
           shl     ax,1                ;column address offset
           add     _c,ax               ;add in 2x=3x, store for later

           mov     ax,_c               ;get one row offset
           shl     ax,1                ;2 rows back
           mov     _2c,ax              ;store 2*columns offset

           xor     bh,bh               ;set page 0
           mov     ah,03h              ;specify read-cursor-position
           int     10h                 ;call video services interrupt
           mov     curs_pos,dx         ;store cursor position for end
           mov     curs_siz,cx         ;store cursor size for end

           push    ds                  ;save data segment
           mov     ax,vidseg           ;transfer video segment into
           mov     ds,ax               ;  data segment register
           xor     si,si               ;start at beginning of screen memory
           mov     ax,cs               ;transfer current segment into
           mov     es,ax               ;  extra segment
           mov     di,OFFSET screenbak ;set up pointer to screen backup mem
           mov     cx,50*80            ;set up number of chars on screen
           repz    movsw               ;transfer everything to backup memory

           xor     si,si               ;clear out to do it again
           xor     di,di               ;also clear this pointer
           mov     ax,ds               ;data segment also as screen ptr,
           mov     es,ax               ;  destination pointer
           mov     cx,50*80            ;rows X columns
blanks:    lodsw                       ;get char and attribute
;          and     ah,070h             ;turn off blink
;          mov     bl,ah               ;copy into bl
;          and     bl,00Fh             ;bl holds foreground
;          mov     bh,ah               ;copy into bh
;          shr     bh,1                ;1 shift
;          shr     bh,1                ;2 shift
;          shr     bh,1                ;3 shift
;          shr     bh,1                ;4 shift, bh holds background
;check character..
           cmp     al,' '              ;see if it's a blank char
           je      writeblank          ;if so, load a blank
           cmp     al,0FFh             ;see if it's a blank-looking char
           je      writeblank          ;if so, load a blank
           cmp     al,000h             ;see if it's a zero
           je      writeblank          ;if so, it's a blank, too
           cmp     al,0DBh             ;see if it's a filled-in char
           jne     charok              ;if not, go on
           test    ah,00Fh             ;see if blank foreground
           jne     charok              ;if not, ok
writeblank:mov     ah,007h             ;if so, force to normal attr, 0720h
           mov     al,' '              ;put blank in al
;
;check attribute..
charok:    stosw                       ;put back in
           loop    blanks              ;loop back
           pop     ds                  ;restore data segment

;do mouse setup...
           cli                         ;disable interrupts
           mov     ax,es:[MOUSVECT]    ;get LSW of mouse interrupt vector
           mov     bx,es:[MOUSVECT+2]  ;get MSW of mouse interrupt vector
           sti                         ;turn interrupts back on
           cmp     ax,0                ;see if points to null
           jne     oktolook            ;if non-zero, it's ok to check
           cmp     bx,0                ;now how 'bout MSW
           jne     oktolook            ;ok to check mouse
           jmp     donemouse           ;done with mouse check

oktolook:  mov     ax,0                ;0 function -- check mouse available
           int     MOUSINT             ;call mouse interrupt
           cmp     ax,0                ;see if it was changed
           jz      donemouse           ;skip if no mouse
           mov     ax,1                ;set flag
           mov     mouse,ax            ;set mouse flag -- mouse is there

           mov     ax,0ah              ;a function -- set cursor type
           mov     bx,0ffffh           ;use hardware cursor
           mov     cx,0                ;start scan line
           mov     dx,0ch              ;end scan line
           int     MOUSINT             ;call mouse interrupt

           mov     ax,1                ;show mouse function
           int     MOUSINT             ;call mouse interrupt
donemouse:


;*****************************************************************************
;
;  Main Life Loop...
;
lifeloop:
;check keyboard first...
           mov     ax,cs               ;explicitly load lflags segment
           mov     ds,ax               ; and put into ds
           mov     ah,0Bh              ;specify check-keyboard function
           int     21h                 ;call DOS services interrupt
           cmp     al,0                ;see if character is available
           jz      chkmouse            ;if not, go on to check mouse
           jmp     do_key              ;if so, go process the keystroke
chkmouse:  cmp     mouse,0             ;see if mouse available
           jz      do_life             ;if not, go on to next life generation
           mov     ax,5                ;5 check on a button press
           mov     bx,0                ;left button
           int     MOUSINT             ;call mouse interrupt
           cmp     bx,0                ;see if any presses
           jz      chkrightb           ;if not, check the right button
           mov     al,'*'              ;put a star with the left button
           push    ax                  ;store it
           shr     dx,1                ;divide row by 2
           shr     dx,1                ;divide row by 2, 4
           shr     dx,1                ;divide row by 2, 8
           shr     cx,1                ;divide column by 2
           shr     cx,1                ;divide column by 2, 4
           shr     cx,1                ;divide column by 2, 8
           mov     dh,dl               ;put row in dh
           mov     dl,cl               ;put column in cl
           xor     bx,bx               ;clear page
           mov     ah,2                ;set-cursor-position function
           int     10h                 ;call video interrupt
           pop     ax                  ;restore character
           int     29h                 ;send it out

chkrightb: mov     ax,5                ;5 check on a button press
           mov     bx,1                ;right button
           int     MOUSINT             ;call mouse interrupt
           cmp     bx,0                ;see if any presses
           jz      do_life             ;if not, do next life generation
           mov     al,' '              ;clear this spot
           push    ax                  ;store it
           shr     dx,1                ;divide row by 2
           shr     dx,1                ;divide row by 2, 4
           shr     dx,1                ;divide row by 2, 8
           shr     cx,1                ;divide column by 2
           shr     cx,1                ;divide column by 2, 4
           shr     cx,1                ;divide column by 2, 8
           mov     dh,dl               ;put row in dh
           mov     dl,cl               ;put column in cl
           xor     bx,bx               ;clear page
           mov     ah,2                ;set-cursor-position function
           int     10h                 ;call video interrupt
           pop     ax                  ;restore character
           int     29h                 ;send it out

do_life:   test    lflags,PAUSED       ;see if paused
           jnz     lifeloop            ;if so, just get keyboard info

dolifloop: push    ds                  ;save ds
           mov     ax,ds
           mov     es,ax               ;have es point to this segment
           mov     di,OFFSET lifetab   ;get life table offset
           mov     cx,rowsxcols        ;get number of chars on screen
           xor     ax,ax               ;clear ax to
clearlin:  stosw                       ;  clear the entire table, char:attr
           stosb                       ;  life bits
           loop    clearlin            ;clear it

           xor     si,si               ;clear out source (screen) pointer
           mov     ax,vidseg           ;have ds point to video memory
           mov     ds,ax               ;  while es still points to this data

           mov     dx,OFFSET lifetab   ;use dx to hold current position
           sub     dx,3                ;actually, use the one to the left
;
;****************************************************************************
;section 0 (upper left corner character)
sect_0:    lodsw                       ;get character:attribute in ax
           cmp     ax,0720h            ;see if it's a blank
           je      sect_1              ;if so, skip to section 1
set0:      mov     di,dx               ;set address to own place
           add     di,es:_rc           ;point to end of array
           DO_NEIGHBOR                 ;== above left
           inc     di                  ;just beyond edge
           sub     di,es:_c            ;subtract a single row
           DO_NEIGHBOR                 ;== above
           inc     di                  ;point to next locale
           DO_NEIGHBOR                 ;== above right
           mov     di,dx               ;reset pointer
           add     di,es:_c            ;add in a single row
           DO_NEIGHBOR                 ;== left
           inc     di                  ;go to next
           DO_NEIGHBOR                 ;== below
           inc     di                  ;go to next
           DO_NEIGHBOR                 ;== below right
           mov     di,dx               ;reset pointer
           add     di,3                ;off by one
           DO_NEIGHBOR                 ;== self
           add     BYT [es:di],10h     ;indicate that it's alive
           inc     di                  ;go to next
           DO_NEIGHBOR                 ;== right
           mov     di,dx               ;reset pointer
           add     di,es:_2c           ;
           DO_NEIGHBOR                 ;== below left
;
;****************************************************************************
sect_1:    mov     cl,es:cols          ;get number of columns
           xor     ch,ch               ;  in cx
           sub     cx,2                ;skip first and last char in line

loop1:     add     dx,3                ;point to next location in lifetab
           lodsw                       ;get character in al with attr in ah
           cmp     ax,0720h            ;see if it's a blank
           je      endsect1            ;if so, go to next
set1:      mov     di,dx               ;reset pointer
           add     di,es:_rc           ;go past end
           sub     di,es:_c            ;back to last row
           DO_NEIGHBOR                 ;== above left
           inc     di                  ;go to next
           DO_NEIGHBOR                 ;== above
           inc     di                  ;go to next
           DO_NEIGHBOR                 ;== above right
           mov     di,dx               ;reset pointer to left
           DO_NEIGHBOR                 ;== left
           inc     di                  ;go to next
           DO_NEIGHBOR                 ;== self
           add     BYT [es:di],10h     ;indicate that it's alive
           inc     di                  ;go to next
           DO_NEIGHBOR                 ;== right
           mov     di,dx               ;reset pointer
           add     di,es:_c            ;point to next row
           DO_NEIGHBOR                 ;== below left
           inc     di                  ;go to next
           DO_NEIGHBOR                 ;== below
           inc     di                  ;go to next
           DO_NEIGHBOR                 ;== below right
endsect1:  loop    loop1               ;do this cx=(rows-2) times
;
;****************************************************************************
sect_2:    add     dx,3                ;put most recent position in dx
           lodsw                       ;get char and attr
           cmp     ax,0720h            ;see if it's a space
           je      sect_3              ;if so, skip down
           mov     di,dx               ;put pointer into di
           DO_NEIGHBOR                 ;== left
           inc     di                  ;point to next
           DO_NEIGHBOR                 ;== self
           add     BYT [es:di],10h     ;indicate that it's alive
           inc     di                  ;go to next
           DO_NEIGHBOR                 ;== below right
           mov     di,dx               ;reset pointer
           add     di,es:_c            ;point to next row
           DO_NEIGHBOR                 ;== below left
           inc     di                  ;go to next
           DO_NEIGHBOR                 ;== below
           mov     di,OFFSET lifetab   ;go to first entry
           DO_NEIGHBOR                 ;== right
           mov     di,OFFSET lifetab   ;back to start of table
           add     di,es:_rc           ;point past end of table
           sub     di,6                ;then back two cells
           DO_NEIGHBOR                 ;== above left
           inc     di                  ;go to next
           DO_NEIGHBOR                 ;== above
           inc     di                  ;point past again
           sub     di,es:_c            ;then back a row
           DO_NEIGHBOR                 ;== above right
;
;****************************************************************************
sect_3:    mov     bl,es:rows          ;get rows
           xor     bh,bh               ;clear upper byte
           sub     bx,2                ;rows-2
middlerows:add     dx,3                ;go to next char
           lodsw                       ;load char and attr
           cmp     ax,0720h            ;see if it's empty, dead
           je      sect_4              ;if so, skip
           mov     di,dx               ;load pointer
           DO_NEIGHBOR                 ;== above left
           inc     di                  ;go to next
           DO_NEIGHBOR                 ;== self
           add     BYT [es:di],10h     ;indicate that it's alive
           inc     di                  ;go to next
           DO_NEIGHBOR                 ;== right
           mov     di,dx               ;reset pointer
           sub     di,es:_c            ;go back one row
           add     di,3                ;and over a cell
           DO_NEIGHBOR                 ;== above
           inc     di                  ;go to next
           DO_NEIGHBOR                 ;== above right
           mov     di,dx               ;reset pointer
           add     di,es:_c            ;go down a row
           DO_NEIGHBOR                 ;== left
           inc     di                  ;go to next
           DO_NEIGHBOR                 ;== below
           inc     di                  ;go to next
           DO_NEIGHBOR                 ;== below right
           mov     di,dx               ;reset pointer
           add     di,es:_2c           ;down yet another row
           DO_NEIGHBOR                 ;== below left
;
;****************************************************************************
sect_4:    mov     cl,es:cols          ;put number of columns per row in count
           xor     ch,ch               ;clear upper byte
           sub     cx,2                ;subtract endpoints

loop4:     add     dx,3                ;point to next place in table
           lodsw                       ;get char and attr
           cmp     ax,0720h            ;see if char is a space
           je      endsect4            ;if so, skip to loop's end
           mov     di,dx               ;set up pointer
           DO_NEIGHBOR                 ;== left
           inc     di                  ;go to next
           DO_NEIGHBOR                 ;== self
           add     BYT [es:di],10h     ;indicate that it's alive
           inc     di                  ;go to next
           DO_NEIGHBOR                 ;== right
           mov     di,dx               ;reset pointer
           sub     di,es:_c            ;and back to above left
           DO_NEIGHBOR                 ;== above left
           inc     di                  ;go to next
           DO_NEIGHBOR                 ;== above
           inc     di                  ;go to next
           DO_NEIGHBOR                 ;== above right
           mov     di,dx               ;reset pointer
           add     di,es:_c            ;to next row
           DO_NEIGHBOR                 ;== below left
           inc     di                  ;go to next
           DO_NEIGHBOR                 ;== below
           inc     di                  ;go to next
           DO_NEIGHBOR                 ;== below right
endsect4:  loop    loop4               ;only go (cols-2) times
;
;****************************************************************************
sect_5:    add     dx,3                ;point to next life entry
           lodsw                       ;get char and attr
           cmp     ax,0720h            ;see if it's a space
           je      endsect5            ;if so, skip
           mov     di,dx               ;load pointer
           DO_NEIGHBOR                 ;== left
           inc     di                  ;go to next
           DO_NEIGHBOR                 ;== self
           add     BYT [es:di],10h     ;indicate that it's alive
           inc     di                  ;go to next
           DO_NEIGHBOR                 ;== below right
           mov     di,dx               ;reset pointer
           sub     di,es:_c            ;back one row
           DO_NEIGHBOR                 ;== above left
           inc     di                  ;go to next
           DO_NEIGHBOR                 ;== above
           inc     di                  ;go to next
           DO_NEIGHBOR                 ;== right
           mov     di,dx               ;reset pointer
           add     di,es:_c            ;one row beyond
           DO_NEIGHBOR                 ;== below left
           inc     di                  ;go to next
           DO_NEIGHBOR                 ;== below
           mov     di,dx               ;reset pointer
           sub     di,es:_2c           ;go back two rows
           add     di,6                ;then to beginning of next
           DO_NEIGHBOR                 ;== above right

endsect5:  dec     bx                  ;decrement center rows left
           cmp     bx,0                ;see if any rows left
           jle     sect_6              ;if none, the end
           jmp     middlerows          ;otherwise, do another row
;
;****************************************************************************
sect_6:    add     dx,3                ;point to next char
           lodsw                       ;load char and attr
           cmp     ax,0720h            ;see if it's dead
           je      sect_7              ;if so, skip
           mov     di,dx               ;otherwise load into pointer
           DO_NEIGHBOR                 ;== above left
           inc     di                  ;go to next
           DO_NEIGHBOR                 ;== self
           add     BYT [es:di],10h     ;indicate living
           inc     di                  ;go to next
           DO_NEIGHBOR                 ;== right
           mov     di,dx               ;reset pointer
           sub     di,es:_c            ;go back one row
           add     di,3                ;directly above
           DO_NEIGHBOR                 ;== above
           inc     di                  ;go to next
           DO_NEIGHBOR                 ;== above right
           mov     di,OFFSET lifetab   ;get first entry
           DO_NEIGHBOR                 ;== below
           inc     di                  ;go to next
           DO_NEIGHBOR                 ;== below right
           mov     di,dx               ;reset pointer
           add     di,es:_c            ;go to last entry in table
           DO_NEIGHBOR                 ;== left
           mov     di,OFFSET lifetab   ;start of table
           add     di,es:_c            ;start of second row
           sub     di,3                ;end of first row
           DO_NEIGHBOR                 ;== below right
;
;****************************************************************************
sect_7:    mov     cl,es:cols          ;load columns into counter
           xor     ch,ch               ;clear counter LSB
           sub     cx,2                ;skip endpoints

loop7:     add     dx,3                ;point to this char
           lodsw                       ;get char and attr
           cmp     ax,0720h            ;see if it's a blank
           je      endsect7            ;skip if so
           mov     di,dx               ;set up pointer
           DO_NEIGHBOR                 ;== left
           inc     di                  ;go to next
           DO_NEIGHBOR                 ;== self
           add     BYT [es:di],10h     ;indicate living
           inc     di                  ;go to next
           DO_NEIGHBOR                 ;== right
           mov     di,dx               ;point to next
           sub     di,es:_c            ;go back one row
           DO_NEIGHBOR                 ;== above left
           inc     di                  ;go to next
           DO_NEIGHBOR                 ;== above
           inc     di                  ;go to next
           DO_NEIGHBOR                 ;== above left
           mov     di,dx               ;reset pointer
           sub     di,es:_rc           ;back before first row
           add     di,es:_c            ;up to first row
           DO_NEIGHBOR                 ;== below left
           inc     di                  ;go to next
           DO_NEIGHBOR                 ;== below
           inc     di                  ;go to next
           DO_NEIGHBOR                 ;== below right
endsect7:  loop    loop7               ;go back until (cols-2) expires
;
;****************************************************************************
sect_8:    add     dx,3                ;point to this char
           lodsw                       ;load char and attr
           cmp     ax,0720h            ;see if dead
           je      lifend              ;if so, the end
           mov     di,dx               ;load pointer
           DO_NEIGHBOR                 ;== left
           inc     di                  ;go to next
           DO_NEIGHBOR                 ;== self
           add     BYT [es:di],10h     ;indicate living
           mov     di,dx               ;reset pointer
           sub     di,es:_c            ;go back one row
           DO_NEIGHBOR                 ;== above left
           inc     di                  ;go to next
           DO_NEIGHBOR                 ;== above
           inc     di                  ;go to next
           DO_NEIGHBOR                 ;== right
           mov     di,dx               ;reset pointer
           sub     di,es:_rc           ;point to before first row
           add     di,es:_c            ;point to first row
           DO_NEIGHBOR                 ;== below left
           inc     di                  ;go to next
           DO_NEIGHBOR                 ;== below
           mov     di,OFFSET lifetab   ;load first entry pointer
           DO_NEIGHBOR                 ;== below right
           mov     di,dx               ;reset pointer
           sub     di,es:_2c           ;go back two rows
           add     di,6                ;over two cells to start of prev row
           DO_NEIGHBOR                 ;== above right
;
;****************************************************************************
lifend:    pop     ds                  ;restore ds
           mov     si,OFFSET lifetab   ;start at beginning of table
           mov     ax,vidseg           ;get video segment
           mov     es,ax               ;make extra segment point to screen
           xor     di,di               ;point to upper left corner of screen
           mov     cx,rowsxcols        ;put number of cells in loop counter
           xor     bx,bx               ;clear table pointer
           test    lflags,RANDOM       ;see if random fill is enabled
           jnz     rrewrite            ;if so, do random rewrite instead
rewrite:   lodsw                       ;otherwise, get char:attribute
           mov     dx,ax               ;keep char:attr
           lodsb                       ;now get count for table value
           mov     bl,al               ;put into indexable register
           cmp     [bx+lifelookup],0   ;see if alive
           jnz     alive               ;if alive, skip
           mov     dx,0720h            ;otherwise, it's dead
alive:     mov     ax,dx               ;restore char:attr
           stosw                       ;store char:attr
;;;;OLD    inc     di                  ;point past attribute on screen
           loop    rewrite             ;loop back
           jmp     dodelay             ;skip down
;========= random rewrite...
rrewrite:  lodsw                       ;get char:attr
           mov     dx,ax               ;keep char:attr
           call    rnd                 ;calculate random number into bx
           cmp     ax,randthresh       ;compare with random threshhold
           ja      testlife            ;if above threshhold, skip to do normal
           inc     si                  ;skip lifetable value
           call    rnd                 ;get a number in bx
           and     ah,07Fh             ;turn off blink
           test    ah,007h             ;check foreground
           jnz     foreok              ;if nonzero, ok
           mov     ah,007h             ;else, set to normal
foreok:    cmp     al,00h              ;see if NUL, a blank-looking character
           je      rblank              ;if so, write a space character
           cmp     al,0FFh             ;if other blank-like char
           je      rblank              ;then also turn into a blank
           mov     dx,ax               ;otherwise, just leave as is
           jmp     rstore              ;now go store
testlife:
           lodsb                       ;get table value (life count)
           xor     bx,bx               ;clear for lookup
           mov     bl,al               ;put into indexable register
           cmp     [bx+lifelookup],0   ;see if alive
           jnz     rstore              ;if alive, skip
rblank:    mov     dx,0720h            ;otherwise, it's dead
rstore:    mov     ax,dx               ;put back into ax for stosw
           stosw                       ;store
;;;OLD     inc     di                  ;point past attribute on screen
           loop    rrewrite            ;loop back

dodelay:   mov     cx,counter          ;get delay counter
           cmp     cx,0                ;see if it's zero
           jnz     do_counter          ;if not, do counter
           jmp     lifeloop            ;go back for next generation
do_counter:xor     ax,ax               ;point segment to
           mov     es,ax               ;  low memory
           cli                         ;diable interrupts
           mov     bx,es:TICKER        ;get value from system ticker
countloop: cli
           mov     ax,es:TICKER        ;get ticker value
           sti
           cmp     ax,bx               ;see if same as before
           je      countloop           ;if so, go back
           mov     bx,ax               ;if not, set up for next round
           loop    countloop           ;do this stuff cx times
           jmp     lifeloop            ;then go back
;
;*****************************************************************************
;
;  Keystroke Interpretation...
;
do_key:    mov     ah,08h              ;specify read-char-without-echo
           int     21h                 ;read a single character from keyboard
           cmp     al,0                ;see if it's a double-code
           jnz     got_char            ;if not, skip
           mov     ah,08h              ;read character without echo
           int     21h                 ;get it
           or      al,80h              ;set upper bit as indicator
got_char:  cmp     al,_ESC             ;see if it's the ESCape character
           jne     not_esc             ;if not, skip
           jmp     endline             ;if so, jump
not_esc:
           cmp     al,_F4              ;see if F4 was pressed
           jne     next1               ;if not, skip
cntrl_D:   cmp     counter,0           ;see if counter is already at lowest
           jnz     deccount            ;if not, go ahead and decrement it
           jmp     endkey              ;otherwise, do nothing
deccount:  sub     counter,COUNTINC    ;decrement counter
           jmp     endkey              ;go to end

next1:     cmp     al,_F3              ;see if F3 was pressed
           jne     next2               ;if not, skip
cntrl_E:   add     counter,COUNTINC    ;increment counter
           jmp     endkey              ;go to end

next2:     cmp     al,_F9              ;see if pausing, F9
           jne     next21              ;if not, skip
           mov     ax,lflags           ;load lflags into ax
           xor     ax,PAUSED           ;toggle pause flag
           mov     lflags,ax           ;write back out
           jmp     endkey

next21:    cmp     al,_F10             ;see if single-stepping, F10
           jne     next22              ;if not, skip
           mov     ax,lflags           ;load lflags into ax
           or      ax,PAUSED           ;set pause flag
           mov     lflags,ax           ;write back out
           jmp     dolifloop           ;do a generation, bypassing count, etc.

next22:    cmp     al,_F2              ;see if random enabling, F2
           jne     next23              ;if not, skip
           mov     ax,lflags           ;load lflags into ax
           xor     ax,RANDOM           ;set random flag
           mov     lflags,ax           ;write back out
           jmp     endkey              ;go to end

next23:    cmp     al,_F5              ;see if lowering randthresh, F5
           jne     next24              ;if not, skip
           mov     ax,randthresh       ;get threshold for randomization
           sub     ax,RANDINC          ;lower threshhold
           jc      nolower             ;if less than 0, then don't reload
           mov     randthresh,ax       ;otherwise, reload threshold
nolower:   jmp     endkey              ;that's it!

next24:    cmp     al,_F6              ;see if raising randthresh, F6
           jne     next25              ;if not, skip
           add     randthresh,RANDINC  ;raise threshhold
           jmp     endkey              ;that's it!

next25:    cmp     al,_F8              ;see if clearing screen, F8
           jne     next3               ;if not, skip
           mov     ax,vidseg           ;put video segment in ax
           mov     es,ax               ;  to be put into es
           xor     di,di               ;point to top left corner of screen
           mov     ax,0720h            ;put blank (dead) cell into al
           mov     cx,rowsxcols        ;put number of cells in counter
next25lup: repz    stosw               ;store dead cell on screen
;;;;;OLD   inc     di                  ;point to next cell -- skip attribute
;;;;;OLD   loop    next25lup           ;keep going
           jmp     endkey              ;that's it!

next3:     cmp     al,_UPAR            ;see if up arrow
           jne     next4               ;if not, skip
           GET_CURSOR                  ;get current cursor position
           dec     dh                  ;decrement row
           jge     setrowcol           ;if not negative, ok
           mov     dh,rows             ;otherwise, go to bottom row
           dec     dh                  ;  0-24
           jmp     setrowcol           ;set it

next4:     cmp     al,_DNAR            ;see if down arrow
           jne     next5               ;if not, skip
           GET_CURSOR                  ;get current cursor position
           inc     dh                  ;decrement row
           cmp     dh,rows             ;see if at least rows
           jl      setrowcol           ;if not, ok
           xor     dh,dh               ;otherwise, go to top row
           jmp     setrowcol           ;set it

next5:     cmp     al,_LTAR            ;see if left arrow
           jne     next6               ;if not, skip
           GET_CURSOR                  ;get current cursor position
           dec     dl                  ;decrement col
           jge     setrowcol           ;if not negative, ok
           mov     dl,cols             ;otherwise, go to right column
           dec     dl                  ;  0-79
           jmp     setrowcol           ;set it

next6:     cmp     al,_RTAR            ;see if right arrow
           jne     next7               ;if not, skip
           GET_CURSOR                  ;get current cursor position
           inc     dl                  ;decrement col
           cmp     dl,cols             ;see if at least number of columns
           jl      setrowcol           ;if not, ok
           xor     dl,dl               ;otherwise, go to left column
           jmp     setrowcol           ;set it

setrowcol: xor     bh,bh               ;page 0
           mov     ah,02h              ;specify set-cursor-position function
           int     10h                 ;set specified cursor position
           jmp     endkey              ;the end

next7:
next10:
           mov     dl,al               ;put character in dl for write
           mov     ah,02h              ;specify write-character function
           int     21h                 ;call DOS services interrupt
endkey:    jmp     lifeloop            ;go back to top in case another char
;
;*****************************************************************************
;
;  End of program...
;


endline:   cmp     mouse,0             ;see if mouse was used
           jz      nomous              ;nope
           mov     ax,2                ;hide-mouse-cursor function
           int     MOUSINT             ;call mouse interrupt
nomous:    mov     ax,vidseg           ;put video segment into
           mov     es,ax               ;destination pointer segment
           xor     di,di               ;point to start of screen memory
           mov     ax,cs               ;make sure our segment is in
           mov     ds,ax               ;the data segment for source pointer
           mov     si,OFFSET screenbak ;screen backup offset is source
           mov     cx,50*80            ;number of characters on max screen
           repz    movsw               ;put everything back the way it was

           mov     cx,curs_siz         ;get initial cursor size
           xor     bx,bx               ;clear page
           mov     ah,1                ;set cursor size function
           int     10h                 ;call video interrupt
           mov     dx,curs_pos         ;get initial cursor position
           dec     dh                  ;go up one row
           dec     dh                  ;no, make it two rows
           xor     bh,bh               ;make sure page is still zero
           mov     ah,02h              ;specify set-cursor-position up one row
           int     10h                 ;call video services interrupt
           int     20h                 ;call return-to-DOS interrupt


END        start                       ;specify START as entry point


