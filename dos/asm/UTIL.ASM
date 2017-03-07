	.model	small
	.code
	.data

	extrn	DsSave:word

	.code
	public	Blank, ScanP, ScanB, Out16, Hex, HexIn
	public	GetEol, HexChk, Address, GetHex, GetHex1, Tab
	public	Backup,PrintMes

	extrn	OutCh:near, PErr:near, Error:near

BacMes		db	8," ",8+80H

;Output one space

BLANK:
	MOV	AL," "
	jmp	OutCh

;Output the number of blanks in CX

TAB:
	CALL	BLANK
	LOOP	TAB
	RET

;Scan for parameters of a command

SCANP:
	CALL	SCANB		;Get first non-blank
	CMP	AL,","		;One comma between params OK
	JNE	EOLCHK		;If not comma, we found param
	INC	SI		;Skip over comma

;Scan command line for next non-blank character

SCANB:
	LODSB
	CMP	AL," "
	JZ	SCANB		;Skip over blanks
	cmp	al,9
	jz	ScanB
	DEC	SI		;Back up to first non-blank
EOLCHK:
	CMP	AL,13
	RET

;Print out 16-bit value in DX in hex

OUT16:
	MOV	AL,DH		;High-order byte first
	CALL	HEX
	MOV	AL,DL		;Then low-order byte

;Output byte in AL as two hex digits

HEX:
	MOV	AH,AL		;Save for second digit
;Shift high digit into low 4 bits
	PUSH	CX
	MOV	CL,4
	SHR	AL,CL
	POP	CX

	CALL	DIGIT		;Output first digit
HIDIG:
	MOV	AL,AH		;Now do digit saved in AH
DIGIT:
	AND	AL,0FH		;Mask to 4 bits
;Trick 6-byte hex conversion works on 8086 too.
	ADD	AL,90H
	DAA
	ADC	AL,40H
	DAA
	jmp	OutCh

;Check if next character in the input buffer is a hex digit
;and convert it to binary if it is. Carry set if not.

HEXIN:
	MOV	AL,[SI]

;Check if AL has a hex digit and convert it to binary if it
;is. Carry set if not.

HEXCHK:
	SUB	AL,"0"		;Kill ASCII numeric bias
	JC	RET2
	CMP	AL,10
	CMC
	JNC	RET2		;OK if 0-9
	AND	AL,5FH		;Convert to upper case
	SUB	AL,7		;Kill A-F bias
	CMP	AL,10
	JC	RET2
	CMP	AL,16
	CMC
Ret2:	RET

;Get an address in Segment:Offset format. Segment may be ommitted
;and a default (kept in BP) will be used, or it may be a segment
;register (DS, ES, SS, CS). Return with segment in AX, Offset in DX.

ADDRESS:
	CALL	SCANP
	CMP	byte ptr [SI+1],"S"	;Is second character "S"?
	JZ	SEGREG
	MOV	CX,4
	CALL	GETHEX		;Get number--may be segment or offset
	MOV	AX,BP		;Get default segment
	CMP	byte ptr [SI],":"	;Segment specification?
	JNZ	RET3
	PUSH	DX		;Save segment while we get offset
GETDISP:
	INC	SI		;Skip over ":"
	MOV	CX,4
	CALL	GETHEX
	POP	AX		;Bring segment back
Ret3:	RET

SEGREG:
	LODSB			;First letter of segment register
	MOV	DI,offset DGroup:SEGLET-1
	MOV	CX,4
CSSCAN:
	INC	DI
	CMP	AL,CS:[DI]
	LOOPNZ	CSSCAN
	JNZ	PERR
	INC	SI		;Skip second letter ("S")
	SHL	CX,1
	MOV	BX,CX
	CMP	byte ptr [SI],":"
	JNZ	ErrorJ
	PUSH	[BX+DSSave]
	jmp	GETDISP

SEGLET	DB	"CSED"


;Get the next parameter, which must be a hex number.
;CX is maximum number of digits the number may have.

GETHEX:
	CALL	SCANP		;Scan to next parameter
GETHEX1:
	XOR	DX,DX		;Initialize the number
	MOV	AH,DH
	CALL	HEXIN		;Get a hex digit
	JC	ErrorJ		;Must be one valid digit
	MOV	DL,AL		;First 4 bits in position
GETLP:
	INC	SI		;Next char in buffer
	DEC	CX		;Digit count
	CALL	HEXIN		;Get another hex digit?
	JC	RET2		;All done if no more digits
	JCXZ	ErrorJ		;Too many digits?
	SHL	DX,1		;Multiply by 16
	SHL	DX,1
	SHL	DX,1
	SHL	DX,1
	OR	DL,AL		;and combine new digit
	jmp	GETLP		;Get more digits

ErrorJ:	jmp	Error

;Make sure there is nothing more on the line except for
;blanks and carriage return. If there is, it is an
;unrecognized parameter and an error.

GETEOL:
	PUSH	AX
	CALL	SCANB		;Skip blanks
	POP	AX
	JNZ	ErrorJ		;Better be a RETURN
	RET

;Physical backspace - blank, backspace, blank

Backup:
	mov	si,offset DGroup:BacMes

;Print ASCII message. Last char has bit 7 set

PrintMes:
	push	cs
	pop	ds
PrintLp:
	lodsb			;Get char to print
	call	OutCh
	shl	al,1		;High bit set?
	jnc	PrintLp
	push	es
	pop	ds
	ret

	end
