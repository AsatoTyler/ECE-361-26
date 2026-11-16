#include <stdio.h>
#include <stdlib.h>
#include "istack.h"

int main(void) {
    istack_t *stack = istack_create();
    if (!stack) {
        return 1; 
    }

    int c;
    size_t line = 1;
    size_t col = 1;
    size_t pairs = 0;
    size_t deepest = 0;

    while ((c = getchar()) != EOF) {
        if (c == '(' || c == '[' || c == '{') {
            istack_push(stack, c);
            size_t current_depth = istack_count(stack);
            if (current_depth > deepest) {
                deepest = current_depth;
            }
        } else if (c == ')' || c == ']' || c == '}') {
            if (istack_is_empty(stack)) {
                printf("line %zu, column %zu: found '%c', nothing open\n", line, col, (char)c);
                istack_destroy(stack);
                return 1;
            }

            int top_val;
            istack_pop(stack, &top_val);
            char top = (char)top_val;

            if ((c == ')' && top != '(') ||
                (c == ']' && top != '[') ||
                (c == '}' && top != '{')) {
                
                char expected = '?';
                if (top == '(') expected = ')';
                else if (top == '[') expected = ']';
                else if (top == '{') expected = '}';

                printf("line %zu, column %zu: found '%c', expected '%c'\n", line, col, (char)c, expected);
                istack_destroy(stack);
                return 1;
            }
            pairs++;
        }

        // Update line and column counters
        if (c == '\n') {
            line++;
            col = 1;
        } else {
            col++;
        }
    }

    // Check for unclosed brackets at EOF
    if (!istack_is_empty(stack)) {
        size_t open_count = istack_count(stack);
        int top_val;
        istack_peek(stack, &top_val);
        printf("end of input: %zu still open, innermost '%c'\n", open_count, (char)top_val);
        istack_destroy(stack);
        return 1;
    }

    printf("balanced: %zu pairs, deepest %zu\n", pairs, deepest);
    istack_destroy(stack);
    return 0;
}