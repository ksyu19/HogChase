; Print.s
; Student names: Karena Yu and Alex Smith
; Last modification date: 11-09-16
; Runs on LM4F120 or TM4C123
; EE319K lab 7 device driver for any LCD
;
; As part of Lab 7, students need to implement these LCD_OutDec and LCD_OutFix
; This driver assumes two low-level LCD functions
; ST7735_OutChar   outputs a single 8-bit ASCII character
; ST7735_OutString outputs a null-terminated string 

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

; Binding
dcurrent	EQU 0 	;32-bit unsigned number
dnext 		EQU 4 	;32-bit unsigned number
LCD_OutDec
; Allocation	
	PUSH {R11,LR}
	SUB SP, #8 					;allocate current
	MOV R11, SP 				;frame pointer 
; Access
	STR R0, [R11, #dcurrent] 	;store current
	CMP R0, #10
	BLO dBaseCase 				;if R0 < #10 begin printing digits
	MOV	R1, #10
	UDIV R0, R1 				;next
	STR R0, [R11, #dnext] 		;store next
	BL LCD_OutDec 				;Recurse if R0 >= #10
; Printing nonBaseCase digits 
	LDR R0, [R11, #dnext] 		;get next
	LDR R1, [R11, #dcurrent] 	;get current
	MOV R2, #10
	MUL R0, R2
	SUB R0, R1, R0 				;current - next*10
	ADD R0, #48 				;ascii value of digit
	BL ST7735_OutChar
	B dExit
dBaseCase
	ADD R0, #48 				;ascii value of digit 
	BL ST7735_OutChar 			;output first digit
dExit 
; Deallocate 
	ADD SP, #8
	POP {R11, PC}

;* * * * * * * * End of LCD_OutDec * * * * * * * *

; -----------------------LCD _OutFix----------------------
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
; Binding
one			EQU 0 ;32-bit unsigned number
tenth		EQU 4 ;32-bit unsigned number
hundredth 	EQU 8 ;32-bit unsigned number
thousandth 	EQU 12 ;32-bit unsigned number
ceil		EQU 9999
LCD_OutFix
; Allocation	
	PUSH {R4,LR}
	SUB SP, #16 	;allocate one, tenth, hundredth, thousandth
; Access
	LDR R1, =ceil
	CMP R0, R1
	BHI PrintStars 	; If R0 > #9999, print out stars
	;fill stack with digits:
	MOV	R2, #10
	MOV R3, #12 	;counter
floop
	UDIV R1, R0, R2
	MUL R4, R1, R2
	SUB R0, R4
	STR R0, [SP, R3]
	MOV R0, R1
	SUBS R3, #4
	BHS floop 		;branch if R3 >=0
; Printing
	LDR R0, [SP, #one]
	ADD R0, #48
	BL ST7735_OutChar 			;print one
	MOV R0, #46 				;ascii value of .
	BL ST7735_OutChar 			;print .
	LDR R0, [SP, #tenth] 		;get tenth
	ADD R0, #48 				;ascii value of digit
	BL ST7735_OutChar
	LDR R0, [SP, #hundredth] 	;get hundredth
	ADD R0, #48 				;ascii value of digit
	BL ST7735_OutChar
	LDR R0, [SP, #thousandth] 	;get thousandth
	ADD R0, #48 				;ascii value of digit
	BL ST7735_OutChar
	B fExit
PrintStars
	MOV R0, #42 				;ascii value of *
	BL ST7735_OutChar
	MOV R0, #46 				;ascii value of .
	BL ST7735_OutChar
	MOV R0, #42 				;ascii value of *
	BL ST7735_OutChar
	MOV R0, #42 				;ascii value of *
	BL ST7735_OutChar
	MOV R0, #42 				;ascii value of *
	BL ST7735_OutChar
fExit 
; Deallocate 
	ADD SP, #16
	POP {R4, PC}    

;* * * * * * * * End of LCD_OutFix * * * * * * * *

     ALIGN                           ; make sure the end of this section is aligned
     END                             ; end of file
