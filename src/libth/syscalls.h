#pragma once

#include <types.h>

i64 _syscall(u64 arg0, u64 arg1, u64 arg2, u64 arg3, u64 arg4, u64 arg5, int syscall_number);

int sys_nanosleep(u64 sec, u64 nsec);

#define AT_FDCWD            (-100)
#define AT_EMPTY_PATH       0x1000
#define AT_SYMLINK_NOFOLLOW 0x100
#define O_ACCMODE   00000003
#define O_RDONLY    00000000
#define O_WRONLY    00000001
#define O_RDWR      00000002
#define O_CREAT     00000100
#define O_EXCL      00000200
#define O_NOCTTY    00000400
#define O_TRUNC     00001000
#define O_APPEND    00002000
#define O_NONBLOCK  00004000
#define O_DSYNC     00010000
#define O_DIRECT    00040000
#define O_LARGEFILE 00100000
#define O_DIRECTORY 00200000
#define O_NOFOLLOW  00400000
#define O_NOATIME   01000000
#define O_CLOEXEC   02000000
#define O_SYNC      04000000
#define O_PATH     010000000
#define O_TMPFILE  020000000
int sys_execveat(int dfd, const char* filename, const char* const argv[], const char* const envp[], int flags);
int sys_openat(int dfd, const char* filename, u32 flags, u32 mode);
int sys_close(int fd);
isize sys_read(int fd, char* buf, usize len);
isize sys_write(int fd, const char* buf, usize len);
isize sys_pwrite64(int fd, const char* buf, usize len, isize off);
int sys_ioctl(int fd, u64 op, void* arg);
int sys_chdir(const char* dirname);

#define DT_UNKNOWN  0
#define DT_FIFO     1
#define DT_CHR      2
#define DT_DIR      4
#define DT_BLK      6
#define DT_REG      8
#define DT_LNK      10
#define DT_SOCK     12
#define DT_WHT      14
typedef struct linux_dirent64 {
  i64  ino;    // 64-bit inode number
  i64  off;    // 64-bit offset to next structure
  u16  reclen; // Size of this dirent
  u8   type;   // File type
  char name[]; // Filename (null-terminated)
} dirent_t;
isize sys_getdents(int dfd, dirent_t* dirents, uint size);

#define MOUNT_RDONLY        (1<<0)  // Mount read-only
#define MOUNT_NOSUID        (1<<1)  // Ignore suid and sgid bits
#define MOUNT_NODEV         (1<<2)  // Disallow access to device special files
#define MOUNT_NOEXEC        (1<<3)  // Disallow program execution
#define MOUNT_SYNCHRONOUS   (1<<4)  // Writes are synced at once
#define MOUNT_REMOUNT       (1<<5)  // Alter flags of a mounted FS
#define MOUNT_MANDLOCK      (1<<6)  // Allow mandatory locks on an FS
#define MOUNT_DIRSYNC       (1<<7)  // Directory modifications are synchronous
                        //  (1<<8)  -- deliberately missing
#define MOUNT_NOSYMFOLLOW   (1<<9)  // Do not follow symlinks
#define MOUNT_NOATIME       (1<<10) // Do not update access times
#define MOUNT_NODIRATIME    (1<<11) // Do not update directory access times
#define MOUNT_BIND          (1<<12)
#define MOUNT_MOVE          (1<<13)
#define MOUNT_REC           (1<<14)
#define MOUNT_SILENT        (1<<15)
#define MOUNT_POSIXACL      (1<<16) // VFS does not apply the umask
#define MOUNT_UNBINDABLE    (1<<17) // change to unbindable
#define MOUNT_PRIVATE       (1<<18) // change to private
#define MOUNT_SLAVE         (1<<19) // change to slave
#define MOUNT_SHARED        (1<<20) // change to shared
#define MOUNT_RELATIME      (1<<21) // Update atime relative to mtime/ctime
#define MOUNT_KERNMOUNT     (1<<22) // this is a kern_mount call
#define MOUNT_I_VERSION     (1<<23) // Update inode I_version field
#define MOUNT_STRICTATIME   (1<<24) // Always perform atime updates
#define MOUNT_LAZYTIME      (1<<25) // Update the on-disk [acm]times lazily
int sys_mount(const char* dev_name, const char* dir_name, const char* type, u64 flags, void* data);

#define UMOUNT_FORCE    0x00000001  // Attempt to forcibily umount
#define UMOUNT_DETACH   0x00000002  // Just detach from the tree
#define UMOUNT_EXPIRE   0x00000004  // Mark for expiry
#define UMOUNT_NOFOLLOW 0x00000008  // Don't follow symlink on umount
#define UMOUNT_UNUSED   0x80000000  // Flag guaranteed to be unused
int sys_umount(const char* name, u64 flags);

#define REBOOT_CMD_HALT     0xcdef0123
#define REBOOT_CMD_RESTART  0x01234567
#define REBOOT_CMD_POWEROFF 0x4321fedc
int sys_reboot(uint cmd);

uintptr sys_brk(uintptr newbrk);

#define PROT_READ           0x1             // page can be read
#define PROT_WRITE          0x2             // page can be written
#define PROT_EXEC           0x4             // page can be executed
#define PROT_SEM            0x8             // page may be used for atomic ops
#define PROT_NONE           0x0             // page can not be accessed
#define PROT_GROWSDOWN      0x01000000      // mprotect flag: extend change to start of growsdown vma
#define PROT_GROWSUP        0x02000000      // mprotect flag: extend change to end of growsup vma
#define MAP_SHARED          0x01            // Share changes
#define MAP_PRIVATE         0x02            // Changes are private
#define MAP_SHARED_VALIDATE 0x03            // share + validate extension flags
#define MAP_TYPE            0x0f            // Mask for type of mapping
#define MAP_FIXED           0x10            // Interpret addr exactly
#define MAP_ANONYMOUS       0x20            // don't use a file
#define MAP_FAILED          ((void *) -1)
void *sys_mmap(void* addr, usize len, int prot, int flags, int fd, isize off);

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
