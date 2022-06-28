/************************************************************************/
/* INCLUDED LIBRARIES/HEADER FILES                                      */
/************************************************************************/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>

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
//Volatile variables
volatile uint32_t num_counts=0;
volatile uint16_t pushbuttons_db=0, F_max=5200,F_min=2400 ,F_span, f_dev,curr_f, F_inc=25, k_tone; 
volatile uint16_t new_top;
volatile uint8_t reg1, reg2; 

/************************************************************************/
/* Initialisation                                                       */
/************************************************************************/
void init() {
	cli();  //Disable interrupts

	//Set sysclk to 16 MHz
	CLKPR = 0x80; // Prescaler change enable
	CLKPR = 0x00; // Set prescaler to zero

	DDRF &= 0x0F; //Set PORTF bits 7-4 as inputs 
	DDRD |= 0xF0; //Set PORTD bits 7-4 as outputs
	PORTD &= 0x0F; // initialise LEDs to off 
	DDRB |=(1<<PINB6); 
	// Initialise Timer 0
	TCCR0A = 0x02; // Timer0,clear Timer on compare 
	TCCR0B = 0x04; // precaler 256
	OCR0A= 0x7D; // 2ms period
	TIMSK0=0x02; // interrupt on Output Compare Match A*/

	TCCR1A |= (1<<COM1B1) | (1<< WGM10) | (1<<WGM11); //Fast PWM setting, TOP = OCR1B clear on top 
	TCCR1B = (1<< WGM13) | (1<<WGM12) | (1<<CS10); //Fast PWM setting, TOP = OCR1A, clock stopped 
	OCR1A= new_top; //
	OCR1B=OCR1A/2; //set for half of OCR1A so only one variable needs to be changed
	TIMSK1=(1<< OCIE1A);
	TCNT1=0;

    F_span=F_max-F_min;
	num_counts=0;
	new_top=(0xF42400)/(F_max);

	//to set the PWM
	DDRF &= 0x0F;    // Set PORTF bits 7-4 as inputs (PBs)	
	DDRD |= 0xF0;    // Set PORTD bits 7-4 as outputs (LEDs)
	PORTD &= 0x0F;   // initalise LEDS to off

	sei();  //Enable interrupts
}

//ISR
ISR(TIMER1_COMPA_vect){
num_counts+= new_top;
if(k_tone==0){

	if(num_counts>=0xF42400){
		num_counts=0;
	}
	else
	{
	f_dev=(F_span*(uint64_t)num_counts)/(0xF42400);
	new_top=(0xF42400)/(F_max-f_dev);	
	OCR1A=new_top;
	OCR1B= new_top/2;
	}
}
if(k_tone==1){
	new_top=(0xF42400)/(F_max);
	OCR1A=new_top;
}
if(k_tone==2){
	new_top=(0xF42400)/(F_min);
	OCR1A=new_top;
}
}

/************************************************************************/
/* MAIN LOOP (CODE ENTRY)                                               */
/************************************************************************/

int main(void) 
{

	uint8_t pb_now=0, pb_prev=0, pb_pressed;
	uint8_t state= STATE1;

	init();
	//main program loop

	while(1)
	{
	
	// determine if any pushbutton pressed
	pb_now = pushbuttons_db;
	pb_pressed = pb_now &(pb_now^pb_prev);
	pb_prev = pb_now;

	switch (state)
	{
		case  STATE1:   PORTD &= LEDS_OFF;
						PORTD |= LED1; //turns LED1 on
						if(pb_pressed & S1)
						state = STATE2;
						k_tone=0;
						if (pb_pressed & S4)
						state =STATE1;
						break;
						
		case  STATE2:   PORTD &= LEDS_OFF;
						PORTD |= LED2;  //turns LED2 on
						k_tone=1;
						
						
						if((F_max<5700) && (pb_pressed & S2))
						{
							F_max= F_max +F_inc;
							curr_f=F_max;
							OCR1A=curr_f;
						}
						if((F_max>2900) && ( pb_pressed & S3)){

							
							F_max= F_max- F_inc;
							curr_f=F_max;
							OCR1A=curr_f;
						}
						if(pb_pressed & S4){
							F_max=5200;
							F_min=2400;
							state= STATE1;
						}
						
						if(pb_pressed &S1)
						state = STATE3;
						break;

		case  STATE3:   PORTD &= LEDS_OFF;
						PORTD |= LED3;  //turns LED3 on 
						k_tone=2;
						
						if((F_min<2900) && (pb_pressed & S2))
						{
							if(pb_pressed & S2)
							F_min= F_min +F_inc;
							curr_f= F_min;
							OCR1A=curr_f;
							
						}
						if((F_min>1900) && (pb_pressed & S3)){
							
							F_min= F_min- F_inc;
							curr_f= F_min;
							OCR1A=curr_f;
							
						}
						if(pb_pressed & S4){
							state = STATE1;
						}
						
						if(pb_pressed & S1)
						state = STATE4;
						break;
		

		default:		state= STATE1;

	} //switch
	}
}


/************************************************************************/
/* INTERRUPT SERVICE ROUTINE   											*/                                              
/************************************************************************/
/**
*ISR: //Timer0 Output Compare Match A
*
*Interrupt serive routine for Timer0 Output Compare Match A
*/
ISR(TIMER0_COMPA_vect)
{
	uint8_t pb0; 	//new value read from pushbuttons
	uint8_t delta;  //difference between new value and present stable state

	pb0 = ~ PINF; // read new value from pushbuttons

	delta  = pb0 ^ pushbuttons_db; //find difference between new value and present stable state

	//pushbuttons_db only updated if difference betweeen new value and present stable state
	pushbuttons_db ^= (reg2 & delta);

	reg2 = (reg1 & delta);   //if differences still persist they are propagate to reg2
	reg1 = delta; 			 //difference stored in reg1

}//ISR END



