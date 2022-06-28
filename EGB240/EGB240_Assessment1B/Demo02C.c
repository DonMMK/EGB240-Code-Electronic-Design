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

#define TOP50 0x0462
#define TOP2  0x7A12
#define TOP1  0xF424

#define ARRAY_SIZE 100

/************************************************************************/
/* GLOBAL VARIABLES                                                     */
/************************************************************************/
volatile uint8_t mod_index = 0;

uint16_t input[ARRAY_SIZE] =  {  25,   44,   62,   81,   99,  118,  137,  
  155,  174,  193,  211,  230,  248,  267,  286,  304,  323,  342,  360, 
  379,  397,  416,  435,  453,  472,  491,  509,  528,  546,  565,  584, 
  602,  621,  640,  658,  677,  695,  714,  733,  751,  770,  789,  807, 
  826,  844,  863,  882,  900,  919,  938,  938,  919,  900,  882,  863,  
   44,  826,  807,  789,  770,  751,  733,  714,  695,  677,  658,  640,  
  621,  602,  584,  565,  546,  528,  509,  491,  472,  453,  435,  416,  
  397,  379,  360,  342,  323,  304,  286,  267,  248,  230,  211,  193,  
  174,  155,  137,  118,   99,   81,   62,   44,   25 };

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
	OCR1A  = TOP1;      // set TOP to 0xF424 for f_PWM = 1Hz
	OCR1B  = TOP1/2;    // initialise to 50% duty cycle
	TIMSK1 = 0x04;      // Output Compare B Match Interrupt Enable
	
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
		OCR1A  = TOP50;     // set TOP to 0xF424 for f_PWM = 50Hz

	  } else if (pb_pressed & S3) {  
		OCR1A  = TOP2;    // set TOP to 0xF424 for f_PWM = 2Hz

	  } else if (pb_pressed & S2) {  
		OCR1A  = TOP1;    // set TOP to 0xF424 for f_PWM = 1Hz
	  } /*if*/
	} /*while*/

}/*end main*/

/**
 *  ISR: Output Compare B Match Interrupt
 * 
 * Get new value for OCR1B
 */
ISR(TIMER1_COMPB_vect) {
	OCR1B = input[mod_index];		// Load new value from array
	mod_index = (mod_index == ARRAY_SIZE-1)? 0 : mod_index + 1;
}/*ISR*/