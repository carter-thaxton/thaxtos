
#include <syscalls.h>

void sys_exit(int status) {
  _syscall((void*) (i64) status, 0, 0, 0, 0, 0, 93);
}

int sys_nanosleep(u64 sec, u64 nsec) {
  struct kernel_timespec {
    u64 sec;
    u64 nsec;
  };

  struct kernel_timespec spec = {
    .sec = sec,
    .nsec = nsec,
  };

  return _syscall(&spec, 0, 0, 0, 0, 0, 101);
}

isize sys_read(int fd, char* buf, usize len) {
  return _syscall((void*) (u64) fd, (void*) buf, (void*) len, 0, 0, 0, 63);
}

isize sys_write(int fd, const char* buf, usize len) {
  return _syscall((void*) (u64) fd, (void*) buf, (void*) len, 0, 0, 0, 64);
}

int sys_reboot(uint cmd) {
  return _syscall((void*) 0xfee1dead, (void*) 0x28121969, (void*) (u64) cmd, 0, 0, 0, 142);
}

int sys_fork() {
  return _syscall((void*) 0x11 /* SIGCHLD */, 0, 0, 0, 0, 0, 220);
}

int sys_getpid() {
  return _syscall(0, 0, 0, 0, 0, 0, 172);
}

int sys_getppid() {
  return _syscall(0, 0, 0, 0, 0, 0, 173);
}

int sys_getuid() {
  return _syscall(0, 0, 0, 0, 0, 0, 174);
}

int sys_geteuid() {
  return _syscall(0, 0, 0, 0, 0, 0, 175);
}

int sys_getgid() {
  return _syscall(0, 0, 0, 0, 0, 0, 176);
}

int sys_getegid() {
  return _syscall(0, 0, 0, 0, 0, 0, 177);
}

int sys_gettid() {
  return _syscall(0, 0, 0, 0, 0, 0, 178);
}

int sys_sysinfo(sysinfo_t* info) {
  return _syscall(info, 0, 0, 0, 0, 0, 179);
}

int sys_waitid(int which, int pid, siginfo_t* info, int options, void* rusage) {
  return _syscall((void*) (u64) which, (void*) (u64) pid, info, (void*) (u64) options, rusage, 0, 95);
}
