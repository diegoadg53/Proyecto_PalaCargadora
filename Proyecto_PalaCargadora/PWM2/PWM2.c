/*
 * PWM2.c
 *
 * Created: 5/19/2024 12:49:01 PM
 *  Author: diego
 */ 

#include "PWM2.h"

void init_PWM2_Fast(void){
	DDRB |= (1 << DDB3);
	DDRD |= (1<< DDD3);
	
	TCCR2A = 0;
	TCCR2B = 0;
	TCCR2A |= (1<<COM2A1);
	TCCR2A |= (1<<COM2B1);
	
	TCCR2A |= (1<<WGM21)|(1<<WGM20);
	
	//PRESCALER DE 1024
	TCCR2B |= (1<<CS22)|(1<<CS21)|(1<<CS20);
	
}

void updateDutyCycle2A(uint8_t duty){
	OCR2A = duty;
}

void updateDutyCycle2B(uint8_t duty){
	OCR2B = duty;
}

uint8_t mapADC_PWM_Timer2(uint8_t valor_ADC){
	return (uint8_t)(((float)valor_ADC/122)*255);
}