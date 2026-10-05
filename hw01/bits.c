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

uint32_t set_field(uint32_t word, int pos, int width, uint32_t value) {
    if (pos < 0 || pos > 32 || width < 1 || width > 32 || pos + width > 32) {
        return word;
    }

    uint32_t mask = (1ULL << width) - 1;

    uint32_t shifted_value = value & mask;

    uint32_t shifted_word = word & ~(mask << pos);

    return shifted_word | (shifted_value << pos);
}

int32_t sign_extend(uint32_t value, int width) {
    if (width < 1 || width > 32) {
        return 0;
    }

    if (width == 32) {
        return (int32_t)value;
    }

    uint32_t sign_bit = (value >> (width - 1)) & 1;

    if (sign_bit) {
        value |= 0xFFFFFFFF << width;
    }
    return (int32_t)value;
}