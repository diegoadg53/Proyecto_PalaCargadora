/*
 * ADC.c
 *
 * Created: 5/8/2024 4:31:18 PM
 *  Author: diego
 */ 

#include "ADC.h"

void init_ADC(void){
	ADMUX = 0;
	// Vref = AVCC = 5V
	ADMUX |= (1<<REFS0);
	// Justificado a la izquierda
	ADMUX |= (1<<ADLAR);
	ADMUX = ((ADMUX & 0xF8)|0x04);
	ADCSRA = 0;
	// Habilitar ADC
	ADCSRA |= (1<<ADEN);
	// Máscara de la interrupción del ADC
	ADCSRA |= (1<<ADIE);
	// Prescaler del ADC a 128
	ADCSRA |= (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);
	
	ADCSRB = 0;
	
	DIDR0 |= (1<<ADC4D)|(1<<ADC5D);
	
	// Iniciar primera conversión
	ADCSRA |= (1<<ADSC);
	
}