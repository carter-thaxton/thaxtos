#pragma once

#include <types.h>

int _syscall(void* arg0, void* arg1, void* arg2, void* arg3, void* arg4, void* arg5, int syscall_number);

void sys_exit(int32 status);
int sys_write(int fd, const char* buf, uint64 len);
int sys_nanosleep(uint64 sec, uint64 nsec);
