#ifndef TALLY_H
#define TALLY_H

#include <stdio.h>

#define TALLY_MAX 4
#define TALLY_NAMES 16
#define TALLY_LEN 15

typedef struct tally tally_t;

/*  
    Takes:      Nothing
    Returns:    Pointer to new tally or NULL if MAX
    Assumes:    
*/ 
tally_t *tally_create(void);

/*
    Takes:      *t
    Returns:    Nothing
    Assumes:    Marks t free so tally_create can reuse, NULL is ignored
*/
void tally_destroy(tally_t *t);

/*
    Takes:      *t and string *name
    Returns:    New count of name on success, -1 on fail
    Assumes:    Fails if t or if name is NULL, empty, too long, or tally is full
*/
int tally_add(tally_t *t, const char *name);

/*
    Takes:      const *t
    Returns:    How often name was added, 0 if never, or for NULL
    Assumes:    t is not modified
*/
int tally_count(const tally_t *t, const char *name);

/*
    Takes:      *t
    Returns:    Nothing
    Assumes:    t is not modified
*/
int tally_size(const tally_t *t);

/*
    Takes:      *t and FILE 
    Returns:    Nothing
    Assumes:    prints one line per entry in alpha order. Nothing if t or out is NULL
*/
void tally_print(const tally_t *t, FILE *out);

#endif