#ifndef LOCK_H
#define LOCK_H

typedef enum { LOCKED, GOT1, GOT2, OPEN, N_STATES } state_t;
typedef enum { E_RIGHT, E_WRONG, E_CLOSE, E_TIMEOUT, N_EVENTS } event_t;
typedef enum { A_UNDEFINED, A_NONE, A_UNLOCK, A_LOCK, A_BEEP, N_ACTIONS } action_t;

typedef struct { 
    state_t next; 
    action_t action; 
} transition_t;

extern const transition_t lock_table[N_STATES][N_EVENTS];

// Performs a single state transition using the lookup table
state_t lock_step(state_t s, event_t e, action_t *action);

// Maps an input character to the appropriate event based on the current state
int lock_event(state_t s, char c);

// Return string representations of states, events, and actions
const char *state_name(state_t s);
const char *event_name(event_t e);
const char *action_name(action_t a);

#endif
