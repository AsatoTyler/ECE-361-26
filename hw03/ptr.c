#include "ptr.h"
#include <stdio.h>
#include <stddef.h>

void swap_float(float *a, float *b) {
    float t = *a;
    *a = *b;
    *b = t;
}

int minmax(const float *a, int n, float *min, float *max) {
    if (n < 1 || a == NULL || min == NULL || max == NULL) {
        return -1;
    }

    const float *p = a;
    const float *end = a + n;

    float local_min = *p;
    float local_max = *p;
    p++;

    while (p < end) {
        if (*p < local_min) {
            local_min = *p;
        }
        if (*p > local_max) {
            local_max = *p;
        }
        p++;
    }

    *min = local_min;
    *max = local_max;

    return 0;
}

const float *find_above(const float *a, int n, float threshold) {
    if (n < 1 || a == NULL) {
        return NULL;
    }

    const float *p = a;
    const float *end = a + n;

    while (p < end) {
        if (*p > threshold) {
            return p;
        }
        p++;
    }

    return NULL;
}

void reverse(float *a, int n) {
    if (n < 2 || a == NULL) {
        return;
    }

    float *left = a;
    float *right = a + n - 1;

    while (left < right) {
        swap_float(left, right);
        left++;
        right--;
    }
}
