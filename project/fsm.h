#ifndef FSM_H
#define FSM_H

#include <stdint.h>
#include "sensor.h"

// ST_UNDEF is set to 0. This is a C trick: because global arrays zero-initialize,
// any unhandled [state][event] combination in the transition table will default 
// to next=ST_UNDEF, action=A_NONE. We can safely catch this in fsm_step().
typedef enum {
    ST_UNDEF = 0,
    DAY,
    NIGHT,
    FEED_PAUSE,
    WATER_CHANGE,
    ALARM,
    N_STATES
} state_t;

typedef enum {
    E_NONE = 0,
    E_TEMP_OUT_OF_BOUNDS, // Temp < 23.0 or > 28.0
    E_ACK_AND_IN_BOUNDS,  // ACK pressed & Temp in [23.0, 28.0]
    E_WC_ON,              // Water change switch ON
    E_WC_OFF,             // Water change switch OFF
    E_FEED_PRESSED,       // Feed switch pressed
    E_FEED_TIMEOUT,       // 60 seconds elapsed in FEED state
    E_DAYLIGHT_ON,        // Daylight switch ON
    E_DAYLIGHT_OFF,       // Daylight switch OFF
    E_TEMP_LOW,           // Temp < 24.5
    E_TEMP_HIGH,          // Temp >= 25.5
    N_EVENTS
} event_t;

// Compound actions allow the FSM to dictate bulk hardware changes on transitions
typedef enum {
    A_NONE = 0,
    A_ENTER_DAY,    // Light ON, Pump ON
    A_ENTER_NIGHT,  // Light OFF, Pump ON
    A_ENTER_FEED,   // Pump OFF (Light stays as is)
    A_ENTER_WC,     // Heater OFF, Pump OFF
    A_ENTER_ALARM,  // Alarm Lamp ON (Pump/Heater preserve state)
    A_EXIT_ALARM,   // Alarm Lamp OFF
    A_HEAT_ON,      // Heater ON
    A_HEAT_OFF,     // Heater OFF
    N_ACTIONS
} action_t;

typedef struct {
    state_t next;
    action_t action;
} transition_t;

extern const transition_t fsm_table[N_STATES][N_EVENTS];

const char* state_name(state_t s);
const char* event_name(event_t e);
const char* action_name(action_t a);

// Helper to convert physical hardware states into an FSM event
event_t fsm_get_event(state_t current, const reading_t *r, uint32_t switches, uint32_t pressed, uint32_t ticks_in_state);

// Core table lookup function
state_t fsm_step(state_t s, event_t e, action_t *action);

#endif