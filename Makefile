CC=gcc
RM=rm -f

all: main

clean:
	$(RM) main

main: main.c
	$(CC) -o main main.c -lm -lSDL2

