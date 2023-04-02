// /*
//  * Tema3.c
//  */



#define F_CPU 1600000UL
#include <avr/io.h>
#include <avr/interrupt.h>


#define PORT_LED0	PORTD
#define PIN_LED0	PIND4
#define PORT_LED1	PORTD
#define PIN_LED1	PIND5
#define PORT_LED2	PORTD
#define PIN_LED2	PIND6
#define PORT_LED3	PORTD
#define PIN_LED3	PIND7


#define BUTTON1_PORT PORTD
#define BUTTON1_PIN  PIND3
#define BUTTON2_PORT PORTD
#define BUTTON2_PIN PIND2

ISR(INT0_vect)
{
	PORT_LED0 |= (1 << PIN_LED0);
	PORT_LED1 |= (1 << PIN_LED1);
	PORT_LED2 |= (1 << PIN_LED2);
	PORT_LED3 |= (1 << PIN_LED3);
}


ISR(INT1_vect)
{
	PORT_LED0 &= ~(1 << PIN_LED0);
	PORT_LED1 &= ~(1 << PIN_LED1);
	PORT_LED2 &= ~(1 << PIN_LED2);
	PORT_LED3 &= ~(1 << PIN_LED3);
}

int main(void)
{
	init_devices(); //off
	init_intr();
	while (1);
}


void init_intr()
{
	EICRA |= (1 << ISC11) | (0 << ISC10) | (1 << ISC01) | (1 << ISC00); //masca pt butoane 00001100 // rising - cand iei degetul
	EIMSK |= (1 << INT0);                                                                           // falling - imd cum apesi
	EIMSK |= (1 << INT1);
	sei();
}

void port_init(void)
{
	DDRD  = 0xF3;
	PORTD = 0x0C;
}

void init_devices(void)
{
	port_init();
}

