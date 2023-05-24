#include <stdio.h>
#include "SDL2/SDL.h"
#include "chip8.h"

int main(int argc, char** argv)
{

    struct chip8 chip8;
    chip8.registers.stack_pointer = 0;
    stack_push(&chip8, 0xff);
    stack_push(&chip8, 0xaa);

    printf("%x\n", stack_pop(&chip8));
    printf("%x\n", stack_pop(&chip8));

    // Test
    chip8_memory_set(&chip8.memory, 0x400, 'Z');
    printf("%c, \n", chip8_memory_get(&chip8.memory, 500));

    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* window = SDL_CreateWindow(
        CHIP8_WINDOW_TITLE,
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        DISPLAY_WIDTH * DISPLAY_MULTIPLAYER, DISPLAY_WIDTH * DISPLAY_MULTIPLAYER,
        SDL_WINDOW_SHOWN
    );

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_TEXTUREACCESS_TARGET);
    while(1)
    {
        SDL_Event event;
        while(SDL_PollEvent(&event))
        {
            if(event.type == SDL_QUIT) goto out;
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
        SDL_Rect r;
        r.x = 0;
        r.y = 0;
        r.w = 40;
        r.h = 40;
        SDL_RenderDrawRect(renderer, &r);
        SDL_RenderPresent(renderer);
    }

    out:

    SDL_DestroyWindow(window);
    return 0;
}