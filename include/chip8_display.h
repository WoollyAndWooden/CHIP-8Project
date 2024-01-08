#ifndef CHIP8_DISPLAY_H
#define CHIP8_DISPLAY_H

#include <stdbool.h>
#include "config.h"

struct chip8_display
{
    bool pixels[DISPLAY_WIDTH][DISPLAY_HEIGHT];
};

void chip8_display_set(struct chip8_display *display, int x, int y);
bool chip8_display_is_set(struct chip8_display *display, int x, int y);
bool chip8_display_draw_sprite(struct chip8_display *display,
    int x, int y,
    const char *sprite, 
    int num);
void chip8_display_clear(struct chip8_display *display);


#endif // CHIP8_DISPLAY_H