#include "reading.h"
#include <stdio.h>
#include <string.h>

#define LINE_LEN 128

void reading_read(int ticks[], float temps[], float hums[], int max_readings, int *count, int *skipped) {
    char line[LINE_LEN];
    
    while (fgets(line, sizeof line, stdin) != NULL) {
        int i = 0;
        while (line[i] == ' ' || line[i] == '\t')
            i++;
        if (line[i] == '\n' || line[i] == '\0' || line[i] == '#')
            continue;
        if (*count == max_readings) {
            fprintf(stderr, "warning: more than %d readings, the rest are ignored\n", max_readings);
            break;
        }
        if (sscanf(line, "%d %f %f", &ticks[*count], &temps[*count], &hums[*count]) == 3)
            (*count)++;
        else
            (*skipped)++;
    }
}