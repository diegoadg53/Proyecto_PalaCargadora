/*
 * UART.h
 *
 * Created: 5/23/2024 1:39:00 PM
 *  Author: diego
 */ 


#ifndef UART_H_
#define UART_H_

#include <stdint.h>
#include <avr/io.h>

void init_UART9600(void);
void writeUART(char caracter);
void writeTextUART(char* texto);


#endif /* UART_H_ */