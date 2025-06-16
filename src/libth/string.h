#pragma once

#include <types.h>

char toupper(char c);
char tolower(char c);

isize strchri(const char* str, char c);

i64 atoi(const char* str, int base);
//char* itoa(i64 val, char* buf, int base);  // TODO


// The following all come from https://github.com/ARM-software/optimized-routines
#define memcpy    __memcpy_aarch64
#define memmove   __memmove_aarch64
#define memset    __memset_aarch64
#define memchr    __memchr_aarch64
#define memrchr   __memrchr_aarch64
#define memcmp    __memcmp_aarch64
#define strcpy    __strcpy_aarch64
#define stpcpy    __stpcpy_aarch64
#define strcmp    __strcmp_aarch64
#define strchr    __strchr_aarch64
#define strrchr   __strrchr_aarch64
#define strchrnul __strchrnul_aarch64
#define strlen    __strlen_aarch64
#define strnlen   __strnlen_aarch64
#define strncmp   __strncmp_aarch64

#ifndef __restrict
#define __restrict
#endif

void *__memcpy_aarch64 (void *__restrict, const void *__restrict, usize);
void *__memmove_aarch64 (void *, const void *, usize);
void *__memset_aarch64 (void *, int, usize);
void *__memchr_aarch64 (const void *, int, usize);
void *__memrchr_aarch64 (const void *, int, usize);
int __memcmp_aarch64 (const void *, const void *, usize);
char *__strcpy_aarch64 (char *__restrict, const char *__restrict);
char *__stpcpy_aarch64 (char *__restrict, const char *__restrict);
int __strcmp_aarch64 (const char *, const char *);
char *__strchr_aarch64 (const char *, int);
char *__strrchr_aarch64 (const char *, int);
char *__strchrnul_aarch64 (const char *, int );
usize __strlen_aarch64 (const char *);
usize __strnlen_aarch64 (const char *, usize);
int __strncmp_aarch64 (const char *, const char *, usize);
