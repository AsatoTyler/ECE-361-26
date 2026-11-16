#include <stdlib.h>
#include "istack.h"

struct istack {
    int *data;
    size_t count;
    size_t capacity;
};

istack_t *istack_create(void) {
    istack_t *s = malloc(sizeof(istack_t));
    if (!s) return NULL;

    s->data = malloc(ISTACK_START * sizeof(int));
    if (!s->data) {
        free(s);
        return NULL;
    }

    s->count = 0;
    s->capacity = ISTACK_START;
    return s;
}

void istack_destroy(istack_t *s) {
    if (!s) return;
    free(s->data);
    free(s);
}

int istack_push(istack_t *s, int v) {
    if (!s) return -1; //-1 if memory runs out

    if (s->count == s->capacity) {
        size_t new_capacity = s->capacity * 2;
        
        int *new_data = realloc(s->data, new_capacity * sizeof(int));
        if (!new_data) return -1;
        
        s->data = new_data;
        s->capacity = new_capacity;
    }

    s->data[s->count++] = v;
    return 0;
}

int istack_pop(istack_t *s, int *v) {
    if (!s || s->count == 0) return -1;

    s->count--;
    if (v) {
        *v = s->data[s->count];
    }
    return 0;
}

int istack_peek(const istack_t *s, int *v) {
    if (!s || s->count == 0) return -1;

    if (v) {
        *v = s->data[s->count - 1];
    }
    return 0;
}

size_t istack_count(const istack_t *s) {
    if (!s) return 0;
    return s->count;
}

bool istack_is_empty(const istack_t *s) {
    if (!s) return true;
    return s->count == 0;
}

size_t istack_capacity(const istack_t *s) {
    if (!s) return 0;
    return s->capacity;
}

int *istack_to_array(const istack_t *s, size_t *n) {
    if (!s || s->count == 0 || !n) {
        if (n) *n = 0;
        return NULL;
    }

    int *arr = malloc(s->count * sizeof(int));
    if (!arr) {
        *n = 0;
        return NULL;
    }

    for (size_t i = 0; i < s->count; i++) {
        arr[i] = s->data[i];
    }

    *n = s->count;
    return arr;
}
