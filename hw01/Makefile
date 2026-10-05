CC = gcc
CFLAGS = -std=c11 -Wall -Wextra

all: bits.o

bits.o: bits.c bits.h
	$(CC) $(CFLAGS) -c bits.c

test: tests/test_bits

tests/test_bits: tests/test_bits.c bits.o
	$(CC) $(CFLAGS) -o tests/test_bits tests/test_bits.c bits.o
	./tests/test_bits

clean:
	rm -f *.o tests/test_bits