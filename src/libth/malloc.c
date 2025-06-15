#include <malloc.h>
#include <syscalls.h>
#include <stdio.h>
#include <lock.h>

#define DEBUG_BRK
#define ALIGN 16

#define ROUND_UP(val, align) ((val + align-1) & -align)

static uintptr brk = 0;
static uintptr cur = 0;
static bool brk_lock = false;

void* malloc(usize n) {
  if (!n) n++;  // always allocate at least 1 byte
  if (n > SIZE_MAX/2) goto toobig;

  usize align = 1;
  while (align < n && align < ALIGN)
    align += align;

  n = ROUND_UP(n, align);

  lock(&brk_lock);
  if (cur == 0) {
#ifdef DEBUG_BRK
    eprintf("BRK(%x)\n", 0);
#endif
    cur = brk = sys_brk(0) + 16;  // get initial brk, leaving space behind pointer
  }

  uintptr base = ROUND_UP(cur, align);
  if (n > SIZE_MAX - PAGE_SIZE - base) goto fail;

  if (base+n > brk) {
    uintptr new = ROUND_UP(base+n, PAGE_SIZE);
#ifdef DEBUG_BRK
    eprintf("BRK(%x)\n", new);
#endif
    if (sys_brk(new) != new) goto fail;
    brk = new;
  }

  cur = base+n;

  unlock(&brk_lock);
  return (void*) base;

fail:
  unlock(&brk_lock);
toobig:
  return 0;
}

void free(void* mem) {
  // TODO: handle this
}
