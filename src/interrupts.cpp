#include <avr/interrupt.h>

#include "core.h"

// a flag bit is 1 if the logic is HIGH, 0 otherwise
uint8_t REHF = 0; // rising edge heaps flag

// a state bit is 1 if the buttons is not currently pressed
uint8_t PHBS = 0xF; // previous heap buttons state

ISR(PCINT2_vect)
{
    uint8_t crt_state = (PIND >> 2) & 0xF;
    uint8_t changed_pins = crt_state ^ PHBS;

    if (changed_pins & (1 << (PD2 - 2))) {
        if ((crt_state & (1 << (PD2 - 2))) == 0) {
            REHF |= (1 << PD2);
        }
    }
    if (changed_pins & (1 << (PD3 - 2))) {
        if ((crt_state & (1 << (PD3 - 2))) == 0) {
            REHF |= (1 << PD3);
        }
    }
    if (changed_pins & (1 << (PD4 - 2))) {
        if ((crt_state & (1 << (PD4 - 2))) == 0) {
            REHF |= (1 << PD4);
        }
    }
    if (changed_pins & (1 << (PD5 - 2))) {
        if ((crt_state & (1 << (PD5 - 2))) == 0) {
            REHF |= (1 << PD5);
        }
    }

    PHBS = crt_state;
}

bool MP = false;   // master button flags register
bool PMS = true;   // previous master state

ISR(PCINT0_vect)
{
    bool crt_state = (PINB & (1 << BTN)) != 0;
    if (crt_state != PMS) {
        if (!crt_state) {
            MP = true;
        }
    }

    PMS = crt_state;
}
