#include "core.h"
#include "usart.h"
#include "game.h"
#include "gfx.h"

static nim game;
extern uint8_t REHF;
extern bool MP;

int main() {
    init();
    USART0_init(CLOCK_SPEED, BAUD);

    start_game(&game);

#ifdef __DEBUG
    _d_game_loop(&game);
#else
    game_loop(&game);
#endif

    return 0;
}
