CC = gcc
CFLAGS = -std=c11 -Wall -Wextra

# The main executable and test executable
TARGET = readings
TEST_TARGET = test_stats

# Object files
OBJS = main.o reading.o stats.o histogram.o
TEST_OBJS = tests/test_stats.o stats.o

# Default target
all: $(TARGET)

# Link the main program
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Link the test program
$(TEST_TARGET): $(TEST_OBJS)
	$(CC) $(CFLAGS) -o $(TEST_TARGET) $(TEST_OBJS)

# Compile object files and track header dependencies
main.o: main.c reading.h stats.h histogram.h
	$(CC) $(CFLAGS) -c main.c -o main.o

reading.o: reading.c reading.h
	$(CC) $(CFLAGS) -c reading.c -o reading.o

stats.o: stats.c stats.h
	$(CC) $(CFLAGS) -c stats.c -o stats.o

histogram.o: histogram.c histogram.h
	$(CC) $(CFLAGS) -c histogram.c -o histogram.o

tests/test_stats.o: tests/test_stats.c stats.h
	$(CC) $(CFLAGS) -c tests/test_stats.c -o tests/test_stats.o

# The test target builds and runs unit tests, then the regression script
test: $(TEST_TARGET) $(TARGET)
	./$(TEST_TARGET)
	sh regress.sh

# Clean up build artifacts
clean:
	rm -f $(OBJS) $(TEST_OBJS) $(TARGET) $(TEST_TARGET)