#pragma once

#define DBG_INT(expr) \
  do { \
    print(#expr " = "); \
    print_int(expr); \
    print("\n"); \
  } while(0);

int print(const char *msg);
int print_int(int val);
