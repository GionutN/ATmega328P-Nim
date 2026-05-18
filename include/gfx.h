#ifndef GFX_H_
#define GFX_H_

void lcd_start_screen();
void render_win(char won, char lost);
void render_lost(char won, char lost);
void clear_screen();
void draw_heap(char heap, char items, bool selected);
void draw_turn_mark(bool player);

#endif
