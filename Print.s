; Print.s
; Student names: Zachary Chilton & Philip Tan
; Last modification date: 03/27/2017
; Runs on LM4F120 or TM4C123
; EE319K lab 7 device driver for any LCD
;
; As part of Lab 7, students need to implement these LCD_OutDec and LCD_OutFix
; This driver assumes two low-level LCD functions
; ST7735_OutChar   outputs a single 8-bit ASCII character
; ST7735_OutString outputs a null-terminated string 

PERIOD 		EQU 0x2E
ASCII		EQU 0x30
STAR		EQU 0x2A

    IMPORT   ST7735_OutChar
    IMPORT   ST7735_OutString
    EXPORT   LCD_OutDec
    EXPORT   LCD_OutFix

    AREA    |.text|, CODE, READONLY, ALIGN=2
    THUMB

;-----------------------LCD_OutDec-----------------------
; Output a 32-bit number in unsigned decimal format
; Input: R0 (call by value) 32-bit unsigned number
; Output: none
; Invariables: This function must not permanently modify registers R4 to R11
Bill EQU 1000000000
HMil EQU 100000000
TMil EQU 10000000
Mill EQU 1000000
HTho EQU 100000
TTho EQU 10000
Thou EQU 1000	
Hun	 EQU 100
Ten  EQU 10
One  EQU 1
	
LCD_OutDec
	PUSH {R11, LR, R4, R5}
	MOV R11, SP
	SUB SP, #8					;Allocating space for local variable in
dig	EQU -4
	
	MOV R1, R0					;Making a copy of in
	LDR R2, =Bill
	STR R2, [R11, #dig]			;Digit is our local variable
	
DigitCheck
	LDR R2, [R11, #dig]
	UDIV R0, R1, R2				;See how many [digit] fits in input
	MOV R4, R0					;Copy this
	BL ConvertToAscii			;Output
	LDR R2, [R11, #dig]
	MUL R0, R4, R2
	SUB R1, R1, R0				;Subtract that many digit
	LDR R2, [R11, #dig]
	CMP R2, #1
	BEQ Exit					;If The digit is 1, exit the loop
	MOV R3, #10
	UDIV R2, R2, R3			;Go to the next digit
	STR R2, [R11, #dig]
	B DigitCheck

Exit
	ADD SP, #8
	POP {R11, LR, R4, R5}
	BX LR
	
ConvertToAscii					;Pass in with R0
	PUSH {LR, R11, R1, R0}
	ADD R0, #0x30
	BL ST7735_OutChar
	POP {LR, R11, R1, R0}
	BX LR

;* * * * * * * * End of LCD_OutDec * * * * * * * *

; -----------------------LCD_OutFix----------------------
; Output characters to LCD display in fixed-point format
; unsigned decimal, resolution 0.001, range 0.000 to 9.999
; Inputs:  R0 is an unsigned 32-bit number
; Outputs: none
; E.g., R0=0,    then output "0.000 "
;       R0=3,    then output "0.003 "
;       R0=89,   then output "0.089 "
;       R0=123,  then output "0.123 "
;       R0=9999, then output "9.999 "
;       R0>9999, then output "*.*** "
; Invariables: This function must not permanently modify registers R4 to R11
LCD_OutFix
	PUSH {LR,R0}
	MOV R1,#9999
	CMP R0,R1
	BHI stars
	MOV R12,R0	;R12 holds input
	MOV R1,#1000
	BL outPlace
	MOV R0,#PERIOD
	BL ST7735_OutChar
	MOV R0,R12
	MOV R1,#100
	BL outPlace
	MOV R1,#10
	BL outPlace
	MOV R1,#1
	BL outPlace
	
	POP	{LR,R0}
	BX LR
	
outPlace	;R0 holds input, R1 holds place to get (e.g. 1000 means the thousands place)
			;R0 holds output	(e.g. 9999 and 1000 returns 9)
			;assumes the place given is the greatest place in the number
	PUSH{LR,R0}
	UDIV R0,R12,R1
	MUL R2,R0,R1
	SUB R12,R2
	ADD R0,#ASCII
	BL ST7735_OutChar
		
	POP{LR,R0}
	BX LR
	
stars
	MOV R0,#STAR
	BL ST7735_OutChar
	MOV R0,#PERIOD
	BL ST7735_OutChar
	MOV R12,#3
	MOV R0,#STAR
starLoop
	MOV R0,#STAR
	BL ST7735_OutChar
	SUBS R12,#1
	BHI starLoop
	POP	{LR,R0}
	BX LR

;* * * * * * * * End of LCD_OutFix * * * * * * * *

	ALIGN                           ; make sure the end of this section is aligned
	END                             ; end of file
