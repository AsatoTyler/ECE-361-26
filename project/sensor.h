#ifndef SENSOR_H
#define SENSOR_H
#include <stdint.h>
// Reads temperature and humidity registers.

// Reads TEMP_REG and HUMID_REG of iom361 and converts them,
// and stores in *temp_c and *rh_pct. Returns 0 on success,
// nothing on failure.
int sensor_read(uint32_t *base, float *temp_c, float *rh_pct);

#endif