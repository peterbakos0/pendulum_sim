CC=gcc
RM=rm -f

all: main

clean:
	$(RM) main

main: main.c
	$(CC) -lm -lSDL2 -o main main.c

