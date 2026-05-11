#ifndef USART_H_
#define USART_H_

#include <avr/io.h>
#include <stdio.h>

void USART0_init(unsigned long fclk, unsigned long baud);
void USART0_transmit(char data);
char USART0_receive();
void USART0_print(const char *data); 

#endif // USART_H_
