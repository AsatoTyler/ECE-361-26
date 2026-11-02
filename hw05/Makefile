CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g

# Default target builds the main lock program
all: lock

lock: main.o lock.o
	$(CC) $(CFLAGS) -o lock main.o lock.o

# Object files and their header dependencies
main.o: main.c lock.h
	$(CC) $(CFLAGS) -c main.c

lock.o: lock.c lock.h
	$(CC) $(CFLAGS) -c lock.c

# Test target builds and runs the test suite
test: test_lock
	./test_lock

test_lock: tests/test_lock.o lock.o
	$(CC) $(CFLAGS) -o test_lock tests/test_lock.o lock.o

tests/test_lock.o: tests/test_lock.c lock.h
	$(CC) $(CFLAGS) -c tests/test_lock.c -o tests/test_lock.o

clean:
	rm -f lock test_lock *.o tests/*.o