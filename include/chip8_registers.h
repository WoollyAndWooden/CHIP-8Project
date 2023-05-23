#ifndef CHIP8_REGISTERS_H
#define CHIP8_REGISTERS_H

#include "config.h"

struct chip8_registers
{
    unsigned char V[NO_OF_REGISTERS];
    unsigned short I;
    unsigned char delay_timer;
    unsigned char sound_timer;
    unsigned short program_counter;
    unsigned char stack_pointer;
};

#endif // CHIP8_REGISTERS_H