#ifndef STORE_H
#define STORE_H
// A binary search key that manages data, will hold record_t and store_t.
#include <stdint.h>
#include "sensor.h"

// Note: You should define your state enum (DAY, NIGHT, FEED_PAUSE, etc.) 
// in fsm.h, and include it here. Using an int here as a placeholder.
typedef struct {
    reading_t reading;
    int state;               // The state of the aquarium at this moment
    uint32_t status_word;    // The packed status word
} record_t;

// Opaque type declaration: callers only see the pointer, never the struct internals
typedef struct store store_t;

// Example function prototypes you will need to implement in store.c
store_t *store_create(void);
void store_destroy(store_t *s);
// Add other prototypes like insert, find by key, etc., as required by the project sheet

#endif
