#include "bits.h"
#include <stdio.h>


void print_binary(uint32_t x, int width) {
    for (int i = width - 1; i >= 0; i--) {
        printf("%u", (x >> i) & 1);
        if (i % 4 == 0 && i != width - 1) {
            printf(" ");
        }
    }
    printf("\n");
}

uint32_t get_field(uint32_t word, int pos, int width) {
    if (pos < 0 || pos > 32 || width < 1 || width > 32 || pos + width > 32) {
        return 0;
    }

    uint32_t shifted_word = word >> pos;

    uint32_t mask = (1ULL << width) - 1;
    return shifted_word & mask;
}

