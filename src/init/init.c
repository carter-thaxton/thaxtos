
#include <syscalls.h>
#include <string.h>
#include <stdio.h>
#include <malloc.h>
#include <exec.h>
#include <fb.h>

int sleep_us(int usec) {
  return sys_nanosleep(usec / 1000000, (usec % 1000000) * 1000);
}

int sleep_ms(int msec) {
  return sys_nanosleep(msec / 1000, (msec % 1000) * 1000000);
}

int sleep_s(int sec) {
  return sys_nanosleep(sec, 0);
}

void clear_screen() {
  printf("%c[2J%c[H", 27, 27);
}

void handle_command(const char* cmd) {
  if (strcmp(cmd, "none") == 0) {
    // ignore
  } else if (strcmp(cmd, "reboot") == 0) {
    sys_reboot(REBOOT_CMD_RESTART);
  } else if (strcmp(cmd, "halt") == 0) {
    sys_reboot(REBOOT_CMD_HALT);
  } else if (strcmp(cmd, "poweroff") == 0 || strcmp(cmd, "q") == 0) {
    sys_reboot(REBOOT_CMD_POWEROFF);
  } else if (strcmp(cmd, "exit") == 0) {
    sys_exit(69);
  } else if (strcmp(cmd, "kill") == 0) {
    int pid = sys_getpid();
    int sig = 9;
    printf("Kill self\n");
    sys_kill(pid, sig);
    printf("Should not get here\n");
  } else if (strcmp(cmd, "fork") == 0) {
    int pid = sys_fork();
    if (pid == 0) {
      // child
      // continue on...
    } else {
      // parent
      // wait for child to exit
      printf("Waiting for child: %d\n", pid);
      siginfo_t info;
      int result = sys_waitid(P_PID, pid, &info, WEXITED);
      printf("waitid returned: %d\n", result);
      if (result == 0) {
        printf("  pid: %d\n", info.pid);
        printf("  uid: %d\n", info.uid);
        printf("  code: %d\n", info.code);
        printf("  status: %d\n", info.status);
        printf("  signo: %d\n", info.signo);
        printf("  errno: %d\n", info.errno);
      }
      printf("\n");
    }
  } else if (strcmp(cmd, "test") == 0) {
    int pid = sys_fork();
    if (pid == 0) {
      // child
      const char* args[] = {"/bin/test", "one", "two", NULL};
      const char* envp[] = {"TEST=69", "TEST2=555", NULL};
      int err = execve("/bin/test", args, envp);
      // should not get here unless error
      sys_exit(-err);
    } else {
      // parent
      printf("[Waiting for child: %d]\n", pid);
      siginfo_t info;
      int result = sys_waitid(P_PID, pid, &info, WEXITED);
      if (result == 0) {
        printf("[PID %d returned %d]\n", info.pid, info.status);
      } else {
        printf("waitid returned: %d\n", result);
      }
    }
  } else if (strcmp(cmd, "pid") == 0) {
    int pid = sys_getpid();
    printf("%d\n", pid);
  } else if (strcmp(cmd, "ppid") == 0) {
    int ppid = sys_getppid();
    printf("%d\n", ppid);
  } else if (strcmp(cmd, "uid") == 0) {
    int uid = sys_getuid();
    printf("%d\n", uid);
  } else if (strcmp(cmd, "euid") == 0) {
    int euid = sys_geteuid();
    printf("%d\n", euid);
  } else if (strcmp(cmd, "gid") == 0) {
    int gid = sys_getgid();
    printf("%d\n", gid);
  } else if (strcmp(cmd, "egid") == 0) {
    int egid = sys_getegid();
    printf("%d\n", egid);
  } else if (strcmp(cmd, "sysinfo") == 0) {
    sysinfo_t info;
    sys_sysinfo(&info);
    printf("System info\n");
    printf("  Uptime: %d\n", info.uptime);
    printf("  Load (1 min): %d\n", info.loads[0]);
    printf("  Load (5 min): %d\n", info.loads[1]);
    printf("  Load (15 min): %d\n", info.loads[2]);
    printf("  Total RAM: %d\n", info.totalram);
    printf("  Free RAM: %d\n", info.freeram);
    printf("  Shared RAM: %d\n", info.sharedram);
    printf("  Buffer RAM: %d\n", info.bufferram);
    printf("  Total swap: %d\n", info.totalswap);
    printf("  Free swap: %d\n", info.freeswap);
    printf("  Processes: %d\n", info.procs);
    printf("  Total high memory: %d\n", info.totalhigh);
    printf("  Free high memory: %d\n", info.freehigh);
    printf("  Memory unit size: %d\n", info.mem_unit);
  } else if (strcmp(cmd, "cd") == 0) {
    // TODO: parse command-line into args
    const char* path = "/proc";
    int err = sys_chdir(path);
    if (err < 0) {
      printf("Error changing directory: %d\n", -err);
    }
  } else if (strcmp(cmd, "ls") == 0) {
    int dfd = sys_openat(AT_FDCWD, ".", 0, O_RDONLY | O_DIRECTORY);
    if (dfd < 0) {
      printf("Error reading directory: %d\n", -dfd);
      return;
    }

    bool printed_header = false;
    char buf[1024];

    while (1) {
      isize sz = sys_getdents(dfd, (dirent_t*)buf, sizeof(buf));
      if (sz < 0) {
        printf("Error reading directory entries: %d\n", -sz);
        sys_close(dfd);
        return;
      } else if (sz == 0) {
        break;
      }

      if (!printed_header) {
        printf("inode\ttype\treclen\tname\n");
        printed_header = true;
      }

      for (usize pos = 0; pos < sz;) {
        dirent_t* d = (dirent_t*) (buf + pos);
        printf("%d\t%d\t%d\t%s\n", d->ino, d->type, d->reclen, d->name);
        pos += d->reclen;
      }
    }

    sys_close(dfd);
  } else if (strcmp(cmd, "cls") == 0) {
    clear_screen();
  } else if (strcmp(cmd, "fb") == 0) {
    int fb = sys_openat(0, "/dev/fb0", O_RDWR, 0);
    if (fb < 0) {
      printf("Error opening /dev/fb0: %d\n", -fb);
      return;
    }

    fb_var_screeninfo info;
    int err = sys_ioctl(fb, FBIOGET_VSCREENINFO, &info);
    if (err < 0) {
      printf("Error getting fb_var_screeninfo from /dev/fb0: %d\n", -err);
      return;
    }

    printf("Current resolution: %d x %d\n", info.xres, info.yres);
    usize len = info.xres * info.yres * sizeof(u32);

    static bool use_mmap = FALSE;
    int delay_ms = 5;
    if (use_mmap) {
      printf("Using mmap to write to fb\n");
      u32* pixbuf = sys_mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED, fb, 0);
      if (pixbuf == MAP_FAILED) {
        printf("Error mapping framebuffer to memory\n");
        return;
      }
      sys_close(fb);
      for (int c=0; c <= 0xFF; c++) {
        memset(pixbuf, c, len);
        sleep_ms(delay_ms);
      }
      // munmap?
    } else {
      printf("Using pwrite64 to write to fb\n");
      u32* pixbuf = malloc(len);
      for (int c=0; c <= 0xFF; c++) {
        memset(pixbuf, c, len);
        err = sys_pwrite64(fb, (char*)pixbuf, len, 0);
        if (err < 0) {
          printf("Error writing to /dev/fb0: %d\n", -err);
          break;
        }
        sleep_ms(delay_ms);
      }
      free(pixbuf);
      sys_close(fb);
    }

    use_mmap = !use_mmap;
    printf("DONE\n");
  } else {
    printf("Executing: %s\n", cmd);
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

  char buf[1024];
  while (1) {
    int pid = sys_getpid();
    printf("%d> ", pid);

    isize len = sys_read(0, buf, sizeof(buf));
    if (len < 0) {
      printf("\nERROR: %d\n", -len);
    } else {
      if (len > 0 && buf[len-1] == '\n') {
        if (len > 1) {
          buf[len-1] = 0;
          handle_command(buf);
        }
      } else {
        printf("\n");
      }
    }
  }

  return 69;
}
