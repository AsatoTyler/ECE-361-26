#ifndef PTR_H
#define PTR_H

// Exchanges *a and *b
void swap_float(float *a, float *b);

// Stores the smallest and largest of a[] in *min and *max and returns 0;
// Returns -1 if n < 0, or any pointer is NULL
int minmax(const float *a, int n, float *min, float *max);

// Returns a pointer to the value greater than threshold, or NULL if none
const float *find_above(const float *a, int n, float threshold);

// Reverses a[] in place. Does nothing if NULL or n < 2
void reverse(float *a, int n);

#endif