#include "tally.h"
#include <string.h>
#include <stdbool.h>

typedef struct {
    char name[TALLY_LEN + 1];
    int count;
} entry_t;

struct tally {
    bool in_use;
    int size;
    entry_t entries[TALLY_NAMES];
};

static tally_t pool[TALLY_MAX];

tally_t *tally_create(void) {
    for(int i = 0; i < TALLY_MAX; i++) {
        if (!pool[i].in_use) {
            pool[i].in_use = true;
            pool[i].size = 0;
            return &pool[i];
        }
    }
    return NULL;
}

void tally_destroy(tally_t *t) {
    if (t == NULL) {
        return;
    }   
    if (t >= pool && t < pool + TALLY_MAX) {
        t->in_use = false;
        t->size = 0;
    }
}

int tally_add(tally_t *t, const char *name) {
    if (t == NULL || !t->in_use || name == NULL) {
        return -1;
    }

    size_t len = strlen(name);
    if (len == 0 || len > TALLY_LEN) {
        return -1;
    }

    int low = 0;
    int high = t->size -1;
    int insert_idx = t->size;

    while (low <= high) {
        int mid = low + (high - low) / 2;
        int cmp = strcmp(name, t->entries[mid].name);

        if (cmp == 0) {
            t->entries[mid].count++;
            return t->entries[mid].count;
        } else if (cmp < 0) {
            insert_idx = mid;
            high = mid - 1;
        } else {
            low = mid + 1;
            insert_idx = low;
        }
    }

    if (t->size >= TALLY_NAMES) {
        return -1;
    }

    for (int j = t->size; j > insert_idx; j--) {
        t->entries[j] = t->entries[j - 1];
    }

    memcpy(t->entries[insert_idx].name, name, len + 1);
    t->entries[insert_idx].count = 1;
    t->size++;

    return 1;
}

int tally_count(const tally_t *t, const char *name) {
    if (t == NULL || !t->in_use || name == NULL) {
        return 0;
    }

    int low = 0;
    int high = t->size - 1;

    while (low <= high) {
        int mid = low + (high - low) / 2;
        int cmp = strcmp(name, t->entries[mid].name);
        if (cmp == 0) {
            return t->entries[mid].count;
        } else if (cmp < 0) {
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }

    return 0;
}

int tally_size(const tally_t *t) {
    if (t == NULL || !t->in_use) {
        return 0;
    }
    return t->size;
}

void tally_print(const tally_t *t, FILE *out) {
    if (t == NULL || !t->in_use || out == NULL) {
        return;
    }
    for (int i = 0; i < t->size; i++) {
        fprintf(out, "%s %d\n", t->entries[i].name, t->entries[i].count);
    }
}