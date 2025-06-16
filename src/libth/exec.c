#include <exec.h>
#include <syscalls.h>

//const char* environ[] = {NULL};

int execve(const char* path, const char* const argv[], const char* const envp[]) {
  return sys_execveat(AT_FDCWD, path, argv, envp, 0);
}
