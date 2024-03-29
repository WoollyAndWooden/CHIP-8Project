#include "chip8_stack.h"
#include "chip8.h"
#include <assert.h>

static void chip8_stack_in_bounds(struct chip8 *chip8)
{
    assert(chip8->registers.stack_pointer < sizeof(chip8->stack.stack));
}

void chip8_stack_push(struct chip8 *chip8, unsigned short val)
{
    chip8->registers.stack_pointer += 1;
    chip8_stack_in_bounds(chip8);
    chip8->stack.stack[chip8->registers.stack_pointer] = val;
}

unsigned short chip8_stack_pop(struct chip8 *chip8)
{
    chip8_stack_in_bounds(chip8);
    unsigned short val = chip8->stack.stack[chip8->registers.stack_pointer]; 
    chip8->registers.stack_pointer -= 1;
    return val;   
}
