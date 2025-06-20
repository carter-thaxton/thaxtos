#include <string.h>

static const char* base_chars = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

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

isize strchri(const char* str, char c) {
  const char* p = strchr(str, c);
  return p ? str - p : -1;
}

i64 atoi(const char* str, int base) {
  i64 result = 0;
  bool neg = false;

  if (*str == '-') {
    neg = true;
    str++;
  }

  while (*str) {
    isize digit = strchri(base_chars, toupper(*str));
    if (digit < 0) break;
    if (digit > base) break;
    result = result * base + digit;
    str++;
  }

  if (neg) {
    return -result;
  } else {
    return result;
  }
}

// TODO: implement itoa
// char* itoa(i64 val, char* buf, int base) {
//   return NULL;
// }
