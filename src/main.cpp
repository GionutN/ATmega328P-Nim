#include <Arduino.h>

#include "usart.h"

#define LED PB5
#define BTN PB7

#define CLOCK_SPEED 16000000UL
#define BAUD 19200UL

#define HEAPS 4
#define OPH 7

char heaps[HEAPS];
char lights_on;

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

void print_game()
{
	const char* names = "ABCD";
	for (int i = 0; i < HEAPS; i++) {
        char buff[11];
        buff[0] = names[i];
        buff[1] = ' ';
        char j;
		for (j = 0; j < heaps[i]; j++) {
            buff[j + 2] = '@';
		}
        buff[j + 2] = '\n';
        buff[j + 3] = '\0';
		USART0_print(buff);
	}

    USART0_print("\n\n");
}

void start_game(void)
{
    lights_on = HEAPS * OPH;
    memset(heaps, OPH, HEAPS);

    DDRB &= ~(1 << BTN);
    PORTB |= (1 << BTN); 
}

void machine_move() {
	char num_2plus = 0;
	char num_ones = 0;
	for (int i = 0; i < HEAPS; i++) {
		if (heaps[i] > 1) {
			num_2plus++;
		}
		else if (heaps[i] == 1) {
			num_ones++;
		}
	}

	if (num_2plus == 0) {
		for (int i = 0; i < HEAPS; i++) {
			if (heaps[i] == 1) {
				heaps[i]--;
				break;
			}
		}
		lights_on--;

		return;
	}

	char X = 0;
	for (int i = 0; i < HEAPS; i++) {
		X ^= heaps[i];
	}

	if (X == 0) {
		for (int i = 0; i < HEAPS; i++) {
			if (heaps[i] > 1) {
				heaps[i]--;
				break;
			}
		}
		lights_on--;

		return;
	}

	num_ones &= 1;

	for (int i = 0; i < HEAPS; i++) {
		char nimheapsum = X ^ heaps[i];
		if (nimheapsum < heaps[i]) {
			if (num_2plus == 1) {
				if (nimheapsum == 1 && num_ones == 1) {
					nimheapsum = 0;
				}
				if (nimheapsum == 0 && num_ones == 0) {
					nimheapsum = 1;
				}
			}

			lights_on -= heaps[i] - nimheapsum;
			heaps[i] = nimheapsum;
			break;
		}
	}
}

void player_move(void) {
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
        if (selected == 'e') {
            selected = op;
            objects = heaps[selected - 'a'];
        }
        if (op == selected) {
            taken = (taken + 1) % (objects + 1);
            heaps[selected - 'a'] = objects - taken;
            print_game();
        }
    }

    lights_on -= taken;
}

int main() {
    USART0_init(CLOCK_SPEED, BAUD);
    USART0_print("ATmega Nim USART\n");
    start_game();
    print_game();

    while (true) {
        machine_move();
        print_game();
        if (lights_on == 0) {
            USART0_print("Win\n");
            break;
        }

        player_move();
        print_game();
        if (lights_on == 0) {
            USART0_print("Lose\n");
            break;
        }
    }

    return 0;
}
