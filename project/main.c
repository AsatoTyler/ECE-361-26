#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "iom361_r4.h"
#include "sensor.h"
#include "scenario.h"
#include "fsm.h"

extern void _iom361_set_sensor1(float temp, float humidity);
extern void _iom361_set_switches(uint32_t value);

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: ./aquarium scenario.txt [--report]\n");
        return 1;
    }

    scenario_t s;
    int load_rc = scenario_load(&s, argv[1]);
    if (load_rc != 0) {
        fprintf(stderr, "Error loading scenario at line %d\n", load_rc);
        return 1;
    }
    if (s.count == 0) return 0;

    int rc;
    uint32_t *base = iom361_initialize(4, 4, &rc);
    if (base == NULL) {
        fprintf(stderr, "Failed to initialize IOM361: %d\n", rc);
        return 1;
    }

    iom361_set_display(false);

    state_t current_state = DAY; 
    uint32_t prev_switches = 0;
    uint32_t ticks_in_state = 0;
    uint32_t last_tick = s.lines[s.count - 1].tick;

    for (uint32_t tick = 0; tick <= last_tick; tick++) {
        
        scenario_line_t line = scenario_get_line(&s, tick);
        _iom361_set_sensor1(line.temp, line.rh);
        _iom361_set_switches(line.switches);

        time_t current_time = s.start_time + tick;

        reading_t r;
        sensor_read(base, current_time, &r);
        uint32_t current_switches = iom361_readReg(base, SWITCHES_REG, &rc);
        uint32_t pressed = panel_pressed(current_switches, prev_switches);

        event_t e = fsm_get_event(current_state, &r, current_switches, pressed, ticks_in_state); 
        
        action_t action;
        current_state = fsm_step(current_state, e, &action);
        
        uint32_t current_leds = iom361_readReg(base, LEDS_REG, &rc);

        uint32_t status_word = 0;

        char time_buf[64];
        struct tm *tm_info = gmtime(&current_time); // UTC time
        strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S", tm_info);

        printf("%s state=%s temp=%.1f rh=%.1f leds=0x%X status=0x%08X\n",
               time_buf, state_name(current_state), r.temp_c, r.rh_pct, current_leds, status_word);
    }

    return 0;
}
