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
	
    DDRF = DDRF & 0b00001111;    // Set PORTF bits 7-4 as inputs (PBs)
	
	DDRD |= 0b11110000;    // Set PORTD bits 7-4 as outputs (LEDs)
	PORTD &= 0b00001111;
	
	sei();  //Enable interrupts	
}/*init*/

/************************************************************************/
/* MAIN LOOP (CODE ENTRY)                                               */
/************************************************************************/
int main(void) 
{
	init();

	// main program loop
    while(1)
	{
	  pushbuttons = ~PINF;

	  PORTD = pushbuttons & 0xF0;

	} /*while*/

}/*end main*/