#pragma once

#include <stdarg.h>
#include <types.h>

#define STDIN   0
#define STDOUT  1
#define STDERR  2

#undef FILE
#define FILE void

#define EOF     (-1)

#define SEEK_SET  0
#define SEEK_CUR  1
#define SEEK_END  2

int printf(const char* fmt, ...);
int eprintf(const char* fmt, ...);
int dprintf(int fd, const char* fmt, ...);
int vdprintf(int fd, const char* fmt, va_list args);

isize snprintf(char* buf, usize size, const char* fmt, ...);
isize vsnprintf(char* buf, usize size, const char* fmt, va_list args);
