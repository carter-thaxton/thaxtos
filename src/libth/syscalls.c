#include <syscalls.h>

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

void sys_exit(int status) {
  _syscall((void*) (i64) status, 0, 0, 0, 0, 0, 93);
}

int sys_kill(int pid, int sig) {
  return _syscall((void*) (i64) pid, (void*) (i64) sig, 0, 0, 0, 0, 129);
}

int sys_fork() {
  // actually clone syscall, with flags to emulate fork()
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

int sys_waitid(uint id_type, i32 id, siginfo_t* info, uint options) {
  return _syscall((void*) (u64) id_type, (void*) (u64) id, info, (void*) (u64) options, NULL /* rusage_t */, 0, 95);
}
