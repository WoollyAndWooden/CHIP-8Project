#include "chip8_display.h"
#include "config.h"
#include <assert.h>

static void chip8_display_check_bounds(int x, int y)
{
    assert(x >= 0 && x < DISPLAY_WIDTH && y >= 0 && y < DISPLAY_HEIGHT);
}

void chip8_display_set(struct chip8_display *display, int x, int y)
{
    chip8_display_check_bounds(x, y);
    display->pixels[x][y] = true;
}
bool chip8_display_is_set(struct chip8_display *display, int x, int y)
{
    chip8_display_check_bounds(x, y);
    return display->pixels[x][y];
}

bool chip8_display_draw_sprite(struct chip8_display *display,
    int x, int y,
    const char *sprite, 
    int num)
{
    bool pixel_collision = false;

    for (int ly = 0; ly < num; ly++)
    {
        char c = sprite[ly];
        for(int lx = 0; lx < num; lx++)
        {
            if ((c & (0b10000000 >> lx)) == 0)
                continue;
            
            pixel_collision = display->pixels[(lx + x) % DISPLAY_WIDTH][(ly + y) % DISPLAY_HEIGHT];
            display->pixels[(lx + x) % DISPLAY_WIDTH][(ly + y) % DISPLAY_HEIGHT] ^= true;
        }
    }

    return pixel_collision;
}