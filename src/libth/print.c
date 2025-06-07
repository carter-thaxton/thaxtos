#include <print.h>
#include <string.h>
#include <syscalls.h>

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
