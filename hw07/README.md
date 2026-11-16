# Homework 7

# How to Build and test

To build program, run:
`make` in the terminal

To run the tests, run:
`make test` to compile and execute tests.

To check for memeory leaks, run:
`make memcheck` to recompile the program and check that there are no memory leaks

To clean build files, run:
`make clean` to remove all object files and test executable

# Part 4

1. Destroy the same stack twice
- Sanitizer Report: `==[PID]==ERROR: AddressSanitizer: double-free on address [address] at pc [address] bp [address] sp [address]`
- What happened in memory: The program tried to call `free()` on a memory address that had already been deallocated.

2. Push onto a stack after destroying it
- Sanitizer Report: `==[PID]==ERROR: AddressSanitizer: heap-use-after-free on address [address] at pc [address] bp [address] sp [address]`
- What happened in memory: The program tried to write an int into an array that had already been destroyed.

3. Leave out `istack_destroy` in `balance.c`
- Sanitizer Report: `==[PID]==ERROR: LeakSanitizer: detected memory leaks`
- The program exited without calling `free()`, not freeing up the dynamically allocated memory.