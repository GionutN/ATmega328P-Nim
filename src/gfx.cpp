#include "gfx.h"
#include "core.h"

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>

static Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

void lcd_start_screen()
{
    tft.initR(INITR_BLACKTAB);
    tft.setRotation(1);
    tft.fillScreen(ST77XX_BLACK);
    tft.setTextColor(ST77XX_WHITE);

    tft.setTextSize(2);
    tft.setCursor(32, 56);
    tft.println("NIM GAME");
    
    tft.setTextSize(1);
    tft.setCursor(47, 75);
    tft.print("Press start");
}

void render_win(char won, char lost)
{
    tft.fillScreen(ST77XX_BLACK);
    tft.setTextColor(ST77XX_WHITE);

    tft.setTextSize(2);
    tft.setCursor(38, 56);
    tft.println("YOU WON");

    tft.setTextSize(1);
    tft.setCursor(5, 80);
    tft.println("Press start to play again");

    tft.setCursor(5, 5);
    tft.printf("W:%u", (unsigned)won);
    tft.setCursor(5, 15);
    tft.printf("L:%u", (unsigned)lost);
}

void render_lost(char won, char lost)
{
    tft.fillScreen(ST77XX_BLACK);
    tft.setTextColor(ST77XX_WHITE);

    tft.setTextSize(2);
    tft.setCursor(32, 56);
    tft.println("YOU LOST");

    tft.setTextSize(1);
    tft.setCursor(5, 80);
    tft.println("Press start to play again");

    tft.setCursor(5, 5);
    tft.printf("W:%u", (unsigned)won);
    tft.setCursor(5, 15);
    tft.printf("L:%u", (unsigned)lost);
}

void clear_screen()
{
    tft.fillScreen(ST77XX_BLACK);
}

void draw_heap(char heap, char items, bool selected)
{
    uint16_t color = selected ? ST77XX_RED : ST77XX_WHITE;
    char vpad = 7;
    char hpad = 5;
    tft.fillRect(24, heap * 32, 135, 32, ST77XX_BLACK);

    tft.setTextSize(3);
    tft.setTextColor(color);
    tft.setCursor(140, (heap + 1) * vpad + heap * 24);
    tft.printf("%u", (unsigned)heap);

    for (char i = 1; i <= items; i++) {
        tft.fillRect(129 - (i * 10 + (i - 1) * hpad), (heap + 1) * vpad + heap * 24 + 5, 10, 10, color);
    }
}

void draw_turn_mark(bool player)
{
    tft.fillRect(0, 0, 30, 128, ST77XX_BLACK);
    tft.setTextSize(2);
    tft.setTextColor(player ? ST77XX_GREEN : ST77XX_RED);

    tft.setCursor(10, 32);
    tft.print(player ? "P" : "W");
    tft.setCursor(10, 48);
    tft.print(player ? "L" : "A");
    tft.setCursor(10, 64);
    tft.print(player ? "A" : "I");
    tft.setCursor(10, 80);
    tft.print(player ? "Y" : "T");
}
