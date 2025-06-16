#pragma once

#include <types.h>

int _syscall(u64 arg0, u64 arg1, u64 arg2, u64 arg3, u64 arg4, u64 arg5, int syscall_number);

int sys_nanosleep(u64 sec, u64 nsec);

#define AT_FDCWD            (-100)
#define AT_EMPTY_PATH       0x1000
#define AT_SYMLINK_NOFOLLOW 0x100
int sys_execveat(int dirfd, const char* pathname, const char* const argv[], const char* const envp[], int flags);
int sys_openat(int dfd, const char* filename, u32 flags, u32 mode);
int sys_close(int fd);
isize sys_read(int fd, char* buf, usize len);
isize sys_write(int fd, const char* buf, usize len);

#define REBOOT_CMD_HALT     (0xcdef0123)
#define REBOOT_CMD_RESTART  (0x01234567)
#define REBOOT_CMD_POWEROFF (0x4321fedc)
int sys_reboot(uint cmd);

uintptr sys_brk(uintptr newbrk);

void sys_exit(int status);
int sys_kill(int pid, int sig);
int sys_fork();  // internally uses clone syscall

int sys_getpid();
int sys_getppid();
int sys_getuid();
int sys_geteuid();
int sys_getgid();
int sys_getegid();
int sys_gettid();

typedef struct sysinfo {
  i64 uptime;     /* Seconds since boot */
  u64 loads[3];   /* 1, 5, and 15 minute load averages */
  u64 totalram;   /* Total usable main memory size */
  u64 freeram;    /* Available memory size */
  u64 sharedram;  /* Amount of shared memory */
  u64 bufferram;  /* Memory used by buffers */
  u64 totalswap;  /* Total swap space size */
  u64 freeswap;   /* Swap space still available */
  u16 procs;      /* Number of current processes */
  u64 totalhigh;  /* Total high memory size */
  u64 freehigh;   /* Available high memory size */
  u32 mem_unit;   /* Memory unit size in bytes */
} sysinfo_t;
int sys_sysinfo(sysinfo_t* info);

// id_type
#define P_ALL     0
#define P_PID     1
#define P_PGID    2
#define P_PIDFD   3
// options
#define WNOHANG       1
#define WSTOPPED      2
#define WEXITED       4
#define WCONTINUED    8
#define WNOWAIT       0x01000000
typedef struct siginfo {
  i32 signo;
  i32 errno;
  i32 code;
  i32 __pad;
  i32 pid;
  i32 uid;
  i32 status;
} siginfo_t;
int sys_waitid(uint id_type, i32 id, siginfo_t* info, uint options);
