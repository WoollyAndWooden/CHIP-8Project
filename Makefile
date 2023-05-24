INCLUDES= -I ./include
FLAGS= -g
OBJECTS=./build/chip8_memory.o ./build/chip8_stack.o

all: ${OBJECTS}
	gcc $(FLAGS) $(INCLUDES) ./src/main.c ${OBJECTS} -L ./lib -lmingw32 -lSDL2main -lSDL2 -o ./bin/main

./build/chip8_memory.o:src/chip8_memory.c
	gcc ${FLAGS} ${INCLUDES} ./src/chip8_memory.c -c -o ./build/chip8_memory.o

./build/chip8_stack.o:src/chip8_stack.c
	gcc ${FLAGS} ${INCLUDES} ./src/chip8_stack.c -c -o ./build/chip8_stack.o

clean:
	del build/*