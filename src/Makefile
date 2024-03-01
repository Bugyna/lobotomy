CC=gcc
CLIBS=-lm -ldl
CFLAGS=-Wall -Wextra -pedantic -std=c11
IGNORE=-Wno-unused-function -Wno-unused-variable -Wno-unused-parameter


main: *.c *.h
	$(CC) main.c -o main $(CFLAGS) $(CLIBS) $(IGNORE)

a: sample_module.c
	$(CC) -o sample_module.o -c sample_module.c $(CFLAGS) $(CLIBS) $(IGNORE) -fpic
b:
	$(CC) -shared -o sample_module.so sample_module.o



sdl: l_sdl.c
	$(CC) -c l_sdl.c $(CFLAGS) $(CLIBS) -lSDL2 -lm -o l_sdl.o $(IGNORE) -fpic
sdla: l_sdl.o
	$(CC) -shared -o l_sdl.so l_sdl.o -lSDL2


run:
	./main