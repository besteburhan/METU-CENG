 /*
  * Group 14
  * Beyza Butun : 2171411
  * Necla Nur Akalin : 2171148
  * Beste Burhan : 2171395
 */

#pragma config OSC = HSPLL, FCMEN = OFF, IESO = OFF, PWRT = OFF, BOREN = OFF, WDT = OFF, MCLRE = ON, LPT1OSC = OFF, LVP = OFF, XINST = OFF, DEBUG = OFF

#include <xc.h>
#include "breakpoints.h"

/* Global Variables*/
/*
 *  RB4Pressed : It will be set, if RB4 is really pressed
 *  timer1Interrupted, timer0Interrupted, ADCInterrupted : They are flags to store the information of their interruption occurred
 *  timer1Value : this value is used in timer1 interrupt to count down from 100 because timer1 has an interrupt in every 50ms 
 *          and we have to call hs_passed() when there is an interrupt 10 times(500ms) and finish the game when it is 100 times interrupted.
 *  timer1Display : When the game over, we count down from 40 in timer1(it interrupted in every 50ms)
 *          in 40-30 and 20-10 range, we turned on 7Segment display, 
 *          in 30-20 and 10-0 range, we turned off 7Segment display
 *  gameFinished : It is flag to store the information of game finished
 *  debouncing_start, debounced : It is set, when RB4 is reset after its succesfull setting. 
 *          After 10ms if RB4 is still reset debounced will be set. Initially debounced = 1 
 */
unsigned int RB4Pressed=0, timer1Interrupted=0, timer0Interrupted=0, ADCInterrupted=0;
unsigned int timer1Value, timer1Display=40;
unsigned int gameFinished=0;
unsigned int debouncing_start = 0, debounced = 1; 


/* Functions*/
/*
 *  initADC, initLedsAnd7Seg, initTimers : They make initial configurations
 *  updateLedsUp, updateLedsDown : Display hints in leds
 *  ADCMapper : Maps 0-1023 to 0-9
 *  turnOffLeds : Clear all hint leds when game over 
 *  displayNumber(displayed_number) : It display mapped adc_value or special number on 7Segment Display
 *  turnOff7Segment : It turn off 7 segment, after game over and displayed number in 10ms interval
 *  RB4Interrupt : It check adc_value(guessed number) and compares it with special number
 *  timer0Interrupt : It set GODONE bit to start A/D conversion
 *  timer1Interrupt : if timer1Value is a multiple of 10, it means 500ms passed and 
 *              hs_passed() will be called. Also if timer1Value is 0, it means 5seconds passed
 *              gameFinished flag will be set and special number will be displayed on 7Segment
 *              If already game finished, It checks the intervals to turn on or turn off 7 segment
 *  ADCInterrupt : It reads and maps adc_value, then display it in 7Segment
 *  In main routine : Initial configurations are made, then in a loop interruption flags are checked
 * 
 */

void initADC(void){
	/*
    * ADCON0 = b'00110000', we use 12th channel
    * ADCON1 = b'00000010', we set 12th channel as analog.
    * ADCON2 = b'10101010', it is right-justified, Acquisition Time => 12TAD, A/D Conversion Clock = Fosc/32 (010) 
    * GO/DONE: A/D Conversion Status bit
    */
    ADCON0 = 48;
    ADCON1 = 0;
    ADCON2 = 170;
  	ADON = 1;			//A/D converter is enabled. Enable AD converter, but not set GO/DONE yet. It will be set when Timer0 is interrupted.
    ADIE = 1;
    
}

void initLedsAnd7Seg(void){
    /* Initialization for 7segment, Buttons, Leds */
    /*7-Segment Display*/
    PORTH = 8;
    PORTJ = 0;
    LATH = 8;
    LATJ = 0;
    TRISH = 0;
    TRISJ = 0;
    
    /* Leds are configured to show up-down.*/
    LATC = 0;  LATD = 0;  LATE = 0; 
    PORTC = 0; PORTD = 0; PORTE = 0;
    TRISC = 0;
    TRISD = 0;
    TRISE = 0;
    
	/* RB4 is configured as input */
    INTCON = 0; 
    LATB = 0; 
    PORTB = 0; 
    TRISB = 0;
	TRISB4 = 1;
    
    /* RBIE Interrupt is enabled-RB4*/
    RBIE = 1;
    
    //set AN12 (Channel 12 as input))
    TRISH4 = 1;
    
    PEIE = 1;
}

void initTimers(void){
    
    /* T0CON = b'00000010'
     * Prescalar = 010 -> 1:8 */
    T0CON = 2 ; 
	TMR0 = 3036; // (65536-3036)*8 = 500000 (50ms))

	/* Timer0 Interrupt is enabled. */
    TMR0IE = 1 ; 
    
    /* T1CON = b'11110000' = d'240'
     * T1CON7 = 16-bit operation(1), T1CON6 = Device clock is derived from Timer1 oscillator(1), 
     * T1CON{5,4} = Prescalar -> 1/8 (11)
     * T1CON0 = Timer1 is enabled  
     */
  	
    timer1Value = 100;
    T1CON = 240; 
    TMR1 = 3036;    // (65536-3036)*8 = 500000 (50ms))
    /* Timer1 Interrupt is enabled*/
    TMR1IE = 1 ;
    // TMR3 will use to track debouncing
  	/* T3CON = b'10110000', 16-bit, prescaler: 1:8 */
  	T3CON = 176;
  	TMR3IE = 1;
  	TMR3 = 53036; // (65536-53036)*8 = 100000 (10ms)
    
    //start timer0 and timer1
    TMR0ON = 1;
    TMR1ON = 1; 
  	
}

void updateLedsUp(void){
    /*Clear all Leds*/
    PORTC = 0;
    PORTD = 0;
    PORTE = 0; 
    LATC = 0;
    LATD = 0;
    LATE = 0; 
    /*Set Leds as Up*/
    LATC1 = 1;
    LATE1 = 1;
    LATD0 = 1;
    LATD1 = 1;
    LATD2 = 1;
    LATD3 = 1;
    latcde_update_complete();
}

void updateLedsDown(void){
    /*Clear all Leds*/
    PORTC = 0;
    PORTD = 0;
    PORTE = 0; 
    LATC = 0;
    LATD = 0;
    LATE = 0;  
    /*Set Leds as Down*/
    LATC2 = 1;
    LATE2 = 1;
    LATD0 = 1;
    LATD1 = 1;
    LATD2 = 1;
    LATD3 = 1;
    latcde_update_complete();
}

int ADCMapper(){
    if(adc_value <= 102) return 0;
    else if(adc_value > 102 && adc_value <= 204) return 1;
    else if(adc_value > 204 && adc_value <= 306) return 2;
    else if(adc_value > 306 && adc_value <= 408) return 3;
    else if(adc_value > 408 && adc_value <= 510) return 4;
    else if(adc_value > 510 && adc_value <= 612) return 5;
    else if(adc_value > 612 && adc_value <= 714) return 6;
    else if(adc_value > 714 && adc_value <= 816) return 7;
    else if(adc_value > 816 && adc_value <= 918) return 8;
    else if(adc_value > 918) return 9;
}

void turnOffLeds(void){
	/*Clear all Leds*/
    PORTC = 0;
    PORTD = 0;
    PORTE = 0; 
    LATC = 0;
    LATD = 0;
    LATE = 0; 
    latcde_update_complete();
}

void displayNumber(int displayed_number){
    /* b'00111111'  ;	63	;	7-Segment = 0
       b'00000110'  ; 	6	;	7-Segment = 1
       b'01011011'  ; 	91  ;	7-Segment = 2
       b'01001111'  ; 	79	;	7-Segment = 3
       b'01100110'  ; 	102	;	7-Segment = 4
       b'01101101'  ; 	109	;	7-Segment = 5
       b'01111101'  ;	125	; ``7-Segment = 6
       b'00000111'  ; 	7	;	7-Segment = 7
       b'01111111'  ; 	127	;	7-Segment = 8
       b'01101111'  ; 	111	;	7-Segment = 9
    */
    LATH=0;
    LATJ=0;
  	LATH3 = 1;
  	if(displayed_number == 0)	LATJ = 63;
    else if(displayed_number == 1)	LATJ = 6;
    else if(displayed_number == 2)	LATJ = 91;
    else if(displayed_number == 3)	LATJ = 79;
    else if(displayed_number == 4)	LATJ = 102;
    else if(displayed_number == 5)	LATJ = 109;
    else if(displayed_number == 6)	LATJ = 125;
    else if(displayed_number == 7)	LATJ = 7;
    else if(displayed_number == 8)	LATJ = 127;
    else if(displayed_number == 9)	LATJ = 111;
    latjh_update_complete();
}

void turnOff7Segment(void){
  	//Blink 7-Segment Display
    PORTJ = 0;
    LATJ = 0;
    latjh_update_complete();
}

void __interrupt() isr(void){
    //Check which interrupt has occurred
    
    if(RBIF==1){
        int portb_read = PORTB;
        RBIF=0;  
        if((portb_read & 16) == 16){
            if(debounced==1){   
                // if rb4 already wait 10ms in reset mode, 
                // start timer3 to check RB4 will be in set mode for 10ms
                TMR3ON = 0;
                TMR3 = 53036;
                TMR3IE = 1;
                TMR3ON = 1;	// start timer3 to control debouncing
            }             
        }
    }
    if(TMR0IF==1){
        if(gameFinished==0){
            timer0Interrupted=1;
            TMR0 = 3036;
        }
        TMR0IF=0;
    }
    if(TMR1IF){
        timer1Interrupted=1;
        TMR1 = 3036;
        TMR1IF=0;
    }
  	if(TMR3IF){
        int portb_read = PORTB;
        TMR3IF = 0;
        if((portb_read & 16) == 16 && debounced == 1){
            // if rb4 was waiting for 10ms in reset mode and 
            // since tmr3 interrupt raised after 10ms, and RB4 is still in set mode
            // RB4 is really pressed
            RB4Pressed = 1;
            rb4_handled();
            debounced = 0;
        }
        else if((portb_read & 16) != 16 && debouncing_start == 0){
            // if RB4 has been set 10ms ago and it is now in the reset state
            // Set the debouncing_start information 
            debouncing_start = 1;
        }
        else if((portb_read & 16) != 16 && debouncing_start == 1){
            // if RB4 has been reset 10ms ago and is still in the reset state
            // Set the debounced information 
            debounced = 1;
        }
    }
    if(ADIF){   //Conversion is done.
        ADCInterrupted=1;
        ADIF=0;
        ADIE = 0 ;
    }
}

void RB4Interrupt(void){
	// check adc_value was read -> GO/DONE bit is 0
    /* We mapped the ADC value to [0-9] interval. 
     * Then, we check whether the guess is correct.
     * If guess is correct, finished the game, turn-off all leds, display special number until it is blinked.
     * If guess is not correct, give some hit.
    */
  	RB4Pressed = 0;
  	int guessedValue = ADCMapper();
  	int specialNumber = (int) special_number();
  	if(specialNumber == guessedValue){
        correct_guess();
    	gameFinished=1;
      	turnOffLeds();
      	ADON = 0;
        TMR0ON = 0;
        RBIE = 0;     					//Disable RB4 interrupt, because the game is over
        displayNumber(specialNumber);   
        TMR1 = 3036;
        TMR1ON = 1;
    }
    else if(guessedValue < specialNumber){
    	updateLedsUp();
    }
  	else{
    	updateLedsDown();
    }
  	
}

void timer0Interrupt(void){
	/* Start AD CONVERSION
     * So, set GO/DONE and AD interrupt
     */
  	timer0Interrupted = 0;
    ADIE=1;
	GODONE = 1;
}

void timer1Interrupt(void){
  	/* Whenever timer1 generates an interrupt, CHECK whether game is finished or not.
     * If game is not finished and time is up(5sec), make some changes like turning off leds and show/blink speacial number
     * If game is finished, show or blink number
    */
  	timer1Interrupted = 0;
    if(gameFinished==0){
        timer1Value=timer1Value-1;
        if(timer1Value % 10 == 0){
          hs_passed();
        }
        if(timer1Value==0){  //Time is up
            game_over();
            gameFinished=1;
            RBIE = 0;     					//Disable RB4 interrupt, because the game is over
            ADON = 0;
            turnOffLeds();
            int specialNumber = (int) special_number();
            displayNumber(specialNumber); 
            TMR1 = 3036;
        }
    }
  	else{
      	//First 500ms display the special number, then 500ms blink and repeat this one more time
      	timer1Display = timer1Display - 1;
        int specialNumber = (int) special_number();
        if(timer1Display % 10 == 0){
          hs_passed();
        }
      	if(timer1Display==30 || timer1Display==10) 
            turnOff7Segment();
      	else if(timer1Display==20 ) 
            displayNumber(specialNumber);
      	else if(timer1Display==0){
          	//Restart the game
            TMR1ON = 0; 
          	GIE=0;
            restart();
          	gameFinished = 0;
          	timer1Display=40;
            //Initialize all values and start interrupts again
            initADC();
            initLedsAnd7Seg();
            initTimers();
            GIE = 1;
            init_complete();    //We initialized all values, SFRs, etc.
        }
    }
}

void ADCInterrupt(void){
	//Read ADC value and save
    ADCInterrupted = 0;
  	adc_value = (ADRESH<<8) + ADRESL;
    ADIE = 0;
    adc_complete();
  	int adcValue = ADCMapper();
    displayNumber(adcValue);
}

void main(void) {
    //Initialize all values and start interrupts
    initADC();
    initLedsAnd7Seg();
    initTimers();
    GIE = 1;
  	init_complete();

    while(1){   
      	if(ADCInterrupted){
        	ADCInterrupt();
        }
        if(timer0Interrupted){
          	timer0Interrupt();
        }
        if(timer1Interrupted){
        	timer1Interrupt();
        }
        if(RB4Pressed){
            RB4Interrupt();
        }
    } 
}