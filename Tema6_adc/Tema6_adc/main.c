
#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>

void port_init(void);
void adc_init(void);
void init_devices(void);

void pinSet(volatile uint8_t *port, uint8_t pin);
void pinReset(volatile uint8_t *port, uint8_t pin);

ISR(ADC_vect);

#define PORT_LED0 PORTD
#define PIN_LED0  PIND4
#define PORT_LED1 PORTD
#define PIN_LED1  PIND5

uint16_t adcValue;
#define compValue_adc 450

void init_devices(void)
{

	cli();

	port_init();
	
	adc_init();
	
	sei();
}

void port_init(void)
{
	DDRB  = 0xFF;
	PORTB  = 0x00;

	DDRC  = 0x00;
	PORTC = 0x00;
}


void adc_init(void){
	
	//reset ADC registers
	ADMUX = 0x00;
	ADCSRA = 0x00;
	
	//set the reference voltage to Avcc
	ADMUX |=  (1 << REFS0); // Set REFS0 bit to 1
	ADMUX &= ~(1 << REFS1); // Set REFS1 bit to 0
	ADMUX &=  ~(1 << ADLAR); // Set ADLAR bit to 0(left adjust the result)
	
	//disable analog comparator
	ACSR |= (1 << ACD); // Set ACD bit(from ACSR) to 1 to disable the Analog Comparator
	
	
	//Enable ADC interrupts
	ADCSRA |= (1 << ADIE);
	sei();
	//Set the prescaler
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (0 << ADPS0) ; //64
	//Enable ADC
	ADCSRA |= (1 << ADEN); //Set ADEN bit to 1 to enable adc
	
	//start ADC conversion
	ADCSRA |= (1 << ADSC); // Set ADSC bit to 1 to start conversion
}


ISR(ADC_vect)
{
	adcValue = ADCL;
	adcValue |= (uint16_t)ADCH<<8;
	if(adcValue > compValue_adc)
	{
		pinSet(&PORT_LED0, PIN_LED0);
		pinReset(&PORT_LED1, PIN_LED1);
	}
	else {
		pinSet(&PORT_LED1, PIN_LED1);
		pinReset(&PORT_LED0, PIN_LED0);
	}
	ADCSRA |= (1 << ADSC); // Set ADSC bit to 1 to start conversion
}

int main(void)
{
	init_devices();
	
	while(1);
}


void pinSet(volatile uint8_t *port, uint8_t pin){
	*port |=  1 << pin;
}

void pinReset(volatile uint8_t *port, uint8_t pin){
	*port &=  ~(1 << pin);
}