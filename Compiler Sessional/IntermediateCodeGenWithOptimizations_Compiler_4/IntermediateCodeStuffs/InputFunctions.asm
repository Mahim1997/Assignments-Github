OUTDEC PROC
	;Prints AX as signed decimal number
	;Input is in AX, Output Nothing
	
	;Push all the registers
	PUSH AX
	PUSH BX
	PUSH CX
	PUSH DX
	;Pushing is done
	
;IF AX < 0 ..
	CMP AX, 0
	JGE PRINT_END_IF1
;Then 
	PUSH AX ;save the number
	MOV AH, 2 
	MOV DL, '-'
	INT 21H	;PRINT - sign
	POP AX	;get AX back
	NEG AX	;AX = -AX now
	
PRINT_END_IF1: 
;get decimal digits now
	XOR CX, CX	;CX counts digits.
	MOV BX, 10D	;BX has divisor

PRINT_REPEAT:
	XOR DX, DX	;prepare high word of dividend
	DIV BX	;AX = quotient, DX = remainder
	PUSH DX	;save remainder on stack
	INC CX	;count++
;repeat until
	OR AX, AX	;quotient = 0 ??
	JNE PRINT_REPEAT	;If not then keep going

;Now convert digits to characters and print 

	MOV AH, 2 
;for count times .. do 
PRINT_LOOP_1:
	POP DX	;digit in DL
	OR DL, 30H	;convert to character
	INT 21H
	LOOP PRINT_LOOP_1
	
;END FOR

;PRINTING NewLine
	MOV AH, 2
	MOV DL, 10
	INT 21H
	MOV DL, 13
	INT 21H

;POPPING ALL REGISTERS
	POP DX
	POP CX
	POP BX
	POP AX
	RET
OUTDEC ENDP
	
