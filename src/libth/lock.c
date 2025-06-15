#include <lock.h>

// TODO: use futexes - for now, these are simple spinlocks

void lock(bool* lock) {
  while (1) {
    if (!__atomic_test_and_set(lock, __ATOMIC_ACQUIRE))
      return;
    while (__atomic_load_n(lock, __ATOMIC_RELAXED))
      asm("yield");
  }
}

bool try_lock(bool* lock) {
  return __atomic_load_n(lock, __ATOMIC_RELAXED) &&
         __atomic_test_and_set(lock, __ATOMIC_ACQUIRE);
}

void unlock(bool* lock) {
  __atomic_store_n(lock, false, __ATOMIC_RELEASE);
}
