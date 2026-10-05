#ifndef STATUS_H
#define STATUS_H

#include <stdint.h>

// Constants
#define HEAT_POS 0
#define HEAT_WIDTH 1

#define COOL_POS 1
#define COOL_WIDTH 1

#define FAN_POS 2
#define FAN_WIDTH 1

#define FAULT_POS 3
#define FAULT_WIDTH 1

#define MODE_POS 4
#define MODE_WIDTH 3

#define RESERVED_POS 7
#define RESERVED_WIDTH 1

#define SETPOINT_POS 8
#define SETPOINT_WIDTH 8

typedef struct {
    uint8_t heat;
    uint8_t cool;
    uint8_t fan;
    uint8_t fault;
    uint8_t mode;
    uint8_t reserved;
    int8_t setpoint;
} status_t;

status_t status_unpack(uint16_t word);

#endif