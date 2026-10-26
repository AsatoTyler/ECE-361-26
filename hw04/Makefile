CC = gcc
CFLAGS = -std=c11 -Wall -Wextra

all: events

events: events.o tally.o
	$(CC) $(CFLAGS) -o $@ $^

events.o: events.c tally.h
	$(CC) $(CFLAGS) -c $<

tally.o: tally.c tally.h
	$(CC) $(CFLAGS) -c $<

test: tests/test_tally
	./tests/test_tally

tests/test_tally: tests/test_tally.o tally.o
	$(CC) $(CFLAGS) -o $@ $^

tests/test_tally.o: tests/test_tally.c tally.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f events tests/test_tally *.o tests/*.o