#pragma once

#define NULL (0)
#define null (0)
#define false (0)
#define FALSE (0)
#define true (1)
#define TRUE (1)

#define INT8_MIN   (-1-0x7f)
#define INT16_MIN  (-1-0x7fff)
#define INT32_MIN  (-1-0x7fffffff)
#define INT64_MIN  (-1-0x7fffffffffffffff)

#define INT8_MAX   (0x7f)
#define INT16_MAX  (0x7fff)
#define INT32_MAX  (0x7fffffff)
#define INT64_MAX  (0x7fffffffffffffff)

#define UINT8_MAX  (0xff)
#define UINT16_MAX (0xffff)
#define UINT32_MAX (0xffffffff)
#define UINT64_MAX (0xffffffffffffffff)

#define INTPTR_MIN      INT64_MIN
#define INTPTR_MAX      INT64_MAX
#define UINTPTR_MAX     UINT64_MAX
#define PTRDIFF_MIN     INT64_MIN
#define PTRDIFF_MAX     INT64_MAX
#define SIZE_MAX        UINT64_MAX

typedef char i8;

typedef unsigned char u8;
typedef unsigned char uchar;
typedef unsigned char bool;

typedef short i16;
typedef short int16;

typedef unsigned short u16;
typedef unsigned short uint16;

typedef int i32;
typedef int int32;

typedef unsigned int u32;
typedef unsigned int uint32;
typedef unsigned int uint;

typedef long i64;
typedef long int64;
typedef long isize;
typedef long intptr;
typedef long ptrdiff;

typedef unsigned long u64;
typedef unsigned long uint64;
typedef unsigned long usize;
typedef unsigned long uintptr;

typedef float f32;

typedef double f64;
