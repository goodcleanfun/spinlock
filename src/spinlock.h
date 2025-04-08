#ifndef SPINLOCK_H
#define SPINLOCK_H

#include <stdatomic.h>

typedef atomic_flag spinlock_t;
#define spinlock_init(name) spinlock_t name = ATOMIC_FLAG_INIT

static inline void spinlock_lock(spinlock_t *lock) {
    do {} while (atomic_flag_test_and_set(lock));
}

static inline void spinlock_unlock(spinlock_t *lock) {
    atomic_flag_clear(lock);
}

#endif /* SPINLOCK_H */
