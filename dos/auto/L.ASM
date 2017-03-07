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
RANDCONST  EQU     0735Dh              ;added random constant term
RANDINC    EQU     00008h              ;amount to raise/lower randthresh
PAUSED     EQU     08000h              ;pause flag
RANDOM     EQU     04000h              ;random flag
RANDSEL    EQU     02000h              ;random word select indicator


DO_NEIGHBOR MACRO                      ;macro to increment neighbor count, etc
           stosb                       ;save this char to take over
           inc     BYT es:di           ;increment neighbor count
           ENDM

GET_CURSOR MACRO                       ;calls video services interrupt
           xor     bh,bh               ;page 0
           mov     ah,03h              ;specify read-cursor-position function
           int     10h                 ;call video services interrupt
           ENDM

DO_RANDOM  MACRO                       ;calculates a random number into bx
           LOCAL   line1,line2
           mov     bx,RANDCONST        ;start with constant term
           add     bx,WRD rand1        ;add in rand1
           add     bx,WRD rand2        ;add in rand2
           test    WRD lflags,RANDSEL  ;see which number is selected
           jz      line1               ;if rand2, go there
           mov     WRD rand1,bx        ;store random number in rand1
           jmp     line2               ;skip
line1:     mov     WRD rand2,bx        ;store random number in rand2
line2:     xor     WRD lflags,RANDSEL  ;toggle which one gets saved for next
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
           db      'Life lookup table',13,10
           db      'Upper nybble, 0=dead, 1=alive',13,10
           db      'Lower nybble holds number of neighbors, inclusive',13,10
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
curs_pos:  dw      1 DUP(?)            ;initial cursor position
rows:      db      1 DUP (?)           ;number of rows on screen
cols:      db      1 DUP (?)           ;number of columns on screen
rowsxcols: dw      1 DUP (?)           ;product of rows and cols
randthresh:dw      00050h              ;random threshhold is 80 dec
rand1:     dw      1 DUP (?)           ;first random number
rand2:     dw      1 DUP (?)           ;second random number
_rc:       dw      1 DUP (?)           ;rows*cols*2 address offset
_c:        dw      1 DUP (?)           ;cols*2
_2c:       dw      1 DUP (?)           ;2*cols*2
lflags:    dw      00000h              ;initially all lflags disabled
counter:   dw      00000h              ;counter for end of each generation
screenbak: dw      50 DUP (80 DUP (0720H))   ;backup copy of initial screen
lifetab:   dw      50 DUP (80 DUP (0720H))   ;table for life propagation data

realstart: cld                         ;make sure we're going forward
           mov     ah,0Fh              ;specify get-video-mode
           int     10h                 ;call video services interrupt
           mov     BYT cols,ah         ;set number of columns
           cmp     al,07h              ;see if monochrome mode
           jne     getcolor            ;skip if not monochrome
           mov     BYT rows,25         ;specify 25 rows
           mov     WRD vidseg,0B000H   ;set to monochrome video memory
           jmp     initstuff           ;skip complex video card stuff

getcolor:  mov     ax,1130h            ;specify get-extended-video-mode
           mov     bh,03H              ;give it a bogus task, 8x8 font pointer
           int     10h                 ;call video services interrupt
           inc     dl                  ;make last row into number of rows
	   mov     BYT rows,dl         ;store number of rows
	   mov     ax,0040h            ;load in low memory location segment
	   mov     es,ax               ;put into extra segment
	   cli                         ;turn off interrupts during read
	   mov     bx,WRD es:[004eh]   ;get offset into video page
           sti                         ;restore interrupts
           mov     cl,4                ;get ready for shift
           shr     bx,cl               ;shift it (hopefully on paragraph)
           and     bx,0fffh            ;keep only lower
           add     WRD vidseg,bx       ;add page in to video segment

initstuff: mov     al,BYT rows         ;get number of rows into accumulator
           xor     ah,ah               ;clear top bits
           mul     BYT cols            ;multiply by columns
           mov     WRD rowsxcols,ax    ;store product in memory
           shl     ax,1                ;shift up for address offset
           mov     WRD _rc,ax          ;store away for later
           xor     ah,ah               ;clear upper byte
           mov     al,BYT cols         ;put columns in al
           shl     ax,1                ;column address offset
           mov     WRD _c,ax           ;store for later
           shl     ax,1                ;2 rows back
           mov     WRD _2c,ax          ;store 2*columns offset
           xor     bh,bh               ;set page 0
           mov     ah,03h              ;specify read-cursor-position
           int     10h                 ;call video services interrupt
           mov     WRD curs_pos,dx     ;store cursor position for end
           xor     ax,ax               ;make es point
           mov     es,ax               ;  to low memory
           cli                         ;clear interrupts for reading timer
           mov     ax,WRD es:TICKER    ;get system timer for random seed
           sti                         ;restore interrupts
           mov     WRD rand1,ax        ;store ticker value

           push    ds                  ;save data segment
           mov     ax,WRD vidseg       ;transfer video segment into
           mov     ds,ax               ;  data segment register
           xor     si,si               ;start at beginning of screen memory
           mov     ax,cs               ;transfer current segment into
           mov     es,ax               ;  extra segment
           mov     di,OFFSET screenbak ;set up pointer to screen backup mem
	   mov     cx,50*80            ;set up number of chars on screen
           repz    movsw               ;transfer everything to backup memory
           xor     si,si               ;clear out to do it again
           xor     di,di               ;also clear this pointer
           mov     ax,ds               ;data segment also as
           mov     es,ax               ;  destination pointer
	   mov     cx,50*80            ;rows X columns
blanks:    lodsw                       ;get char and attribute
           cmp     al,255              ;see if it's a blank-looking char
           je      writeblank          ;if so, load a blank
           cmp     al,0                ;see if it's a zero
           je      writeblank          ;if so, it's a blank, too
           jmp     writeout            ;write it
writeblank:mov     al,' '              ;put blank in al
writeout:  stosw                       ;put back in
           loop    blanks              ;loop back
           pop     ds                  ;restore data segment


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
           jz      do_life             ;if not, go on to generate screen
           jmp     do_key              ;if so, go process the keystroke

do_life:   test    WRD lflags,PAUSED    ;see if paused
           jnz     lifeloop            ;if so, just get keyboard info

dolifloop: push    ds                  ;save ds
           mov     ax,ds
           mov     es,ax               ;have es point to this segment
           mov     di,OFFSET lifetab   ;get life table offset
           mov     cx,WRD rowsxcols    ;get number of chars on screen
           xor     ax,ax               ;clear ax to
           repz    stosw               ;  clear the entire table
           xor     si,si               ;clear out source (screen) pointer
           mov     ax,WRD vidseg       ;have ds point to video memory
           mov     ds,ax               ;  while es still points to this data

           mov     dx,OFFSET lifetab   ;use dx to hold current position
           sub     dx,2                ;actually, use the one to the left

;section 0 (upper left corner character)
sect_0:    lodsw                       ;get character in al, ignore attribute
           cmp     al,' '              ;see if it's a blank
           je      sect_1              ;if so, skip to section 1
set0:      mov     di,dx               ;set address to own place
           add     di,WRD es:_rc          ;point to end of array
           DO_NEIGHBOR                 ;== above left
           mov     di,dx               ;reset pointer
           add     di,WRD es:_rc          ;point to directly above
           sub     di,WRD es:_c           ;subtract a single row
           add     di,2                ;we're off by one, remember?
           DO_NEIGHBOR                 ;== above
           inc     di                  ;point to next locale
           DO_NEIGHBOR                 ;== above right
           mov     di,dx               ;reset pointer
           add     di,WRD es:_c           ;add in a single row
           DO_NEIGHBOR                 ;== left
           inc     di                  ;go to next
           DO_NEIGHBOR                 ;== below
           inc     di                  ;go to next
           DO_NEIGHBOR                 ;== below right
           mov     di,dx               ;reset pointer
           add     di,2                ;off by one
           DO_NEIGHBOR                 ;== self
           add     BYT es:di,10h       ;indicate that it's alive
           inc     di                  ;go to next
           DO_NEIGHBOR                 ;== right
           mov     di,dx               ;reset pointer
           add     di,WRD es:_2c          ;
           DO_NEIGHBOR                 ;== below left

sect_1:    mov     cl,BYT es:cols         ;get number of columns
           xor     ch,ch               ;  in cx
           sub     cx,2                ;skip first and last char in line

loop1:     add     dx,2                ;point to next location in lifetab
           lodsw                       ;get character in al with attr in ah
           cmp     al,' '              ;see if it's a blank
           je      endsect1            ;if so, go to next
set1:      mov     di,dx               ;reset pointer
           add     di,WRD es:_rc          ;go past end
           sub     di,WRD es:_c           ;back to last row
           DO_NEIGHBOR                 ;== above left
           inc     di                  ;go to next
           DO_NEIGHBOR                 ;== above
           inc     di                  ;go to next
           DO_NEIGHBOR                 ;== above right
           mov     di,dx               ;reset pointer to left
           DO_NEIGHBOR                 ;== left
           inc     di                  ;go to next
           DO_NEIGHBOR                 ;== self
           add     BYT es:di,10h       ;indicate that it's alive
           inc     di                  ;go to next
           DO_NEIGHBOR                 ;== right
           mov     di,dx               ;reset pointer
           add     di,WRD es:_c           ;point to next row
           DO_NEIGHBOR                 ;== below left
           inc     di                  ;go to next
           DO_NEIGHBOR                 ;== below
           inc     di                  ;go to next
           DO_NEIGHBOR                 ;== below right
endsect1:  loop    loop1               ;do this cx=(rows-2) times

sect_2:    add     dx,2                ;put most recent position in dx
           lodsw                       ;get char and attr
           cmp     al,' '              ;see if it's a space
           je      sect_3              ;if so, skip down
           mov     di,dx               ;put pointer into di
           DO_NEIGHBOR                 ;== left
           inc     di                  ;point to next
           DO_NEIGHBOR                 ;== self
           add     BYT es:di,10h       ;indicate that it's alive
           inc     di                  ;go to next
           DO_NEIGHBOR                 ;== below right
           mov     di,dx               ;reset pointer
           add     di,WRD es:_c           ;point to next row
           DO_NEIGHBOR                 ;== below left
           inc     di                  ;go to next
           DO_NEIGHBOR                 ;== below
           mov     di,OFFSET lifetab   ;go to first entry
           DO_NEIGHBOR                 ;== right
           dec     di                  ;back to start of table
           add     di,WRD es:_rc       ;point past end of table
           sub     di,4                ;then back two cells
           DO_NEIGHBOR                 ;== above left
           inc     di                  ;go to next
           DO_NEIGHBOR                 ;== above
           inc     di                  ;point past again
           sub     di,WRD es:_c        ;then back a row
           DO_NEIGHBOR                 ;== above right

sect_3:    mov     bl,BYT es:rows         ;get rows
           xor     bh,bh               ;clear upper byte
           sub     bx,2                ;rows-2
middlerows:add     dx,2                ;go to next char
           lodsw                       ;load char and attr
           cmp     al,' '              ;see if it's empty, dead
           je      sect_4              ;if so, skip
           mov     di,dx               ;load pointer
           DO_NEIGHBOR                 ;== above left
           inc     di                  ;go to next
           DO_NEIGHBOR                 ;== self
           add     BYT es:di,10h       ;indicate that it's alive
           inc     di                  ;go to next
           DO_NEIGHBOR                 ;== right
           mov     di,dx               ;reset pointer
           sub     di,WRD es:_c        ;go back one row
           add     di,2                ;and over a cell
           DO_NEIGHBOR                 ;== above
           inc     di                  ;go to next
           DO_NEIGHBOR                 ;== above right
           mov     di,dx               ;reset pointer
           add     di,WRD es:_c        ;go down a row
           DO_NEIGHBOR                 ;== left
           inc     di                  ;go to next
           DO_NEIGHBOR                 ;== below
           inc     di                  ;go to next
           DO_NEIGHBOR                 ;== below right
           mov     di,dx               ;reset pointer
           add     di,WRD es:_2c       ;down yet another row
           DO_NEIGHBOR                 ;== below left

sect_4:    mov     cl,BYT es:cols         ;put number of columns per row in count
           xor     ch,ch               ;clear upper byte
           sub     cx,2                ;subtract endpoints

loop4:     add     dx,2                ;point to next place in table
           lodsw                       ;get char and attr
           cmp     al,' '              ;see if char is a space
           je      endsect4            ;if so, skip to loop's end
           mov     di,dx               ;set up pointer
           DO_NEIGHBOR                 ;== left
           inc     di                  ;go to next
           DO_NEIGHBOR                 ;== self
           add     BYT es:di,10h       ;indicate that it's alive
           inc     di                  ;go to next
           DO_NEIGHBOR                 ;== right
           mov     di,dx               ;reset pointer
           sub     di,WRD es:_c           ;and back to above left
           DO_NEIGHBOR                 ;== above left
           inc     di                  ;go to next
           DO_NEIGHBOR                 ;== above
           inc     di                  ;go to next
           DO_NEIGHBOR                 ;== above right
           mov     di,dx               ;reset pointer
           add     di,WRD es:_c           ;to next row
           DO_NEIGHBOR                 ;== below left
           inc     di                  ;go to next
           DO_NEIGHBOR                 ;== below
           inc     di                  ;go to next
           DO_NEIGHBOR                 ;== below right
endsect4:  loop    loop4               ;only go (cols-2) times

sect_5:    add     dx,2                ;point to next life entry
           lodsw                       ;get char and attr
           cmp     al,' '              ;see if it's a space
           je      endsect5            ;if so, skip
           mov     di,dx               ;load pointer
           DO_NEIGHBOR                 ;== left
           inc     di                  ;go to next
           DO_NEIGHBOR                 ;== self
           add     BYT es:di,10h       ;indicate that it's alive
           inc     di                  ;go to next
           DO_NEIGHBOR                 ;== below right
           mov     di,dx               ;reset pointer
           sub     di,WRD es:_c        ;back one row
           DO_NEIGHBOR                 ;== above left
           inc     di                  ;go to next
           DO_NEIGHBOR                 ;== above
           inc     di                  ;go to next
           DO_NEIGHBOR                 ;== right
           mov     di,dx               ;reset pointer
           add     di,WRD es:_c        ;one row beyond
           DO_NEIGHBOR                 ;== below left
           inc     di                  ;go to next
           DO_NEIGHBOR                 ;== below
           mov     di,dx               ;reset pointer
           sub     di,WRD es:_2c       ;go back two rows
           add     di,4                ;then to beginning of next
           DO_NEIGHBOR                 ;== above right

endsect5:  dec     bx                  ;decrement center rows left
           cmp     bx,0                ;see if any rows left
           jle     sect_6              ;if none, the end
           jmp     middlerows          ;otherwise, do another row

sect_6:    add     dx,2                ;point to next char
           lodsw                       ;load char and attr
           cmp     al,' '              ;see if it's dead
           je      sect_7              ;if so, skip
           mov     di,dx               ;otherwise load into pointer
           DO_NEIGHBOR                 ;== above left
           inc     di                  ;go to next
           DO_NEIGHBOR                 ;== self
           add     BYT es:di,10h       ;indicate living
           inc     di                  ;go to next
           DO_NEIGHBOR                 ;== right
           mov     di,dx               ;reset pointer
           sub     di,WRD es:_c        ;go back one row
           add     di,2                ;directly above
           DO_NEIGHBOR                 ;== above
           inc     di                  ;go to next
           DO_NEIGHBOR                 ;== above right
           mov     di,OFFSET lifetab   ;get first entry
           DO_NEIGHBOR                 ;== below
           inc     di                  ;go to next
           DO_NEIGHBOR                 ;== below right
           mov     di,dx               ;reset pointer
           add     di,WRD es:_c        ;go to last entry in table
           DO_NEIGHBOR                 ;== left
           mov     di,OFFSET lifetab   ;start of table
           add     di,WRD es:_c        ;start of second row
           sub     di,2                ;end of first row
           DO_NEIGHBOR                 ;== below right

sect_7:    mov     cl,BYT es:cols      ;load columns into counter
           xor     ch,ch               ;clear counter LSB
           sub     cx,2                ;skip endpoints

loop7:     add     dx,2                ;point to this char
           lodsw                       ;get char and attr
           cmp     al,' '              ;see if it's a blank
           je      endsect7            ;skip if so
           mov     di,dx               ;set up pointer
           DO_NEIGHBOR                 ;== left
           inc     di                  ;go to next
           DO_NEIGHBOR                 ;== self
           add     BYT es:di,10h       ;indicate living
           inc     di                  ;go to next
           DO_NEIGHBOR                 ;== right
           mov     di,dx               ;point to next
           sub     di,WRD es:_c        ;go back one row
           DO_NEIGHBOR                 ;== above left
           inc     di                  ;go to next
           DO_NEIGHBOR                 ;== above
           inc     di                  ;go to next
           DO_NEIGHBOR                 ;== above left
           mov     di,dx               ;reset pointer
           sub     di,WRD es:_rc       ;back before first row
           add     di,WRD es:_c        ;up to first row
           DO_NEIGHBOR                 ;== below left
           inc     di                  ;go to next
           DO_NEIGHBOR                 ;== below
           inc     di                  ;go to next
           DO_NEIGHBOR                 ;== below right
endsect7:  loop    loop7               ;go back until (cols-2) expires

sect_8:    add     dx,2                ;point to this char
           lodsw                       ;load char and attr
           cmp     al,' '              ;see if dead
           je      lifend              ;if so, the end
           mov     di,dx               ;load pointer
           DO_NEIGHBOR                 ;== left
           inc     di                  ;go to next
           DO_NEIGHBOR                 ;== self
           add     BYT es:di,10h       ;indicate living
           mov     di,dx               ;reset pointer
           sub     di,WRD es:_c        ;go back one row
           DO_NEIGHBOR                 ;== above left
           inc     di                  ;go to next
           DO_NEIGHBOR                 ;== above
           inc     di                  ;go to next
           DO_NEIGHBOR                 ;== right
           mov     di,dx               ;reset pointer
           sub     di,WRD es:_rc       ;point to before first row
           add     di,WRD es:_c        ;point to first row
           DO_NEIGHBOR                 ;== below left
           inc     di                  ;go to next
           DO_NEIGHBOR                 ;== below
           mov     di,OFFSET lifetab   ;load first entry pointer
           DO_NEIGHBOR                 ;== below right
           mov     di,dx               ;reset pointer
           sub     di,WRD es:_2c       ;go back two rows
           add     di,4                ;over two cells to start of prev row
           DO_NEIGHBOR                 ;== above right

lifend:    pop     ds                  ;restore ds
           mov     si,OFFSET lifetab   ;start at beginning of table
           mov     ax,WRD vidseg       ;get video segment
           mov     es,ax               ;make extra segment point to screen
           xor     di,di               ;point to upper left corner of screen
           mov     cx,WRD rowsxcols    ;put number of cells in loop counter
           xor     bx,bx               ;clear table pointer
           test    WRD lflags,RANDOM   ;see if random fill is enabled
           jnz     rrewrite            ;if so, do random rewrite instead
rewrite:   lodsw                       ;otherwise, get table value
           mov     bl,ah               ;put into indexable register
           cmp     [bx+lifelookup],0   ;see if alive
           jnz     alive               ;if alive, skip
           mov     al,' '              ;otherwise, it's dead
alive:     stosb                       ;store
           inc     di                  ;point past attribute on screen
           loop    rewrite             ;loop back
           jmp     dodelay             ;skip down

rrewrite:  lodsw                       ;get table value
           DO_RANDOM                   ;calculate random number into bx
           cmp     bx,WRD randthresh   ;compare with random threshhold
           ja      testlife            ;if above threshhold, skip to do normal
           DO_RANDOM                   ;get a number in bx
           cmp     bl,0                ;see if NUL, a blank-looking character
           je      rblank              ;if so, write a space character
           cmp     bl,255              ;if other blank-like char
           je      rblank              ;then also turn into a blank
           mov     al,bl               ;otherwise, just leave as is
           jmp     rstore              ;now go store
testlife:
           xor     bx,bx               ;clear for lookup
           mov     bl,ah               ;put into indexable register
           cmp     [bx+lifelookup],0   ;see if alive
           jnz     rstore              ;if alive, skip
rblank:    mov     al,' '              ;otherwise, it's dead
rstore:    stosb                       ;store
           inc     di                  ;point past attribute on screen
           loop    rrewrite            ;loop back

dodelay:   mov     cx,WRD counter      ;get delay counter
           cmp     cx,0                ;see if it's zero
           jnz     do_counter          ;if not, do counter
           jmp     lifeloop            ;go back for next generation
do_counter:xor     ax,ax               ;point segment to
	   mov     es,ax               ;  low memory
	   cli                         ;diable interrupts
           mov     bx,WRD es:TICKER    ;get value from system ticker
countloop: cli
	   mov     ax,WRD es:TICKER    ;get ticker value
	   sti
           cmp     ax,bx               ;see if same as before
           je      countloop           ;if so, go back
           mov     bx,ax               ;if not, set up for next round
           loop    countloop           ;do this stuff cx times
           jmp     lifeloop            ;then go back

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
cntrl_D:   cmp     WRD counter,0       ;see if counter is already at lowest
           jnz     deccount            ;if not, go ahead and decrement it
           jmp     endkey              ;otherwise, do nothing
deccount:  sub     WRD counter,COUNTINC  ;decrement counter
           jmp     endkey              ;go to end

next1:     cmp     al,_F3              ;see if F3 was pressed
           jne     next2               ;if not, skip
cntrl_E:   add     WRD counter,COUNTINC  ;increment counter
           jmp     endkey              ;go to end

next2:     cmp     al,_F9              ;see if pausing, F9
           jne     next21              ;if not, skip
           mov     ax,WRD lflags       ;load lflags into ax
           xor     ax,PAUSED           ;toggle pause flag
           mov     WRD lflags,ax       ;write back out
           jmp     endkey

next21:    cmp     al,_F10             ;see if single-stepping, F10
           jne     next22              ;if not, skip
           mov     ax,WRD lflags       ;load lflags into ax
           or      ax,PAUSED           ;set pause flag
           mov     WRD lflags,ax       ;write back out
           jmp     dolifloop           ;do a generation, bypassing count, etc.

next22:    cmp     al,_F2              ;see if random enabling, F2
           jne     next23              ;if not, skip
           mov     ax,WRD lflags       ;load lflags into ax
           xor     ax,RANDOM           ;set random flag
           mov     WRD lflags,ax       ;write back out
           jmp     endkey              ;go to end

next23:    cmp     al,_F5              ;see if lowering randthresh, F5
	   jne     next24              ;if not, skip
	   mov     ax,WRD randthresh   ;get threshold for randomization
	   sub     ax,RANDINC          ;lower threshhold
           jc      nolower             ;if less than 0, then don't reload
           mov     WRD randthresh,ax   ;otherwise, reload threshold
nolower:   jmp     endkey              ;that's it!

next24:    cmp     al,_F6              ;see if raising randthresh, F6
           jne     next25              ;if not, skip
           add     WRD randthresh,RANDINC   ;raise threshhold
           jmp     endkey              ;that's it!

next25:    cmp     al,_F8              ;see if clearing screen, F8
           jne     next3               ;if not, skip
           mov     ax,WRD vidseg       ;put video segment in ax
           mov     es,ax               ;  to be put into es
           xor     di,di               ;point to top left corner of screen
           mov     al,' '              ;put blank (dead) cell into al
           mov     cx,WRD rowsxcols    ;put number of cells in counter
next25lup: stosb                       ;store dead cell on screen
           inc     di                  ;point to next cell -- skip attribute
           loop    next25lup           ;keep going
           jmp     endkey              ;that's it!

next3:     cmp     al,_UPAR            ;see if up arrow
           jne     next4               ;if not, skip
           GET_CURSOR                  ;get current cursor position
           dec     dh                  ;decrement row
           jge     setrowcol           ;if not negative, ok
           mov     dh,BYT rows         ;otherwise, go to bottom row
           dec     dh                  ;  0-24
           jmp     setrowcol           ;set it

next4:     cmp     al,_DNAR            ;see if down arrow
           jne     next5               ;if not, skip
           GET_CURSOR                  ;get current cursor position
           inc     dh                  ;decrement row
           cmp     dh,BYT rows         ;see if at least rows
           jl      setrowcol           ;if not, ok
           xor     dh,dh               ;otherwise, go to top row
           jmp     setrowcol           ;set it

next5:     cmp     al,_LTAR            ;see if left arrow
           jne     next6               ;if not, skip
           GET_CURSOR                  ;get current cursor position
           dec     dl                  ;decrement col
           jge     setrowcol           ;if not negative, ok
           mov     dl,BYT cols         ;otherwise, go to right column
           dec     dl                  ;  0-79
           jmp     setrowcol           ;set it

next6:     cmp     al,_RTAR            ;see if right arrow
           jne     next7               ;if not, skip
           GET_CURSOR                  ;get current cursor position
           inc     dl                  ;decrement col
           cmp     dl,BYT cols         ;see if at least number of columns
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

;*****************************************************************************
;
;  End of program...
;


endline:   mov     ax,WRD vidseg       ;put video segment into
           mov     es,ax               ;destination pointer segment
           xor     di,di               ;point to start of screen memory
           mov     ax,cs               ;make sure our segment is in
           mov     ds,ax               ;the data segment for source pointer
           mov     si,OFFSET screenbak ;screen backup offset is source
	   mov     cx,50*80            ;number of characters on max screen
           repz    movsw               ;put everything back the way it was

           mov     dx,WRD curs_pos     ;get initial cursor position
	   dec     dh                  ;go up one row
	   dec     dh                  ;no, make it two rows
           xor     bh,bh               ;make sure page is still zero
           mov     ah,02h              ;specify set-cursor-position up one row
           int     10h                 ;call video services interrupt
           int     20h                 ;call return-to-DOS interrupt


END        start                       ;specify START as entry point
