#include "timer.h"

#include <avr/io.h>
#include <avr/interrupt.h>

#include "usart.h"

void timer_init()
{
    TCCR2A = 0;
    TCCR2B = 0;
    TCNT2 = 0;

    // ctc
    TCCR2A |= (1 << WGM21);

    // compare value for 1 ms at 16 MHz, prescaler 64
    OCR2A = 249;

    // enable interrupt
    TIMSK2 |= (1 << OCIE2A);

    // prescaler 64
    TCCR2B |= (1 << CS22);
}

static volatile unsigned long millis = 0;

ISR(TIMER2_COMPA_vect)
{
    millis++;
}

unsigned long get_millis()
{
    unsigned long ms;

    // disable interrupts while copying 4 bytes on 8 bit mcu
    cli();
    ms = millis;
    sei();

    return ms;
}
