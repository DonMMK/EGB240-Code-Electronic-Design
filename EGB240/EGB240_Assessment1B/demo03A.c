/************************************************************************/
 /* demo01B_main.c - EGB240 Demo01B                                      */
 /*                                                                      */
 /* Simple I/O                                                           */
 /* If pushbuttons are pressed, lights up corresponding LED.             */
 /* Shorter code than demo01A.c                                          */
 /*                                                                      */
 /* Jasmine Banks                                                        */
 /* Revised Mar 2020                                                     */
 /*                                                                      */
 /************************************************************************/ 

 /************************************************************************/
 /* INCLUDED LIBRARIES/HEADER FILES                                      */
 /************************************************************************/
#include <avr/io.h>
#include <avr/interrupt.h>

/************************************************************************/
/* DEFINES                                                              */
/************************************************************************/
#define S1 0x10
#define S2 0x20
#define S3 0x40
#define S4 0x80

#define LED1 0x10
#define LED2 0x20
#define LED3 0x40
#define LED4 0x80

#define LEDS_OFF 0x0F

/************************************************************************/
/* ENUM DEFINITIONS                                                     */
/************************************************************************/
enum {
	STATE1,
	STATE2,
	STATE3,
	STATE4
};

/************************************************************************/
/* GLOBAL VARIABLES                                                     */
/************************************************************************/
uint8_t pushbuttons;

/************************************************************************/
/* Initialisation                                                       */
/************************************************************************/
void init() 
{
	cli();  //Disable interrupts
	
	//Set sysclk to 16 MHz
	CLKPR = 0x80; // Prescaler change enable
	CLKPR = 0x00; // Set prescaler to zero
	
    DDRF &= 0x0F;    // Set PORTF bits 7-4 as inputs (PBs)	
	DDRD |= 0xF0;    // Set PORTD bits 7-4 as outputs (LEDs)
	PORTD &= 0x0F;   // initalise LEDS to off
	
	sei();  //Enable interrupts	
}/*init*/

/************************************************************************/
/* MAIN LOOP (CODE ENTRY)                                               */
/************************************************************************/
int main(void) 
{
	uint8_t pb_now=0, pb_prev=0, pb_pressed;
	uint8_t state = STATE1;

	init();

	// main program loop
    while(1)
	{
		pushbuttons = ~PINF;

    	// determine if any pushbutton pressed
    	pb_now = pushbuttons;
		pb_pressed = pb_now & (pb_now ^ pb_prev);
		pb_prev = pb_now;

		if (pb_pressed & S3)
	        state = STATE1;

		switch (state)
        {
	        case STATE1:   PORTD &= LEDS_OFF;
			               PORTD |= LED1;           // turn LED1 on
	                       if (pb_pressed & S4)
	                           state = STATE2;
	                       break;

  	        case STATE2:   PORTD &= LEDS_OFF;
			               PORTD |= LED2;           // turn LED2 on
	                       if (pb_pressed & S4)
	                           state = STATE3;
	                       break;

	        case STATE3:   PORTD &= LEDS_OFF;
			               PORTD |= LED3;           // turn LED3 on
	                       if (pb_pressed & S4)
	                           state = STATE4;
	                       break;

	        case STATE4:   PORTD &= LEDS_OFF;
			               PORTD |= LED4;           // turn LED4 on
	                       if (pb_pressed & S4)
	                           state = STATE1;
 	                       break;
						   
			default:       state = STATE1;	                       
		}//switch
	} /*while*/

}/*end main*/