#include <stdio.h>
#include <stdlib.h>
#include "../istack.h"

int passes = 0;
int fails = 0;

void check(bool condition, const char *test_name) {
    if (condition) {
        printf("PASS: %s\n", test_name);
        passes++;
    } else {
        printf("FAIL: %s\n", test_name);
        fails++;
    }
}

int main(void) {
    istack_t *s = istack_create();
    int v = 999;

    /* 1. pop and peek on an empty stack leave *v alone */
    istack_pop(s, &v);
    check(v == 999, "Empty pop leaves *v alone");
    istack_peek(s, &v);
    check(v == 999, "Empty peek leaves *v alone");

    /* 2. last in, first out */
    istack_push(s, 10);
    istack_push(s, 20);
    istack_peek(s, &v);
    check(v == 20, "Peek gets last in");
    istack_pop(s, &v);
    check(v == 20, "Pop gets last in (LIFO)");
    istack_pop(s, &v);
    check(v == 10, "Pop gets first in (LIFO)");

    /* 3. full at 8, 9th push doubles room to 16 and keeps every value */
    for (int i = 1; i <= 8; i++) {
        istack_push(s, i);
    }
    check(istack_capacity(s) == 8, "Capacity is 8 when full");
    istack_push(s, 9);
    check(istack_capacity(s) == 16, "9th push doubles room to 16");
    
    bool kept_values = true;
    for (int i = 9; i >= 1; i--) {
        istack_pop(s, &v);
        if (v != i) kept_values = false;
    }
    check(kept_values, "Kept every value after resize");

    /* 4. 1000 pushes and pops */
    for (int i = 0; i < 1000; i++) {
        istack_push(s, i);
    }
    check(istack_count(s) == 1000, "1000 pushes successful");
    bool thousand_ok = true;
    for (int i = 999; i >= 0; i--) {
        istack_pop(s, &v);
        if (v != i) thousand_ok = false;
    }
    check(thousand_ok, "1000 pops successful and verified");

    /* 5. istack_to_array is bottom first and a copy */
    istack_push(s, 5);
    istack_push(s, 10);
    size_t n;
    int *arr = istack_to_array(s, &n);
    check(n == 2 && arr[0] == 5 && arr[1] == 10, "istack_to_array is bottom first");
    istack_pop(s, &v); 
    check(arr[1] == 10, "Array is a copy (unaffected by pop)");
    free(arr);
    istack_destroy(s);

    /* 6. NULL arguments */
    check(istack_push(NULL, 5) == -1, "Push handles NULL");
    check(istack_pop(NULL, &v) == -1, "Pop handles NULL");
    check(istack_count(NULL) == 0, "Count handles NULL");
    check(istack_is_empty(NULL) == true, "Is_empty handles NULL");
    check(istack_capacity(NULL) == 0, "Capacity handles NULL");
    check(istack_to_array(NULL, &n) == NULL && n == 0, "To_array handles NULL");

    /* 7. two stacks are independent */
    istack_t *s1 = istack_create();
    istack_t *s2 = istack_create();
    istack_push(s1, 100);
    istack_push(s2, 200);
    istack_pop(s1, &v);
    check(v == 100, "Stack 1 is independent");
    istack_pop(s2, &v);
    check(v == 200, "Stack 2 is independent");
    istack_destroy(s1);
    
    /* 8. destroying a stack that is not empty */
    istack_push(s2, 42);
    istack_push(s2, 84);
    istack_destroy(s2);
    check(true, "Destroyed non-empty stack without crashing");

    /* Summary */
    printf("\nTest Summary: %d Passed, %d Failed\n", passes, fails);
    return (fails > 0) ? 1 : 0;
}