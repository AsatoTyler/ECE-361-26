#include <stdio.h>
#include "iom361_r4.h"
#include "sensor.h"

int main(void) {
    int rc;

    uint32_t *base = iom361_initialize(4, 4, &rc);
    if (base == NULL) {
        printf("Failed to initialize IOM361: %d\n", rc);
        return 1;
    }

    // Set sensor to typical value
    _iom361_set_sensor1(25.0f, 60.0f);

    float temp_c, rh_pct;
    int read_status = sensor_read(base, &temp_c, &rh_pct);

    if(read_status == 0) {
        printf("Device: Aquarium Controller\n");
        printf("Reading: %.1f C, %.1f %%RH\n", temp_c, rh_pct);
    } else {
        printf("Failed to read sensor. Error code: %d\n", read_status);
    }

    return 0;
}
