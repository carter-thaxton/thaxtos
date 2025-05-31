
#include <syscalls.h>

long strlen(char* str) {
  long len = 0;
  while (*str) {
    len++;
    str++;
  }
  return len;
}

int print(char *msg) {
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

int main() {
  print("Welcome to Thaxtos!\n");

  print("sizeof int: ");
  print_int(sizeof(int));
  print("\n");

  print("sizeof long: ");
  print_int(sizeof(long));
  print("\n");

  while (1) {
    sleep_ms(500);
    print("TICK\n");
  }

  return 69;
}
