#define F_CPU 16000000UL
#define PORT_LED0 PORTD
#define PORT_LED1 PORTD
#define PORT_BUTTON PORTD
#define PIN_LED0 PIND4
#define PIN_LED1 PIND5
#define PIN_BUTTON PIND2

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdbool.h>
void pinToggle(volatile uint8_t *port, uint8_t pin);
void init_devices(void);

volatile bool buton = false;

int main(void)
{
	init_devices();
	init_intr();
	while(1){
		pinToggle(&PORT_LED0, PIN_LED0);
		_delay_ms(500);
		if (buton){
			PORT_LED1 |= (1 << PIN_LED1);
			buton = false;
		}
		else{
			PORT_LED1 &= ~(1 << PIN_LED1);
		}
	}
}
void pinToggle(volatile uint8_t *port, uint8_t pin)
{
	*port ^= 1 << pin;
}

void init_devices(void)
{
	// -- LED-urile ca OUTPUT -- //
	DDRD |= (1 << PIN_LED0) | (1 << PIN_LED1);
	// ------------------------  //

	// -- Butonul ca INPUT -- //
	DDRD &= ~(1 << PIN_BUTTON);
	PORTD |= (1 << PIN_BUTTON);  // asta activeaza rezistenta de pull - up
	// ------------------------  //

	// -- Activam LED-ul 0 -- //
	PORTD |= (1 << PIN_LED0);
	// --------------------- //
}


void init_intr(){
	
	// -- Intrerupere pe falling edge (apasare de buton) -- //
	EICRA |= (1 << ISC00) | (1<<ISC01);
	// --------------------------------------------------- //

	// -- Activeaza intreruperea externa 0 -- //
	EIMSK |= (1 << INT0);
	// ------------------------------------- //

	// -- Functie care activeaza intreruperea globala -- //
	sei();
	// ------------------------------------------------ //

	
}
ISR(INT0_vect)
{
	// -- Citim starea butonului (vezi ca e volatile pentru a-i spune compilatorului sa nu retina valoarea (sa o cache-uiasca) si sa o citeasca mereu) -- //

	// -- Aici e conditia pentru a porni LED-ul 1 in cazul in care butonul a fost apasat -- //
	if ((PIND, PIN_BUTTON)) {
		buton = true;
		} else {
		buton = false;
	}
	// ----------------------------------------------------------------------------------- //
}