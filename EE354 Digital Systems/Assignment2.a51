; Clayton Brutus
; Assignment 2
; 9/16/2016
;
; With LED's on P1.0 - P1.2 (active low)
; and button on P3.4
; Turns one LED on a time,
; blinking left to right if P3.4 is low, and right to left if P3.4 is high

MainSeg SEGMENT CODE
CSEG at 0
	ljmp Start
RSEG MainSeg

Start:	
		Call Read ; read button state
		
		; Closed (left -> right)
		Call Left
		Call Delay
		Call Mid
		Call Delay
		Call Right
		Call Delay
		Call Off
		Call Delay
		Call Delay
		Call Delay
		
		jmp Start
		
Open:	; Open (right -> left)
		Call Right
		Call Delay
		Call Mid
		Call Delay
		Call Left
		Call Delay
		Call Off
		Call Delay
		Call Delay
		Call Delay
		
		jmp Start
		
Read:	mov c, P3.4		; read button state
		jc Open			; jump to Open if button is not pressed
		RET
		
		
		; Delays approx. .05 seconds
Delay:	mov r1, #128
		L2:	dec r1
			mov r0, #255
		L1:	dec r0
			mov a, r0
			jnz L1
			mov a, r1
			jnz L2
			RET
			
		; turns all 3 LED's off
Off:	orl P1, #07h ; P1 = XXXXX111
		RET

		; turns left LED on, turns other 2 off
Left:	anl P1, #0F8h  	; P1 = XXXXX000
		orl P1, #03h  	; P1 = XXXXX011
		RET
		
		; turns middle LED on, turns other 2 off
Mid:	anl P1, #0F8h  	; P1 = XXXXX000
		orl P1, #05h  	; P1 = XXXXX101
		RET
		
		; turns right LED on, turns other 2 off
Right:	anl P1, #0F8h  	; P1 = XXXXX000
		orl P1, #06h  	; P1 = XXXXX110
		RET

END
