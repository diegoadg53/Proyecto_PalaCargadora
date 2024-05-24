/*
 * PWM1.h
 *
 * Created: 4/17/2024 11:14:29 AM
 *  Author: diego
 */ 


#ifndef PWM1_H_
#define PWM1_H_

#include <stdint.h>
#include <avr/io.h>

void init_PWM1_Fast(void);
void updateDutyCycle1A(uint16_t duty1);
void updateDutyCycle1B(uint16_t duty2);
void updateDutyCycle1C(uint16_t duty3);
uint16_t mapADC_PWM_Timer1(uint8_t valor_ADC1);

#endif /* PWM1_H_ */