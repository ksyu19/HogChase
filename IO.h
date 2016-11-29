; IO.s
; Student names: Karena Yu
; Last modification date: 10/26/2016
; Runs on LM4F120/TM4C123

; EE319K lab 7 device driver for the switch and LED.
; You are allowed to use any switch and any LED,
; although the Lab suggests the SW1 switch PF4 and Red LED PF1

; As part of Lab 7, students need to implement these three functions

; negative logic SW2 connected to PF0 on the Launchpad
; red LED connected to PF1 on the Launchpad
; blue LED connected to PF2 on the Launchpad
; green LED connected to PF3 on the Launchpad
; negative logic SW1 connected to PF4 on the Launchpad

        EXPORT   IO_Init
        EXPORT   IO_Touch
        EXPORT   IO_HeartBeat

GPIO_PORTF_DATA_R  EQU 0x400253FC
GPIO_PORTF_DIR_R   EQU 0x40025400
GPIO_PORTF_AFSEL_R EQU 0x40025420
GPIO_PORTF_PUR_R   EQU 0x40025510
GPIO_PORTF_DEN_R   EQU 0x4002551C
GPIO_PORTF_LOCK_R  EQU 0x40025520
GPIO_PORTF_CR_R    EQU 0x40025524
GPIO_PORTF_AMSEL_R EQU 0x40025528
GPIO_PORTF_PCTL_R  EQU 0x4002552C
GPIO_LOCK_KEY      EQU 0x4C4F434B  ; Unlocks the GPIO_CR register
PF0       EQU 0x40025004
PF1       EQU 0x40025008
PF2       EQU 0x40025010
PF3       EQU 0x40025020
PF4       EQU 0x40025040
LEDS      EQU 0x40025038
RED       EQU 0x02
BLUE      EQU 0x04
GREEN     EQU 0x08
SWITCHES  EQU 0x40025044
SW1       EQU 0x10                 ; on the left side of the Launchpad board
SW2       EQU 0x01                 ; on the right side of the Launchpad board
SYSCTL_RCGCGPIO_R  EQU 0x400FE608
DELAYCOUNT EQU 266667				;80MHz bus clock, delay for 10 ms
    
        AREA    |.text|, CODE, READONLY, ALIGN=2
        THUMB



;------------IO_Init------------
; Initialize GPIO Port for a switch and an LED (switch: PF4, LED: PF1)
; Input: none
; Output: none
; This is a public function
; Invariables: This function must not permanently modify registers R4 to R11
IO_Init
;Turn on Port F clock
	 LDR R0, =SYSCTL_RCGCGPIO_R
	 LDR R1, [R0]
	 ORR R1, #0x20 
	 STR R1, [R0]
;Wait two bus cycles
	 NOP 
	 NOP
;Unlock GPIO Port F
	 LDR R0, =GPIO_PORTF_LOCK_R
	 LDR R1, =GPIO_LOCK_KEY 
	 STR R1, [R0]
;Allow changes to PF1, PF3
	 LDR R0, =GPIO_PORTF_CR_R
	 LDR R1, [R0]
	 ORR R1, #0x12
	 STR R1, [R0]
;Clear AMSEL to disable analog
	 LDR R0, =GPIO_PORTF_AMSEL_R
	 LDR R1, [R0]
	 BIC R1, #0x12
	 STR R1, [R0]
;Clear PCTL to select GPIO
	 LDR R0, =GPIO_PORTF_PCTL_R
	 LDR R1, [R0]
	 BIC R1, #0x12
	 STR R1, [R0]
;Set input/output
	 LDR R0, =GPIO_PORTF_DIR_R  
	 LDR R1, [R0]
	 BIC R1, #0x10 ;Set Pin 4 as input
	 ORR R1, #0x02 ;Set Pin 1 as output
	 STR R1, [R0]
;Clear AFSEL for regular input/output
	 LDR R0, =GPIO_PORTF_AFSEL_R   
	 LDR R1, [R0]
	 BIC R1, #0x12
	 STR R1, [R0]
;Enable internal pull-up
	 LDR R0, =GPIO_PORTF_PUR_R
	 LDR R1, [R0]
	 ORR R1, #0x10 ;Set Pin 4 as PUR
	 STR R1, [R0]
;Enable data pins
	 LDR R0, =GPIO_PORTF_DEN_R  
	 LDR R1, [R0]
	 ORR R1, #0x12 ;Enable Ports 1,4
	 STR R1, [R0]
    BX  LR
;* * * * * * * * End of IO_Init * * * * * * * *

;------------IO_HeartBeat------------
; Toggle the output state of the LED.
; Input: none
; Output: none
; This is a public function
; Invariables: This function must not permanently modify registers R4 to R11
IO_HeartBeat
    LDR R0, =GPIO_PORTF_DATA_R
	LDR R1, [R0]
	EOR R1, #0x02
    STR R1, [R0]
    BX  LR                          ; return
;* * * * * * * * End of IO_HeartBeat * * * * * * * *

;------------IO_Touch------------
; First: wait for the release of the switch
; and then: wait for the touch of the switch (prevent bounce)
; Input: none
; Input: none
; Output: none
; This is a public function
; Invariables: This function must not permanently modify registers R4 to R11
IO_Touch
	PUSH {LR}
	LDR R0, =GPIO_PORTF_DATA_R
waitrelease
	BL 	delay10ms
	LDR R1, [R0] 
	ANDS R1, #0x10 ;(PF4 switch has negative logic: 0 = pressed, 1 = unpressed)
	BEQ waitrelease ;branch to waitrelease if switch pressed (R1 == 0)
waittouch
	BL 	delay10ms
	LDR R1, [R0] 
	ANDS R1, #0x10 ;(PF4 switch has negative logic: 0 = pressed, 1 = unpressed)
	BEQ touchdone ;branch to touchdone if switch pressed (R1 == 0)
	B 	waittouch
delay10ms
	LDR R2, =DELAYCOUNT
wait SUBS R2, R2, #1
	BNE wait ;if not zero, keep subtracting
    BX  LR	;return from delay10ms
touchdone	
	POP {PC} ;return from IO_Touch
	
;* * * * * * * * End of IO_Touch * * * * * * * *

    ALIGN                           ; make sure the end of this section is aligned
    END                             ; end of file
	