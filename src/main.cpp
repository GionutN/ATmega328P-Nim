#include <Arduino.h>

#define LED PB5
#define BTN PD2

bool pressed_last[4];

int main() {
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
