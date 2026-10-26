# Homework 4

This Library contains the C pointer library.

# Build and Test

To build program, run:
`make` in the terminal

To run the tests, run:
`make test` to compile and execute tests.

To run program with sample data:
`./events < data/door.txt` after running make

To clean build files, run:
`make clean` to remove all object files and test executable

# What opaque prevents

When doing `t->size` in a file that only includes `tally.h`, the compiler generates an error message. 
Message: `error: dereferencing pointer to incomplete type ‘tally_t’ {aka ‘struct tally’}`

Opaque prevents the file from using or modifying the variables inside of `struct tally` because the compiler does not know what variables the struct holds, it throws an error while compiling. This is useful because it prevents third parties from viewing or modifying hidden data or variables, with them only being able to interact with them through functions we create.