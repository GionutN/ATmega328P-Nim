#include "core.h"
#include "usart.h"
#include "game.h"

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

static nim game;

int main() {
    USART0_init(CLOCK_SPEED, BAUD);
    USART0_print("ATmega Nim USART\n");

    start_game(&game);
    print_game(&game);

    while (true) {
        machine_move(&game);
        print_game(&game);
        if (game_ended(&game)) {
            USART0_print("Win\n");
            break;
        }

        player_move(&game);
        print_game(&game);
        if (game_ended(&game)) {
            USART0_print("Lose\n");
            break;
        }
    }

    return 0;
}
