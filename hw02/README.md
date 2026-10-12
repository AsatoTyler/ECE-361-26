# Homework 2

# Build and Test

To build the program, run:
'make' in the terminal

To run the tests, run:
'make test' to compile and execute tests, and regress.sh script.

To clean build files, run:
'make clean' to remove all generated object files

# Module Diagram

            ------------------------------
            |                            |
            |                            |
            |            main            |
            |                            |
            |                            |
            ------------------------------
                  /        |        \
                 /         |         \
                /          |          \
        -----------    -----------   -----------
        |         |    |         |   |         |
        | reading |    |  stats  |   |histogram|
        |         |    |         |   |         |
        -----------    -----------   -----------
