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
    lcd_start_screen();
    start_game(&game);
    for (char i = 0; i < HEAPS; i++) {
        draw_heap(i, game.heaps[i], false);
    }
    draw_turn_mark();

    /*start_game(&game);

#ifdef __DEBUG
    _d_game_loop(&game);
#else
    game_loop(&game);
#endif
*/
    return 0;
}
