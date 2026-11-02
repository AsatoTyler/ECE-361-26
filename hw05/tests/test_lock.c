#include <stdio.h>
#include <stdlib.h>
#include "../lock.h"

int failures = 0;

void run_test(int condition, const char *test_name) {
    if (condition) {
        printf("PASS: %s\n", test_name);
    } else {
        printf("FAIL: %s\n", test_name);
        failures++;
    }
}

int test_sequence(const char *seq, state_t expected_final_state) {
    state_t s = LOCKED;
    for (int i = 0; seq[i] != '\0'; i++) {
        int e = lock_event(s, seq[i]);
        if (e == -1) continue;
        action_t act;
        s = lock_step(s, (event_t)e, &act);
    }
    return s == expected_final_state;
}

int main(void) {
    action_t act;
    
    // --- 1. Test ALL 16 table entries ---
    
    // LOCKED state
    run_test(lock_step(LOCKED, E_RIGHT, &act) == GOT1 && act == A_NONE, "Table: LOCKED + RIGHT -> GOT1");
    run_test(lock_step(LOCKED, E_WRONG, &act) == LOCKED && act == A_BEEP, "Table: LOCKED + WRONG -> LOCKED (BEEP)");
    run_test(lock_step(LOCKED, E_CLOSE, &act) == LOCKED && act == A_NONE, "Table: LOCKED + CLOSE -> LOCKED");
    run_test(lock_step(LOCKED, E_TIMEOUT, &act) == LOCKED && act == A_NONE, "Table: LOCKED + TIMEOUT -> LOCKED");

    // GOT1 state
    run_test(lock_step(GOT1, E_RIGHT, &act) == GOT2 && act == A_NONE, "Table: GOT1 + RIGHT -> GOT2");
    run_test(lock_step(GOT1, E_WRONG, &act) == LOCKED && act == A_BEEP, "Table: GOT1 + WRONG -> LOCKED (BEEP)");
    run_test(lock_step(GOT1, E_CLOSE, &act) == LOCKED && act == A_NONE, "Table: GOT1 + CLOSE -> LOCKED");
    run_test(lock_step(GOT1, E_TIMEOUT, &act) == LOCKED && act == A_NONE, "Table: GOT1 + TIMEOUT -> LOCKED");

    // GOT2 state
    run_test(lock_step(GOT2, E_RIGHT, &act) == OPEN && act == A_UNLOCK, "Table: GOT2 + RIGHT -> OPEN (UNLOCK)");
    run_test(lock_step(GOT2, E_WRONG, &act) == LOCKED && act == A_BEEP, "Table: GOT2 + WRONG -> LOCKED (BEEP)");
    run_test(lock_step(GOT2, E_CLOSE, &act) == LOCKED && act == A_NONE, "Table: GOT2 + CLOSE -> LOCKED");
    run_test(lock_step(GOT2, E_TIMEOUT, &act) == LOCKED && act == A_NONE, "Table: GOT2 + TIMEOUT -> LOCKED");

    // OPEN state
    run_test(lock_step(OPEN, E_RIGHT, &act) == OPEN && act == A_NONE, "Table: OPEN + RIGHT -> OPEN");
    run_test(lock_step(OPEN, E_WRONG, &act) == OPEN && act == A_NONE, "Table: OPEN + WRONG -> OPEN");
    run_test(lock_step(OPEN, E_CLOSE, &act) == LOCKED && act == A_LOCK, "Table: OPEN + CLOSE -> LOCKED (LOCK)");
    run_test(lock_step(OPEN, E_TIMEOUT, &act) == LOCKED && act == A_LOCK, "Table: OPEN + TIMEOUT -> LOCKED (LOCK)");

    // --- 2. Test lock_event for states, 'c', 't', and other characters ---
    
    // LOCKED state (needs '3')
    run_test(lock_event(LOCKED, '3') == E_RIGHT, "Event: '3' in LOCKED is RIGHT");
    run_test(lock_event(LOCKED, '1') == E_WRONG, "Event: '1' in LOCKED is WRONG");
    
    // GOT1 state (needs '1')
    run_test(lock_event(GOT1, '1') == E_RIGHT, "Event: '1' in GOT1 is RIGHT");
    run_test(lock_event(GOT1, '4') == E_WRONG, "Event: '4' in GOT1 is WRONG");
    
    // GOT2 state (needs '4')
    run_test(lock_event(GOT2, '4') == E_RIGHT, "Event: '4' in GOT2 is RIGHT");
    run_test(lock_event(GOT2, '3') == E_WRONG, "Event: '3' in GOT2 is WRONG");
    
    // OPEN state
    run_test(lock_event(OPEN, '3') == E_WRONG, "Event: '3' in OPEN is WRONG");
    
    // Special characters
    run_test(lock_event(LOCKED, 'c') == E_CLOSE, "Event: 'c' is CLOSE");
    run_test(lock_event(LOCKED, 't') == E_TIMEOUT, "Event: 't' is TIMEOUT");
    run_test(lock_event(LOCKED, 'x') == -1, "Event: 'x' is unrecognized (-1)");

    // --- 3. Test required assignment sequences ---
    run_test(test_sequence("314", OPEN), "Sequence: 3 1 4 (opens)");
    run_test(test_sequence("3314", LOCKED), "Sequence: 3 3 1 4 (does not)");
    run_test(test_sequence("3114", LOCKED), "Sequence: 3 1 1 4 (does not)");
    run_test(test_sequence("314c", LOCKED), "Sequence: 3 1 4 c (locks again)");

    printf("\nTest Summary: %d failures\n", failures);
    
    // Return nonzero exit status on failure
    if (failures > 0) {
        return 1;
    }
    return 0;
}