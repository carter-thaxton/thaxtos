#include <exec.h>
#include <syscalls.h>

//const char* environ[] = {NULL};

int execve(const char* path, const char* const argv[], const char* const envp[]) {
  return sys_execveat(AT_FDCWD, path, argv, envp, 0);
}

int run_command(const char* const argv[], const char* const envp[]) {
  int pid = sys_fork();

  if (pid < 0) {
    // fork error
    return -1;
  } else if (pid == 0) {
    // child
    int err = execve(argv[0], argv, envp);
    sys_exit(-err);
    return -1;  // should not get here
  } else {
    // parent
    siginfo_t info;
    int err = sys_waitid(P_PID, pid, &info, WEXITED);
    if (err == 0) {
      // return child exit status
      return info.status;
    } else {
      // error waiting for exit
      return -2;
    }
  }
}
