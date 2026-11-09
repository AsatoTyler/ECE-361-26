#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sensor.h"
#include "panel.h"
#include "scenario.h"
#include "iom361_r4.h"

// Forward declarations for iom361 injection
extern void _iom361_set_sensor1(float temp, float humidity);

int failures = 0;

void run_test(int condition, const char *test_name) {
    if (condition) {
        printf("PASS: %s\n", test_name); // PASS per test
    } else {
        printf("FAIL: %s\n", test_name); // FAIL per test[cite: 2]
        failures++;
    }
}

// Helper to write temporary scenario files for testing
void create_temp_file(const char *filename, const char *content) {
    FILE *f = fopen(filename, "w");
    if (f) {
        fputs(content, f);
        fclose(f);
    }
}

int main(void) {
    int rc;
    uint32_t *base = iom361_initialize(4, 4, &rc);
    
    // 1. Sensor reads 91.0°C[cite: 2]
    _iom361_set_sensor1(91.0f, 50.0f);
    reading_t r = {0, 0.0f, 0.0f};
    sensor_read(base, 1000, &r);
    run_test(r.temp_c == 91.0f, "Sensor reads 91.0C as 91.0");

    // 2. Failed read changes nothing[cite: 2]
    r.temp_c = 42.0f;
    r.rh_pct = 42.0f;
    // Pass NULL base to force an iom361 error code
    int err = sensor_read(NULL, 1005, &r);
    run_test(err != 0 && r.temp_c == 42.0f && r.rh_pct == 42.0f, "Failed read changes nothing");

    // 3. Panel pressed and released (press, release, held)[cite: 2]
    uint32_t before = 0xA; // 1010 in binary (switches 1 and 3 held)
    uint32_t now    = 0xC; // 1100 in binary (switch 2 pressed, 3 held, 1 released)
    run_test(panel_pressed(now, before) == 0x4, "panel_pressed identifies new press");
    run_test(panel_released(now, before) == 0x2, "panel_released identifies release");
    run_test((now & before) == 0x8, "held switch logic holds true outside functions");

    // 4. LED modifications preserve others[cite: 2]
    iom361_writeReg(base, LEDS_REG, 0x5, &rc); // 0101 (LEDs 0, 2 ON)
    panel_led(base, 1, 1); // Turn on LED 1 -> 0111 (0x7)
    run_test(iom361_readReg(base, LEDS_REG, &rc) == 0x7, "panel_led turns on without disturbing others");
    panel_toggle(base, 0); // Toggle LED 0 -> 0110 (0x6)
    run_test(iom361_readReg(base, LEDS_REG, &rc) == 0x6, "panel_toggle switches state without disturbing others");

    // 5 & 6. Scenario loading: comments, blank lines, and holding values[cite: 2]
    const char *valid_scenario = 
        "# A comment\n"
        "\n"
        "start 1000\n"
        "0 25.0 50.0 0x0\n"
        "10 26.0 55.0 0x1\n";
    create_temp_file("test_valid.txt", valid_scenario);
    scenario_t s;
    run_test(scenario_load(&s, "test_valid.txt") == 0, "Scenario loads with comments and blanks");
    scenario_line_t held = scenario_get_line(&s, 5);
    run_test(held.temp == 25.0f && held.switches == 0x0, "Scenario values hold between lines");
    remove("test_valid.txt");

    // 7. Scenario errors: no start, missing field, non-increasing ticks[cite: 2]
    create_temp_file("err_nostart.txt", "0 25.0 50.0 0x0\n");
    run_test(scenario_load(&s, "err_nostart.txt") != 0, "Scenario fails without start line");
    remove("err_nostart.txt");

    create_temp_file("err_missing.txt", "start 1000\n0 25.0 50.0\n"); // Missing switches field
    run_test(scenario_load(&s, "err_missing.txt") != 0, "Scenario fails with missing field");
    remove("err_missing.txt");

    create_temp_file("err_ticks.txt", "start 1000\n10 25.0 50.0 0x0\n5 26.0 55.0 0x1\n");
    run_test(scenario_load(&s, "err_ticks.txt") != 0, "Scenario fails with non-increasing ticks");
    remove("err_ticks.txt");

    return failures > 0 ? 1 : 0; // Nonzero exit status on failure[cite: 2]
}