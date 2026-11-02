# Homework 5

This assignment implements a state machine (FLM) combination lock.
The keycode is `314`

# Build and Test

To build program, run:
`make` in the terminal

To run the program manually, run:
`./lock` The program accepts typed characters followed with `Enter` as inputs. Press `Ctrl+D` to end program and get final state.

To run the tests, run:
`make test` to compile and execute tests.

To clean build files, run:
`make clean` to remove all object files and test executable

# Transition Table

STATE/EVENT         E_RIGHT             E_WRONG             E_CLOSE             E_TIMEOUT
LOCKED              GOT1, A_NONE(3)     LOCKED, A_BEEP      LOCKED, A_NONE      LOCKED, A_NONE

GOT1                GOT2, A_NONE(1)     LOCKED, A_BEEP      LOCKED, A_NONE      LOCKED, A_NONE

GOT2                OPEN, A_UNLOCK(4)   LOCKED, A_BEEP      LOCKED, A_NONE      LOCKED, A_NONE

OPEN                OPEN, A_NONE        OPEN, A_NONE        LOCKED, A_NONE      LOCKED, A_LOCK

# State Diagram

               RIGHT (3)           RIGHT (1)           RIGHT (4)
   +--------+ -----------> +------+ -----------> +------+ -----------> +------+
   |        |              |      |              |      |              |      |
   | LOCKED |              | GOT1 |              | GOT2 |              | OPEN |
   |        | <----------- |      | <----------- |      | <----------- |      |
   +--------+   WRONG,     +------+   WRONG,     +------+    CLOSE,    +------+
    ^  |        CLOSE,                CLOSE,                 TIMEOUT    |  ^
    |  |        TIMEOUT               TIMEOUT                           |  |
    +--+                                                                +--+
   WRONG,                                                              RIGHT,
   CLOSE,                                                              WRONG
   TIMEOUT
