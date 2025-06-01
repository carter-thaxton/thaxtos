#include <string.h>

usize strlen(const char* str) {
  usize len = 0;
  while (*str) {
    len++;
    str++;
  }
  return len;
}

int strcmp(const char* a, const char* b) {
    while (*a && (*a == *b)) {
        a++;
        b++;
    }
    return (*a > *b) - (*b > *a);
}
