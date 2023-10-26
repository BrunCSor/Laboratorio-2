/*
 * main.c
 *
 * Created: 10/20/2023 3:47:49 PM
 *  Author: Bruno
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>


#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1

volatile uint8_t sistema_encendido = 0;
uint8_t velocidades[4] = {64, 128, 192, 255}; // Definir las 4 velocidades
uint8_t indice_velocidad = 0;

void ADC_init() {

	ADMUX = (1<<REFS0);

	ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
}

uint16_t ADC_read(uint8_t ch) {

	ch &= 0b00000111;
	ADMUX = (ADMUX & 0xF8)|ch;

	ADCSRA |= (1<<ADSC);

	while(!(ADCSRA & (1<<ADIF)));

	ADCSRA|=(1<<ADIF);

	return (ADC);
}

void setup() {
	// Configurar el PWM
	TCCR0A |= (1 << COM0A1) | (1 << WGM00); // Modo no invertido, modo rápido PWM
	TCCR0B |= (1 << CS00); // Sin preescalador
	DDRD |= (1 << PD5) | (1 << PD6); // Salida PD5 y PD6

	// Configurar interrupciones externas para el pulsador
	EICRA |= (1 << ISC01); // Interrupción en flanco de bajada
	EIMSK |= (1 << INT0); // Habilitar interrupción externa INT0

	sei(); // Habilitar interrupciones globales

	ADC_init(); 
}

ISR(INT0_vect) {
	sistema_encendido = !sistema_encendido; // Cambiar estado del sistema

	if(sistema_encendido) {
		PORTB |= (1 << PB0); // Encender LED verde
		PORTB &= ~(1 << PB1); // Apagar LED rojo
		} else {
		PORTB &= ~(1 << PB0); // Apagar LED verde
		PORTB |= (1 << PB1); // Encender LED rojo
		OCR0A = OCR0B = 0; // Parar los motores
		PORTB &= ~(1 << PB2); // Apagar LED blanco
	}
}

int main(void) {
	setup();
	
	while(1) {
		if(sistema_encendido) {
			uint16_t adc_result = ADC_read(0); 

			if(adc_result > 512) {
				PORTB |= (1 << PB2); // Encender LED blanco
				} else {
				PORTB &= ~(1 << PB2); // Apagar LED blanco
			}

			_delay_ms(3000); 

			OCR0A = OCR0B = velocidades[indice_velocidad]; // Control de velocidad del motor

			indice_velocidad++;
			if(indice_velocidad > 3) {
				indice_velocidad = 0;
				OCR0A = OCR0B = 0; //Apagar motores
			}
		}

		_delay_ms(3000); 
	}

	return 0;
}