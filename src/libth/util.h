#pragma once

#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define CMP(x, y) ((x) < (y) ? -1 : (x) > (y) ? 1 : 0)

#define DBG_INT(expr) \
  do { \
    print(#expr " = "); \
    print_int(expr); \
    print("\n"); \
  } while(0);
