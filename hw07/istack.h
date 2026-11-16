#ifndef ISTACK_H
#define ISTACK_H

#include <stddef.h>
#include <stdbool.h>

#define ISTACK_START 8

// Opaque type integer stack
typedef struct istack istack_t;

/* 
istack_create
Takes: void
Returns: A pointer to a new, empty stack with room for ISTACK_START values. 
         Returns NULL if memory runs out (with no memory leaked)
*/
istack_t *istack_create(void);

/* 
istack_destroy
Takes: A pointer to the stack 's'.
Returns: void
Assumes: Frees the stack. If 's' is NULL, it does nothing.
*/
void istack_destroy(istack_t *s);

/* 
istack_push
Takes: A pointer to the stack 's', and an int 'v'.
Returns: 0 on success. Returns -1 if 's' is NULL or if memory runs out.
Assumes: If the stack is full, it first doubles the room with realloc. 
         If memory runs out during reallocation, the stack is left unchanged.
*/
int istack_push(istack_t *s, int v);

/* 
istack_pop
Takes: A pointer to the stack 's', and a pointer to an int 'v'.
Returns: 0 on success. Returns -1 if 's' is NULL or if the stack is empty.
Assumes: Pops the top value into '*v' and removes it from the stack. 
         'v' may be NULL, in which case the value is removed but not stored. 
         If the function fails, '*v' is left unchanged.
*/
int istack_pop(istack_t *s, int *v);

/* 
istack_peek
Takes: A const pointer to the stack 's', and a pointer to an int 'v'.
Returns: 0 on success. Returns -1 if 's' is NULL or if the stack is empty.
Assumes: Copies the top value into '*v' without removing it from the stack. 
         'v' may be NULL. If the function fails, '*v' is left unchanged.
*/
int istack_peek(const istack_t *s, int *v);

/* 
istack_count
Takes: A const pointer to the stack 's'.
Returns: The number of values currently held in the stack. Returns 0 if 's' is NULL.
Assumes: 's' is valid or NULL.
*/
size_t istack_count(const istack_t *s);

/* 
istack_is_empty
Takes: A const pointer to the stack 's'.
Returns: true if the stack contains no values, false otherwise. Returns true if 's' is NULL.
Assumes: 's' is valid or NULL.
*/
bool istack_is_empty(const istack_t *s);

/* 
istack_capacity
Takes: A const pointer to the stack 's'.
Returns: The current maximum capacity (room) of the stack. Returns 0 if 's' is NULL.
Assumes: 's' is valid or NULL.
*/
size_t istack_capacity(const istack_t *s);

/* 
istack_to_array
Takes: A const pointer to the stack 's', and a pointer to a size_t 'n'.
Returns: A pointer to a new dynamically allocated array containing the stack's values, bottom first. 
         Returns NULL if 's' is NULL, if the stack is empty, or if memory runs out.
Assumes: The caller owns the returned array and is responsible for freeing it. 
         If it returns NULL, '*n' is set to 0. Otherwise, '*n' is set to the number of elements.
*/
int *istack_to_array(const istack_t *s, size_t *n);

#endif
