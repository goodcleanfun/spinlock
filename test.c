#include <stdio.h>
#include <stdlib.h>

#include "greatest/greatest.h"
#include "threading/threading.h"

#include "spinlock.h"

TEST spinlock_test(void) {
    atomic_int a;
    atomic_init(&a, 10);

    atomic_store(&a, 20);
    ASSERT_EQ(20, atomic_load(&a));

    atomic_store(&a, -5);
    ASSERT_EQ(-5, atomic_load(&a));

    PASS();
}

static int counter = 0;

int increment_counter(void *arg) {
    spinlock_t *lock = (spinlock_t *)arg;
    for (int i = 0; i < 1000; i++) {
        spinlock_lock(lock);
        counter++;
        spinlock_unlock(lock);
    }
    return 0;
}

TEST spinlock_multithread_test(void) {
    thrd_t threads[10];

    spinlock_t lock = SPINLOCK_INIT;

    counter = 0;
    for (int i = 0; i < 10; i++) {
        ASSERT_EQ(thrd_success, thrd_create(&threads[i], increment_counter, &lock));
    }
    for (int i = 0; i < 10; i++) {
        thrd_join(threads[i], NULL);
    }

    spinlock_lock(&lock);
    ASSERT_FALSE(spinlock_trylock(&lock));
    spinlock_unlock(&lock);

    ASSERT_EQ(10000, counter); // 10 threads x 1000 increments each

    PASS();
}


// Main test suite
SUITE(spinlock_tests) {
    RUN_TEST(spinlock_test);
    RUN_TEST(spinlock_multithread_test);
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv) {
    GREATEST_MAIN_BEGIN();

    RUN_SUITE(spinlock_tests);

    GREATEST_MAIN_END();
}