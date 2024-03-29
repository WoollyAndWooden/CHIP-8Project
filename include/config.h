#ifndef CONFIG_H
#define CONFIG_H

#define WINDOW_TITLE "Chip-8 Emulator"

// Chip-8 memory is capable of accessing 4096B of RAM
#define MEMORY_SIZE 4096
#define PROGRAM_LOAD_ADDRESS 0x200

// Chip-8 display resolution
#define DISPLAY_WIDTH 64
#define DISPLAY_HEIGHT 32
#define DISPLAY_MULTIPLIER 10

// Chip-8 registers
#define NO_OF_REGISTERS 16

// Chip-8 stack
#define STACK_DEPTH 16

// Chip-8 keyboard
#define TOTAL_KEYS 16

// Chip-8 character set
#define CHARSET_LOAD_ADDRESS 0x00
#define DEFAULT_SPRITE_HEIGHT 5

#endif // CONFIG_H