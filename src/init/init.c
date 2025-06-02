
#include <syscalls.h>
#include <string.h>

int print(const char *msg) {
  usize len = strlen(msg);
  return sys_write(1, msg, len);
}

int print_int(int val) {
  bool neg = false;
  if (val < 0) {
    neg = true;
    val = -val;
  }

  char buf[16];
  char *p = &buf[sizeof(buf)-1];
  *p = 0;

  do {
    char c = '0' + (val % 10);
    val = val / 10;
    *(--p) = c;
  } while (val > 0);

  if (neg) {
    *(--p) = '-';
  }

  print(p);
}

int sleep_us(int usec) {
  return sys_nanosleep(usec / 1000000, (usec % 1000000) * 1000);
}

int sleep_ms(int msec) {
  return sys_nanosleep(msec / 1000, (msec % 1000) * 1000000);
}

int sleep_s(int sec) {
  return sys_nanosleep(sec, 0);
}

void handle_command(const char* cmd) {
  if (strcmp(cmd, "none") == 0) {
    // ignore
  } else if (strcmp(cmd, "reboot") == 0) {
    sys_reboot(REBOOT_CMD_RESTART);
  } else if (strcmp(cmd, "halt") == 0) {
    sys_reboot(REBOOT_CMD_HALT);
  } else if (strcmp(cmd, "poweroff") == 0) {
    sys_reboot(REBOOT_CMD_POWEROFF);
  } else if (strcmp(cmd, "exit") == 0) {
    sys_exit(69);
  } else if (strcmp(cmd, "kill") == 0) {
    int pid = sys_getpid();
    int sig = 9;
    print("Kill self\n");
    int result = sys_kill(pid, sig);
    print("Should not get here\n");
  } else if (strcmp(cmd, "fork") == 0) {
    int pid = sys_fork();
    if (pid == 0) {
      // child
      // continue on...
    } else {
      // parent
      // wait for child to exit
      print("Waiting for child: ");
      print_int(pid);
      print("\n");
      siginfo_t info;
      int result = sys_waitid(P_PID, pid, &info, WEXITED);
      print("waitid returned: ");
      print_int(result);
      if (result == 0) {
        print("\n  pid: ");
        print_int(info.pid);
        print("\n  uid: ");
        print_int(info.uid);
        print("\n  code: ");
        print_int(info.code);
        print("\n  status: ");
        print_int(info.status);
        print("\n  signo: ");
        print_int(info.signo);
        print("\n  errno: ");
        print_int(info.errno);
      }
      print("\n");
    }
  } else if (strcmp(cmd, "pid") == 0) {
    int pid = sys_getpid();
    print_int(pid);
    print("\n");
  } else if (strcmp(cmd, "ppid") == 0) {
    int ppid = sys_getppid();
    print_int(ppid);
    print("\n");
  } else if (strcmp(cmd, "uid") == 0) {
    int uid = sys_getuid();
    print_int(uid);
    print("\n");
  } else if (strcmp(cmd, "euid") == 0) {
    int uid = sys_geteuid();
    print_int(uid);
    print("\n");
  } else if (strcmp(cmd, "gid") == 0) {
    int gid = sys_getgid();
    print_int(gid);
    print("\n");
  } else if (strcmp(cmd, "egid") == 0) {
    int gid = sys_getegid();
    print_int(gid);
    print("\n");
  } else if (strcmp(cmd, "sysinfo") == 0) {
    sysinfo_t info;
    sys_sysinfo(&info);
    print("System info");
    print("\n  Uptime: ");
    print_int(info.uptime);
    print("\n  Load (1 min): ");
    print_int(info.loads[0]);
    print("\n  Load (5 min): ");
    print_int(info.loads[1]);
    print("\n  Load (15 min): ");
    print_int(info.loads[2]);
    print("\n  Total RAM: ");
    print_int(info.totalram);
    print("\n  Free RAM: ");
    print_int(info.freeram);
    print("\n  Shared RAM: ");
    print_int(info.sharedram);
    print("\n  Buffer RAM: ");
    print_int(info.bufferram);
    print("\n  Total swap: ");
    print_int(info.totalswap);
    print("\n  Free swap: ");
    print_int(info.freeswap);
    print("\n  Processes: ");
    print_int(info.procs);
    print("\n  Total high memory: ");
    print_int(info.totalhigh);
    print("\n  Free high memory: ");
    print_int(info.freehigh);
    print("\n  Memory unit size: ");
    print_int(info.mem_unit);
    print("\n");
  } else {
    print("Executing: ");
    print(cmd);
    print("\n");
  }
}

#define PRINT_SIZE(type) \
  do { \
    print("sizeof " #type ": "); \
    print_int(sizeof(type)); \
    print("\n"); \
  } while(0);

void print_sizes() {
  PRINT_SIZE(char);
  PRINT_SIZE(short);
  PRINT_SIZE(int);
  PRINT_SIZE(long);
  PRINT_SIZE(float);
  PRINT_SIZE(double);

  PRINT_SIZE(u8);
  PRINT_SIZE(u16);
  PRINT_SIZE(u32);
  PRINT_SIZE(u64);

  PRINT_SIZE(i8);
  PRINT_SIZE(i16);
  PRINT_SIZE(i32);
  PRINT_SIZE(i64);

  PRINT_SIZE(f32);
  PRINT_SIZE(f64);
}

int main() {
  print("Welcome to Thaxtos!\n");
  print_sizes();

  char buf[1024];
  while (1) {
    int pid = sys_getpid();
    print_int(pid);
    print("> ");

    isize len = sys_read(0, buf, sizeof(buf));
    if (len < 0) {
      print("\nERROR: ");
      print_int(-len);
      print("\n");
    } else {
      if (len > 0 && buf[len-1] == '\n') {
        if (len > 1) {
          buf[len-1] = 0;
          handle_command(buf);
        }
      } else {
        print("\n");
      }
    }
  }

  return 69;
}
