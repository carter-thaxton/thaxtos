#pragma once

#include <stdarg.h>
#include <types.h>

#define STDIN   (0)
#define STDOUT  (1)
#define STDERR  (2)

#define DBG_INT(expr) \
  do { \
    print(#expr " = "); \
    print_int(expr); \
    print("\n"); \
  } while(0);

int print(const char *msg);
int print_int(int val);

int printf(const char* fmt, ...);
int eprintf(const char* fmt, ...);
int fprintf(int fd, const char* fmt, ...);
int vfprintf(int fd, const char* fmt, va_list args);

int snprintf(char *buf, usize size, const char* fmt, ...);
int vsnprintf(char *buf, usize size, const char* fmt, va_list args);
