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
    while (atomic_exchange(lock, (uint8_t) true)) {
        while (atomic_load(lock)) {
            cpu_relax();
        }
    }
}

static inline bool spinlock_trylock(spinlock_t *lock) {
    return !atomic_load(lock) && !atomic_exchange(lock, (uint8_t) true);
}

static inline void spinlock_unlock(spinlock_t *lock) {
    atomic_store(lock, (uint8_t)false);
}

#endif /* SPINLOCK_H */
