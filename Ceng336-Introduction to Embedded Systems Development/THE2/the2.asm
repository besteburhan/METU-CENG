; Beyza Butun : 2171411
; Necla Nur Akalin : 2171148
; Beste Burhan : 2171395
; We implemented THE2_v2 on simulation environment
; We checked the assignment on board.
;   1-) Although 7-Segment Display work without a problem in simulator, on board we couldn't
;	display level and health point at the same time. It means, 7-Segment Display
;	showed only level or health, not both on board.
;   2-) In simulation environment, whenever a bar couldn't catch a ball in PortA,
;	the health point is decreased by one. However, when we tried it on board,
;	when bar couldn't catch the ball in PortA, the health point didn't decrease.
    
LIST P = 18F8722
#include <p18f8722.inc>

CONFIG OSC = HSPLL, FCMEN = OFF, IESO = OFF, PWRT = OFF, BOREN = OFF, WDT = OFF, MCLRE = ON, LPT1OSC = OFF, LVP = OFF, XINST = OFF, DEBUG = OFF
    
level	    udata 0x20
level
	    
health_point	    udata 0x21
health_point

timer0_val	udata 0x22
timer0_val

timer1_val	udata 0x23
timer1_val

ball_count	udata 0x24
ball_count

timer0_overflowed_flag	udata 0x25
timer0_overflowed_flag

mod_value		udata 0x26
mod_value

bar_left 		udata 0x27
bar_left

shift_value_L		udata 0x28
shift_value_L

shift_value_H		udata 0x29
shift_value_H

last_ball udata	0x30		; LAST_BALLS
last_ball			

 
ORG 0x00
goto main
    
ORG 0x08
goto isr_timeinterval
   
TABLE_7SEGMENT:
    RLNCF   WREG, W    ; multiply index X2
    ADDWF   PCL, f	    ; modify program counter
    RETLW b'00111111' ;0 representation in 7-seg. disp. portJ
    RETLW b'00000110' ;1 representation in 7-seg. disp. portJ
    RETLW b'01011011' ;2 representation in 7-seg. disp. portJ
    RETLW b'01001111' ;3 representation in 7-seg. disp. portJ
    RETLW b'01100110' ;4 representation in 7-seg. disp. portJ
    RETLW b'01101101' ;5 representation in 7-seg. disp. portJ

init:
    MOVLW   h'0F'	; configure A/D
    MOVWF   ADCON1	; for digital inputs
    
    ; disable interrupts
    CLRF    INTCON
    
    ; Initialize Timer0 
    MOVLW   b'01000110' 
    MOVWF   T0CON
    CLRF TMR0
    
    ;Initialize Timer1
    MOVLW   b'1110000' 	
    MOVWF   T1CON 
    CLRF TMR1L
    CLRF TMR1H
    BSF T1CON, 0    ;enable timer1
    
    CLRF    PORTA
    CLRF    PORTB
    CLRF    PORTC
    CLRF    PORTD
    CLRF    LATA
    CLRF    LATB
    CLRF    LATC
    CLRF    LATD
    MOVLW   b'00000000'	
    MOVWF   TRISA
    MOVWF   TRISB
    MOVWF   TRISC
    MOVWF   TRISD
    
    ; for 7 display segment :
    CLRF    PORTH		
    CLRF    LATH		
    CLRF    PORTJ
    CLRF    LATJ
    MOVWF   TRISH
    MOVWF   TRISJ
    
    CLRF    PORTG
    CLRF    LATG
    MOVLW   b'00001101'	
    MOVWF   TRISG	; set RG0, RG2, RG3
    
init_variables:    
    ; initially bar in RA5-RB5
    MOVLW   b'00100000'
    MOVWF   LATA
    MOVWF   LATB
    MOVLW d'0'
    MOVWF   bar_left
    MOVLW h'05'
    MOVWF health_point
    MOVLW h'01'
    MOVWF level
    
    call display_health_level
    
    MOVLW  d'36'	; LAST_BALLS
    MOVWF last_ball
    
    MOVLW   d'30'
    MOVWF   ball_count 
    return
    
check_RG0: 			;check pressed_rg0
    BTFSS PORTG, 0		
    GOTO    check_RG0		; if RG0 not pressed
    GOTO    check_RG0_release
    
check_RG0_release:
    BTFSC   PORTG, 0
    GOTO    check_RG0_release
    CALL    init_variables
    BSF	T0CON, 7	; enables timer0
    BCF T1CON, 0	; disable timer1
    BSF INTCON, 5	; enable timer0 interrupt
    BSF INTCON, 7
    CALL    tmr0_500ms
    CALL    balls_create_down
    GOTO    level_1  
    
level_1:			;Start with level1	
    CALL    check_overflow
    CALL    check_RG2		; to move the bar right
    CALL    check_RG3		; to move the bar left
    MOVLW   d'26'							
    CPFSLT  ball_count		;if less than 26 which means if it is 25 go next level		
    GOTO    level_1
    MOVLW   d'2'
    MOVWF   level
    CALL    display_health_level
    CALL    tmr0_400ms        
    GOTO    level_2
      
check_RG3:
    BTFSS PORTG, 3	
    return 	; if RG3 not pressed
    GOTO    check_RG3_release
    
check_RG3_release:
    BTFSC	PORTG, 3
    GOTO	check_RG3_release
    MOVLW d'1'
    CPFSLT bar_left,0
    GOTO	move_left_bar_2
    return					; cant move the bar left

move_left_bar_2:
    MOVLW d'2'
    CPFSLT bar_left,0
    GOTO	move_left_bar_3
    BSF		PORTA, 5 		; sets the new left of the bar
    BCF		PORTC, 5		; clears the right of the bar
    MOVLW	d'0'			; updates bar_left
    MOVWF	bar_left
    return
    
move_left_bar_3:
    MOVLW   d'3'
    CPFSLT  bar_left,0
    return
    BSF	PORTB, 5 		; sets the new left of the bar
    BCF	PORTD, 5		; clears the right of the bar
    MOVLW   d'1'			; updates bar_left
    MOVWF   bar_left
    return
 
check_RG2:
    BTFSS PORTG, 2		
    return 	; if RG2 not pressed
    GOTO    check_RG2_release
    
check_RG2_release:
    BTFSC   PORTG, 2
    GOTO    check_RG2_release
    MOVLW d'1'
    CPFSLT  bar_left,0
    GOTO    move_right_bar_2
    BCF	PORTA, 5 		; clears it as moved to right
    BSF	PORTC, 5		; sets new right of the bar
    MOVLW   d'1'		; updates bar_left
    MOVWF   bar_left
    return

move_right_bar_2:
    MOVLW d'2'
    CPFSLT bar_left,0
    return			; cant move the bar right
    BCF	PORTB, 5 		; clears it as moved to right
    BSF	PORTD, 5		; sets new right of the bar
    MOVLW   d'2'		; updates bar_left
    MOVWF   bar_left
    return

      
check_overflow:
    BTFSS timer0_overflowed_flag, 0	;Bit Test f, Skip if set
    return
    CALL    balls_create_down
    CALL    set_tmr_wrt_level    
    CLRF TMR0
    BCF	timer0_overflowed_flag, 0
    return

balls_create_down:
    DECF    last_ball
    MOVLW   B'00000111'
    ANDWF   TMR1L,W,0		;Take last 3 bits of timer1
    MOVWF   mod_value		;mod_value=last 3 bits of timer1
    MOVLW d'4'
    CPFSLT  mod_value,0
    SUBWF   mod_value,1,0	
    RLNCF LATA, 1
    RLNCF LATB, 1
    RLNCF LATC, 1
    RLNCF LATD, 1
    CALL    check_barleft
    TSTFSZ  ball_count
    CALL    create_random_ball
    TSTFSZ  ball_count
    DECF    ball_count
    TSTFSZ  ball_count
    CALL    timer1_shifter
    return
    
decrease_health:
    DECF    health_point				; not sure whether to end the game immediately after hits zero -nec
    CALL    display_health_level
    MOVLW d'0'
    CPFSEQ  health_point, 0		; checks whether there is still health after collisions, goes to main if no left
    return
    CALL    zero_health
    GOTO    check_RG0

zero_health:
    BCF T0CON, 7	; disable timer0
    BSF T1CON, 0	; enable timer1
    BCF INTCON, 5	; disable timer0 interrupt
    BCF INTCON, 7
    CLRF TMR0
    CLRF TMR1L
    CLRF TMR1H
    CLRF PORTA
    CLRF PORTB
    CLRF PORTC
    CLRF PORTD
    MOVLW   b'00100000'
    MOVWF   LATA
    MOVWF   LATB
    return

check_barleft:
    MOVLW d'1'
    CPFSLT  bar_left,0
    GOTO    check_barleft1
    BCF LATA,6
    BCF LATB,6
    BSF LATA,5
    BSF LATB,5
    BTFSC LATC,6
    CALL    decrease_health
    BCF PORTC, 6
    BTFSC LATD,6
    CALL    decrease_health
    BCF PORTD, 6
    return
check_barleft1:
    MOVLW d'2'
    CPFSLT  bar_left,0
    GOTO    check_barleft2
    BCF LATB,6
    BCF LATC,6
    BSF LATB,5
    BSF LATC,5
    BTFSC LATA,6
    CALL    decrease_health
    BCF PORTA, 6
    BTFSC LATD,6
    CALL    decrease_health
    BCF PORTD, 6
    return
check_barleft2:
    BCF LATC,6
    BCF LATD,6
    BSF LATC,5
    BSF LATD,5
    BTFSC LATA,6
    CALL    decrease_health
    BCF PORTA, 6
    BTFSC LATB,6
    CALL    decrease_health
    BCF PORTB, 6
    return
	
create_random_ball:
    MOVLW d'1'
    CPFSLT  mod_value,0		;if mod_value==0
    GOTO    random_ball_1
    BSF LATA,0
    return
random_ball_1:
    MOVLW d'2'
    CPFSLT  mod_value,0		;if mod_value==1
    GOTO    random_ball_2
    BSF LATB,0
    return
random_ball_2:
    MOVLW d'3'
    CPFSLT  mod_value,0		;if mod_value==2
    GOTO    random_ball_3
    BSF LATC,0
    return
random_ball_3:
    BSF LATD,0
    return
	
timer1_shifter:						;Shift timer1 value wrt level
    MOVLW d'01'
    CPFSGT  level	; if level>1; skip next instruction
    GOTO    timer1_shifter_level1
    MOVLW d'02'
    CPFSGT  level	; if level>2; skip next instruction
    GOTO    timer1_shifter_level2
    GOTO    timer1_shifter_level3
    
timer1_shifter_level1:
    RRNCF TMR1L,1,0
    RRNCF TMR1H,1,0
    MOVLW B'10000000'
    ANDWF TMR1H,W,0		;Take first 1 bit of shifted timer1H
    MOVWF shift_value_H
    MOVLW B'10000000'
    ANDWF TMR1L,W,0		;Take first 1 bit of shifted timer1L
    MOVWF shift_value_L
    MOVLW B'01111111'
    ANDWF TMR1L,W,0		
    IORWF shift_value_H,W,0
    MOVWF TMR1L
    MOVLW B'01111111'
    ANDWF TMR1H,W,0		
    IORWF shift_value_L,W,0
    MOVWF TMR1H
    return
timer1_shifter_level2:
    RRNCF TMR1L,1,0
    RRNCF TMR1L,1,0
    RRNCF TMR1L,1,0
    RRNCF TMR1H,1,0
    RRNCF TMR1H,1,0
    RRNCF TMR1H,1,0
    MOVLW B'11100000'
    ANDWF TMR1H,W,0		;Take first 3 bits of shifted timer1H
    MOVWF shift_value_H
    MOVLW B'11100000'
    ANDWF TMR1L,W,0		;Take first 3 bits of shifted timer1L
    MOVWF shift_value_L
    MOVLW B'00011111'
    ANDWF TMR1L,W,0		
    IORWF shift_value_H,W,0
    MOVWF TMR1L
    MOVLW B'00011111'
    ANDWF TMR1H,W,0
    IORWF shift_value_L,W,0
    MOVWF TMR1H
    return
    
timer1_shifter_level3:
    RLNCF TMR1L,1,0
    RLNCF TMR1L,1,0
    RLNCF TMR1L,1,0                                                                               
    RLNCF TMR1H,1,0
    RLNCF TMR1H,1,0
    RLNCF TMR1H,1,0                                                                               
    MOVLW B'11111000'
    ANDWF TMR1H,W,0		;Take first 5 bits of shifted timer1H
    MOVWF shift_value_H
    MOVLW B'11111000'
    ANDWF TMR1L,W,0		;Take first 5 bits of shifted timer1L
    MOVWF shift_value_L
    MOVLW B'00000111'
    ANDWF TMR1L,W,0		
    IORWF shift_value_H,W,0
    MOVWF TMR1L
    MOVLW B'00000111'
    ANDWF TMR1H,W,0		
    IORWF shift_value_L,W,0
    MOVWF TMR1H
    return
	
level_2:
    CALL    check_overflow
    CALL    check_RG2			
    CALL    check_RG3
    MOVLW   d'16'
    CPFSLT  ball_count
    GOTO    level_2
    MOVLW   d'3'
    MOVWF   level
    CALL    display_health_level
    CALL    tmr0_350ms
    GOTO    level_3		; if ball_count <= 15
                                                                               
level_3:
    CALL    check_overflow
    CALL    check_RG2					
    CALL    check_RG3
    TSTFSZ  ball_count		
    GOTO    level_3				
    GOTO    check_last_ball

check_last_ball:
    TSTFSZ  last_ball
    GOTO    level_3
    CALL    zero_health
    GOTO    check_RG0
    
tmr0_500ms:
    MOVLW   d'153'
    MOVWF   timer0_val
    return
    
tmr0_400ms:
    MOVLW   d'123'
    MOVWF   timer0_val
    return
    
tmr0_350ms:
    MOVLW   d'107'
    MOVWF   timer0_val
    return

isr_timeinterval:
    BTFSC   INTCON, TMR0IF  ; checkes whether tmr0 overflows, skips if not
    BRA     tmr0_overflowed
                            ; HERE add interrupt for tmr1 ; no, no interrupts are needed for tmr1
    RETFIE

tmr0_overflowed:
    BCF     INTCON, TMR0IF
    DECFSZ    timer0_val     
    RETFIE
    BSF	timer0_overflowed_flag, 0
    RETFIE 
    
set_tmr_wrt_level:
    MOVLW d'01'
    CPFSGT level    ; if level>1; skip next instruction
    GOTO tmr0_500ms 
    MOVLW d'02'
    CPFSGT level    ; if level>2; skip next instruction
    GOTO tmr0_400ms 
    GOTO tmr0_350ms
    
display_health_level:
    CLRF LATH
    BSF LATH, 0
    MOVF level, W      
    CALL TABLE_7SEGMENT
    MOVWF LATJ
    CLRF LATH
    BSF LATH, 3
    MOVF health_point, W      
    CALL TABLE_7SEGMENT
    MOVWF LATJ
    return   
main
    CALL    init
    CALL    check_RG0
    GOTO    main		
    end 
