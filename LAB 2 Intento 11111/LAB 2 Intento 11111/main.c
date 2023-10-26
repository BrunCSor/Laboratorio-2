#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>



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


void confPWM(){
	// PB1 Activado
	DDRD |= (1 << DDD5)|(1 << DDD6);
	
	// no invertido
	TCCR1A |= (1 << COM1A1)|(1 << COM1B1);
	
	// Fast PWM mode usando ICR1 como TOP
	TCCR1A |= (1 << WGM11);
	TCCR1B |= (1 << WGM12)|(1 << WGM13);
	
	// Timer con preescaler de 8
	TCCR1B |= (1 << CS11);

}

void confPWM_2(){
	// PB1 Activado
	DDRD |= (1 << DDD5);
	DDRD &= ~(1 << DDD6);
	// no invertido
	TCCR1A |= (1 << COM1A1)|(1 << COM1B1);
	
	// Fast PWM mode usando ICR1 como TOP
	TCCR1A |= (1 << WGM11);
	TCCR1B |= (1 << WGM12)|(1 << WGM13);
	
	// Timer con preescaler de 8
	TCCR1B |= (1 << CS11);

}

void confPWM_3(){
	// PB1 Activado
	DDRD &= ~(1 << DDD5);
	DDRD |= (1 << DDD6);
	// Configurar Timer0 en modo Fast PWM
	TCCR0A |= (1 << WGM00)|(1 << WGM01);
	
	// Configurar Timer0 para no invertir la señal PWM
	TCCR0A |= (1 << COM0A1)|(1 << COM0B1);
	
	// Iniciar Timer0 con preescaler de 8
	TCCR0B |= (1 << CS01);

}

void PWM_1(){
	#define PWM_Frecuencia 250
	#define Valor_PWM_1 20
	
	// Top a 500
	ICR1 = F_CPU / (8 * PWM_Frecuencia) - 1;
	
	OCR0A = Valor_PWM_1;
	OCR0B = Valor_PWM_1;
}

void PWM_2(){
	#define PWM_Frecuencia 250
	#define Valor_PWM_2 50
	
	// Top a 500
	ICR1 = F_CPU / (8 * PWM_Frecuencia) - 1;
	
	OCR0A = Valor_PWM_2;
	OCR0B = Valor_PWM_2;
}

void PWM_3(){
	#define PWM_Frecuencia 250
	#define Valor_PWM_3 150
	
	// Top a 500
	ICR1 = F_CPU / (8 * PWM_Frecuencia) - 1;
	
	OCR0A = Valor_PWM_3;
	OCR0B = Valor_PWM_3;
}

void PWM_4(){
	#define PWM_Frecuencia 250
	#define Valor_PWM_4 255
	
	// Top a 500
	ICR1 = F_CPU / (8 * PWM_Frecuencia) - 1;
	
	OCR0A = Valor_PWM_4;
	OCR0B = Valor_PWM_4;
}

void SEN(){
	int i = 200;
	while(i > 100) {
		uint16_t adc_result = ADC_read(0); // read adc value at PC0

		if(adc_result < 400) {
			PORTD |= (1 << PD1); // turn on PB0 if light level is high
			} else {
			PORTD &= ~(1 << PD1); // turn off PB0 if light level is low
		}
		i--;
		_delay_ms(0); // wait a bit before next reading
	}
	
}

int main(void) {
	DDRD |= (1 << PD1); // set PB0 for output
	DDRB |= (1 << PB2)|(1 << PB4); // Set PB2 and PB4 as outputs
	DDRB |= (1 << PB1)|(1 << PB3); // Set PB1 and PB3 as outputs
	//DDRB = 0xFF;
	ADC_init(); // initialize ADC
	//confPWM();
	
	while(1) {
		confPWM();
		SEN();
		PORTB |= (1 << PORTB1);
		PORTB |= (1 << PORTB3);
		PWM_1();
		_delay_ms(3000);
		
		confPWM_2();
		SEN();
		PWM_2();
		_delay_ms(3000);
		
		SEN();
		confPWM_3();
		PWM_3();
		_delay_ms(3000);
		
		SEN();
		confPWM();

		PORTB &= ~(1 << PORTB1);
		PORTB &= ~(1 << PORTB3);
		PORTB = 0b00001010;
		PORTB |= (1 << PORTB4);

		PWM_4();
		_delay_ms(3000);
	}

	return 0;
}




