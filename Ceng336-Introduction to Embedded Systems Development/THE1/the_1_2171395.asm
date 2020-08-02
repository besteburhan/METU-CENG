    LIST P = 18F8722
    #include <p18f8722.inc>

    CONFIG OSC = HSPLL, FCMEN = OFF, IESO = OFF, PWRT = OFF, BOREN = OFF, WDT = OFF, MCLRE = ON, LPT1OSC = OFF, LVP = OFF, XINST = OFF, DEBUG = OFF

t1	res 1	; used in delay
t2	res 1	; used in delay
t3	res 1	; used in delay
	
operation_type udata 0x20
operation_type

portb_counter udata 0x21
portb_counter

portc_counter udata 0x22
portc_counter

    org 0; start code at 0
    call init
    goto turn_on_rb_rc_rd

init:
    MOVLW 0FH		; configure A/D
    MOVWF ADCON1	; for digital inputs
    CLRF PORTA	
    CLRF LATA
    MOVLW b'00010000'	; set RA[4] input
    MOVWF TRISA
    CLRF PORTB
    CLRF LATB
    CLRF PORTC
    CLRF LATC
    CLRF PORTD
    CLRF LATD
    MOVLW b'00000000'	; all of them output
    MOVWF TRISB
    MOVWF TRISC
    MOVWF TRISD
    CLRF PORTE
    CLRF LATE
    MOVLW b'00011000'	; set RE[3-4]
    MOVWF TRISE
    return

DELAY:	; Time Delay Routine with 3 nested loops
    MOVLW 82	; Copy desired value to W
    MOVWF t3	; Copy W into t3
    _loop3:
	MOVLW 0xA0  ; Copy desired value to W
	MOVWF t2    ; Copy W into t2
	_loop2:
	    MOVLW 0x9F	; Copy desired value to W
	    MOVWF t1	; Copy W into t1
	    _loop1:
		decfsz t1,F ; Decrement t1. If 0 Skip next instruction
		GOTO _loop1 ; ELSE Keep counting down
		decfsz t2,F ; Decrement t2. If 0 Skip next instruction
		GOTO _loop2 ; ELSE Keep counting down
		decfsz t3,F ; Decrement t3. If 0 Skip next instruction
		GOTO _loop3 ; ELSE Keep counting down
		return
    
turn_on_rb_rc_rd:
    MOVLW b'00001111'
    MOVWF LATB
    MOVWF LATC
    MOVLW b'11111111'
    MOVWF LATD
    NOP	; to debug turn on leds
    ; wait for 1 second
turn_off_rb_rc_rd:
    call DELAY
    MOVLW b'00000000'
    MOVWF LATB
    MOVWF LATC
    MOVWF LATD
    NOP	; to debug turn off leds
    

compute_operation: 
    ; initialize operation_type, portb_counter, portb_counter
    MOVLW 0
    MOVWF portb_counter
    MOVWF portc_counter
    MOVLW -1
    MOVWF operation_type
    press_ra4_first_time:
	btfss PORTA, 4		; if RA4 pressed, next instruction will not be computed
	goto press_ra4_first_time
    release_ra4_first_time:
	btfsc   PORTA, 4	; if RA4 released, next instruction will not be computed
	goto release_ra4_first_time
	incf operation_type	; operation is addition firstly
	goto change_portb

change_portb:
    pressed_re3_for_portb:
	btfss PORTE, 3	    ; if RE3 pressed, next instruction will not be computed
	goto check_operation_change	; operation will change
    released_re3_for_portb:
	btfsc PORTE, 3	    ; if RA3 released(portb selected), next instruction will not be computed
	goto released_re3_for_portb
	call portb_enter_value
    
check_operation_change:
    press_ra4:
	btfss PORTA, 4	    ; if RA4 pressed, next instruction will not be computed
	goto change_portb
    release_ra4:
	btfsc   PORTA, 4	    ; if RA4 released, next instruction will not be computed
	goto release_ra4
	TSTFSZ operation_type	    ; if operation_type = 0(addition), next instruction will not be computed
	decfsz operation_type	    ; if operation_type = 0, next instruction will not be computed
	incf operation_type
	goto change_portb

portb_enter_value:
    press_re4_for_portb:
	btfss PORTE, 4	    ; if RE4 pressed, skip next instruction
	goto change_portc
    release_re4_for_portb:
	btfsc PORTE, 4	    ; if RE4 released, skip next instruction
	goto release_re4_for_portb
	MOVLW b'00001111'
	CPFSLT portb_counter    ; if portb count < 4, skip next instruction
	goto set_zero_portb
	rlncf portb_counter
	incf portb_counter
	MOVFF portb_counter, LATB  
	goto portb_enter_value
	
set_zero_portb:
    MOVLW 0
    MOVWF portb_counter
    MOVFF portb_counter, LATB   ; ??? 2 yi LATB ye 00000010 diye atacaksa s?k?nt? bak bi
    goto portb_enter_value
    
change_portc:
    pressed_re3_for_portc:
	btfss PORTE, 3	    ; if RE3 pressed, next instruction will not be computed
	goto portb_enter_value
    released_re3_for_portc:
	btfsc PORTE, 3	    ; if RA3 released(portb selected), next instruction will not be computed
	goto released_re3_for_portc
	call portc_enter_value

portc_enter_value:
    press_re4_for_portc:
	btfss PORTE, 4	    ; if RE4 pressed, skip next instruction
	goto change_portd
    release_re4_for_portc:
	btfsc PORTE, 4	    ; if RE4 released, skip next instruction
	goto release_re4_for_portc
	MOVLW b'00001111'
	CPFSLT portc_counter    ; if portc count < 4, skip next instruction
	goto set_zero_portc
	rlncf portc_counter
	incf portc_counter
	MOVFF portc_counter, LATC  
	goto portc_enter_value
	
set_zero_portc:
    MOVLW 0
    MOVWF portc_counter
    MOVFF portc_counter, LATC  
    goto portc_enter_value
     
change_portd:
    pressed_re3_for_portd:
	btfss PORTE, 3	    ; if RE3 pressed, next instruction will not be computed
	goto portc_enter_value
    released_re3_for_portd:
	btfsc PORTE, 3	    ; if RA3 released(portb selected), next instruction will not be computed
	goto released_re3_for_portd
	call compute_portd

compute_portd:
    TSTFSZ operation_type   ; if operation_type = 0, skip next instruction
    goto subtraction
    goto addition
    
subtraction:
    MOVF portb_counter, W
    CPFSGT portc_counter, W ; if c > b, skip next instruction
    goto shifting_b  ; c < b
    MOVFF portc_counter, portb_counter
    MOVWF portc_counter
    goto shifting_b  
        
shifting_b:
    TSTFSZ portc_counter
    goto shift_right_b
    MOVF portb_counter, W
    MOVWF LATD
    goto turn_off_rb_rc_rd
    
shift_right_b:
    RRCF portb_counter 
    BCF portb_counter, 7
    RRCF portc_counter 
    BCF portc_counter, 7
    goto shifting_b

addition:
    TSTFSZ portb_counter
    goto shift_left
    MOVF portc_counter, W
    MOVWF LATD
    goto turn_off_rb_rc_rd
    
shift_left:
    rlncf portc_counter
    incf portc_counter
    rrncf portb_counter
    BCF portb_counter, 7
    goto addition
  	
    end