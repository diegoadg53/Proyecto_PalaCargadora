/*
 * PWM2.h
 *
 * Created: 5/19/2024 12:48:46 PM
 *  Author: diego
 */ 


#ifndef PWM2_H_
#define PWM2_H_

#include <stdint.h>
#include <avr/io.h>

void init_PWM2_Fast(void);
void updateDutyCycle2A(uint8_t duty);
void updateDutyCycle2B(uint8_t duty);
uint8_t mapADC_PWM_Timer2(uint8_t valor_ADC);

#endif /* PWM2_H_ */