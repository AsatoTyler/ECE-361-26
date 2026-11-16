CC = gcc
CFLAGS = -Wall -Wextra -std=c17 -g

.PHONY: all test memcheck clean

all: balance tests/test_istack

balance: balance.c istack.c istack.h
	$(CC) $(CFLAGS) -o balance balance.c istack.c

tests/test_istack: tests/test_istack.c istack.c istack.h
	$(CC) $(CFLAGS) -o tests/test_istack tests/test_istack.c istack.c

test: all
	./tests/test_istack
	./regress.sh

# The memcheck target modifies CFLAGS, cleans, and builds/tests from scratch
memcheck: CFLAGS += -fsanitize=address,undefined
memcheck: clean test

clean:
	rm -f balance tests/test_istack