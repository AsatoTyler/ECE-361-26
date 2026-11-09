#include "panel.h"
#include "iom361_r4.h"

#include "panel.h"
#include "iom361_r4.h"

uint32_t panel_pressed(uint32_t now, uint32_t before) {
    return now & ~before;
}

uint32_t panel_released(uint32_t now, uint32_t before) {
    return ~now & before;
}

int panel_led(uint32_t *base, int n, int on) {
    if (n < 0 || n > 31) {
        return -1;
    }
    
    int rc;
    uint32_t current_leds = iom361_readReg(base, LEDS_REG, &rc);
    if (rc != 0) {
        return rc; 
    }
    
    if (on) {
        current_leds |= (1u << n);
    } else {
        current_leds &= ~(1u << n);
    }
    
    // Write back the new state
    iom361_writeReg(base, LEDS_REG, current_leds, &rc);
    
    return rc == 0 ? 0 : rc;
}

int panel_toggle(uint32_t *base, int n) {
    if (n < 0 || n > 31) {
        return -1;
    }
    
    int rc;
    uint32_t current_leds = iom361_readReg(base, LEDS_REG, &rc);
    if (rc != 0) {
        return rc;
    }
    
    current_leds ^= (1u << n);
    
    // Write back the modified state
    iom361_writeReg(base, LEDS_REG, current_leds, &rc);
    
    return rc == 0 ? 0 : rc;
}