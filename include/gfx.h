#ifndef GFX_H_
#define GFX_H_

void lcd_start_screen();
void render_win();
void render_lost();
void clear_screen();
void draw_heap(char heap, char items, bool selected);
void draw_turn_mark();

#endif
