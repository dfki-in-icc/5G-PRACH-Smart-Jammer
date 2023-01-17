#ifndef SPINLOCK_MIR_H
#define SPINLOCK_MIR_H

#include <stdatomic.h>
#include <stdbool.h>

typedef struct{
  _Atomic bool lock;  
} spinlock_t;

void lock_spinlock(spinlock_t* s);

bool try_lock_spinlock(spinlock_t* s);

void unlock_spinlock(spinlock_t* s);

#endif

