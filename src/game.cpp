#include "game.h"

#include "usart.h"

#include <avr/interrupt.h>

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
}

void game_loop(nim* game)
{
	_d_print_game(game);
	while (true) {
        machine_move(game);
        _d_print_game(game);
        if (game_ended(game)) {
            USART0_print("Win\n");
            break;
        }

        player_move(game);
        _d_print_game(game);
        if (game_ended(game)) {
            USART0_print("Lose\n");
            break;
        }
    }
}

void machine_move(nim* game) {
	char num_2plus = 0; // count of heaps with at least 2 elements
	char num_ones = 0;  // count of heaps with one element
	for (int i = 0; i < HEAPS; i++) {
		if (game->heaps[i] > 1) {
			num_2plus++;
		}
		else if (game->heaps[i] == 1) {
			num_ones++;
		}
	}

	if (num_2plus == 0) {
        // heaps with at most 1 element, make a random move
		for (int i = 0; i < HEAPS; i++) {
			if (game->heaps[i] == 1) {
				game->heaps[i]--;
				break;
			}
		}
		game->lights_on--;

		return;
	}

	char X = 0;
	for (int i = 0; i < HEAPS; i++) {
		X ^= game->heaps[i];
	}

	if (X == 0) {
        // if the nimsum is 0, then there is no correct move
		for (int i = 0; i < HEAPS; i++) {
			if (game->heaps[i] > 1) {
				game->heaps[i]--;
				break;
			}
		}
		game->lights_on--;

		return;
	}

    // only the parity of num_ones is of interest
	num_ones &= 1;

	for (int i = 0; i < HEAPS; i++) {
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
			break;
		}
	}
}

void _d_player_move(nim* game) {
    char taken = 0;
    char selected = 'e';
    char objects = 0;

    while (true) {
        if ((PINB & (1 << BTN)) == 0) {
            if (taken != 0) {
                break;
            }
        }

        char op = USART0_receive();
        if (op == -1) {
            continue;
        }
        if (selected == 'e') {
            selected = op;
            objects = game->heaps[selected - 'a'];
        }
        if (op == selected) {
            // allow the player to go back to the original number of elements
            taken = (taken + 1) % (objects + 1);
            game->heaps[selected - 'a'] = objects - taken;
            _d_print_game(game);
        }
    }

    game->lights_on -= taken;
}

void player_move(nim* game)
{
	char taken = 0;
    char selected = 'e';
    char objects = 0;

    while (true) {
        if ((PINB & (1 << BTN)) == 0) {
            if (taken != 0) {
                break;
            }
        }

        char op = USART0_receive();
        if (op == -1) {
            continue;
        }
        if (selected == 'e') {
            selected = op;
            objects = game->heaps[selected - 'a'];
        }
        if (op == selected) {
            // allow the player to go back to the original number of elements
            taken = (taken + 1) % (objects + 1);
            game->heaps[selected - 'a'] = objects - taken;
            _d_print_game(game);
        }
    }

    game->lights_on -= taken;
}
