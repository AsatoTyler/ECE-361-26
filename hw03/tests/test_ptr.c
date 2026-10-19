#include <stdio.h>
#include <stdlib.h>
#include "../ptr.h"

int main() {
    int failed_tests = 0;
    int total_tests = 0;

    printf("Running tests for ptr.c...\n");

    // Test swap_float
    total_tests++;
    float a = 1.5f, b = 2.5f;
    swap_float(&a, &b);
    if (a == 2.5f && b == 1.5f) {
        printf("PASS: swap_float.\n");
    } else {
        printf("FAIL: swap_float: Expected a = 2.5f, b = 1.5f; Got a = %f, b = %f\n", a, b);
        failed_tests++;
    }

    // Test minmax
    float min = -99.0f, max = 99.0f;

    total_tests++;
    float arr1[] = {3.0f, 1.0f, 4.0f, 2.0f};
    int res = minmax(arr1, 4, &min, &max);

    if(res == 0 && min == 1.0f && max == 4.0f) {
        printf("PASS: minmax with several values.\n");
    } else {
        printf("FAIL: minmax with several values: Expected min = 1.0f, max = 4.0f; Got min = %f, max = %f\n", min, max);
        failed_tests++;
    }

    // minmax with one value
    total_tests++;
    float arr2[] = {2.0f};
    res = minmax(arr2, 1, &min, &max);
    if(res == 0 && min == 2.0f && max == 2.0f) {
        printf("PASS: minmax with one value.\n");
    } else {
        printf("FAIL: minmax with one value: Expected min = 2.0f, max = 2.0f; Got min = %f, max = %f\n", min, max);
        failed_tests++;
    }

    // minmax with n = 0
    total_tests++;
    min = -99.0f; max = 99.0f;
    res = minmax(arr1, 0, &min, &max);
    if(res == -1 && min == -99.0f && max == 99.0f) {
        printf("PASS: minmax with n = 0.\n");
    } else {
        printf("FAIL: minmax with n = 0: Expected return = -1, min = -99.0f, max = 99.0f; Got return = %d, min = %f, max = %f\n", res, min, max);
        failed_tests++;
    }

    // minmax with NULL pointer
    total_tests++;
    res = minmax(NULL, 4, &min, &max);
    if(res == -1 && min == -99.0f && max == 99.0f) {
        printf("PASS: minmax with NULL pointer.\n");
    } else {
        printf("FAIL: minmax with NULL pointer.\n");
        failed_tests++;
    }

    // Test find_above
    float arr3[] = {1.0f, 2.0f, 3.0f, 4.0f};
    total_tests++;

    const float *match = find_above(arr3, 4, 2.5f);
    if(match != NULL && *match == 3.0f) {
        printf("PASS: find_above with match.\n");
    } else {
        printf("FAIL: find_above with match.\n");
        failed_tests++;
    }

    // find_above with no match
    total_tests++;
    match = find_above(arr3, 4, 10.0f);
    if(match == NULL) {
        printf("PASS: find_above with no match.\n");
    } else {
        printf("FAIL: find_above with no match.\n");
        failed_tests++;
    }

    // find_above with n = 0
    total_tests++;
    match = find_above(arr3, 0, 5.0f);
    if(match == NULL) {
        printf("PASS: find_above with n = 0.\n");
    } else {
        printf("FAIL: find_above with n = 0.\n");
        failed_tests++;
    }

    // Test reverse with even length
    total_tests++;
    float arr4[] = {1.0f, 2.0f, 3.0f, 4.0f};
    reverse(arr4, 4);
    if(arr4[0] == 4.0f && arr4[1] == 3.0f && arr4[2] == 2.0f && arr4[3] == 1.0f) {
        printf("PASS: reverse with even length.\n");
    } else {
        printf("FAIL: reverse with even length.\n");
        failed_tests++;
    }

    // reverse with odd length
    total_tests++;
    float arr5[] = {1.0f, 2.0f, 3.0f};
    reverse(arr5, 3);
    if(arr5[0] == 3.0f && arr5[1] == 2.0f && arr5[2] == 1.0f) {
        printf("PASS: reverse with odd length.\n");
    } else {
        printf("FAIL: reverse with odd length.\n");
        failed_tests++;
    }

    // Summary of test results
    printf("\nTest Summary: %d/%d tests passed, %d failed.\n", total_tests - failed_tests, total_tests, failed_tests);
    
    if (failed_tests > 0) {
        return 1;
    }
    
    return 0;
}
