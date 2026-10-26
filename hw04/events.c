#include "tally.h"
#include <stdio.h>
#include <string.h>

#define LINE_MAX 256

int main(void) {
    tally_t *t = tally_create();
    if (t == NULL) {
        fprintf(stderr, "Error: tally_create failed\n");
        return 1;
    }

    char line[LINE_MAX];
    int skipped = 0;

    while (fgets(line, sizeof(line), stdin) != NULL) {
        
        /* Removes the newline */
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
            len--;
        }

        if (len == 0) {
            continue;
        }
        if (line[0] == '#') {
            continue;
        }
        if (tally_add(t, line) == -1) {
            skipped++;
        }
    }

    tally_print(t, stdout);
    printf("names: %d, lines skipped: %d\n", tally_size(t), skipped);

    tally_destroy(t);
    return 0;
}