#ifndef BITS_H
#define BITS_H

#include <stdint.h>

void print_binary(uint32_t x, int width); // Prints the lowest width bits of x, MSB first, in groups of 4 bits
uint32_t get_field(uint32_t word, int pos, int width); // Returns bits pos to pos-width+1 of word, shifted down 1 bit
uint32_t set_field(uint32_t word, int pos, int width, uint32_t value); // Returns word with bits pos to pos+width-1 replaced by the lowest width bits of value.
int32_t sign_extend(uint32_t value, int width); // Interprets the lowest width bits of value as two's comp. number and returns it as an int32_t.

#endif