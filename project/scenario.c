#include "scenario.h"
#include <stdio.h>
#include <string.h>

int scenario_load(scenario_t *s, const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) return 1;

    char buffer[256];
    int file_line_num = 0;
    int has_start = 0;
    s->count = 0;
    uint32_t last_tick = 0;

    while (fgets(buffer, sizeof(buffer), f)) {
        file_line_num++;
        
        // Skip leading whitespace to check for comments or blank lines[cite: 1, 2]
        char *ptr = buffer;
        while (*ptr == ' ' || *ptr == '\t') ptr++;
        if (*ptr == '#' || *ptr == '\n' || *ptr == '\r' || *ptr == '\0') {
            continue;
        }

        // Check for start line[cite: 1]
        if (!has_start) {
            long long start_val;
            if (sscanf(ptr, "start %lld", &start_val) == 1) {
                s->start_time = (time_t)start_val;
                has_start = 1;
                continue;
            } else {
                fclose(f);
                return file_line_num; // Missing or malformed start line[cite: 2]
            }
        }

        // Parse tick, temp, rh, switches (hex)[cite: 1]
        if (s->count >= 1000) {
            fclose(f);
            return file_line_num;
        }

        scenario_line_t *line = &s->lines[s->count];
        if (sscanf(ptr, "%u %f %f %x", &line->tick, &line->temp, &line->rh, &line->switches) != 4) {
            fclose(f);
            return file_line_num; // Missing field[cite: 2]
        }

        // Ticks must be increasing[cite: 1, 2]
        if (s->count > 0 && line->tick <= last_tick) {
            fclose(f);
            return file_line_num; 
        }
        
        last_tick = line->tick;
        s->count++;
    }

    fclose(f);
    
    // Fail if no start line was found[cite: 2]
    if (!has_start) return 1; 
    
    return 0; // Success[cite: 1]
}

scenario_line_t scenario_get_line(const scenario_t *s, uint32_t tick) {
    scenario_line_t active_line = {0};
    if (s->count == 0) return active_line;

    // Values hold between lines, so find the latest line that is <= current tick[cite: 1]
    active_line = s->lines[0];
    for (int i = 1; i < s->count; i++) {
        if (s->lines[i].tick <= tick) {
            active_line = s->lines[i];
        } else {
            break;
        }
    }
    return active_line;
}