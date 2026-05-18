#include "core.h"
#include "usart.h"
#include "game.h"

static nim game;

int main() {

    USART0_init(CLOCK_SPEED, BAUD);
    USART0_print("ATmega Nim USART\n");

    start_game(&game);
    game_loop(&game);

    return 0;
}
