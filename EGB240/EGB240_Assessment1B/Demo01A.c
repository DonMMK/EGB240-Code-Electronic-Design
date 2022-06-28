/************************************************************************/
 /* demo01_main.c - EGB240 Demo01                                        */
 /*                                                                      */
 /* Simple I/O                                                           */
 /* Checks if each pushbutton in turn is pressed, and lights up a        */
 /* corresponding LED.                                                   */
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
/* Initialisation                                                       */
/************************************************************************/
void init() 
{
	cli();  //Disable interrupts
	
	//Set sysclk to 16 MHz
	CLKPR = 0x80; // Prescaler change enable
	CLKPR = 0x00; // Set prescaler to zero
	
    DDRF = 0b00001111;    // Set PORTF bits 7-4 as inputs (PBs)
	
	DDRD |= 0b11110000;    // Set PORTD bits 7-4 as outputs (LEDs)
	PORTD &= 0b00001111;	//So LEDs will intially be switched off
	
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
	  if (~PINF & 0b00010000)   // S4 
	     PORTD |= 0b00010000;   // turn LED4 on

	  if (~PINF & 0b00100000)   // S3 
	    PORTD |= 0b00100000;   // turn LED3 on

	  if (~PINF & 0b01000000)   // S2 
	    PORTD |= 0b01000000;   // turn LED2 on
	
	  if (~PINF & 0b10000000)   // S1 
		 PORTD |= 0b10000000;   // turn LED1 on  		
		  
      if (PINF & 0b11110000)    // no push buttons pressed
	     PORTD &= 0b00001111;   // turn all LEDs off
	} /*while*/

}/*end main*/