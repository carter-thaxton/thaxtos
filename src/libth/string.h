#pragma once

#include <types.h>

usize strlen(const char* str);
int strcmp(const char* a, const char* b);

char toupper(char c);
char tolower(char c);

char* strchr(const char* str, char c);
isize strchri(const char* str, char c);

i64 atoi(const char* str, int base);
//char* itoa(i64 val, char* buf, int base);  // TODO
