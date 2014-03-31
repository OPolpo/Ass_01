CC = gcc
CFLAGS = -g -Wall -pthread

all: ass01 clear

dft.o: dft.c
	$(CC) -c $(CFLAGS) $^

complex.o: complex.c
	$(CC) -c $(CFLAGS) $^

ass01: complex.o dft.o
	$(CC) -o ass01 main.c $(CFLAGS) $^


clear:
	rm *.o

