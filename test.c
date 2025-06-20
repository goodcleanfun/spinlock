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

typedef struct {
    int counter;
    spinlock_t lock;
} spinlock_test_t;

int increment_counter(void *arg) {
    spinlock_test_t *test = (spinlock_test_t *)arg;
    for (int i = 0; i < 100000; i++) {
        spinlock_lock(&test->lock);
        test->counter++;
        spinlock_unlock(&test->lock);
    }
    return 0;
}

TEST spinlock_multithread_test(void) {
    thrd_t threads[16];

    spinlock_test_t *test = (spinlock_test_t *)malloc(sizeof(spinlock_test_t));

    test->counter = 0;
    spinlock_t lock;
    spinlock_init(&lock);
    test->lock = lock;

    for (int i = 0; i < 16; i++) {
        ASSERT_EQ(thrd_success, thrd_create(&threads[i], increment_counter, test));
    }
    for (int i = 0; i < 16; i++) {
        thrd_join(threads[i], NULL);
    }

    spinlock_lock(&test->lock);
    ASSERT_FALSE(spinlock_trylock(&test->lock));
    spinlock_unlock(&test->lock);

    printf("counter: %d\n", test->counter);

    ASSERT_EQ(1600000, test->counter); // 10 threads x 1000 increments each

    free(test);

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