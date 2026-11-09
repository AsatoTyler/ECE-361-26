#include "fsm.h"

// The state transition table (using C99 designated initializers)[cite: 10]
// Any omitted [event] automatically initializes to { ST_UNDEF, A_NONE }
const transition_t fsm_table[N_STATES][N_EVENTS] = {
    [DAY] = {
        [E_TEMP_OUT_OF_BOUNDS] = { ALARM, A_ENTER_ALARM },
        [E_WC_ON]              = { WATER_CHANGE, A_ENTER_WC },
        [E_FEED_PRESSED]       = { FEED_PAUSE, A_ENTER_FEED },
        [E_DAYLIGHT_OFF]       = { NIGHT, A_ENTER_NIGHT },
        [E_TEMP_LOW]           = { DAY, A_HEAT_ON },
        [E_TEMP_HIGH]          = { DAY, A_HEAT_OFF }
    },
    [NIGHT] = {
        [E_TEMP_OUT_OF_BOUNDS] = { ALARM, A_ENTER_ALARM },
        [E_WC_ON]              = { WATER_CHANGE, A_ENTER_WC },
        [E_FEED_PRESSED]       = { FEED_PAUSE, A_ENTER_FEED },
        [E_DAYLIGHT_ON]        = { DAY, A_ENTER_DAY },
        [E_TEMP_LOW]           = { NIGHT, A_HEAT_ON },
        [E_TEMP_HIGH]          = { NIGHT, A_HEAT_OFF }
    },
    [FEED_PAUSE] = {
        [E_TEMP_OUT_OF_BOUNDS] = { ALARM, A_ENTER_ALARM },
        [E_WC_ON]              = { WATER_CHANGE, A_ENTER_WC },
        [E_FEED_TIMEOUT]       = { DAY, A_ENTER_DAY }, // Will snap to NIGHT immediately next tick if switch is off
        [E_TEMP_LOW]           = { FEED_PAUSE, A_HEAT_ON },
        [E_TEMP_HIGH]          = { FEED_PAUSE, A_HEAT_OFF }
    },
    [WATER_CHANGE] = {
        [E_TEMP_OUT_OF_BOUNDS] = { ALARM, A_ENTER_ALARM },
        [E_WC_OFF]             = { DAY, A_ENTER_DAY }  
    },
    [ALARM] = {
        [E_ACK_AND_IN_BOUNDS]  = { DAY, A_EXIT_ALARM } // Will evaluate daylight/WC on next tick
    }
};

// One table lookup replacing a chain of ifs[cite: 10]
state_t fsm_step(state_t s, event_t e, action_t *action) {
    // Bounds check to prevent array overflow
    if (s <= ST_UNDEF || s >= N_STATES || e < 0 || e >= N_EVENTS) {
        *action = A_NONE;
        return s; 
    }
    
    transition_t t = fsm_table[s][e];
    
    // If the entry was empty/unhandled, stay in current state and do nothing
    if (t.next == ST_UNDEF) {
        *action = A_NONE;
        return s;
    }
    
    *action = t.action;
    return t.next;
}

// Generates the highest-priority event based on current state and hardware[cite: 11]
event_t fsm_get_event(state_t current, const reading_t *r, uint32_t switches, uint32_t pressed, uint32_t ticks_in_state) {
    float t = r->temp_c;
    int wc_on = (switches & (1 << 2)) != 0;
    int day_on = (switches & (1 << 0)) != 0;
    int ack_pressed = (pressed & (1 << 3)) != 0;
    int feed_pressed = (pressed & (1 << 1)) != 0;

    // 1. Alarm Conditions (Highest Priority)[cite: 6]
    if (t < 23.0f || t > 28.0f) {
        if (current != ALARM) return E_TEMP_OUT_OF_BOUNDS;
    }
    
    if (current == ALARM) {
        if (ack_pressed && (t >= 23.0f && t <= 28.0f)) {
            return E_ACK_AND_IN_BOUNDS;
        }
        return E_NONE; // Block normal operations while in ALARM
    }

    // 2. Water Change[cite: 6]
    if (wc_on && current != WATER_CHANGE) return E_WC_ON;
    if (!wc_on && current == WATER_CHANGE) return E_WC_OFF;
    if (current == WATER_CHANGE) return E_NONE; // Block feed/temp rules during WC

    // 3. Feed Pause[cite: 6]
    if (feed_pressed && current != FEED_PAUSE) return E_FEED_PRESSED;
    if (current == FEED_PAUSE && ticks_in_state >= 60) return E_FEED_TIMEOUT;
    
    // 4. Day / Night mode changes[cite: 6]
    // (We don't transition day/night during a feed pause, it waits until timeout)
    if (current != FEED_PAUSE) {
        if (day_on && current == NIGHT) return E_DAYLIGHT_ON;
        if (!day_on && current == DAY) return E_DAYLIGHT_OFF;
    }

    // 5. Heater Control (Applies in DAY, NIGHT, and FEED_PAUSE)[cite: 6]
    if (t < 24.5f) return E_TEMP_LOW;
    if (t >= 25.5f) return E_TEMP_HIGH;

    return E_NONE;
}

// String mapping helpers
const char* state_name(state_t s) {
    static const char* names[] = { "UNDEF", "DAY", "NIGHT", "FEED_PAUSE", "WATER_CHANGE", "ALARM" };
    if (s >= ST_UNDEF && s < N_STATES) return names[s];
    return "?";
}

const char* event_name(event_t e) {
    static const char* names[] = { 
        "NONE", "TEMP_OUT_OF_BOUNDS", "ACK_AND_IN_BOUNDS", "WC_ON", "WC_OFF", 
        "FEED_PRESSED", "FEED_TIMEOUT", "DAYLIGHT_ON", "DAYLIGHT_OFF", "TEMP_LOW", "TEMP_HIGH" 
    };
    if (e >= E_NONE && e < N_EVENTS) return names[e];
    return "?";
}

const char* action_name(action_t a) {
    static const char* names[] = { 
        "NONE", "ENTER_DAY", "ENTER_NIGHT", "ENTER_FEED", "ENTER_WC", 
        "ENTER_ALARM", "EXIT_ALARM", "HEAT_ON", "HEAT_OFF" 
    };
    if (a >= A_NONE && a < N_ACTIONS) return names[a];
    return "?";
}