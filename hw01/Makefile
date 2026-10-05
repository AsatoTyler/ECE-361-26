CC = gcc
CFLAGS = -std=c11 -Wall -Wextra

all: bits.o status.o

bits.o: bits.c bits.h
	$(CC) $(CFLAGS) -c bits.c

test: tests/test_bits

tests/test_bits: tests/test_bits.c bits.o status.o
	$(CC) $(CFLAGS) -o tests/test_bits tests/test_bits.c bits.o status.o
	./tests/test_bits

clean:
	rm -f *.o tests/test_bits