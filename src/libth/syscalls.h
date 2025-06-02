#pragma once

#include <types.h>

int _syscall(void* arg0, void* arg1, void* arg2, void* arg3, void* arg4, void* arg5, int syscall_number);

#define REBOOT_CMD_HALT     (0xcdef0123)
#define REBOOT_CMD_RESTART  (0x01234567)
#define REBOOT_CMD_POWEROFF (0x4321fedc)

void sys_exit(int32 status);
int sys_nanosleep(uint64 sec, uint64 nsec);
isize sys_read(int fd, char* buf, usize len);
isize sys_write(int fd, const char* buf, usize len);
int sys_reboot(uint cmd);
