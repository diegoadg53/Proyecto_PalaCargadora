/*
 * UART.c
 *
 * Created: 5/23/2024 1:39:10 PM
 *  Author: diego
 */ 

#include "UART.h"

void init_UART9600(void){
	
	// Configurar pines TX y RX
	DDRD &= ~(1<<DDD0);
	DDRD |= (1<<DDD1);
	
	//Configurar A modo Fast U2X0 = 1
	UCSR0A = 0;
	UCSR0A |= (1<<U2X0);
	
	//Configurar B Habilitar ISR RX
	UCSR0B = 0;
	UCSR0B |= (1<<RXCIE0)|(1<<RXEN0)|(1<<TXEN0);
	
	// Paso 4: Configurar C Frame: 8 bits datos, no paridad, 1 bit stop
	UCSR0C = 0;
	UCSR0C |= (1<<UCSZ01)|(1<<UCSZ00);
	
	// Baudrate = 9600
	UBRR0 = 103;
	
}

void writeUART(char caracter){
	while (!(UCSR0A & (1<<UDRE0)));
	UDR0 = caracter;
}

void writeTextUART(char* texto){
	uint8_t i;
	for(i = 0; texto[i] != '\0'; i++){
		while (!(UCSR0A & (1<<UDRE0)));
		UDR0 = texto[i];
	}
	writeUART(10);
	writeUART(13);
}