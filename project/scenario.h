#ifndef SCENARIO_H
#define SCENARIO_H

#include <stdint.h>
#include <time.h>

typedef struct {
    uint32_t tick;
    float temp;
    float rh;
    uint32_t switches;
} scenario_line_t;

typedef struct {
    time_t start_time;
    scenario_line_t lines[1000];
    int count;
} scenario_t;

int scenario_load(scenario_t *s, const char *filename);
scenario_line_t scenario_get_line(const scenario_t *s, uint32_t tick);

#endif