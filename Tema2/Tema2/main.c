/*
 * Tema2.c
 *
 * Created: 12.03.2023 18:54:50
 * Author : Elena
 */ 
#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>

void pinToggle(volatile uint8_t *port, uint8_t pin);
void pinSet(volatile uint8_t *port, uint8_t pin);
void pinReset(volatile uint8_t *port, uint8_t pin);

int main(void)
{
    DDRB = 0xFF;
	DDRC = 0xFF;
	DDRD = 0xFF;
	PORTB = 0x00;
	PORTC = 0x00;
	PORTD = 0x00;
	pinSet(&PORTD, PIND4);
    while (1) 
    {
		
		pinSet(&PORTD, PIND5);
		
		_delay_ms(100);
		
		pinReset(&PORTD, PIND4);
		pinSet(&PORTD, PIND6);
		_delay_ms(100);
		
		pinReset(&PORTD, PIND5);
		pinSet(&PORTD, PIND7);
		_delay_ms(100);
		
		pinReset(&PORTD, PIND6);
		pinSet(&PORTB, PINB0);
		
		_delay_ms(100);
		
		pinReset(&PORTD, PIND7);
		pinSet(&PORTB, PINB1);
		
		_delay_ms(100);
		
		pinReset(&PORTB, PINB0);
		pinSet(&PORTB, PINB2);
		
		_delay_ms(100);
		
		pinReset(&PORTB, PINB1);
		pinSet(&PORTB, PINB3);
		
		_delay_ms(100);
		
		pinReset(&PORTB, PINB2);
		pinSet(&PORTD, PIND4);
		_delay_ms(100);
		
		pinReset(&PORTB, PINB3);
		
		
    }
}

void pinToggle(volatile uint8_t *port, uint8_t pin)
{
	*port ^= 1 << pin;
}


void pinSet(volatile uint8_t *port, uint8_t pin)
{
	*port |= 1 << pin;
}

void pinReset(volatile uint8_t *port, uint8_t pin)
{
	*port &= ~(1 << pin);
}