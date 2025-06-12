#include <stdio.h>
#include <string.h>
#include <syscalls.h>

#define F_WRITE   (1 << 1)
#define F_READ    (1 << 2)

#undef FILE
#define FILE struct stdio_file

struct stdio_file {
  u32 flags;
  int fd;
  char* buf;
  usize buf_size;
  usize offset;
};

static int stdio_flush(FILE* f) {
  if ((f->flags & F_WRITE) == 0) {
    // can't write to file
    return -1;
  }

  // if backed by string, then nothing to do
  if (f->fd < 0) {
    return 0;
  }

  if (f->offset > 0) {
    int ret = sys_write(f->fd, f->buf, f->offset);
    if (ret < 0) return ret;
    f->offset = 0;
  }

  return 0;
}

static int stdio_putc(FILE* f, char c) {
  if ((f->flags & F_WRITE) == 0) {
    // can't write to file
    return -1;
  }

  if (f->offset < f->buf_size) {
    f->buf[f->offset] = c;
  }

  f->offset++;

  // flush on every character once offset reaches buf_size
  if (f->offset >= f->buf_size) {
    return stdio_flush(f);
  }

  return 0;
}

static int stdio_print_str(FILE* f, const char* str) {
  char c;
  while ((c = *str++)) {
    int ret = stdio_putc(f, c);
    if (ret < 0) return ret;
  }
  return 0;
}

// TODO: handle various options, e.g. hex
static int stdio_print_int(FILE* f, i64 val) {
  bool neg = false;
  if (val < 0) {
    neg = true;
    val = -val;
  }

  char buf[32];  // max length of 64-bit integer, and then some
  char *p = &buf[sizeof(buf)-1];
  *(--p) = 0;

  do {
    char c = '0' + (val % 10);
    val = val / 10;
    *(--p) = c;
  } while (val > 0);

  if (neg) {
    *(--p) = '-';
  }

  return stdio_print_str(f, p);
}

static int stdio_format(FILE* f, const char* fmt, va_list args) {
  int ret;
  while (1) {
    char c = *fmt++;
    switch (c) {
    case 0:
      // end of string
      return 0;
    case '%':
      char spec = *fmt++;
      switch (spec) {
      case '%':
        // literal %
        ret = stdio_putc(f, '%');
        break;
      case 's':
        // string
        ret = stdio_print_str(f, va_arg(args, const char*));
        break;
      case 'd':
        // int
        ret = stdio_print_int(f, va_arg(args, int));
        break;
      case 'x':
        // pointer - TODO: print as hex
        ret = stdio_print_int(f, va_arg(args, u64));
        break;
      // case 'l':
      // long, handle next char
      //   break;
      default:
        // illegal format
        return -1;
      }
      break;
    default:
      ret = stdio_putc(f, c);
    }
    if (ret < 0) {
      // io error
      return ret;
    }
  }
}

int printf(const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  int result = vdprintf(STDOUT, fmt, args);
  va_end(args);
  return result;
}

int eprintf(const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  int result = vdprintf(STDERR, fmt, args);
  va_end(args);
  return result;
}

int dprintf(int fd, const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  int result = vdprintf(fd, fmt, args);
  va_end(args);
  return result;
}

int vprintf(const char* fmt, va_list args) {
  return vdprintf(STDOUT, fmt, args);
}

int vdprintf(int fd, const char* fmt, va_list args) {
  char buf[1024];
  FILE file = {
    .flags = F_WRITE,
    .fd = fd,
    .buf = buf,
    .buf_size = sizeof(buf),
    .offset = 0,
  };

  int ret = stdio_format(&file, fmt, args);
  if (ret < 0) return ret;

  return stdio_flush(&file);
}

isize snprintf(char *buf, usize size, const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  int result = vsnprintf(buf, size, fmt, args);
  va_end(args);
  return result;
}

isize vsnprintf(char *buf, usize size, const char* fmt, va_list args) {
  FILE file = {
    .flags = F_WRITE,
    .fd = -1,
    .buf = buf,
    .buf_size = size,
    .offset = 0,
  };

  int ret = stdio_format(&file, fmt, args);
  if (ret < 0) return ret;

  if (file.offset < size) {
    buf[file.offset] = 0; // nul-terminate if it fits
  }

  return file.offset;
}
