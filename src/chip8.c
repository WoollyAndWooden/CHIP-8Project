#include "chip8.h"
#include <memory.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

const char chip8_default_character_set[] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0,
    0x20, 0x60, 0x20, 0x20, 0x70,
    0xF0, 0x10, 0xF0, 0x80, 0xF0, 
    0xF0, 0x10, 0xF0, 0x10, 0xF0, 
    0x90, 0x90, 0xF0, 0x10, 0x10, 
    0xF0, 0x80, 0xF0, 0x10, 0xF0,
    0xF0, 0x80, 0xF0, 0x90, 0xF0,
    0xF0, 0x10, 0x20, 0x40, 0x40, 
    0xF0, 0x90, 0xF0, 0x90, 0xF0,
    0xF0, 0x90, 0xF0, 0x10, 0xF0, 
    0xF0, 0x90, 0xF0, 0x90, 0x90, 
    0xE0, 0x90, 0xE0, 0x90, 0xE0, 
    0xF0, 0x80, 0x80, 0x80, 0xF0, 
    0xE0, 0x90, 0x90, 0x90, 0xE0, 
    0xF0, 0x80, 0xF0, 0x80, 0xF0, 
    0xF0, 0x80, 0xF0, 0x80, 0x80

};
void chip8_init(struct chip8 *chip8)
{
    memset(chip8, 0, sizeof(struct chip8));
    memcpy(chip8->memory.memory,  chip8_default_character_set, sizeof(chip8_default_character_set));
}

// Bitwise operations to get values
static unsigned short get_nnn(unsigned short opcode)
{
    return opcode & 0x0fff;
}
static unsigned short get_x(unsigned short opcode)
{
    return (opcode >> 8) & 0x0f;
}
static unsigned short get_y(unsigned short opcode)
{
    return (opcode >> 4) & 0x00f;
}
static unsigned short get_kk(unsigned short opcode)
{
    return opcode & 0x00ff;
}
static unsigned short get_last(unsigned short opcode)
{
    return opcode & 0x000f;
}

// To avoid switch in switch, 0x8 instructions go here
static void chip8_exec_0x8(struct chip8 *chip8, unsigned short opcode)
{
    unsigned short x = get_x(opcode);
    unsigned short y = get_y(opcode);
    switch(get_last(opcode))
    {
        //LD Vx, Vy: Stores Vy in Vx
        case 0x0:
            chip8->registers.V[x] = chip8->registers.V[y];
        break;
        // Set Vx OR Vy
        case 0x1:
            chip8->registers.V[x] = chip8->registers.V[x] | chip8->registers.V[y];
        break;

        // Set Vx AND Vy
        case 0x2:
            chip8->registers.V[x] = chip8->registers.V[x] & chip8->registers.V[y];
        break;

        // Set Vx XOR Vy
        case 0x3:
            chip8->registers.V[x] = chip8->registers.V[x] ^ chip8->registers.V[y];
        break;

        // ADD Vx, Vy: Vx = Vx + Vy, if > 255 set VF to 1, else 0.
        case 0x4:
            if (chip8->registers.V[x] + chip8->registers.V[y] > 255)
                chip8->registers.V[0xf] = 1;
            else
                chip8->registers.V[0xf] = 0;
            chip8->registers.V[x] = (chip8->registers.V[x] + chip8->registers.V[y]) & 0xff;
        break;

        // SUB Vx, Vy: If Vx > Vy, set VF to 1, else 0. Then Vx = Vx - Vy
        case 0x5:
            if (chip8->registers.V[x] > chip8->registers.V[y])
                chip8->registers.V[0xf] = 1;
            else
                chip8->registers.V[0xf] = 0;
            chip8->registers.V[x] -= chip8->registers.V[y]; 
        break;

        // SHR Vx {, Vy}: If the least-significant bit of Vx is 1, then VF is set to 1, else 0.
        // Then Vx is divided by 2.
        case 0x6:
            if (get_last(chip8->registers.V[x]) == 1)
                chip8->registers.V[0xf] = 1;
            else
                chip8->registers.V[0xf] = 0;
            chip8->registers.V[x] /= 2;
        break;

        // SUBN Vx , Vy: If Vy > Vx is 1, then VF is set to 1, else 0.
        // Then Vx = Vy - Vx.
        case 0x7:
            if (chip8->registers.V[y] > chip8->registers.V[x])
                chip8->registers.V[0xf] = 1;
            else
                chip8->registers.V[0xf] = 0;
            chip8->registers.V[x] = chip8->registers.V[y] - chip8->registers.V[x]; 
        break;

        // SHL Vx {, Vy}: If the most-significant bit of Vx is 1, then VF is set to 1, else 0.
        // Then Vx is multiplied by 2.
        case 0xE:
            if ((chip8->registers.V[x] & 0xf0) == 1)
                chip8->registers.V[0xf] = 1;
            else
                chip8->registers.V[0xf] = 0;
            chip8->registers.V[x] *= 2;
        break;
        
    }
}

// To avoid switch in switch, 0xF instructions go here
static void chip8_exec_0xF(struct chip8 *chip8, unsigned short opcode)
{
    unsigned short x = get_x(opcode);
    unsigned short kk = get_kk(opcode);

    switch(kk)
    {
        // LD Vx, DT: Set Vx = delay timer
        case 0x07:
            chip8->registers.V[x] = chip8->registers.delay_timer;
        break;

        // Fx0A - LD Vx, K: wait for a key press, then store value of key in Vx
        case 0x0A:
        break;

        // Fx15 - LD DT, Vx: delay timer = Vx
        case 0x15:
            chip8->registers.delay_timer = chip8->registers.V[x];
        break;

        // LD ST, Vx: sound timer = Vx
        case 0x18:
            chip8->registers.sound_timer = chip8->registers.V[x];
        break;

        // ADD I, Vx: I += Vx
        case 0x1E:
            chip8->registers.I += chip8->registers.V[x];
        break;

        // LD F, Vx: I = location of sprite for Vx
        case 0x29:

        break;



        // LD B, Vx: Store BCD representation of Vx in memory locations I, I+1 and I+2
        case 0x33:
        break;


        // LD [I], Vx: Store registers through V0 to Vx in memory, starting at I
        case 0x55:
        break;


        // Fx65 - LD Vx, [I]: Read memory registers V0 through Vx starting at I
        case 0x65:
        break;


    }
}

static void chip8_exec_extended(struct chip8 *chip8, unsigned short opcode)
{
    
    switch(opcode & 0xf000)
    {
        // JP addr: 1nnn Jump to location nnn
        case 0x1000:
            chip8->registers.program_counter = get_nnn(opcode);
        break;

        // CAL addr: Call subroutine at nnn
        case 0x2000:
            chip8_stack_push(chip8, chip8->registers.program_counter);
            chip8->registers.program_counter = get_nnn(opcode);
        break;

        // SE Vx, kk: Skip next instruction if Vx = kk
        case 0x3000:
            if (chip8->registers.V[get_x(opcode)] == get_kk(opcode))
                chip8->registers.program_counter += 2;
        break;

        // SNE Vx: Skip next instruction if Vx != kk
        case 0x4000:
            if (chip8->registers.V[get_x(opcode)] != get_kk(opcode))
                chip8->registers.program_counter += 2;
        break;

        // SE Vx, Vy: Skip next instruction if Vx == Vy
        case 0x5000:
            if (chip8->registers.V[get_x(opcode)] == chip8->registers.V[get_y(opcode)])
                chip8->registers.program_counter += 2;
        break;

        // LD Vx, Vy: Set Vx to kk
        case 0x6000:
            chip8->registers.V[get_x(opcode)] = get_kk(opcode);
        break;

        // ADD Vx: Adds kk to Vx then stores in Vx
        case 0x7000:
            chip8->registers.V[get_x(opcode)] += get_kk(opcode);
        break;

        // Under 8xyZ there is set of instructions defined by Z 
        case 0x8000:
            chip8_exec_0x8(chip8, opcode);
        break;

        // SNE Vx, Vy: Skip next instruction if Vx != Vy
        case 0x9000:
            if (chip8->registers.V[get_x(opcode)] != chip8->registers.V[get_y(opcode)])
                chip8->registers.program_counter += 2;
        break;

        // LD I: set I register to nnn
        case 0xA000:
            chip8->registers.I = get_nnn(opcode);
        break;

        // JP V0 addr: Jump to location nnn + V0
        case 0xB000:
            chip8->registers.program_counter = get_nnn(opcode) + chip8->registers.V[0];
        break;

        // Rnd Vx, kk: Vx = random byte AND kk
        case 0xC000:
            time_t t;
            srand((unsigned) time(&t));

            chip8->registers.V[get_x(opcode)] = (rand() % 256) & get_kk(opcode);
        
        // DRW Vx, Vy, nibble: Display n-byte starting at memory location I at (Vx, Vy), set VF = collision
        case 0xD000:
        {
            unsigned short nibble = get_last(opcode);
            unsigned short x = get_x(opcode);
            unsigned short y = get_y(opcode);
            const char *sprite = &chip8->memory.memory[chip8->registers.I];
            if (chip8_display_draw_sprite(
                &chip8->display,
                chip8->registers.V[x],
                chip8->registers.V[y], 
                sprite,
                nibble))
                chip8->registers.V[0xf] = 1;
            else
                chip8->registers.V[0xf] = 0;
        }
        break;

        // Two skip under 0xE
        case 0xE000:
        {
            unsigned short kk = get_kk(opcode);

            // if key under Vx is pressed, skip next instruction
            if (kk == 0x9E)
                if(chip8_keyboard_is_down(&chip8->keyboard, chip8->registers.V[get_x(opcode)]))
                    chip8->registers.program_counter += 2;

            // if key under Vx is not pressed, skip next instruction
            if (kk == 0xA1)
                if(!chip8_keyboard_is_down(&chip8->keyboard, chip8->registers.V[get_x(opcode)]))
                    chip8->registers.program_counter += 2;
        }

        // There are several instructions under 0xF
        case 0xF:

        break;



    }
}
void chip8_exec(struct chip8 *chip8, unsigned short opcode)
{
    switch(opcode) 
    {
        // CLS: Clears the display
        case 0x00E0:
            chip8_display_clear(&chip8->display);
        break;

        // RET: Return from the subroutine
        case 0x00EE:
            chip8->registers.program_counter = chip8_stack_pop(chip8);
        break;

        // Non basic instructions
        default:
            chip8_exec_extended(chip8, opcode);

    }
}
void chip8_load(struct chip8 *chip8, const char *buf, size_t size)
{
    assert(size + PROGRAM_LOAD_ADDRESS < MEMORY_SIZE);
    memcpy(&chip8->memory.memory[PROGRAM_LOAD_ADDRESS], buf, size);
    chip8->registers.program_counter = PROGRAM_LOAD_ADDRESS;
    
}