/*
 * Tema7_seriala.c
 *
 * Created: 23.05.2023 20:23:54
 * Author : Elena
 */ 

#include <avr/io.h>
#include <util/delay.h>

#define F_CPU 16000000UL 
#define BAUD 57600
#define MYUBRR (F_CPU/16/BAUD)-1

void USART_Init(unsigned int ubrr); //initializare registrii si baud rate
void USART_Transmit(unsigned char data);

uint8_t data;

int main(void)
{
	USART_Init(MYUBRR);
	USART_Transmit(84);//T 
	_delay_ms(1000);
	USART_Transmit(69);//E
	_delay_ms(1000);
	USART_Transmit(83);//S
	_delay_ms(1000);
	USART_Transmit(84);//T
	
	
	while (1);
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
}

void USART_Transmit(unsigned char data)
{
	//wait for empty transmit buffer
	while(!(UCSR0A & (1<<UDRE0))); 
	
	//put data into buffer, sends the data
	UDR0 = data;
}





