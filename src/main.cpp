#include "core.h"
#include "usart.h"
#include "game.h"
#include "gfx.h"
#include "interrupts.h"
#include "timer.h"

static nim game;

void initialize_mcu() {
    // set game data
    start_game(&game);

	// enable buttons
    DDRB &= ~(1 << BTN);
    PORTB |= (1 << BTN);

	// heaps buttons used: PD2-PD5
	DDRD &= ~(0xF << PD2);
	PORTD |= (0xF << PD2);

	// enable interrupts
	sei();
	PCICR |= (1 << PCIE2) | (1 << PCIE0);
	PCMSK2 |= (0xF << PCINT18); // heap buttons interrupts
    PCMSK0 |= (1 << PCINT7);    // onboard button interrupts

    timer_init();

    lcd_start_screen();
    while (!MP);
    MP = false;
}

int main() {
    init();
    USART0_init(CLOCK_SPEED, BAUD);
    char nwon = 0, nlost = 0;

    initialize_mcu();

#ifdef __DEBUG
    _d_game_loop(&game);
#else
    while (true) {
        start_game(&game);
        bool won = game_loop(&game);
        if (won) {
            nwon++;
            render_win(nwon, nlost);
        } else {
            nlost++;
            render_lost(nwon, nlost);
        }
        while (!MP);
        MP = false;
    }
#endif

    return 0;
}
