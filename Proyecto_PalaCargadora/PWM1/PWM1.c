/*
 * CFile1.c
 *
 * Created: 4/17/2024 11:16:27 AM
 *  Author: diego
 */ 

#include "PWM1.h"

void init_PWM1_Fast(void){
	DDRB |= (1 << DDB0)|(1 << DDB1)|(1<< DDB2)|(1<<DDB5);
	
	PORTB &= ~(1<<PORTB5);
	
	TCCR1A = 0;
	TCCR1B = 0;
	TCCR1A |= (1<<COM1A1);
	TCCR1A |= (1<<COM1B1);
	
	TCCR1A |= (1<<WGM11);
	TCCR1B |= (1<<WGM12)|(1<<WGM13);
	
	//PRESCALER DE 64
	TCCR1B |= (1<<CS11)|(1<<CS10);
	
	//TOP DE 2499
	ICR1H = 0x09;
	ICR1L = 0xC3;
	
}

void updateDutyCycle1A(uint16_t duty){
	/*uint8_t temp = duty;
	OCR1AL = temp;
	duty >>= 8;
	temp = duty;
	OCR1AH = temp;*/
	OCR1AH = (duty & 0xFF00) >> 8;
	OCR1AL = (duty & 0x00FF);
	/*OCR1AH = (duty1 & 0xFF00) >> 8;
	OCR1AL = (duty1 & 0x00FF);
	//OCR1A = duty1;
	PORTB |= (1<<PORTB5);*/
}

void updateDutyCycle1B(uint16_t duty2){
	//OCR1BH = (duty & 0xFF00) >> 8;
	//OCR1BL = (duty & 0x00FF);
	OCR1B = duty2;
}

void updateDutyCycle1C(uint16_t duty3){
	if (TCNT1 <= duty3)
	{
		PORTB |= (1 << PORTB0);
	} else
	{
		PORTB &= ~(1 << PORTB0);
	}
}

uint16_t mapADC_PWM_Timer1(uint8_t valor_ADC1){
	return (uint16_t)(((float)valor_ADC1/255.0)*(149.88-37.47)+37.47);
}