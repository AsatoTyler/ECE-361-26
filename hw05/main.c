#include <stdio.h>
#include "lock.h"

int main(void) {
    state_t current_state = LOCKED;
    char c;

    // Reads standard input character by character
    while ((c = getchar()) != EOF) {
        int e = lock_event(current_state, c);
        
        // Skip characters that return -1 (out of range/unrecognized)
        if (e == -1) {
            continue;
        }
        
        action_t action;
        state_t next_state = lock_step(current_state, (event_t)e, &action);
        
        // Prints one line per event: state, character, event, next state, action
        printf("%s %c %s -> %s %s\n", 
               state_name(current_state), 
               c, 
               event_name((event_t)e), 
               state_name(next_state), 
               action_name(action));
               
        current_state = next_state;
    }

    printf("final: %s\n", state_name(current_state));
    return 0;
}
