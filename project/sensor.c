#include "sensor.h"
#include <math.h>
#include "iom361_r4.h"

int sensor_read(uint32_t *base, float *temp_c, float *rh_pct) {
    int rc;

    // Read TEMP_REG
    uint32_t raw_temp = iom361_readReg(base, TEMP_REG, &rc);
    if (rc != 0) {
        return rc;
    }

    // Read HUMID_REG
    uint32_t raw_humid = iom361_readReg(base, HUMID_REG, &rc);
    if (rc != 0) {
        return rc;
    }

    // Convert using AHT20 formulas
    float t = (raw_temp / 1048576.0f) * 200.0f - 50.0f; // Temp in Celsius
    float rh = (raw_humid / 1048576.0f) * 100.0f; // Humidity in percent

    // Round to 0.1
    *temp_c = roundf(t * 10.0f) / 10.0f;
    *rh_pct = roundf(rh * 10.0f) / 10.0f;

    return 0;
}