#include <stdio.h>
#include "SDL2/SDL.h"
#include "chip8.h"

const char keyboard_map[TOTAL_KEYS] = {
    SDLK_0, SDLK_1, SDLK_2, SDLK_3,
    SDLK_4, SDLK_5, SDLK_6, SDLK_7, 
    SDLK_8, SDLK_9, SDLK_a, SDLK_b, 
    SDLK_c, SDLK_d, SDLK_e, SDLK_f
};

int main(int argc, char** argv)
{

    struct chip8 chip8;
    chip8_init(&chip8);

    chip8_display_draw_sprite(&chip8.display, 0, 0, &chip8.memory.memory[0x00], 5);
    chip8_display_draw_sprite(&chip8.display, 2, 0, &chip8.memory.memory[0x00], 5);
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* window = SDL_CreateWindow(
        CHIP8_WINDOW_TITLE,
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        DISPLAY_WIDTH * DISPLAY_MULTIPLIER, 
        DISPLAY_HEIGHT * DISPLAY_MULTIPLIER,
        SDL_WINDOW_SHOWN
    );

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_TEXTUREACCESS_TARGET);
    while(1)
    {
        SDL_Event event;
        while(SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_QUIT:
                    goto out;
                break;

                case SDL_KEYDOWN:
                {
                    char key = event.key.keysym.sym;
                    int vkey = chip8_keyboard_map(keyboard_map, key);
                    if(vkey != -1)
                    {
                        chip8_keyboard_down(&chip8.keyboard, vkey);
                    }
                }
                break;

                case SDL_KEYUP:
                {
                    char key = event.key.keysym.sym;
                    int vkey = chip8_keyboard_map(keyboard_map, key);
                    if(vkey != -1)
                    {
                        chip8_keyboard_up(&chip8.keyboard, vkey);
                    }
                }
                break;
            }
        }
        while(SDL_PollEvent(&event))
        {
            if(event.type == SDL_QUIT) goto out;
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
        for (int x = 0; x < DISPLAY_WIDTH; x++)
        {
            for (int y = 0; y < DISPLAY_HEIGHT; y++)
            {
                if (chip8_display_is_set(&chip8.display, x, y))
                {
                    SDL_Rect r;
                    r.x = x * DISPLAY_MULTIPLIER;
                    r.y = y * DISPLAY_MULTIPLIER;
                    r.w = DISPLAY_MULTIPLIER;
                    r.h = DISPLAY_MULTIPLIER;
                    SDL_RenderFillRect(renderer, &r);
                }
            }
        }
        
        SDL_RenderPresent(renderer);
    }

    out:

    SDL_DestroyWindow(window);
    return 0;
}