# CHIP-8 Project

## Introduction

A personal study project to create CHIP-8 emulator with SDL2.
It's created fully by me, using [Daniel McCarthy's course](https://intel.udemy.com/course/creating-a-chip-8-emulator-in-c) as a base, [technical reference](http://devernay.free.fr/hacks/chip8/C8TECH10.HTM) and [this guide](https://tobiasvl.github.io/blog/write-a-chip-8-emulator/#instructions) to understand the machine better. After the course, I tested the emulator with [Timendus test suite](https://github.com/Timendus/chip8-test-suite), and corrected the code to pass everything up to test four perfectly.

## Execution

After downloading .exe file from Release tab or compiling the program, open terminal and call `[path_to_executable]\main.exe [path_to_rom_file]`


## Next steps

Emulator opens roms and displays image correctly, and opcodes work as intended but program isn't optimised yet I'll work on making it run smoothly and pass quirk test next, then improve keyboard's response time, and create better layout. When everything will be ready, I'll create a proper GUI so it's not running from terminal and allows proper keyboard and resolution customisation.
