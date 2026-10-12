#include <stdio.h>
#include <stdlib.h>
#include "../stats.h"

int main() {
    int fails = 0;

    float arr1[] = {5.0f};
    if (stats_max(arr1, 1) != 5.0f) {
        printf("FAIL: max of one value\n");
        fails++;
    } else {
        printf("PASS: max of one value\n");
    }

    float arr2[] = {10.0f, 2.0f, 3.0f};
    if (stats_max(arr2, 3) != 10.0f) {
        printf("FAIL: max at the start\n");
        fails++;
    } else {
        printf("PASS: max at the start\n");
    }

    float arr3[] = {1.0f, 2.0f, 10.0f};
    if (stats_max(arr3, 3) != 10.0f) {
        printf("FAIL: max at the end\n");
        fails++;
    } else {
        printf("PASS: max at the end\n");
    }

    float arr4[] = {35.0f, 30.0f, 35.0f};
    int start_index;
    int run_len = stats_longest_run_above(arr4, 3, 30.0f, &start_index);
    if (run_len != 1) {
        printf("FAIL: value exactly equal to threshold ends a run\n");
        fails++;
    } else {
        printf("PASS: value exactly equal to threshold ends a run\n");
    }

    float *arr5 = malloc(10000 * sizeof(float));
    for (int i = 0; i < 10000; i++) arr5[i] = (float)i;
    arr5[5000] = 99999.0f; //set a know max val
    if (stats_max(arr5, 10000) != 99999.0f) {
        printf("FAIL: max of 10,000 values\n");
        fails++;
    } else {
        printf("PASS: max of 10,000 values\n");
    }
    free(arr5);

    if (fails > 0) {
        printf("Summary: %d tests FAILED\n", fails);
        return 1;
    } else {
        printf("Summary: all tests PASSED\n");
        return 0;
    }
}
