#include "lock.h"

const transition_t lock_table[N_STATES][N_EVENTS] = {
    [LOCKED] = {
        [E_RIGHT]   = {GOT1, A_NONE},
        [E_WRONG]   = {LOCKED, A_BEEP},
        [E_CLOSE]   = {LOCKED, A_NONE},
        [E_TIMEOUT] = {LOCKED, A_NONE}
    },
    [GOT1] = {
        [E_RIGHT]   = {GOT2, A_NONE},
        [E_WRONG]   = {LOCKED, A_BEEP},
        [E_CLOSE]   = {LOCKED, A_NONE},
        [E_TIMEOUT] = {LOCKED, A_NONE}
    },
    [GOT2] = {
        [E_RIGHT]   = {OPEN, A_UNLOCK},
        [E_WRONG]   = {LOCKED, A_BEEP},
        [E_CLOSE]   = {LOCKED, A_NONE},
        [E_TIMEOUT] = {LOCKED, A_NONE}
    },
    [OPEN] = {
        [E_RIGHT]   = {OPEN, A_NONE},
        [E_WRONG]   = {OPEN, A_NONE},
        [E_CLOSE]   = {LOCKED, A_LOCK},
        [E_TIMEOUT] = {LOCKED, A_LOCK}
    }
};

state_t lock_step(state_t s, event_t e, action_t *action) {
    *action = lock_table[s][e].action;
    return lock_table[s][e].next;
}

int lock_event(state_t s, char c) {
    if (c == 'c') return E_CLOSE;
    if (c == 't') return E_TIMEOUT;
    
    if (s == LOCKED && c == '3') return E_RIGHT;
    if (s == GOT1 && c == '1') return E_RIGHT;
    if (s == GOT2 && c == '4') return E_RIGHT;
    
    if (c >= '0' && c <= '9') return E_WRONG;
    
    return -1; 
}

const char *state_name(state_t s) {
    static const char *names[] = {"LOCKED", "GOT1", "GOT2", "OPEN"};
    if (s >= 0 && s < N_STATES) return names[s];
    return "?";
}

const char *event_name(event_t e) {
    static const char *names[] = {"E_RIGHT", "E_WRONG", "E_CLOSE", "E_TIMEOUT"};
    if (e >= 0 && e < N_EVENTS) return names[e];
    return "?";
}

const char *action_name(action_t a) {
    static const char *names[] = {"A_UNDEFINED", "A_NONE", "A_UNLOCK", "A_LOCK", "A_BEEP"};
    if (a >= 0 && a < N_ACTIONS) return names[a];
    return "?";
}