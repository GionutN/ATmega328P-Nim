#include "core.h"
#include "usart.h"
#include "game.h"

#define BTNT PD2

static nim game;
extern uint8_t REHF;
extern bool MP;

int main() {

    USART0_init(CLOCK_SPEED, BAUD);
    USART0_print("ATmega Nim USART\n");

    start_game(&game);
    //_d_print_game(&game);
    //game_loop(&game);

    while (true) {
        if (REHF & (1 << PD2)) {
            REHF &= ~(1 << PD2);
            USART0_print("1\n");
        }
        if (REHF & (1 << PD3)) {
            REHF &= ~(1 << PD3);
            USART0_print("2\n");
        }
        if (REHF & (1 << PD4)) {
            REHF &= ~(1 << PD4);
            USART0_print("3\n");
        }
        if (REHF & (1 << PD5)) {
            REHF &= ~(1 << PD5);
            USART0_print("4\n");
        }
        if (MP) {
            MP = false;
            USART0_print("M\n");
        }
    }

    return 0;
}
