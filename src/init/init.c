
#include <syscalls.h>
#include <string.h>

int print(const char *msg) {
  usize len = strlen(msg);
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
  } else if (strcmp(cmd, "exit") == 0) {
    sys_exit(0);
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
