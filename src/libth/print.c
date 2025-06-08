#include <print.h>
#include <string.h>
#include <syscalls.h>

typedef int (vio_putc_fn)(void* vio, char c);

static int vio_print_str(vio_putc_fn vio_putc, void* vio, const char* str) {
  char c;
  while (c = *str++) {
    int ret = vio_putc(vio, c);
    if (ret < 0) return ret;
  }
  return 0;
}

// TODO: handle various options, e.g. hex
static int vio_print_int(vio_putc_fn vio_putc, void* vio, i64 val) {
  bool neg = false;
  if (val < 0) {
    neg = true;
    val == -val;
  }

  char buf[16];
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

  return vio_print_str(vio_putc, vio, p);
}

static int vio_format(vio_putc_fn vio_putc, void* vio, const char* fmt, va_list args) {
  int ret;
  while (1) {
    char c = *fmt++;
    switch (c) {
    case 0:
      // end of string
      return 0;
    case '%':
      char f = *fmt++;
      switch (f) {
      case '%':
        // literal %
        ret = vio_putc(vio, '%');
        break;
      case 's':
        // string
        ret = vio_print_str(vio_putc, vio, va_arg(args, const char*));
        break;
      case 'd':
        // int
        ret = vio_print_int(vio_putc, vio, va_arg(args, int));
        break;
      case 'x':
        // pointer - TODO: print as hex
        ret = vio_print_int(vio_putc, vio, va_arg(args, u64));
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
      ret = vio_putc(vio, c);
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

struct fdbuf {
  char* buf;
  int len;
  int cur;
  int fd;
};

static int fdbuf_putc(void* vio, char c) {
  struct fdbuf* fb = (struct fdbuf*) vio;
  fb->buf[fb->cur] = c;
  fb->cur++;
  if (fb->cur >= fb->len) {
    int ret = sys_write(fb->fd, fb->buf, fb->cur);
    if (ret < 0) return ret;
    fb->cur = 0;
  }
  return 0;
};

int vfprintf(int fd, const char* fmt, va_list args) {
  char buf[1024];
  struct fdbuf fb = {
    .buf = buf,
    .len = sizeof(buf),
    .cur = 0,
    .fd = fd,
  };

  int ret = vio_format(fdbuf_putc, &fb, fmt, args);
  if (ret < 0) return ret;

  if (fb.cur > 0) {
    return sys_write(fd, fb.buf, fb.cur);
  } else {
    return 0;
  }
}

isize snprintf(char *buf, usize size, const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  int result = vsnprintf(buf, size, fmt, args);
  va_end(args);
  return result;
}

struct stringbuf {
  char* buf;
  usize len;
  usize cur;
};

static int stringbuf_putc(void* vio, char c) {
  struct stringbuf* sb = (struct stringbuf*) vio;
  if (sb->cur < sb->len) {
    sb->buf[sb->cur] = c;
  }
  sb->cur++;
  return 0;
}

isize vsnprintf(char *buf, usize size, const char* fmt, va_list args) {
  struct stringbuf sb = {
    .buf = buf,
    .len = size,
    .cur = 0,
  };

  isize result = vio_format(stringbuf_putc, &sb, fmt, args);
  if (result < 0) {
    return result;
  }

  if (sb.cur < size) {
    buf[sb.cur] = 0; // nul-terminate if it fits
  }

  return sb.cur;
}
