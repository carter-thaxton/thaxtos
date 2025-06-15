#include <lock.h>

// TODO: use futexes - for now, these are simple spinlocks

#ifndef __ATOMIC_HLE_ACQUIRE  // HLE (hardware lock-elision) is only defined on x86
#define __ATOMIC_HLE_ACQUIRE 0
#define __ATOMIC_HLE_RELEASE 0
#endif

// TODO: define this per arch
#define __yield() asm("yield")

void lock(bool* lock) {
  // TTAS spinlock
  while (1) {
    if (!__atomic_test_and_set(lock, __ATOMIC_ACQUIRE|__ATOMIC_HLE_ACQUIRE))
      return;
    while (__atomic_load_n(lock, __ATOMIC_RELAXED))
      __yield();
  }
}

bool try_lock(bool* lock) {
  return __atomic_load_n(lock, __ATOMIC_RELAXED) &&
         __atomic_test_and_set(lock, __ATOMIC_ACQUIRE|__ATOMIC_HLE_ACQUIRE);
}

void unlock(bool* lock) {
  __atomic_clear(lock, __ATOMIC_RELEASE|__ATOMIC_HLE_RELEASE);
}
