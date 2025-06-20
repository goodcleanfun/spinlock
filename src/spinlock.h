#ifndef SPINLOCK_H
#define SPINLOCK_H

#include <stdint.h>
#include <stdatomic.h>
#include <stdbool.h>
#include "cpu_relax/cpu_relax.h"

typedef atomic_uint_fast8_t spinlock_t;

static inline void spinlock_init(spinlock_t *lock) {
    atomic_init(lock, (uint8_t)false);
}


static inline void spinlock_lock(spinlock_t *lock) {
    while (true) {
        if (!atomic_exchange_explicit(lock, (uint8_t) true, memory_order_acquire)) {
            break;
        }
        while (atomic_load_explicit(lock, memory_order_relaxed)) {
            cpu_relax();
        }
    }
}

static inline bool spinlock_trylock(spinlock_t *lock) {
    return !atomic_load_explicit(lock, memory_order_relaxed) && !atomic_exchange_explicit(lock, (uint8_t) true, memory_order_acquire);
}

static inline void spinlock_unlock(spinlock_t *lock) {
    atomic_store_explicit(lock, (uint8_t)false, memory_order_release);
}

#endif /* SPINLOCK_H */
