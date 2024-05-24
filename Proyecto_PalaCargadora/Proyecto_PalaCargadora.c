/*
 * Proyecto_PalaCargadora.c
 *
 * Created: 5/8/2024 2:55:44 AM
 * Author : diego
 */ 

#define F_CPU 4000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdio.h>
#include <avr/eeprom.h>
#include "ADC/ADC.h"
#include "PWM1/PWM1.h"
#include "PWM2/PWM2.h"
#include "UART/UART.h"

volatile uint8_t valorADC_pot1 = 0;
uint8_t valorADAf_pot1 = 0;
uint8_t valor_pot1 = 0;

uint16_t dutyCycle1A = 0;

volatile uint8_t valorADC_pot2 = 0;
uint8_t valorADAf_pot2 = 0;
uint8_t valor_pot2 = 0;

uint16_t dutyCylce1B = 0;

volatile uint8_t valorADC_pot3 = 0;
uint8_t valorADAf_pot3 = 0;
uint8_t valor_pot3 = 0;

uint8_t dutyCycle2B = 0;

uint16_t dutyCylce1C = 0;

volatile uint8_t valorADC_pot4 = 0;
uint8_t valorADAf_pot4 = 0;
uint8_t valor_pot4 = 0;

uint8_t dutyCycle2A = 0;

volatile uint8_t alternador_pots = 0;

volatile uint8_t modo = 0; 
// modo 0 -> Manual
// modo 1 -> EEPROM
// modo 2 -> Adafruit

volatile uint8_t actualizar_eeprom = 0;

uint16_t eeprom_pot1_address = 0x0000;

uint16_t eeprom_pot2_address = 0x0001;

uint16_t eeprom_pot3_address = 0x0002;

uint16_t eeprom_pot4_address = 0x0003;

volatile char bufferRX;
volatile uint8_t bandera_lectura = 0;
volatile char* mensaje_recibido;


void setup(void);

uint16_t mapADC_PWM_Timer1_ServoDelantero(uint8_t valor_ADC1){
	return (uint16_t)(((float)valor_ADC1/255.0)*(217.41-149.94)+149.94);
}

uint16_t mapADC_PWM_Timer1_ServoElevacion(uint8_t valor_ADC1){
	return (uint16_t)(((float)valor_ADC1/255.0)*(217.41-114.95)+114.95);
}

uint16_t mapADC_PWM_Timer1_ServoGiro(uint8_t valor_ADC1){
	return (uint16_t)(((float)valor_ADC1/255.0)*(139.88-94.96)+94.96);
}

uint8_t mapADC_PWM_Timer2_ServoGiro(uint8_t valor_ADC){
	return (uint8_t)(((float)valor_ADC/255)*(14-9)+9);
}

int main(void)
{
    CLKPR = (1<<CLKPCE);
    CLKPR = (1<<CLKPS0); // CLK a 8 MHz
	setup();
    while (1) 
    {
		if (modo == 0)
		{
			PORTD |= (1<<PORTD5);
			PORTD &= ~((1<<PORTD4)|(1<<PORTD2));
			valor_pot1 = valorADC_pot1;
			valor_pot2 = valorADC_pot2;
			valor_pot3 = valorADC_pot3;
			valor_pot4 = valorADC_pot4;
		} else if (modo == 1) {
			PORTD |= (1<<PORTD4);
			PORTD &= ~((1<<PORTD5)|(1<<PORTD2));
			valor_pot1 = eeprom_read_byte((const uint8_t*) eeprom_pot1_address);
			valor_pot2 = eeprom_read_byte((const uint8_t*) eeprom_pot2_address);
			valor_pot3 = eeprom_read_byte((const uint8_t*) eeprom_pot3_address);
			valor_pot4 = eeprom_read_byte((const uint8_t*) eeprom_pot4_address);
		} else {
			PORTD |= (1<<PORTD2);
			PORTD &= ~((1<<PORTD4)|(1<<PORTD5));
			valor_pot1 = valorADAf_pot1;
			valor_pot2 = valorADAf_pot2;
			valor_pot3 = valorADAf_pot3;
			valor_pot4 = valorADAf_pot4;
		}
		if (actualizar_eeprom == 1){
			eeprom_update_byte((uint8_t*) eeprom_pot1_address, valorADC_pot1);
			eeprom_update_byte((uint8_t*) eeprom_pot2_address, valorADC_pot2);
			eeprom_update_byte((uint8_t*) eeprom_pot3_address, valorADC_pot3);
			eeprom_update_byte((uint8_t*) eeprom_pot4_address, valorADC_pot4);
			actualizar_eeprom = 0;
		}
		if (bandera_lectura == 1) {
			uint8_t i;
			if (mensaje_recibido[0] == "1"){
				for (i = 0; mensaje_recibido[i] != '\0'; i++){
					writeUART(mensaje_recibido[i]);
				}
				valorADAf_pot1 = mensaje_recibido[1];
			} else if (mensaje_recibido[0] == "2"){
				for (i = 0; mensaje_recibido[i] != '\0'; i++){
					writeUART(mensaje_recibido[i]);
				}
				valorADAf_pot2 = mensaje_recibido[1];
			} else if (mensaje_recibido[0] == "3"){
				for (i = 0; mensaje_recibido[i] != '\0'; i++){
					writeUART(mensaje_recibido[i]);
				}
				valorADAf_pot3 = mensaje_recibido[1];
			} else if (mensaje_recibido[0] == "4"){
				for (i = 0; mensaje_recibido[i] != '\0'; i++){
					writeUART(mensaje_recibido[i]);
				}
				valorADAf_pot4 = mensaje_recibido[1];
			}
			bandera_lectura = 0;
		}
		
		dutyCycle1A = mapADC_PWM_Timer1_ServoDelantero(valor_pot3);
		updateDutyCycle1A(dutyCycle1A);
		dutyCylce1B = mapADC_PWM_Timer1_ServoElevacion(valor_pot2);
		updateDutyCycle1B(dutyCylce1B);
		dutyCycle2B = mapADC_PWM_Timer2_ServoGiro(valor_pot1);
		updateDutyCycle2B(dutyCycle2B);
		if (valor_pot4 <= 122){
			// para atras
			PORTD &= ~(1<<PORTD7);
			PORTD |= (1<<PORTD6);
			dutyCycle2A = mapADC_PWM_Timer2((valor_pot4-122)*(-1));
			updateDutyCycle2A(dutyCycle2A);
		} else if (valor_pot4 <= 132){
			// idle
			PORTD &= ~((1<<PORTD7)|(1<<PORTD6));
			updateDutyCycle2A(0);
		} else {
			// para adelante
			PORTD &= ~(1<<PORTD6);
			PORTD |= (1<<PORTD7);
			dutyCycle2A = mapADC_PWM_Timer2(valor_pot4-133);
			updateDutyCycle2A(dutyCycle2A);
		}
    }
}

void setup(void){
	
	cli();
	DDRC = 0;
	PCMSK1 |= 0x07;
	PCICR |= (1 << PCIE1);
	DDRD |= (1<<DDD7)|(1<<DDD6)|(1<<DDD5)|(1<<DDD4)|(1<<DDD2);
	init_PWM1_Fast();
	init_PWM2_Fast();
	init_UART9600();
	init_ADC();
	sei();
}

ISR(PCINT1_vect){
	if (!(PINC & (1 << PINC0))) // Decrementar
	{
		if (modo == 0){
			modo = 1;
			writeTextUART("1");
		} else if (modo == 1){
			modo = 2;
			writeTextUART("2");
		} else if (modo == 2){
			modo = 0;
			writeTextUART("0");
		}
	}
	else if (!(PINC & (1 << PINC1))) // Incrementar
	{
		if (modo == 0){
			actualizar_eeprom = 1;
		} else if (modo == 2) {
			actualizar_eeprom = 2;
		}
	}
}

ISR(ADC_vect){
	ADCSRA &= ~(1<<ADEN);
	if (alternador_pots == 0){
		valorADC_pot1 = ADCH;
		ADMUX = ((ADMUX & 0xF8)|0x05);
		alternador_pots += 1;
	} else if (alternador_pots == 1){
		valorADC_pot2 = ADCH;
		ADMUX = ((ADMUX & 0xF8)|0x06);
		alternador_pots += 1;
	} else if (alternador_pots == 2){
		valorADC_pot3 = ADCH;
		ADMUX = ((ADMUX & 0xF8)|0x07);
		alternador_pots += 1;
	} else {
		valorADC_pot4 = ADCH;
		ADMUX = ((ADMUX & 0xF8)|0x04);
		alternador_pots = 0;
	}
	
	ADCSRA |= (1<<ADEN);
	ADCSRA |= (1<<ADSC);
	//ADCSRA |= (1<<ADIF);
}

ISR(USART_RX_vect){
	bandera_lectura = 1;
	mensaje_recibido = UDR0;
	
}