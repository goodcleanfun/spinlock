#ifndef SPINLOCK_H
#define SPINLOCK_H

#include <stdatomic.h>
#include <stdbool.h>
typedef atomic_flag spinlock_t;
#define SPINLOCK_INIT ATOMIC_FLAG_INIT

static inline void spinlock_lock(spinlock_t *lock) {
    do {} while (atomic_flag_test_and_set(lock));
}

static inline bool spinlock_trylock(spinlock_t *lock) {
    return !atomic_flag_test_and_set(lock);
}

static inline void spinlock_unlock(spinlock_t *lock) {
    atomic_flag_clear(lock);
}

#endif /* SPINLOCK_H */
