#ifndef GAME_H_
#define GAME_H_

#define HEAPS 4
#define OPH 7

struct nim
{
    char heaps[HEAPS];
    char lights_on;
};

void start_game(nim* game);
bool game_ended(nim* game);
void print_game(nim* game);
void machine_move(nim* game);
void player_move(nim* game);

#endif
