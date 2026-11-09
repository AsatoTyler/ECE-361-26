#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "scenario.h"
#include "sensor.h"
#include "panel.h"
#include "iom361_r4.h"

// Forward declarations for iom361 simulator "world" functions[cite: 1]
extern void _iom361_set_sensor1(float temp, float humidity);
extern void _iom361_set_switches(uint32_t value);

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: ./replay scenario.txt\n");
        return 1;
    }

    scenario_t s;
    int load_rc = scenario_load(&s, argv[1]);
    if (load_rc != 0) {
        fprintf(stderr, "Error loading scenario at line %d\n", load_rc);
        return 1;
    }

    if (s.count == 0) {
        return 0; // Nothing to replay
    }

    int rc;
    // Initialize iom361 with 4 switches and 4 LEDs[cite: 1]
    uint32_t *base = iom361_initialize(4, 4, &rc);
    if (!base) {
        fprintf(stderr, "iom361_initialize failed\n");
        return 1;
    }

    // Turn the display off[cite: 1]
    iom361_set_display(false);

    uint32_t last_tick = s.lines[s.count - 1].tick;
    uint32_t prev_switches = 0;

    // Loop for every tick from 0 to the last one[cite: 1]
    for (uint32_t tick = 0; tick <= last_tick; tick++) {
        // Get line in force[cite: 1]
        scenario_line_t line = scenario_get_line(&s, tick);

        // Set the "world"[cite: 1]
        _iom361_set_sensor1(line.temp, line.rh);
        _iom361_set_switches(line.switches);

        // The clock is start + tick[cite: 1]
        time_t current_time = s.start_time + tick;

        // Read sensor and switches through registers[cite: 1]
        reading_t r;
        sensor_read(base, current_time, &r);
        
        uint32_t current_switches = iom361_readReg(base, SWITCHES_REG, &rc);
        
        // Toggle LED k for every switch k just pressed[cite: 1]
        uint32_t pressed = panel_pressed(current_switches, prev_switches);
        for (int k = 0; k < 4; k++) {
            if ((pressed >> k) & 1) {
                panel_toggle(base, k);
            }
        }
        
        uint32_t current_leds = iom361_readReg(base, LEDS_REG, &rc);

        // Format time in UTC[cite: 2]
        char time_buf[64];
        struct tm *tm_info = gmtime(&current_time);
        strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S", tm_info);

        // Print trace line[cite: 1]
        printf("%s temp=%.1f rh=%.1f switches=0x%X pressed=0x%X leds=0x%X\n",
               time_buf, r.temp_c, r.rh_pct, current_switches, pressed, current_leds);

        prev_switches = current_switches;
    }

    return 0;
}