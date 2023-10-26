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
	DDRB |= (1 << DDB2)|(1 << DDB1);
	
	// no invertido
	TCCR1A |= (1 << COM1A1)|(1 << COM1B1);
	
	// Fast PWM mode usando ICR1 como TOP
	TCCR1A |= (1 << WGM11);
	TCCR1B |= (1 << WGM12)|(1 << WGM13);
	
	// Timer con preescaler de 8
	TCCR1B |= (1 << CS11);

}

void PWM_1(){
	#define PWM_Frecuencia 250
	#define Valor_PWM_1 60

	// Top a 500
	ICR1 = F_CPU / (8 * PWM_Frecuencia) - 1;
	
	OCR1A = Valor_PWM_1;
	OCR1B = Valor_PWM_1;
}

void PWM_2(){
	#define PWM_Frecuencia 250
	#define Valor_PWM_2 400
	
	// Top a 500
	ICR1 = F_CPU / (8 * PWM_Frecuencia) - 1;
	
	OCR1A = Valor_PWM_2;
	OCR1B = Valor_PWM_2;
}

void PWM_3(){
	#define PWM_Frecuencia 250
	#define Valor_PWM_3 1000

	// Top a 500
	ICR1 = F_CPU / (8 * PWM_Frecuencia) - 1;
	
	OCR1A = Valor_PWM_3;
	OCR1B = Valor_PWM_3;
}


int main(void) {
	DDRD |= (1 << PD1); // set PB0 for output
	ADC_init(); // initialize ADC
	confPWM();  
	/*while(1) {
		uint16_t adc_result = ADC_read(0); // read adc value at PC0

		if(adc_result < 400) {
			PORTD |= (1 << PD1); // turn on PB0 if light level is high
			} else {
			PORTD &= ~(1 << PD1); // turn off PB0 if light level is low
		}

		_delay_ms(50); // wait a bit before next reading
	}*/
	while(1) {
		PWM_1();
		_delay_ms(3000);
		PWM_2();
		_delay_ms(3000);
		PWM_3();
		_delay_ms(3000);
	}

	return 0;
}
	
	
	

