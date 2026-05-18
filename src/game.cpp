#include "game.h"

#include "usart.h"
#include "gfx.h"

extern volatile uint8_t REHF;
extern volatile bool MP;

bool game_ended(nim* game)
{
    return game->lights_on == 0;
}

void start_game(nim* game)
{
	// set game data
    game->lights_on = HEAPS * OPH;
    memset(game->heaps, OPH, HEAPS);

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

    lcd_start_screen();
    while (!MP);
    MP = false;
}

#ifdef __DEBUG
void _d_game_loop(nim* game)
{
    _d_print_game(game);
	while (true) {
        machine_move(game);
        _d_print_game(game);
        if (game_ended(game)) {
            USART0_print("Win\n");
            break;
        }

        _d_player_move(game);
        _d_print_game(game);
        if (game_ended(game)) {
            USART0_print("Lose\n");
            break;
        }
    }
}
#else
void game_loop(nim* game)
{
    for (char i = 0; i < HEAPS; i++) {
        draw_heap(i, game->heaps[i], false);
    }

    char heap;
	while (true) {
        heap = machine_move(game);
        if (game_ended(game)) {
            render_win();
            break;
        }
        draw_heap(heap, game->heaps[heap], false);

        heap = player_move(game);
        if (game_ended(game)) {
            render_lost();
            break;
        }
        draw_heap(heap, game->heaps[heap], false);
    }
}
#endif

char machine_move(nim* game) {
	char num_2plus = 0; // count of heaps with at least 2 elements
	char num_ones = 0;  // count of heaps with one element
    char heap;
	for (char i = 0; i < HEAPS; i++) {
		if (game->heaps[i] > 1) {
			num_2plus++;
		}
		else if (game->heaps[i] == 1) {
			num_ones++;
		}
	}

	if (num_2plus == 0) {
        // heaps with at most 1 element, make a random move
		for (char i = 0; i < HEAPS; i++) {
			if (game->heaps[i] == 1) {
				game->heaps[i]--;
                heap = i;
				break;
			}
		}
		game->lights_on--;

		return heap;
	}

	char X = 0;
	for (char i = 0; i < HEAPS; i++) {
		X ^= game->heaps[i];
	}

	if (X == 0) {
        // if the nimsum is 0, then there is no correct move
		for (char i = 0; i < HEAPS; i++) {
			if (game->heaps[i] > 1) {
				game->heaps[i]--;
                heap = i;
				break;
			}
		}
		game->lights_on--;

		return heap;
	}

    // only the parity of num_ones is of interest
	num_ones &= 1;

	for (char i = 0; i < HEAPS; i++) {
		char nimheapsum = X ^ game->heaps[i];
		if (nimheapsum < game->heaps[i]) {
			if (num_2plus == 1) {
				if (nimheapsum == 1 && num_ones == 1) {
					nimheapsum = 0;
				}
				if (nimheapsum == 0 && num_ones == 0) {
					nimheapsum = 1;
				}
			}

			game->lights_on -= game->heaps[i] - nimheapsum;
			game->heaps[i] = nimheapsum;
            heap = i;
			break;
		}
	}

    return heap;
}

#ifdef __DEBUG
void _d_print_game(nim* game)
{
	const char* names = "ABCD";
	for (int i = 0; i < HEAPS; i++) {
        char buff[11];
        buff[0] = names[i];
        buff[1] = ' ';
        char j;
		for (j = 0; j < game->heaps[i]; j++) {
            buff[j + 2] = '@';
		}
        buff[j + 2] = '\n';
        buff[j + 3] = '\0';
		USART0_print(buff);
	}

    USART0_print("\n\n");
}

void _d_player_move(nim* game) {
    char taken = 0;
    char selected = -1;
    char objects = 0;

    while (true) {
        if (MP) {
            MP = false;
            if (taken != 0) {
                break;
            }
        }

        for (char i = 0; i < HEAPS; i++) {
            if (REHF & (1 << (HB1 + i))) {
                REHF &= ~(1 << (HB1 + i));
                if (selected == -1) {
                    selected = i;
                    objects = game->heaps[selected];

                    // if an empty heap was chosen, let the player press another button
                    if (objects == 0) {
                        selected = -1;
                    }
                }
                if (i == selected) {
                    // allow the player to go back to the original number of elements
                    taken = (taken + 1) % (objects + 1);
                    game->heaps[selected] = objects - taken;
                    _d_print_game(game);
                }
            }
        }
    }

    game->lights_on -= taken;
}
#else
char player_move(nim* game)
{
	char taken = 0;
    char selected = -1;
    char objects = 0;

    while (true) {
        if (MP) {
            MP = false;
            if (taken != 0) {
                break;
            }
        }

        for (char i = 0; i < HEAPS; i++) {
            if (REHF & (1 << (HB1 + i))) {
                REHF &= ~(1 << (HB1 + i));
                if (selected == -1) {
                    // make the first heap be the last and vice versa
                    // so that the game screen corresponds to the buttons' positions
                    selected = 3 - i;
                    objects = game->heaps[selected];

                    // if an empty heap was chosen, let the player press another button
                    if (objects == 0) {
                        selected = -1;
                    }
                }
                if ((3 - i) == selected) {
                    // allow the player to go back to the original number of elements
                    taken = (taken + 1) % (objects + 1);
                    game->heaps[selected] = objects - taken;
                    draw_heap(selected, game->heaps[selected], true);
                }
            }
        }
    }

    game->lights_on -= taken;
    return selected;
}
#endif
