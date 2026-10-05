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
    printf("--Testing print_binary--\n");
    printf("Expected: 0001 1010\n");
    printf("Actual:   ");
    print_binary(0x1A, 8);
    printf("\n");

    printf("--Testing get_field--\n");
    check(get_field(0xFFFFFFFF, 0, 32), 0xFFFFFFFF, "get_field: width 32 boundary");
    check(get_field(0x80000000, 31, 1), 1, "get_field: width 1, pos 31 boundary");
    check(get_field(0xABCD1234, 4, 4), 3, "get_field: mid-word extraction");
    printf("\n");

    printf("--Testing set_field--\n");
    check(set_field(0x00000000, 0, 32, 0xFFFFFFFF), 0xFFFFFFFF, "set_field: width 32 boundary");
    check(set_field(0xFFFFFFFF, 31, 1, 0), 0x7FFFFFFF, "set_field: width 1, pos 31 boundary");
    check(set_field(0x00000000, 4, 4, 0xF), 0x00000F0, "set_field: value too wide for field");
    printf("\n");

    printf("--Testing sign_extend--\n");
    check(sign_extend(0xF8, 8), -8, "sign_extend: negative value (0xF8, 8");
    check(sign_extend(0x7F, 8), 127, "sign_extend: positive value (0x7F, 8)");
    check(sign_extend(0x80000000, 32), -2147483648, "sign_extend: most negative 32-bit value (0x80000000, 32)");
    printf("\n");

    printf("--Test Summary--\n");
    if (failed_tests == 0) {
        printf("All Tests Passed!\n");
        return 0;
    } else {
        printf("Failed Tests: %d\n", failed_tests);
        return 1;
    }
}