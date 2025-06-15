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

char toupper(char c) {
  if (c >= 'a' && c <= 'z') {
    return (c - 'a') + 'A';
  }
  return c;
}

char tolower(char c) {
  if (c >= 'A' && c <= 'Z') {
    return (c - 'A') + 'a';
  }
  return c;
}

isize str_find_char(char c, const char* str) {
  if (c == 0) return -1;
  for (isize i=0; str[i]; i++)
    if (str[i] == c)
      return i;
  return -1;
}

i64 str_to_int(const char* str) {
  static char* dec_chars = "0123456789ABCDEF";
  i64 result = 0;
  bool neg = false;

  if (*str == '-') {
    neg = true;
    str++;
  }

  while (*str) {
    isize d = str_find_char(*str, dec_chars);
    if (d < 0) break;
    result = result * 10 + d;
    str++;
  }

  if (neg) {
    return -result;
  } else {
    return result;
  }
}

u64 str_to_hex(const char* str) {
  static char* hex_chars = "0123456789ABCDEF";
  i64 result = 0;

  while (*str) {
    isize d = str_find_char(toupper(*str), hex_chars);
    if (d < 0) break;
    result = result * 16 + d;
    str++;
  }

  return result;
}
