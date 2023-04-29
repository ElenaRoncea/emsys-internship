/*
 * Tema5_timer.c
 *
 * Created: 06.04.2023 11:48:34
 * Author : Elena
 */ 
#define F_CPU 16000000UL

#define PORT_LED0 PORTD
#define PIN_LED0  PIND4
#define PORT_LED1 PORTD
#define PIN_LED1  PIND5
#define PORT_LED2 PORTD
#define PIN_LED2  PIND6

#define OVF_TIMER_LED2 50
#define OVF_TIMER_LED3 100

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

void pinToggle(volatile uint8_t *port, uint8_t pin);
uint16_t OVFCountL2 = 0;
uint16_t OVFCountL3 = 0;


int main(void)
{
	init_devices();
	
    while (1) ;
  
}

void pinToggle(volatile uint8_t *port, uint8_t pin)
{
	*port ^= 1 << pin;
}

void init_devices(void)
{
	cli();
	port_init();
	timer0_init();
	sei();
}

void port_init(void)
{

	DDRD  = 0xFF;
	PORTD = 0x00;
}


void timer0_init(void){
	//Put Timer0 in CTC Mode, no compare match output
	TCCR0A = 0x00;
	TCCR0A = (1 << WGM01);
	
	//Set the prescaler. This will divide the input frequency by 1024
	TCCR0B = (1 << CS02) | (0 << CS01) | (1 << CS00);

	TCNT0 = 0x00;
	//The output frequency if Fclock / prescaler / OCR.
	OCR0A = 0x9B; //155
	
	//Enable Timer/Counter Output Compare Match Interrupt
	TIMSK0 |= (1 << OCIE0A);
	
}

ISR(TIMER0_COMPA_vect)
{
	pinToggle(&PORT_LED0, PIN_LED0);
	
	OVFCountL2++;
	OVFCountL3++;
	
	if(OVFCountL2 == OVF_TIMER_LED2){
		pinToggle(&PORT_LED1, PIN_LED1);
		OVFCountL2 = 0;
	}
	if(OVFCountL3 == OVF_TIMER_LED3){
		pinToggle(&PORT_LED2, PIN_LED2);
		OVFCountL3 = 0;
	}
	
}