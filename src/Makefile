CC=gcc
CLIBS=-lm -ldl
CFLAGS=-Wall -Wextra -pedantic -std=c11
IGNORE=-Wno-unused-function -Wno-unused-variable -Wno-unused-parameter


CFILES=$(wildcard %.c %.h)




main: *.c *.h
	$(CC) -o main main.c $(CFLAGS) $(CLIBS) $(IGNORE)


a: sample_module.c
	$(CC) -o sample_module.o -c sample_module.c $(CFLAGS) $(CLIBS) $(IGNORE) -fpic
b:
	$(CC) -shared -o sample_module.so sample_module.o


run:
	./main