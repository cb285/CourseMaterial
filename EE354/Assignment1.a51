; Clayton Brutus
; Assignment 1
; 9/1/2016
; 
; Connect LED's (active low) to pins P1.0 and P1.1
; Connect pushbutton to P3.7
; when P3.7 is pulled low: Flash both LED's approx. 10 times a second
; when P3.7 is pulled high: LED on P1.0 is on, LED on P1.1 is off

MainSeg SEGMENT CODE
CSEG at 0
	ljmp Start
RSEG MainSeg
	
	Start:	mov a, P3    	; read P3 into accumulator
			anl a, #80h  	; see if P3.7 is 1 or 0
			jz L5       	; if P3.7 is 0, then jump to L5
			
			clr P1.0		; turn P1.0 LED on
			clr P1.1		; turn P1.1 LED on
			; delay approx. .05 seconds
			mov r2, #128
		L2:	dec r2
			mov r1, #255
		L1:	dec r1
			mov a, r1
			jnz L1
			mov a, r2
			jnz L2
			
			setb P1.0 		; turn P1.0 LED off
			setb P1.1		; turn P1.1 LED on
			
			; delay approx. .05 seconds
			mov r2, #128
		L4:	dec r2
			mov r1, #255
		L3:	dec r1
			mov a, r1
			jnz L3
			mov a, r2
			jnz L4
			
			jmp Start 		; jump back to start
			
		L5: setb P1.1 		; turn P1.1 led off
			clr P1.0  		; turn P1.0 led on
			
			jmp Start 		; jump back to start
			
END
