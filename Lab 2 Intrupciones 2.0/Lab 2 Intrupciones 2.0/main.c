#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define PWM_Frecuencia 250
#define Valor_PWM_1 20
#define Valor_PWM_2 50
#define Valor_PWM_3 150
#define Valor_PWM_4 255
int on=0;


ISR(INT1_vect) // Función que se ejecuta cuando se produce la interrupción INT0
{
	_delay_ms(3000); // pausa de 3s después de presionar el botón
	if (on == 1){
		on = 0;
		} else {
		on = 1;
	}
}


uint16_t ADC_read(uint8_t ch) {

	ch &= 0b00000111;
	ADMUX = (ADMUX & 0xF8)|ch;
	ADCSRA |= (1<<ADSC);
	
	while(!(ADCSRA & (1<<ADIF)));
	ADCSRA|=(1<<ADIF);

	return (ADC);
}

void ADC_init();
void confPWM();
void confPWM_2();
void confPWM_3();
void PWM_1();
void PWM_2();
void PWM_3();
void PWM_4();
void SEN();
void EICRACONFIG();


int main(void) {
	ICR1 = F_CPU / (8 * PWM_Frecuencia) - 1;
	EICRACONFIG();

	//confPWM();
	DDRD |= (1 << PD1)|(1 << PD2)|(1 << PD4);
	DDRC |= (1 << PC2)|(1 << PC4);
	DDRC |= (1 << PC1)|(1 << PC3);
	//DDRB = 0xFF;
	ADC_init();
	
	while(1) {
		if (on == 1){
			
			SEN();
			PORTD &= ~(1 << PORTD4); //apago led rojo
			PORTD |= (1 << PORTD2); //prendo led verde
			PWM_1();
			PORTC &= ~(1 << PORTC2);
			PORTC &= ~(1 << PORTC4);
			PORTC |= (1 << PORTC1);
			PORTC |= (1 << PORTC3);
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
			PORTD &= ~(1 << PORTD2); //apago led verde
			PORTD |= (1 << PORTD4); //prendo led rojo

			PORTC &= ~(1 << PORTC1);
			PORTC &= ~(1 << PORTC3);
			PORTC |= (1 << PORTC4);
			PORTC |= (1 << PORTC2);
			PWM_4();
			_delay_ms(3000);
			} else {
			PORTD |= (1 << PORTD4); //prendo led rojo
			_delay_ms(3000);
			PORTD &= ~(1 << PORTD4); //apago led rojo
			_delay_ms(3000);
		}
	}

	return 0;
}


void ADC_init() {
	ADMUX = (1<<REFS0);

	ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
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
	TCCR0B |= (1 << CS11);

}

void PWM_1(){
	OCR0A = Valor_PWM_1;
	OCR0B = Valor_PWM_1;
	
}

void PWM_2(){

	
	OCR0A = Valor_PWM_2;
	OCR0B = Valor_PWM_2;
}

void PWM_3(){
	
	
	OCR0A = Valor_PWM_3;
	OCR0B = Valor_PWM_3;
}

void PWM_4(){

	
	OCR0A = Valor_PWM_4;
	OCR0B = Valor_PWM_4;
	
	
}

void SEN(){
	int i = 200;
	while(i > 100) {
		uint16_t adc_result = ADC_read(0);

		if(adc_result < 400) {
			PORTD |= (1 << PD1);
			} else {
			PORTD &= ~(1 << PD1);
		}
		i--;
		_delay_ms(0);
	}
	
}

void EICRACONFIG(){
	DDRD &= ~(1 << PD3); // Configurar el pin PD3 como entrada
	//PORTD |= (1 << PD3); // Desactivar la resistencia pull-up interna del pin PD3
	// Configurar el pin PD3 como entrada con pull-up
	EICRA |= (1 << ISC10); // INT1 por cambio de valor
	// Habilitar la interrupción
	EIMSK |= (1 << INT1);
	sei();
}
