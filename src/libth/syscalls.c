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

  return _syscall((u64) &spec, 0, 0, 0, 0, 0, 101);
}

int sys_openat(int dfd, const char* filename, u32 flags, u32 mode) {
  return _syscall((u64) dfd, (u64) filename, (u64) flags, (u64) mode, 0, 0, 56);
}

int sys_close(int fd) {
  return _syscall((u64) fd, 0, 0, 0, 0, 0, 57);
}

isize sys_read(int fd, char* buf, usize len) {
  return _syscall((u64) fd, (u64) buf, len, 0, 0, 0, 63);
}

isize sys_write(int fd, const char* buf, usize len) {
  return _syscall((u64) fd, (u64) buf, len, 0, 0, 0, 64);
}

int sys_reboot(uint cmd) {
  return _syscall(0xfee1dead, 0x28121969, (u64) cmd, 0, 0, 0, 142);
}

void sys_exit(int status) {
  _syscall((i64) status, 0, 0, 0, 0, 0, 93);
}

int sys_kill(int pid, int sig) {
  return _syscall((i64) pid, (i64) sig, 0, 0, 0, 0, 129);
}

int sys_fork() {
  // actually clone syscall, with flags to emulate fork()
  return _syscall(0x11 /* SIGCHLD */, 0, 0, 0, 0, 0, 220);
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
  return _syscall((u64) info, 0, 0, 0, 0, 0, 179);
}

int sys_waitid(uint id_type, i32 id, siginfo_t* info, uint options) {
  return _syscall((u64) id_type, (u64) id, (u64) info, (u64) options, NULL /* rusage_t */, 0, 95);
}
