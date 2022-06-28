#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

//Serial debug
#include <serial.h>
#include <stdio.h>

void pwm_init() {
	// Timer configuration here
}

void init() {
	cli();  //Disable interrupts

	//Set sysclk to 16 MHz
	CLKPR = 0x80; // Prescaler change enable
	CLKPR = 0x00; // Set prescaler to zero

	//Configure PORT D4-D7 (LEDs) as outputs
	PORTD = 0x00;
	DDRD = 0xF0;

	pwm_init();
	serial_init();

	sei();  //Enable interrupts
}

//volatile uint16_t isrCount = 0;
ISR(TIMER1_OVF_vect) {
	
	//Interrupt-driven frequency toggle example
	isrCount++;
	if (isrCount == 1000) {
		OCR1A = 5926; // 2.7 kHz
	} else if (isrCount > 1999) {
		OCR1A = 8000;
		isrCount = 0;
	}
	

	
	Rough frequency sweep example
	if (OCR1A > 8000) {	// 2 kHz
		OCR1A = 5926; 	// 2.7 kHz
	} else {
		OCR1A++;
	}
	
}

int main(void) {

	uint32_t count = 0xFFFF0000;

	// Initialisation
	init();

	//Pin set example
	//PORTD |= (1<<PIND7);

	// Main loop (loop forever)
	for(;;) {

		/*
		// Frequency toggle example
		OCR1A = 8000;	// 2 kHz
		_delay_ms(500);
		OCR1A = 5926;	// 2.7 kHz
		_delay_ms(500);
		*/

		/*
		// Serial debug example
		count++;
		printf("TICK! ");
		_delay_ms(500);
		printf("TOCK! %08lX\n", count);
		_delay_ms(500);
		*/

		/*
		// Pushbutton debug example
		pb = PINF;
		if (pb & (1<<PINF4)) {
			PORTD &= ~(1<<PORTD7); 
		} else {
			PORTD |= (1<<PORTD7); 
		}
		*/

		// 1 Hz flash on LED4 (white)
		//PORTD ^= 0x80;	// Toggle LED4
		//_delay_ms(500);	// 500ms delay
	}
}