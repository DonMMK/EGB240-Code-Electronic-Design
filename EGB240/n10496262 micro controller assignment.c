//n10496262 micro controller assignment
//References and Sources of code mentioned in report

//Standard library
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <string.h>
#include <util/delay.h>
#include <inttypes.h>
#include <stdlib.h>

// Lecture 8 notes code definitions
#define SET_BIT(reg, pin)     (reg) |= (1 << (pin))
#define CLEAR_BIT(reg, pin)     (reg) &= ~(1 << (pin))
#define WRITE_BIT(reg, pin, value)  (reg) = (((reg) & ~(1 << (pin))) | ((value) << (pin)))
#define BIT_VALUE(reg, pin)     (((reg) >> (pin)) & 1)
#define BIT_IS_SET(reg, pin)    (BIT_VALUE((reg),(pin))==1)
#define F_CPU 16000000UL
#define BAUD 9600
#define MYUBRR   (F_CPU/16/BAUD-1)
//  https://www.tinkercad.com/things/hGYL4k0Uoal

// --== WIRING ==--
// LCD GND  -> GND
// LCD VCC  -> 5V
// LCD V0   -> GND
// LCD RW   -> GND
// LCD LED Anode    -> 220 Ohm -> 5V
// LCD LED Cathode  -> GND

// Lecture 11 LCD Screen definitions
#define LCD_USING_4PIN_MODE (1)

#define LCD_DATA4_DDR (DDRD)
#define LCD_DATA5_DDR (DDRD)
#define LCD_DATA6_DDR (DDRD)
#define LCD_DATA7_DDR (DDRD)

#define LCD_DATA4_PORT (PORTD)
#define LCD_DATA5_PORT (PORTD)
#define LCD_DATA6_PORT (PORTD)
#define LCD_DATA7_PORT (PORTD)

#define LCD_DATA4_PIN (4)
#define LCD_DATA5_PIN (5)
#define LCD_DATA6_PIN (6)
#define LCD_DATA7_PIN (7)

#define LCD_RS_DDR (DDRB)
#define LCD_ENABLE_DDR (DDRB)

#define LCD_RS_PORT (PORTB)
#define LCD_ENABLE_PORT (PORTB)

#define LCD_RS_PIN (1)
#define LCD_ENABLE_PIN (0)

//DATASHEET: https://s3-us-west-1.amazonaws.com/123d-circuits-datasheets/uploads%2F1431564901240-mni4g6oo875bfbt9-6492779e35179defaf4482c7ac4f9915%2FLCD-WH1602B-TMI.pdf

// commands
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

#define BAUD 9600
#define F_CPU 16000000UL
#define MYUBRR (F_CPU/16/BAUD -1)
#define FREQ 16000000.0
#define PRESCALE 1024.0

void lcd_init(void);
void lcd_write_string(uint8_t x, uint8_t y, char string[]);
void lcd_write_char(uint8_t x, uint8_t y, char val);

void lcd_clear(void);
void lcd_home(void);

void lcd_createChar(uint8_t, uint8_t[]);
void lcd_setCursor(uint8_t, uint8_t);

void lcd_noDisplay(void);
void lcd_display(void);
void lcd_noBlink(void);
void lcd_blink(void);
void lcd_noCursor(void);
void lcd_cursor(void);
void lcd_leftToRight(void);
void lcd_rightToLeft(void);
void lcd_autoscroll(void);
void lcd_noAutoscroll(void);
void scrollDisplayLeft(void);
void scrollDisplayRight(void);

size_t lcd_write(uint8_t);
void lcd_command(uint8_t);

void lcd_send(uint8_t, uint8_t);
void lcd_write4bits(uint8_t);
void lcd_write8bits(uint8_t);
void lcd_pulseEnable(void);

void timer0(void);
void timer1(void);
void timer2(void);

void adjustSpeed(void);

uint8_t _lcd_displayfunction;
uint8_t _lcd_displaycontrol;
uint8_t _lcd_displaymode;

// END Definitions

//game definitions
#define BTNDDR      DDRC
#define BTNPIN      PINC
#define A_BTN    2
#define B_BTN    3
#define C_BTN    4
#define D_BTN    5

#define LEDDDR      DDRB
#define LEDPORT     PORTB
#define LED_A     2
#define LED_B     3
#define LED_C     4
#define LED_D     5

#define SCREEN_HEIGHT 2
#define SCREEN_WIDTH 16

#define BOXA    0b11111100
#define BOXB    0b11111100
#define BOXC    0b11111100
#define BOXD    0b11111100
#define ARROWPR 0b11000111 //>>>>
#define ARROWPL 0b11001000 //<<<<
#define A       0b01000001
#define B       0b01000010
#define C       0b01000011
#define D       0b01000100

//#define FAST   20
//#define MEDIUM 50
//#define SLOW   80


void setup_lcd(void);
void setup_IO(void);
void process(void);
void move_character(uint8_t x, uint8_t y, uint8_t character);

void lcdcontinue(void);
void setupADC(void);


//defining global variables
static uint8_t BOX1x = 4;
static uint8_t BOX1y = 0;

static uint8_t BOX2x = 11;
static uint8_t BOX2y = 0;

static uint8_t BOX3x = 4;
static uint8_t BOX3y = 1;

static uint8_t BOX4x = 11;
static uint8_t BOX4y = 1;

static uint8_t arrowplx = 6;
static uint8_t arrowply = 0;

static uint8_t arrowprx = 9;
static uint8_t arrowpry = 0;

static uint8_t Ax = 3;
static uint8_t Ay = 0;

static uint8_t Bx = 12;
static uint8_t By = 0;

static uint8_t Cx = 3;
static uint8_t Cy = 1;

static uint8_t Dx = 12;
static uint8_t Dy = 1;

volatile uint8_t bit_counter = 0;
volatile uint8_t pressed = 0;

int whack_score = 0;
int reaction_time = 0;
double time = 0;
int pos = 0; //Can be changed and tested for efficiency of buttons
int speed = 0;
int lighter = 0;

/****************************************************************************************
MAIN
*****************************************************************************************/
int main(void){

  setupADC();
  uart_init(MYUBRR);
  setup_lcd();
  setup_IO();
  timer0();
  timer1();
  timer2();
  adjustSpeed();
  lcdcontinue();
  SET_BIT(TIMSK0,TOIE0);
  while(1){
    process();
    if (whack_score >= 100) {
     lcd_clear() ;
      break;
    }
  }

}



/****************************************************************************************
SETUP
*****************************************************************************************/
void setup_lcd(void) {

    lcd_init();

      lcd_write_string(0,0, "  WELCOME TO WHACK THE BOX  ");
      for(int i = 0; i< 11; i++){
        scrollDisplayLeft();
        _delay_ms(400);
      }
      lcd_clear();
     
}


void setup_IO(void){

    //setting buttons for input
    CLEAR_BIT(BTNDDR,A_BTN);
    CLEAR_BIT(BTNDDR,B_BTN);
    CLEAR_BIT(BTNDDR,C_BTN);
    CLEAR_BIT(BTNDDR,D_BTN);

    //setting LED's for output
    SET_BIT(LEDDDR,LED_A);
    SET_BIT(LEDDDR,LED_B);
    SET_BIT(LEDDDR,LED_C);
    SET_BIT(LEDDDR,LED_D);

}

void next(){

  pos = rand() % 4;
  time = 0;
  lcd_clear();

}

/****************************************************************************************
PROCESS
*****************************************************************************************/
void setupADC() {

  SET_BIT(ADCSRA,ADEN);
  SET_BIT(ADCSRA,ADPS2);
  SET_BIT(ADCSRA,ADPS1);
  SET_BIT(ADCSRA,ADPS1);
  SET_BIT(ADMUX,REFS0);

}

void adjustSpeed() {
  _delay_ms(1000);
  lcd_write_string(0,0,"PLEASE ADJUST");
  lcd_write_string(0,1,"SPEED");
  _delay_ms(5000);
  lcd_clear();
  uint16_t value0 = 0;
  while(1) {
    char buf[64];
    SET_BIT(ADCSRA,ADSC);
    while (ADCSRA & (1 << ADSC) ) {}

    if (ADC < 500 && ADC > 5) {
      speed = 1;
    }
    else if (ADC > 500 && ADC < 900) {
      speed = 2;
    }
    else if (ADC > 900) {
      speed = 3;
    }
    if (ADC < 5) {
      lcd_write_string(0,0,"USE THE ");
      lcd_write_string(0,1,"POTENTIOMETER ");
    }
    else {
      snprintf(buf, sizeof(buf), "SPEED LEVEL: %d", speed);
      if (speed != value0) {
        SET_BIT(LEDPORT,LED_A);
        lcd_write_string(0,0,buf);
        value0 = speed;
      }
      else {
        CLEAR_BIT(LEDPORT,LED_A);
      }
      lcd_write_string(0,1,"GREEN LED >>>");
      SET_BIT(LEDPORT,LED_B);
      if (pressed != 0) {
        CLEAR_BIT(LEDPORT,LED_B);
        lcd_clear();
        break;
      }
    }
  }
}

void lcdcontinue() {

      lcd_write_string(0,0,"Start Wacking!!!");
      _delay_ms(2000);
      lcd_clear();
      _delay_ms(1000);

}


void process(void) {
 
char ReactionTime [5];
 
move_character(BOX1x,BOX1y,BOXA);
move_character(BOX2x,BOX2y,BOXB);
move_character(BOX3x,BOX3y,BOXC);
move_character(BOX4x,BOX4y,BOXD);
move_character(Ax,Ay,A);
move_character(Bx,By,B);
move_character(Cx,Cy,C);
move_character(Dx,Dy,D);

if(time > 4000){

  next();
}

if(pos == 0){
  SET_BIT(LEDPORT,LED_A);
  move_character(arrowplx,0, ARROWPL);
  if(!BIT_VALUE(BTNPIN,A_BTN)){
    itoa(time/1000, ReactionTime, 3);
      uart_putstring((unsigned char*)"Reaction Time: ");
      uart_putstring((unsigned char*)ReactionTime);
       uart_putchar('\n');
    whack_score += 10;
        lcd_clear();
    lcd_write_string(0,0,"YOU WHACKED IT");
    CLEAR_BIT(LEDPORT,LED_A);
    _delay_ms(750);
    next();
  }
}
if(pos == 1){
  SET_BIT(LEDPORT,LED_B);
  move_character(arrowprx,0, ARROWPR);
    if(!BIT_VALUE(BTNPIN,B_BTN)){
      itoa(time/1000, ReactionTime, 3);
      uart_putstring((unsigned char*)"Reaction Time: ");
      uart_putstring((unsigned char*)ReactionTime);
      uart_putchar('\n');
      whack_score += 10;
          lcd_clear();
    lcd_write_string(0,0,"YOU WHACKED IT");
        CLEAR_BIT(LEDPORT,LED_B);
    _delay_ms(750);
    next();
  }
}
if(pos == 2){
  SET_BIT(LEDPORT,LED_C);
  move_character(arrowplx,1, ARROWPL);
    if(!BIT_VALUE(BTNPIN,C_BTN)){
      itoa(time/1000, ReactionTime, 3);
      uart_putstring((unsigned char*)"Reaction Time: ");
      uart_putstring((unsigned char*)ReactionTime);
      uart_putchar('\n');
      whack_score += 10;
          lcd_clear();
    lcd_write_string(0,0,"YOU WHACKED IT");
        CLEAR_BIT(LEDPORT,LED_C);
    _delay_ms(750);
    next();
  }
}
 
 
if(pos == 3){

  SET_BIT(LEDPORT,LED_D);
  move_character(arrowprx,1, ARROWPR);
    if(!BIT_VALUE(BTNPIN,D_BTN)){
      itoa(time/1000, ReactionTime, 3);
      uart_putstring((unsigned char*)"Reaction Time: ");
      uart_putstring((unsigned char*)ReactionTime);
      uart_putchar('\n');
    whack_score += 10;
        lcd_clear();
    lcd_write_string(0,0,"YOU WHACKED IT");
        CLEAR_BIT(LEDPORT,LED_D);
    _delay_ms(750);
    next();

  }
}
}

/****************************************************************************************
TIMERS
*****************************************************************************************/

//PWM
void timer0(void){

  TCCR0A = 0;

  //prescaler of 1024
  SET_BIT(TCCR0B,CS02);

  SET_BIT(TCCR0B,CS00);

  //enable global interrupts
  sei();

}

ISR(TIMER0_OVF_vect){

  lighter++;

  if (lighter > 30 && BIT_VALUE(LEDPORT,LED_D) == 1){

    CLEAR_BIT(LEDPORT,LED_D);

    lighter = 0;
  }

  if (lighter > 30 && BIT_VALUE(LEDPORT,LED_D) == 0){

    SET_BIT(LEDPORT,LED_D);

    lighter = 0;
  }
}

//From topic 9 exercise 3
void timer1(void){ //debouncing

  //prescaler of 1 --> 0.004 seconds overflow
  SET_BIT(TCCR1B,CS10);

  //timer overflow
  TIMSK1 |= (1<<TOIE1);

  //global interrupts
  sei();

  //clearing for input
  CLEAR_BIT(BTNPIN,A_BTN);

}

ISR(TIMER1_OVF_vect){

    bit_counter = (bit_counter << 1);

    uint8_t mask = 0b00011111;

    bit_counter &= mask;

    WRITE_BIT(bit_counter, 0, BIT_VALUE(BTNPIN,A_BTN));

    if (bit_counter == mask){
      pressed = 1;
    }

    else if (bit_counter == 0){
      pressed = 0;
    }
}


void timer2(void){
  TCCR2A = 0;

  //prescaler of 8
  SET_BIT(TCCR2B,CS21);

  //timer overflow
  SET_BIT(TIMSK2,TOIE2);

  //global interrupts
  sei();
}

ISR(TIMER2_OVF_vect){

  time++;

}

// uart setup
void uart_setup(void) {
uart_init(MYUBRR);
timer2();
}

// initialize the UART
void uart_init(unsigned int ubrr) {
UBRR0H = (unsigned char)(ubrr>>8);
  UBRR0L = (unsigned char)(ubrr);
SET_BIT(UCSR0B,RXEN0);
SET_BIT(UCSR0B,TXEN0);
SET_BIT(UCSR0C,UCSZ00);
SET_BIT(UCSR0C,UCSZ01);
}

// transmit a data
void uart_putchar(unsigned char data) {
    while (!( UCSR0A & (1<<UDRE0))); // Wait for empty transmit buffer
  UDR0 = data; // Put data into buffer, sends the data
}

// receive data
unsigned char uart_getchar(void) {
  while (!(UCSR0A & (1<<RXC0))); // Wait for data to be received
  return UDR0; // Get and return received data from buffer
}

// transmit a string
void uart_putstring(unsigned char* s) {
while(*s > 0) uart_putchar(*s++); // transsmit character until NULL is reached
}

/* ********************************************/
// START LIBRARY FUNCTIOMNS

void move_character(uint8_t x, uint8_t y, uint8_t character){
    lcd_setCursor(x,y);
    lcd_write(character);
}

void lcd_init(void){
  //dotsize
  if (LCD_USING_4PIN_MODE){
    _lcd_displayfunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;
  } else {
    _lcd_displayfunction = LCD_8BITMODE | LCD_1LINE | LCD_5x8DOTS;
  }

  _lcd_displayfunction |= LCD_2LINE;

  // RS Pin
  LCD_RS_DDR |= (1 << LCD_RS_PIN);
  // Enable Pin
  LCD_ENABLE_DDR |= (1 << LCD_ENABLE_PIN);

  #if LCD_USING_4PIN_MODE
    //Set DDR for all the data pins
    LCD_DATA4_DDR |= (1 << 4);
    LCD_DATA5_DDR |= (1 << 5);
    LCD_DATA6_DDR |= (1 << 6);
    LCD_DATA7_DDR |= (1 << 7);

  #else
    //Set DDR for all the data pins
    LCD_DATA0_DDR |= (1 << LCD_DATA0_PIN);
    LCD_DATA1_DDR |= (1 << LCD_DATA1_PIN);
    LCD_DATA2_DDR |= (1 << LCD_DATA2_PIN);
    LCD_DATA3_DDR |= (1 << LCD_DATA3_PIN);
    LCD_DATA4_DDR |= (1 << LCD_DATA4_PIN);
    LCD_DATA5_DDR |= (1 << LCD_DATA5_PIN);
    LCD_DATA6_DDR |= (1 << LCD_DATA6_PIN);
    LCD_DATA7_DDR |= (1 << LCD_DATA7_PIN);
  #endif

  // SEE PAGE 45/46 OF Hitachi HD44780 DATASHEET FOR INITIALIZATION SPECIFICATION!

  // according to datasheet, we need at least 40ms after power rises above 2.7V
  // before sending commands. Arduino can turn on way before 4.5V so we'll wait 50
  _delay_us(50000);
  // Now we pull both RS and Enable low to begin commands (R/W is wired to ground)
  LCD_RS_PORT &= ~(1 << LCD_RS_PIN);
  LCD_ENABLE_PORT &= ~(1 << LCD_ENABLE_PIN);

  //put the LCD into 4 bit or 8 bit mode
  if (LCD_USING_4PIN_MODE) {
    // this is according to the hitachi HD44780 datasheet
    // figure 24, pg 46

    // we start in 8bit mode, try to set 4 bit mode
    lcd_write4bits(0b0111);
    _delay_us(4500); // wait min 4.1ms

    // second try
    lcd_write4bits(0b0111);
    _delay_us(4500); // wait min 4.1ms

    // third go!
    lcd_write4bits(0b0111);
    _delay_us(150);

    // finally, set to 4-bit interface
    lcd_write4bits(0b0010);
  } else {
    // this is according to the hitachi HD44780 datasheet
    // page 45 figure 23

    // Send function set command sequence
    lcd_command(LCD_FUNCTIONSET | _lcd_displayfunction);
    _delay_us(4500);  // wait more than 4.1ms

    // second try
    lcd_command(LCD_FUNCTIONSET | _lcd_displayfunction);
    _delay_us(150);

    // third go
    lcd_command(LCD_FUNCTIONSET | _lcd_displayfunction);
  }

  // finally, set # lines, font size, etc.
  lcd_command(LCD_FUNCTIONSET | _lcd_displayfunction);

  // turn the display on with no cursor or blinking default
  _lcd_displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
  lcd_display();

  // clear it off
  lcd_clear();

  // Initialize to default text direction (for romance languages)
  _lcd_displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
  // set the entry mode
  lcd_command(LCD_ENTRYMODESET | _lcd_displaymode);
}


/********** high level commands, for the user! */
void lcd_write_string(uint8_t x, uint8_t y, char string[]){
  lcd_setCursor(x,y);
  for(int i=0; string[i]!='\0'; ++i){
    lcd_write(string[i]);
  }
}

void lcd_write_char(uint8_t x, uint8_t y, char val){
  lcd_setCursor(x,y);
  lcd_write(val);
}

void lcd_clear(void){
  lcd_command(LCD_CLEARDISPLAY);  // clear display, set cursor position to zero
  _delay_us(2000);  // this command takes a long time!
}

void lcd_home(void){
  lcd_command(LCD_RETURNHOME);  // set cursor position to zero
  _delay_us(2000);  // this command takes a long time!
}


// Allows us to fill the first 8 CGRAM locations
// with custom characters
void lcd_createChar(uint8_t location, uint8_t charmap[]) {
  location &= 0x7; // we only have 8 locations 0-7
  lcd_command(LCD_SETCGRAMADDR | (location << 3));
  for (int i=0; i<8; i++) {
    lcd_write(charmap[i]);
  }
}


void lcd_setCursor(uint8_t col, uint8_t row){
  if ( row >= 2 ) {
    row = 1;
  }

  lcd_command(LCD_SETDDRAMADDR | (col + row*0x40));
}

// Turn the display on/off (quickly)
void lcd_noDisplay(void) {
  _lcd_displaycontrol &= ~LCD_DISPLAYON;
  lcd_command(LCD_DISPLAYCONTROL | _lcd_displaycontrol);
}
void lcd_display(void) {
  _lcd_displaycontrol |= LCD_DISPLAYON;
  lcd_command(LCD_DISPLAYCONTROL | _lcd_displaycontrol);
}

// Turns the underline cursor on/off
void lcd_noCursor(void) {
  _lcd_displaycontrol &= ~LCD_CURSORON;
  lcd_command(LCD_DISPLAYCONTROL | _lcd_displaycontrol);
}
void lcd_cursor(void) {
  _lcd_displaycontrol |= LCD_CURSORON;
  lcd_command(LCD_DISPLAYCONTROL | _lcd_displaycontrol);
}

// Turn on and off the blinking cursor
void lcd_noBlink(void) {
  _lcd_displaycontrol &= ~LCD_BLINKON;
  lcd_command(LCD_DISPLAYCONTROL | _lcd_displaycontrol);
}
void lcd_blink(void) {
  _lcd_displaycontrol |= LCD_BLINKON;
  lcd_command(LCD_DISPLAYCONTROL | _lcd_displaycontrol);
}

// These commands scroll the display without changing the RAM
void scrollDisplayLeft(void) {
  lcd_command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}
void scrollDisplayRight(void) {
  lcd_command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

// This is for text that flows Left to Right
void lcd_leftToRight(void) {
  _lcd_displaymode |= LCD_ENTRYLEFT;
  lcd_command(LCD_ENTRYMODESET | _lcd_displaymode);
}

// This is for text that flows Right to Left
void lcd_rightToLeft(void) {
  _lcd_displaymode &= ~LCD_ENTRYLEFT;
  lcd_command(LCD_ENTRYMODESET | _lcd_displaymode);
}

// This will 'right justify' text from the cursor
void lcd_autoscroll(void) {
  _lcd_displaymode |= LCD_ENTRYSHIFTINCREMENT;
  lcd_command(LCD_ENTRYMODESET | _lcd_displaymode);
}

// This will 'left justify' text from the cursor
void lcd_noAutoscroll(void) {
  _lcd_displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
  lcd_command(LCD_ENTRYMODESET | _lcd_displaymode);
}

/*********** mid level commands, for sending data/cmds */

inline void lcd_command(uint8_t value) {
  //
  lcd_send(value, 0);
}

inline size_t lcd_write(uint8_t value) {
  lcd_send(value, 1);
  return 1; // assume sucess
}

/************ low level data pushing commands **********/

// write either command or data, with automatic 4/8-bit selection
void lcd_send(uint8_t value, uint8_t mode) {
  //RS Pin
  LCD_RS_PORT &= ~(1 << LCD_RS_PIN);
  LCD_RS_PORT |= (!!mode << LCD_RS_PIN);

  if (LCD_USING_4PIN_MODE) {
    lcd_write4bits(value>>4);
    lcd_write4bits(value);
  } else {
    lcd_write8bits(value);
  }
}

void lcd_pulseEnable(void) {
  //Enable Pin
  LCD_ENABLE_PORT &= ~(1 << LCD_ENABLE_PIN);
  _delay_us(1);
  LCD_ENABLE_PORT |= (1 << LCD_ENABLE_PIN);
  _delay_us(1);    // enable pulse must be >450ns
  LCD_ENABLE_PORT &= ~(1 << LCD_ENABLE_PIN);
  _delay_us(100);   // commands need > 37us to settle
}

void lcd_write4bits(uint8_t value) {
  //Set each wire one at a time

  LCD_DATA4_PORT &= ~(1 << LCD_DATA4_PIN);
  LCD_DATA4_PORT |= ((value & 1) << LCD_DATA4_PIN);
  value >>= 1;

  LCD_DATA5_PORT &= ~(1 << LCD_DATA5_PIN);
  LCD_DATA5_PORT |= ((value & 1) << LCD_DATA5_PIN);
  value >>= 1;

  LCD_DATA6_PORT &= ~(1 << LCD_DATA6_PIN);
  LCD_DATA6_PORT |= ((value & 1) << LCD_DATA6_PIN);
  value >>= 1;

  LCD_DATA7_PORT &= ~(1 << LCD_DATA7_PIN);
  LCD_DATA7_PORT |= ((value & 1) << LCD_DATA7_PIN);

  lcd_pulseEnable();
}

void lcd_write8bits(uint8_t value) {
  //Set each wire one at a time

  #if !LCD_USING_4PIN_MODE
    LCD_DATA0_PORT &= ~(1 << LCD_DATA0_PIN);
    LCD_DATA0_PORT |= ((value & 1) << LCD_DATA0_PIN);
    value >>= 1;

    LCD_DATA1_PORT &= ~(1 << LCD_DATA1_PIN);
    LCD_DATA1_PORT |= ((value & 1) << LCD_DATA1_PIN);
    value >>= 1;

    LCD_DATA2_PORT &= ~(1 << LCD_DATA2_PIN);
    LCD_DATA2_PORT |= ((value & 1) << LCD_DATA2_PIN);
    value >>= 1;

    LCD_DATA3_PORT &= ~(1 << LCD_DATA3_PIN);
    LCD_DATA3_PORT |= ((value & 1) << LCD_DATA3_PIN);
    value >>= 1;

    LCD_DATA4_PORT &= ~(1 << LCD_DATA4_PIN);
    LCD_DATA4_PORT |= ((value & 1) << LCD_DATA4_PIN);
    value >>= 1;

    LCD_DATA5_PORT &= ~(1 << LCD_DATA5_PIN);
    LCD_DATA5_PORT |= ((value & 1) << LCD_DATA5_PIN);
    value >>= 1;

    LCD_DATA6_PORT &= ~(1 << LCD_DATA6_PIN);
    LCD_DATA6_PORT |= ((value & 1) << LCD_DATA6_PIN);
    value >>= 1;

    LCD_DATA7_PORT &= ~(1 << LCD_DATA7_PIN);
    LCD_DATA7_PORT |= ((value & 1) << LCD_DATA7_PIN);

    lcd_pulseEnable();
  #endif
}