#pragma once

#include <types.h>

usize strlen(const char* str);
int strcmp(const char* a, const char* b);

char toupper(char c);
char tolower(char c);

isize str_find_char(char c, const char* str);

i64 str_to_int(const char* str);
u64 str_to_hex(const char* str);
