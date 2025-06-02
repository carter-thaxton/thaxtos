#pragma once

#include <types.h>

int _syscall(void* arg0, void* arg1, void* arg2, void* arg3, void* arg4, void* arg5, int syscall_number);

void sys_exit(int status);
int sys_nanosleep(uint64 sec, uint64 nsec);

isize sys_read(int fd, char* buf, usize len);
isize sys_write(int fd, const char* buf, usize len);

#define REBOOT_CMD_HALT     (0xcdef0123)
#define REBOOT_CMD_RESTART  (0x01234567)
#define REBOOT_CMD_POWEROFF (0x4321fedc)
int sys_reboot(uint cmd);

int sys_getpid();
int sys_getppid();
int sys_getuid();
int sys_geteuid();
int sys_getgid();
int sys_getegid();
int sys_gettid();

int sys_fork();  // internally uses clone syscall

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

// which
#define P_ALL   0
#define P_PID   1
#define P_PGID    2
#define P_PIDFD   3
// options
#define WSTOPPED      2
#define WEXITED       4
#define WCONTINUED    8
#define WNOWAIT       0x01000000
typedef struct siginfo {
  i32 signo;
  i32 code;
  i32 errno;
  i32 __pad;
  i32 pid;
  i32 uid;
  i32 status;
} siginfo_t;
int sys_waitid(int which, int pid, siginfo_t* info, int options, void* rusage);
