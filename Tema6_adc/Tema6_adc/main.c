
#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

void port_init(void);
void adc_init(void);
void init_devices(void);
void start_conversion(void);

void pinSet(volatile uint8_t *port, uint8_t pin);
void pinReset(volatile uint8_t *port, uint8_t pin);

void USART_Init(unsigned int ubrr); //initializare registrii si baud rate
void USART_Transmit(unsigned char data);
void USART_Transmit_string(char *string);
uint8_t data;

ISR(ADC_vect);
ISR(USART_RX_vect);

#define PORT_LED0 PORTD
#define PIN_LED0  PIND4
#define PORT_LED1 PORTD
#define PIN_LED1  PIND5

#define voltage_ref 5
#define compValue 3
#define resValue 1024 //full scale for 10-bit resolution (2^10)
#define BAUD 57600
#define MYUBRR (F_CPU/16/BAUD)-1


uint16_t adcValue;
bool flag = 0;
char buffer[20];

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
	USART_Init(MYUBRR);
	while(1)
	{
		if(flag == 1)
		{
			int voltage = ( adcValue * voltage_ref) / resValue; //conversion formula
			if(voltage < compValue)
			{
				pinSet(&PORT_LED0, PIN_LED0); //Turn on LED0
				pinReset(&PORT_LED1, PIN_LED1); //Turn off LED1
			}
			else {
				pinSet(&PORT_LED1, PIN_LED1); //Turn on LED1
				pinReset(&PORT_LED0, PIN_LED0); //Turn off LED0
			}
			sprintf(buffer, "%d ", voltage);
			USART_Transmit_string(buffer);
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



ISR(USART_RX_vect)
{
	
	 UDR0 = data;
}


void USART_Init(unsigned int ubrr)
{
	//set baud rate
	UBRR0H = (unsigned char) (ubrr>>8);
	UBRR0L = (unsigned char) ubrr;
	
	//activ doar sa transmita si sa primeasca
	UCSR0B |= (1<<RXEN0)|(1<<TXEN0);
	
	//setez frame format : 8 data , 1 bit stop
	UCSR0C = (0<<USBS0)|(3<<UCSZ00);
	sei();
}

void USART_Transmit(unsigned char data)
{
	//wait for empty transmit buffer
	while(!(UCSR0A & (1<<UDRE0)));
	
	//put data into buffer, sends the data
	UDR0 = data;
}

void USART_Transmit_string(char *string)//functie care ia un sir de caractere si extrage cate un caracter pe rand, pana la terminarea sirului
{
	while(*string!= 0x00)
	{
		USART_Transmit(*string);
		string++;
	}
	
}
