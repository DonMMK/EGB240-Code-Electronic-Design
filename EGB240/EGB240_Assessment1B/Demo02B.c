/************************************************************************/
/* demo02A_main.c - EGB240 Demo02A                                      */
/*                                                                      */
/* PWM demo2A                                                           */
/* Sets B6 to PWM output using OC1B                                     */
/* Use pushbuttons to select:                                           */
/*     f_PWM = 50 Hz, and duty cycle = 50%                              */
/*     f_PWM = 2 Hz, and duty cycle = 50%                               */
/*     f_PWM = 1 Hz, and duty cycle = 50%                               */
/*                                                                      */
/* Jasmine Banks                                                        */
/* Mar 2020                                                             */
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

	DDRB |=  0x40;   // Set PORTB bit 6 as output

	// Initialise Timer1 
	// Using OC1B, connected to B6
	TCCR1A = 0x23;      // fast PWM, clear on CMP, TOP in OCR1A
	TCCR1B = 0x1C;      // fast PWM, TOP in OCR1A, prescaler 256
	OCR1A  = 0xF424;    // set TOP to 0xF424 for f_PWM = 1Hz
	OCR1B  = 0xF424/2;  // initialise to 50% duty cycle
	
	sei();  //Enable interrupts	
}/*init*/

/************************************************************************/
/* MAIN LOOP (CODE ENTRY)                                               */
/************************************************************************/
int main(void) 
{
	uint8_t pb_now=0, pb_prev=0, pb_pressed;

	init();

	// main program loop
    while(1)
	{
	  pushbuttons = ~PINF;           // read pushbuttons

      // determine if any pushbutton pressed
      pb_now = pushbuttons;
	  pb_pressed = pb_now & (pb_now ^ pb_prev);
	  pb_prev = pb_now;

	  PORTD = pushbuttons & 0xF0;    // output PBs to LEDs

      if (pb_pressed & S4) {
		OCR1A  = 0x0462;    // set TOP to 0xF424 for f_PWM = 50Hz
		OCR1B  = 0x0462/2;  // set to 50% duty cycle

	  } else if (pb_pressed & S3) { 
		OCR1A  = 0x7A12;    // set TOP to 0xF424 for f_PWM = 2Hz
		OCR1B  = 0x7A12/2;  // set to 50% duty cycle

	  } else if (pb_pressed & S2) { 
		OCR1A  = 0xF424;    // set TOP to 0xF424 for f_PWM = 1Hz
		OCR1B  = 0xF424/2;  // set to 50% duty cycle
	  } /*if*/
	} /*while*/

}/*end main*/