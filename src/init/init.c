
#include <syscalls.h>
#include <string.h>

int print(const char *msg) {
  long len = strlen(msg);
  return sys_write(1, msg, len);
}

int print_int(int val) {
  do {
    char c = '0' + val % 10;
    val = val / 10;
    sys_write(1, &c, 1);
  } while (val > 0);
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
  } else {
    print("Executing: ");
    print(cmd);
    print("\n");
  }
}

int main() {
  print("Welcome to Thaxtos!\n");

  print("sizeof int: ");
  print_int(sizeof(int));
  print("\n");

  print("sizeof long: ");
  print_int(sizeof(long));
  print("\n");

  char buf[1024];
  while (1) {
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
