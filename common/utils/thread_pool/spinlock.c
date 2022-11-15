#include "spinlock.h"

#include <assert.h>
#include <stdlib.h>

void lock_spinlock(spinlock_t* s)
{
  assert(s != NULL);

  s->lock = true;

    for (;;) {
      // Optimistically assume the lock is free on the first try
      if(!atomic_exchange_explicit(&s->lock, true, memory_order_acquire) ){
        return;
      }

      // Wait for lock to be released without generating cache misses
      while (atomic_load_explicit(&s->lock, memory_order_relaxed)){
        // Issue X86 PAUSE or ARM YIELD instruction to reduce contention between
        // hyper-threads
        __builtin_ia32_pause();
      }
    }

}

bool try_lock_spinlock(spinlock_t* s)
{
  assert(s != NULL);
    // First do a relaxed load to check if lock is free in order to prevent
    // unnecessary cache misses if someone does while(!try_lock())
    return !atomic_load_explicit(&s->lock, memory_order_relaxed) &&
           !atomic_exchange_explicit(&s->lock, true, memory_order_acquire);
}

void unlock_spinlock(spinlock_t* s)
{
  assert(s != NULL);
  atomic_store_explicit(&s->lock, false, memory_order_release);
  assert(s->lock == false);
}

