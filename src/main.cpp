#include <Arduino.h>

#include "usart.h"

#define LED PB5
#define BTN PD2

#define CLOCK_SPEED 16000000UL
#define BAUD 19200UL

void test_buttons(void)
{
    bool pressed_last[4];
    DDRB |= (1 << LED);

    for (int i = 0; i < 4; i++) {
        DDRD &= ~(1 << (BTN + i));
        PORTD |= (1 << (BTN + i));
    }
    
    while (true) {
        for (int i = 0; i < 4; i++) {
            if ((PIND & (1 << (BTN + i))) == 0) {
                if (pressed_last[i]);
                else {
                    pressed_last[i] = true;
                    PORTB ^= (1 << LED);
                }
            }
            else {
                pressed_last[i] = false;
            }
        }
    }
}

int main() {
    USART0_init(CLOCK_SPEED, BAUD);
    USART0_print("merge usart\n");

    return 0;
}
