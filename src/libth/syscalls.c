
#include <syscalls.h>

void sys_exit(int32 status) {
  _syscall((void*) (int64) status, 0, 0, 0, 0, 0, 93);
}

int sys_write(int fd, const char* buf, uint64 len) {
  return _syscall((void*) (uint64) fd, (void*) buf, (void*) len, 0, 0, 0, 64);
}

int sys_nanosleep(uint64 sec, uint64 nsec) {
  struct kernel_timespec {
    uint64 sec;
    uint64 nsec;
  };

  struct kernel_timespec spec = {
    .sec = sec,
    .nsec = nsec,
  };

  return _syscall(&spec, 0, 0, 0, 0, 0, 101);
}
