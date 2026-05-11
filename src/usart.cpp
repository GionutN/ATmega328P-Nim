#include "usart.h"

void USART0_init(unsigned long fclk, unsigned long baud)
{
    unsigned int ubrr = fclk / 16UL / baud - 1UL;

    // set baud rate
    UBRR0H = (unsigned char)(ubrr>>8);
    UBRR0L = (unsigned char)ubrr;

    // start transmitter
    UCSR0B = (1<<RXEN0)|(1<<TXEN0);

    // 8 bits, no parity
    UCSR0C = (3 << UCSZ00);
}

void USART0_transmit(char data)
{
    // wait until the buffer is empty
    while(!(UCSR0A & (1<<UDRE0)));

    UDR0 = data;
}

char USART0_receive()
{
    // wait while the buffer is empty
    while(!(UCSR0A & (1<<RXC0)));

    return UDR0;
}

void USART0_print(const char *data)
{
    char i = 0;
    while(data[i] != '\0') {
        USART0_transmit(data[i++]);
    }
}