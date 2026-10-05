#include "status.h"
#include "bits.h"

#include <string.h>

status_t status_unpack(uint16_t word) {
    status_t status;

    status.heat = get_field(word, HEAT_POS, HEAT_WIDTH);
    status.cool = get_field(word, COOL_POS, COOL_WIDTH);
    status.fan = get_field(word, FAN_POS, FAN_WIDTH);
    status.fault = get_field(word, FAULT_POS, FAULT_WIDTH);
    status.mode = get_field(word, MODE_POS, MODE_WIDTH);
    status.reserved = get_field(word, RESERVED_POS, RESERVED_WIDTH);

    uint32_t raw_setpoint = get_field(word, SETPOINT_POS, SETPOINT_WIDTH);
    status.setpoint = sign_extend(raw_setpoint, SETPOINT_WIDTH);

    // if mode 5,6,7, set it to 0 (OFF)
    if (status.mode > 4) {
        status.mode = 0;
    }

    return status;
}