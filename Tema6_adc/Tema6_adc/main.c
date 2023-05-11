
#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdbool.h>

void port_init(void);
void adc_init(void);
void init_devices(void);
void start_conversion(void);

void pinSet(volatile uint8_t *port, uint8_t pin);
void pinReset(volatile uint8_t *port, uint8_t pin);

ISR(ADC_vect);

#define PORT_LED0 PORTD
#define PIN_LED0  PIND4
#define PORT_LED1 PORTD
#define PIN_LED1  PIND5

#define voltage_ref 5
#define compValue 3
#define resValue 1024 //full scale for 10-bit resolution (2^10)

uint16_t adcValue;
bool flag = 0;


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

void start_conversion(void){
	ADCSRA |= (1 << ADSC);
}


ISR(ADC_vect)
{
	//get ADC value
	adcValue = ADCL;
	adcValue |= (uint16_t)ADCH<<8;
	flag = 1; //conversion done
}

int main(void)
{
	init_devices();
	
	while(1)
	{
		if(flag == 1)
		{
			float voltage = ( adcValue * voltage_ref) / resValue; //conversion formula
			if(voltage < compValue)
			{
				pinSet(&PORT_LED0, PIN_LED0); //Turn on LED0
				pinReset(&PORT_LED1, PIN_LED1); //Turn off LED1
			}
			else {
				pinSet(&PORT_LED1, PIN_LED1); //Turn on LED1
				pinReset(&PORT_LED0, PIN_LED0); //Turn off LED0
			}
			flag = 0;
			start_conversion();
		}
			
	}
}

void pinSet(volatile uint8_t *port, uint8_t pin){
	*port |=  1 << pin;
}

void pinReset(volatile uint8_t *port, uint8_t pin){
	*port &=  ~(1 << pin);
}