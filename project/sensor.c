#include "sensor.h"
#include "iom361_r4.h"
#include <math.h>

int sensor_read(uint32_t *base, time_t now, reading_t *r) {
    int rc;
    
    // Read the temperature register
    uint32_t temp_raw = iom361_readReg(base, TEMP_REG, &rc);
    if (rc != 0) {
        return rc; // Return the error code immediately, changing nothing[cite: 1]
    }
    
    // Read the humidity register
    uint32_t humid_raw = iom361_readReg(base, HUMID_REG, &rc);
    if (rc != 0) {
        return rc; // Return the error code immediately, changing nothing[cite: 1]
    }
    
    // Keep only bits 19..0 for each reading[cite: 1]
    temp_raw &= 0x0FFFFFu;
    humid_raw &= 0x0FFFFFu;
    
    // Convert using the provided iom361 formulas[cite: 1, 4]
    float temp_calc = ((float)temp_raw / 1048576.0f) * 200.0f - 50.0f;
    float humid_calc = ((float)humid_raw / 1048576.0f) * 100.0f;
    
    // Round to 0.1 and store the results[cite: 1]
    r->temp_c = roundf(temp_calc * 10.0f) / 10.0f;
    r->rh_pct = roundf(humid_calc * 10.0f) / 10.0f;
    
    // Store the timestamp[cite: 1]
    r->t = now;
    
    return 0;
}