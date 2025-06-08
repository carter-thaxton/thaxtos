#pragma once

#include <stdarg.h>
#include <types.h>

#define STDIN   0
#define STDOUT  1
#define STDERR  2

#define EOF     (-1)

int printf(const char* fmt, ...);
int eprintf(const char* fmt, ...);
int fprintf(int fd, const char* fmt, ...);
int vfprintf(int fd, const char* fmt, va_list args);

isize snprintf(char* buf, usize size, const char* fmt, ...);
isize vsnprintf(char* buf, usize size, const char* fmt, va_list args);
