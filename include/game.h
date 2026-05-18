#ifndef GAME_H_
#define GAME_H_

#include "core.h"

struct nim
{
    char heaps[HEAPS];
    char lights_on;
};

void start_game(nim* game);
bool game_ended(nim* game);
void game_loop(nim* game);
void _d_print_game(nim* game);
void machine_move(nim* game);
void _d_player_move(nim* game);
void player_move(nim* game);

#endif
