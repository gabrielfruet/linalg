CC = gcc
CFLAGS = -W -Wall -pedantic -lm -std=c11

all: linalg

linalg: 
	$(CC) -o linalg ./main.c ./container.c ./vector.c ./matrix.c ./solve.c ./utils.c $(CFLAGS)

