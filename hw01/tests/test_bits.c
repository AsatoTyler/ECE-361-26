#include <stdio.h>
#include <stdint.h>
#include "../bits.h"

int failed_tests = 0;

void check(uint32_t actual, uint32_t expected, const char *test_name) {
    if (actual == expected) {
        printf("PASS: %s\n", test_name);
    }
    else {
        printf("FAIL: %s (Expected: %u [0x%08X], Actual: %u [0x%08X])\n", 
            test_name, expected, expected, actual, actual);
        failed_tests++;
    }
}

int main(void) {
    printf("Testing print_binary\n");
    printf("Expected: 0001 1010\n");
    printf("Actual:   ");
    print_binary(0x1A, 8);
    printf("\n");

    printf("Testing get_field\n");
    check(get_field(0xFFFFFFFF, 0, 32), 0xFFFFFFFF, "get_field: width 32 boundary");
    check(get_field(0x80000000, 31, 1), 1, "get_field: pos 31 boundary");
    check(get_field(0xABCD1234, 4, 4), 3, "get_field: std mid-word extraction");

    printf("Test Summary\n");
    if (failed_tests == 0) {
        printf("All Tests Passed!\n");
        return 0;
    } else {
        printf("Failed Tests: %d\n", failed_tests);
        return 1;
    }
}