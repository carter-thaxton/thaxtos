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

int printf(const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  int result = vfprintf(STDOUT, fmt, args);
  va_end(args);
  return result;
}

int eprintf(const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  int result = vfprintf(STDERR, fmt, args);
  va_end(args);
  return result;
}

int fprintf(int fd, const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  int result = vfprintf(fd, fmt, args);
  va_end(args);
  return result;
}

int vfprintf(int fd, const char* fmt, va_list args) {
  // TODO: use vsnprintf in a loop, over some block size that can fit on stack, e.g. 1024
  return 0;
}

int snprintf(char *buf, usize size, const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  int result = vsnprintf(buf, size, fmt, args);
  va_end(args);
  return result;
}

int vsnprintf(char *buf, usize size, const char* fmt, va_list args) {
  // TODO: the real meat
  return 0;
}
