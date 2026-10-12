# AI Usage

* **Tools Used:** Gemini
* **What I used it for:** I used the AI to help me structure the requirements for the C code refactoring, write the `Makefile`, and draft the initial unit tests.
* **What it got wrong / What I had to fix:** During the testing phase, I encountered an `implicit declaration of function` error for `stats_max` and `stats_longest_run_above` when running `make test`. 
* **How I found and fixed it:** I found the error in the terminal output when the compiler stopped building `test_stats.o`. The issue occurred because the compiler couldn't find the function prototypes. I managed to fix the issue by cleaning the build and checking that the `test_stats.c` and `stats.h` files were correct, and found that I had misspelled the `#ifndef STATS_H` preprocessor.