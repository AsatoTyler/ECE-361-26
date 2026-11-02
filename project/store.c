#include "store.h"
#include <stddef.h>

// Define a reasonable maximum for the static array implementation since malloc is introduced in week 7[cite: 10, 11].
#define STORE_MAX 256 

// The concrete struct hidden from the rest of the program[cite: 7, 11].
struct store {
    record_t records[STORE_MAX];
    int count;
};

// Internal static instance to avoid malloc[cite: 10].
static struct store instance;
static int in_use = 0;

// Compares two records based on relative humidity quantized to 1%[cite: 7].
// Returns < 0 if a < b, 0 if equal, > 0 if a > b.
int record_compare(const record_t *a, const record_t *b) {
    int rh_a = (int)a->reading.rh_pct;
    int rh_b = (int)b->reading.rh_pct;
    
    if (rh_a != rh_b) {
        return rh_a - rh_b;
    }
    
    // Tie-breaker for duplicates: chronological order using the timestamp[cite: 7].
    if (a->reading.timestamp != b->reading.timestamp) {
        return (a->reading.timestamp < b->reading.timestamp) ? -1 : 1;
    }
    return 0;
}

store_t *store_create(void) {
    if (in_use) {
        return NULL; 
    }
    in_use = 1;
    instance.count = 0;
    return &instance;
}

void store_destroy(store_t *s) {
    if (s == &instance) {
        in_use = 0;
    }
}

int store_insert(store_t *s, const record_t *rec) {
    if (s == NULL || rec == NULL || s->count >= STORE_MAX) {
        return -1;
    }

    // Find the correct insertion point to keep the array sorted[cite: 10, 11].
    int i = 0;
    while (i < s->count && record_compare(&s->records[i], rec) < 0) {
        i++;
    }

    // Shift later entries up by one to make room for the new record[cite: 10, 11].
    for (int j = s->count; j > i; j--) {
        s->records[j] = s->records[j - 1];
    }

    s->records[i] = *rec;
    s->count++;
    
    return 0;
}

const record_t *store_find_by_key(const store_t *s, float rh_target) {
    if (s == NULL || s->count == 0) {
        return NULL;
    }

    int target_quantized = (int)rh_target;

    // Linear search is sufficient for this temporary array implementation[cite: 11].
    for (int i = 0; i < s->count; i++) {
        if ((int)s->records[i].reading.rh_pct == target_quantized) {
            return &s->records[i];
        }
    }
    return NULL;
}

void store_walk(const store_t *s, void (*callback)(const record_t *)) {
    if (s == NULL || callback == NULL) {
        return;
    }
    
    // In-order walk is just iterating from start to finish on a sorted array[cite: 7, 11].
    for (int i = 0; i < s->count; i++) {
        callback(&s->records[i]);
    }
}

int store_count(const store_t *s) {
    if (s == NULL) return 0;
    return s->count;
}

int store_height(const store_t *s) {
    (void)s; // Suppress unused warning
    // Return 0 for now; height will be properly implemented when replaced by a binary search tree in week 8[cite: 7, 11].
    return 0; 
}