CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -g -I.

OBJS = main.o sensor.o panel.o scenario.o iom361_r4.o
TEST_OBJS = tests/test_hw06.o sensor.o panel.o scenario.o iom361_r4.o

.PHONY: all test clean

all: replay

# Builds replay from all modules with no warnings[cite: 2]
replay: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ -lm

test_runner: $(TEST_OBJS)
	$(CC) $(CFLAGS) -o $@ $^ -lm

# Each .o lists the headers it includes[cite: 2]
main.o: main.c scenario.h sensor.h panel.h iom361_r4.h
	$(CC) $(CFLAGS) -c $< -o $@

sensor.o: sensor.c sensor.h iom361_r4.h
	$(CC) $(CFLAGS) -c $< -o $@

panel.o: panel.c panel.h iom361_r4.h
	$(CC) $(CFLAGS) -c $< -o $@

scenario.o: scenario.c scenario.h
	$(CC) $(CFLAGS) -c $< -o $@

iom361_r4.o: iom361_r4.c iom361_r4.h
	$(CC) $(CFLAGS) -c $< -o $@

tests/test_hw06.o: tests/test_hw06.c sensor.h panel.h scenario.h iom361_r4.h
	$(CC) $(CFLAGS) -c $< -o $@

# make test runs unit tests, then comparisons via regress.sh[cite: 2]
test: test_runner replay
	./test_runner
	bash regress.sh

# make clean removes all generated files[cite: 2]
clean:
	rm -f *.o tests/*.o replay test_runner trace.tmp