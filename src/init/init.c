
#include <syscalls.h>
#include <string.h>
#include <stdio.h>
#include <malloc.h>
#include <exec.h>

void clear_screen() {
  printf("%c[2J%c[H", 27, 27);
}


int run_cmd(const char* cmd) {
  int pid = sys_fork();
  if (pid == 0) {
    // child
    const char* args[] = {cmd, NULL};
    const char* envp[] = {NULL};
    int err = execve(cmd, args, envp);
    // should not get here unless error
    sys_exit(-err);
    return -1;
  } else {
    // parent
    printf("[%s started (PID %d)]\n", cmd, pid);
    siginfo_t info;
    int result = sys_waitid(P_PID, pid, &info, WEXITED);
    if (result == 0) {
      printf("[%s returned %d (PID %d)]\n", cmd, info.status, info.pid);
    } else {
      printf("ERROR waitid returned: %d\n", result);
    }
    return result;
  }
}

void mount_procfs() {
  int err = sys_mount("proc", "/proc", "proc", 0, 0);
  if (err < 0) {
    printf("ERROR mounting /proc: %d\n", -err);
  }
}

int main() {
  clear_screen();
  printf("Welcome to ThaxtOS!\n");
  mount_procfs();

  while (true) {
    run_cmd("/bin/tsh");
  }

  return -1;
}
