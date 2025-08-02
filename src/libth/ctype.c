#include <ctype.h>

// https://stackoverflow.com/questions/5149725/c-isgraph-function

bool isalnum(char c) {
  return isalpha(c) || isdigit(c);
}

bool isalpha(char c) {
  return isupper(c) || islower(c);
}

bool isascii(char c) {
  return (c >= 0) && (c <= 127);
}

bool isblank(char c) {
  return (c == ' ') || (c == '\t');
}

bool iscntrl(char c) {
  return ((c >= 0x00) && (c <= 0x1f)) || (c == 0x7f);
}

bool isdigit(char c) {
  return (c >= '0') && (c <= '9');
}

bool isgraph(char c) {
  return (c >= 0x21) && (c <= 0x7e);
}

bool islower(char c) {
  return (c >= 'a') && (c <= 'z');
}

bool isprint(char c) {
  return (c >= 0x20) && (c <= 0x7e);
}

bool ispunct(char c) {
  return isgraph(c) && !isalnum(c);
}

bool isspace(char c) {
  return (c == ' ') || ((c >= 0x09) && (c <= 0x0d));
}

bool isupper(char c) {
  return (c >= 'A') && (c <= 'Z');
}

bool isxdigit(char c) {
  return isdigit(c) || ((c >= 'A') && (c <= 'F')) || ((c >= 'a') && (c <= 'f'));
}
