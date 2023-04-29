/*
 * Tema6_adc.c
 *
 * Created: 21.04.2023 10:35:31
 * Author : Elena
 */ 

#define F_CPU 16000000UL

#define PORT_LED0 PORTD
#define PIN_LED0  PIND4
#define PORT_LED1 PORTD
#define PIN_LED1  PIND5
#define PORT_LED2 PORTD
#define PIN_LED2  PIND6

#include <avr/io.h>
#include <util/delay.h>

uint16_t ADC_read(uint8_t);

int main(void)
{
	init_devices();
	
	while (1)
	{
		PORTD ^= 1 << PIN_LED2;
		uint16_t adcValue = ADC_read(0); //Read ADC value from ADC0 (PORTF0)
		float voltage = (adcValue * 5) / 1024; // Convert ADC value to voltage (assuming Vref = 5V and 10-bit ADC)
		
		if (voltage < 4)
		{
			PORTD |= (1 << PIN_LED0); //Turn on LED0
			PORTD &= ~(1 << PIN_LED1); //Turn off LED1
		}
		else
		{
			PORTD &= ~(1 << PIN_LED0); //Turn off LED0
			PORTD |= (1 << PIN_LED1); //Turn on LED1
		}
		
		 _delay_ms(500); // Delay for stability
	}
	
}


void init_devices(void)
{
	port_init();
	adc_init();
}

void port_init(void)
{

	DDRD  = 0xFF;
	PORTD = 0x00;
	
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
	
	//Enable ADC
	ADCSRA |= (1 << ADEN); //Set ADEN bit to 1 to enable adc
	
	//Enable ADC interrupts
	ADCSRA |= (1 << ADIE);
	
	//Set the prescaler
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (0 << ADPS0); //64
	
}

uint16_t ADC_read(uint8_t channel)
{
	
	uint16_t ADC_result;
	//select ADC input channels(canalele multiplexorului)
	ADMUX = (ADMUX & 0xF0) | (channel & 0x0F); 
	
	//start ADC conversion
	ADCSRA |= (1 << ADSC); // Set ADSC bit to 1 to start conversion
	
	while (ADCSRA & (1 << ADSC)); // Wait for ADC conversion to complete  
	//ALTA MODALITATE while((ADCSRA&(1<<ADIF))==0);	/* Monitor end of conversion interrupt */
	
	_delay_us(10);
	ADC_result  = (uint16_t)ADCL;
	ADC_result |= (uint16_t)ADCH<<8;
	
	return ADC_result; // Return ADC result
}

